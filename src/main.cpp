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

RgbImage _decode(std::vector<uint8_t>& file) {
    std::vector<uint8_t> hevcBuffer;
    
    ISOBMFF::Parser parser;
    parser.Parse(file);
    auto isobmffFile = parser.GetFile();
    auto ftyp = isobmffFile->GetTypedBox<ISOBMFF::FTYP>("ftyp");
    if (ftyp->GetMajorBrand() != "heic") {
        throw "Not a HEIC file";
    }

    getHeader(isobmffFile, hevcBuffer);
    getImageData(file, isobmffFile, hevcBuffer);

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
}
