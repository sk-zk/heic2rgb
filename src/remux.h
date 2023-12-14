#ifndef REMUX_H
#define REMUX_H

std::vector<uint8_t> muxHevcToMp4(const uint8_t* hevcData, size_t hevcSize, int width, int height);

#endif
