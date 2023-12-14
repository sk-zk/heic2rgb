#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
#include <pybind11/pytypes.h>

#include <iostream>
#include "ISOBMFF.hpp"
#include <fstream>
#include <filesystem>
#include <string>

#include "decoder.h"
#include "heif.h"
#include "remux.h"

std::shared_ptr<ISOBMFF::File> parseIsobmffFile(std::vector<uint8_t>& file) {
    ISOBMFF::Parser parser;
    parser.Parse(file);
    auto isobmffFile = parser.GetFile();
    auto ftyp = isobmffFile->GetTypedBox<ISOBMFF::FTYP>("ftyp");
    if (ftyp->GetMajorBrand() != "heic") {
        throw "Not a HEIC file";
    }
    return isobmffFile;
}

void getBitstream(std::vector<uint8_t>& heicBuffer, std::shared_ptr<ISOBMFF::File> file,
    std::vector<uint8_t>& hevcBuffer)
{
    getHeader(file, hevcBuffer);
    getImageData(heicBuffer, file, hevcBuffer);
}

RgbImage _decode(std::vector<uint8_t>& heicBuffer) {
    std::vector<uint8_t> hevcBuffer;  
    auto file = parseIsobmffFile(heicBuffer);
    getBitstream(heicBuffer, file, hevcBuffer);
    auto rgb = convertToRgb(hevcBuffer);
    return rgb;
}

RgbImage decode(std::string file) {
    //            ^this is mapped to a bytes param. 
    //             I should probably use pybind11::bytes to clarify
    //             but I don't know how to cast that to a vector
    auto vec = std::vector<uint8_t>(file.c_str(), file.c_str() + file.size());
    return _decode(vec);
}

std::vector<uint8_t> _toMp4(std::vector<uint8_t>& heicBuffer) {
    std::vector<uint8_t> hevcBuffer;
    auto file = parseIsobmffFile(heicBuffer);
    getBitstream(heicBuffer, file, hevcBuffer);
    auto dimensions = getDimensions(file);

    uint8_t* mp4Buffer;
    uint8_t* hevcData = hevcBuffer.data();
    auto hevcSize = hevcBuffer.size();
    auto size = muxHevcToMp4(hevcData, hevcSize, &mp4Buffer,
        dimensions.width, dimensions.height);

    std::vector<uint8_t> mp4(mp4Buffer, mp4Buffer + size);
    return mp4;
}

pybind11::bytes toMp4(std::string file) {
    auto vec = std::vector<uint8_t>(file.c_str(), file.c_str() + file.size());
    auto mp4 = _toMp4(vec);
    return pybind11::bytes((char*)mp4.data(), mp4.size());
}

namespace py = pybind11;
PYBIND11_MODULE(_core, m) {
    m.doc() = R"pbdoc(
      heic2rgb
      -----------------------
      .. currentmodule:: heic2rgb
      .. autosummary::
         :toctree: _generate
         decode
    )pbdoc";
    
    py::class_<RgbImage>(m, "RgbImage")
        .def(py::init())
        .def_readonly("width", &RgbImage::width)
        .def_readonly("height", &RgbImage::height)
        .def_readonly("data", &RgbImage::data);

    m.def("decode", &decode, R"pbdoc(
        Converts the main image of a HEIC file to raw RGB pixels.
    )pbdoc");

    m.def("to_mp4", &toMp4, R"pbdoc(
        Remuxes the main image of a HEIC file into a MP4 container.
    )pbdoc");
}
