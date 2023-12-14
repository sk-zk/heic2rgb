#include <vector>

extern "C" {
  #include <libavformat/avformat.h>
  #include <libavcodec/avcodec.h>
  #include <libavutil/opt.h>
  #include <libavutil/mathematics.h>
  #include <libavutil/imgutils.h>
  #include <libavutil/mem.h>
}

std::vector<uint8_t> muxHevcToMp4(const uint8_t* hevcData, size_t hevcSize, int width, int height) {
    AVFormatContext* inputFormatCtx = avformat_alloc_context();
    AVFormatContext* outputFormatCtx = avformat_alloc_context();

    AVIOContext* inputAvioCtx = avio_alloc_context(const_cast<uint8_t*>(hevcData), hevcSize, 0, nullptr, nullptr, nullptr, nullptr);
    inputFormatCtx->pb = inputAvioCtx;

    auto inputFormat = av_find_input_format("hevc");
    inputFormatCtx->iformat = inputFormat;

    avformat_open_input(&inputFormatCtx, "", inputFormat, nullptr);

    inputFormatCtx->streams[0]->r_frame_rate = av_make_q(25, 1);
    inputFormatCtx->streams[0]->codecpar->width = width;
    inputFormatCtx->streams[0]->codecpar->height = height;
    inputFormatCtx->streams[0]->codecpar->format = AV_PIX_FMT_YUVJ420P;
    inputFormatCtx->streams[0]->codecpar->profile = 3;
    inputFormatCtx->streams[0]->codecpar->level = 180;
    inputFormatCtx->streams[0]->codecpar->color_range = AVCOL_RANGE_JPEG;
    inputFormatCtx->streams[0]->codecpar->chroma_location = AVCHROMA_LOC_LEFT;

    AVIOContext* outputAvioCtx = nullptr;

    avio_open_dyn_buf(&outputAvioCtx);
    outputFormatCtx->pb = outputAvioCtx;

    auto outputFormat = av_guess_format("mp4", nullptr, nullptr);
    outputFormatCtx->oformat = outputFormat;

    AVStream* videoStream = avformat_new_stream(outputFormatCtx, nullptr);

    avcodec_parameters_copy(videoStream->codecpar, inputFormatCtx->streams[0]->codecpar);

    avformat_write_header(outputFormatCtx, nullptr);

    AVPacket packet;
    av_init_packet(&packet);
    packet.data = nullptr;
    packet.size = 0;

    while (av_read_frame(inputFormatCtx, &packet) == 0) {
        if (packet.stream_index == 0) {
            packet.pts = av_rescale_q(packet.pts, inputFormatCtx->streams[0]->time_base,
                videoStream->time_base);
            packet.dts = av_rescale_q(packet.dts, inputFormatCtx->streams[0]->time_base,
                videoStream->time_base);
            packet.duration = av_rescale_q(packet.duration, inputFormatCtx->streams[0]->time_base,
                videoStream->time_base);
            av_interleaved_write_frame(outputFormatCtx, &packet);
        }
        av_packet_unref(&packet);
    }

    av_write_trailer(outputFormatCtx);

    uint8_t* mp4Data = nullptr;
    size_t mp4Size = avio_close_dyn_buf(outputAvioCtx, &mp4Data);
    std::vector<uint8_t> mp4Out(mp4Data, mp4Data + mp4Size);
    av_free(mp4Data);

    avformat_free_context(inputFormatCtx);
    avformat_free_context(outputFormatCtx);
    avio_context_free(&inputAvioCtx);

    return mp4Out;
}
