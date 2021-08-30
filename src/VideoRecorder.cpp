#include "../include/VideoRecorder.h"

VideoRecorder::VideoRecorder() {
    c = NULL;
    sws_context = NULL;
    frame = NULL;
}

void VideoRecorder::ffmpeg_encoder_set_frame_yuv_from_rgb(uint8_t* rgb) {
    const int in_linesize[1] = { 4 * c->width };
    sws_context = sws_getCachedContext(sws_context,
        c->width, c->height, AV_PIX_FMT_RGB32,
        c->width, c->height, AV_PIX_FMT_YUV420P,
        0, NULL, NULL, NULL);
    sws_scale(sws_context, (const uint8_t* const*)&rgb, in_linesize, 0,
        c->height, frame->data, frame->linesize);
}

void VideoRecorder::ffmpeg_encoder_start(const char* filename, int fps, int width, int height) {
    const AVCodec* codec;
    int ret;
    AVCodecID codec_id = AV_CODEC_ID_MPEG1VIDEO;
    //avcodec_register_all();
    codec = avcodec_find_encoder(codec_id);
    if (!codec) {
        fprintf(stderr, "Codec not found\n");
        exit(1);
    }
    c = avcodec_alloc_context3(codec);
    if (!c) {
        fprintf(stderr, "Could not allocate video codec context\n");
        exit(1);
    }
    c->bit_rate = 400000;
    c->width = width;
    c->height = height;
    c->time_base.num = 1;
    c->time_base.den = fps;
    c->gop_size = 10;
    c->max_b_frames = 1;
    c->pix_fmt = AV_PIX_FMT_YUV420P;
    if (codec_id == AV_CODEC_ID_H264)
        av_opt_set(c->priv_data, "preset", "slow", 0);
    if (avcodec_open2(c, codec, NULL) < 0) {
        fprintf(stderr, "Could not open codec\n");
        exit(1);
    }
    file = fopen(filename, "wb");
    if (!file) {
        fprintf(stderr, "Could not open %s\n", filename);
        exit(1);
    }
    frame = av_frame_alloc();
    if (!frame) {
        fprintf(stderr, "Could not allocate video frame\n");
        exit(1);
    }
    frame->format = c->pix_fmt;
    frame->width = c->width;
    frame->height = c->height;
    ret = av_image_alloc(frame->data, frame->linesize, c->width, c->height, c->pix_fmt, 32);
    if (ret < 0) {
        fprintf(stderr, "Could not allocate raw picture buffer\n");
        exit(1);
    }
}

void VideoRecorder::ffmpeg_encoder_finish() {
    uint8_t endcode[] = { 0, 0, 1, 0xb7 };
    int got_output, ret;
    do {
        fflush(stdout);
        ret = avcodec_receive_packet(c, &pkt);
        if (ret < 0) {
            fprintf(stderr, "Error encoding frame\n");
            exit(1);
        }
        if (ret >= 0) {
            fwrite(pkt.data, 1, pkt.size, file);
            av_packet_unref(&pkt);
        }
    } while (ret >= 0);
    fwrite(endcode, 1, sizeof(endcode), file);
    fclose(file);
    avcodec_close(c);
    av_free(c);
    av_freep(&frame->data[0]);
    av_frame_free(&frame);
}

void VideoRecorder::ffmpeg_encoder_encode_frame(uint8_t* rgb) {
    int ret, got_output;
    ffmpeg_encoder_set_frame_yuv_from_rgb(rgb);
    pkt.data = NULL;
    pkt.size = 0;
    avcodec_send_frame(c, frame);
    ret = avcodec_receive_packet(c, &pkt);
    if (ret >= 0) {
        fwrite(pkt.data, 1, pkt.size, file);
        av_packet_unref(&pkt);
    }
}

void VideoRecorder::ffmpeg_encoder_glread_rgb(uint8_t** rgb, GLubyte** pixels, unsigned int width, unsigned int height, int nframes) {
    size_t i, j, k, cur_gl, cur_rgb, nvals;
    const size_t format_nchannels = 4;
    nvals = format_nchannels * width * height;
    frame->pts = nframes;
    *pixels = (GLubyte*)realloc(*pixels, nvals * sizeof(GLubyte));
    *rgb = (uint8_t*)realloc(*rgb, nvals * sizeof(uint8_t));
    // Get RGBA to align to 32 bits instead of just 24 for RGB. May be faster for FFmpeg.
    glReadPixels(0, 0, width, height, GL_RGBA, GL_UNSIGNED_BYTE, *pixels);
    for (i = 0; i < height; i++) {
        for (j = 0; j < width; j++) {
            cur_gl = format_nchannels * (width * (height - i - 1) + j);
            cur_rgb = format_nchannels * (width * i + j);
            for (k = 0; k < format_nchannels; k++)
                (*rgb)[cur_rgb + k] = (*pixels)[cur_gl + k];
        }
    }
}