#ifndef REMUX_H
#define REMUX_H

size_t muxHevcToMp4(const uint8_t* hevc_data, size_t hevc_size, uint8_t** mp4_data, 
	int width, int height);

#endif
