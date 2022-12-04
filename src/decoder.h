#ifndef DECODER_H
#define DECODER_H

#include <pybind11/pytypes.h>
#include <vector>
#include <string>

struct RgbImage {
    int width;
    int height;
    pybind11::bytes data;
};

RgbImage convertToRgb(std::vector<uint8_t>& hevcBuffer);

#endif
