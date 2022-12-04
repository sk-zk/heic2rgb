#ifndef HEIF_H
#define HEIF_H

#include <vector>
#include "ISOBMFF.hpp"

void getHeader(std::shared_ptr<ISOBMFF::File>& file, std::vector<uint8_t>& buffer);
void getImageData(std::vector<uint8_t>& fileBuffer, std::shared_ptr<ISOBMFF::File>& file, std::vector<uint8_t>& buffer);

#endif
