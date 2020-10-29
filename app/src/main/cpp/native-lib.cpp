#include <jni.h>
#include <android/log.h>
#include <string>

extern "C" {
#include <libavformat/avformat.h>
#include <libavcodec/avcodec.h>
#include <libswresample/swresample.h>
}

#define LOGI(FORMAT, ...) __android_log_print(ANDROID_LOG_INFO, "SeanFFmpegPlayer", FORMAT, ##__VA_ARGS__);
#define LOGE(FORMAT, ...) __android_log_print(ANDROID_LOG_ERROR, "SeanFFmpegPlayer", FORMAT, ##__VA_ARGS__);


extern "C" JNIEXPORT void JNICALL
Java_com_sean_ffmpegdemo_MainActivity_decodeAudioCpp(
        JNIEnv *env,
        jobject /* this */, jstring _src, jstring _out) {
    const char *src = env->GetStringUTFChars(_src, nullptr);
    const char *out = env->GetStringUTFChars(_out, nullptr);

    AVFormatContext *fmt_ctx = avformat_alloc_context();

    if (avformat_open_input(&fmt_ctx, src, nullptr, nullptr) < 0) {
        LOGE("open file error");
        return;
    }

    if (avformat_find_stream_info(fmt_ctx, nullptr) < 0) {
        LOGE("find stream info error");
        return;
    }

    int audio_stream_index = -1;
    for (int i = 0; i < fmt_ctx->nb_streams; i++) {
        if (fmt_ctx->streams[i]->codecpar->codec_type == AVMEDIA_TYPE_AUDIO) {
            audio_stream_index = i;
            LOGI("find audio stream index");
            break;
        }
    }

    AVCodecContext *codec_ctx = avcodec_alloc_context3(nullptr);
    avcodec_parameters_to_context(codec_ctx, fmt_ctx->streams[audio_stream_index]->codecpar);
    AVCodec *codec = avcodec_find_decoder(codec_ctx->codec_id);
    if (avcodec_open2(codec_ctx, codec, nullptr) < 0) {
        LOGE("could not open codec");
        return;
    }

    AVPacket *avPacket = av_packet_alloc();
    AVFrame *avFrame = av_frame_alloc();
    int got_frame = 0;
    int index = 0;
    FILE *out_file = fopen(out, "wb");
    while (av_read_frame(fmt_ctx, avPacket) == 0) {
        if (avPacket->stream_index == audio_stream_index) {
            if (avcodec_receive_frame(codec_ctx, avFrame) < 0 || avcodec_receive_packet(codec_ctx, avPacket) < 0) {
                LOGE("decode error:%d", index);
                break;
            }
            if (got_frame > 0) {
                LOGI("decode frame:%d", index++);
                fwrite(avFrame->data[0], 1, static_cast<size_t>(avFrame->linesize[0]), out_file);
            }
        }
    }
    LOGI("decode finish...");
    av_packet_unref(avPacket);
    av_frame_free(&avFrame);
    avcodec_close(codec_ctx);
    avformat_close_input(&fmt_ctx);
    fclose(out_file);

}
