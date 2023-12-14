#ifndef HEIF_H
#define HEIF_H

#include <vector>
#include "ISOBMFF.hpp"

struct HevcConfiguration {
    int width;
    int height;
    int generalLevelIdc;
};

void getHeader(std::shared_ptr<ISOBMFF::File>& file, std::vector<uint8_t>& buffer);
HevcConfiguration getHevcConfiguration(std::shared_ptr<ISOBMFF::File>& file);
void getImageData(std::vector<uint8_t>& fileBuffer, std::shared_ptr<ISOBMFF::File>& file, std::vector<uint8_t>& buffer);

#endif
