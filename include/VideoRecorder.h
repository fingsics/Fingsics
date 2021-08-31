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

// Most of the code is from https://stackoverflow.com/a/14324292

class VideoRecorder {
private:
	AVCodecContext* c;
	AVPacket pkt;
	AVFrame* frame;
	FILE* file;
	struct SwsContext* sws_context;
	void ffmpeg_encoder_set_frame_yuv_from_rgb(uint8_t*);
	bool errorFound;
public:
	VideoRecorder();
	void ffmpeg_encoder_start(const char*, int, int, int);
	void ffmpeg_encoder_encode_frame(uint8_t*);
	void ffmpeg_encoder_glread_rgb(uint8_t**, GLubyte**, unsigned int, unsigned int, int);
	void ffmpeg_encoder_finish();
};

#endif