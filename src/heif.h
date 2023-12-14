#ifndef HEIF_H
#define HEIF_H

#include <vector>
#include "ISOBMFF.hpp"

struct Dimensions {
    int width;
    int height;
};

void getHeader(std::shared_ptr<ISOBMFF::File>& file, std::vector<uint8_t>& buffer);
Dimensions getDimensions(std::shared_ptr<ISOBMFF::File>& file);
void getImageData(std::vector<uint8_t>& fileBuffer, std::shared_ptr<ISOBMFF::File>& file, std::vector<uint8_t>& buffer);

#endif
