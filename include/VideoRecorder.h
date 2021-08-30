#ifndef VIDEORECORDER_H
#define VIDEORECORDER_H

#include <freeglut.h>

extern "C"
{
#include <libavcodec/avcodec.h>
#include <libavutil/imgutils.h>
#include <libavutil/opt.h>
#include <libswscale/swscale.h>
}

using namespace std;

class VideoRecorder {
private:
	AVCodecContext* c;
	AVPacket pkt;

	AVFrame* frame;
	FILE* file;
	struct SwsContext* sws_context;
	void ffmpeg_encoder_set_frame_yuv_from_rgb(uint8_t* rgb);
public:
	VideoRecorder();
	void ffmpeg_encoder_start(const char* filename, int fps, int width, int height);
	void ffmpeg_encoder_encode_frame(uint8_t* rgb);
	void ffmpeg_encoder_glread_rgb(uint8_t** rgb, GLubyte** pixels, unsigned int width, unsigned int height, int frame);
	void ffmpeg_encoder_finish();
};

#endif