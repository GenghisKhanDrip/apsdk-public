﻿
#ifndef es_player_h_
#define es_player_h_

#include <condition_variable>
#include <deque>
#include <memory>
#include <mutex>

#ifdef __cplusplus
extern "C" {
#endif

#define SDL_MAIN_HANDLED
#include <SDL2/SDL.h>
#include <SDL2/SDL_rect.h>
#include <SDL2/SDL_render.h>
#include <SDL2/SDL_video.h>
#include <libavcodec/avcodec.h>
#include <libavformat/avformat.h>
#include <libavutil/imgutils.h>
#include <libswscale/swscale.h>

#ifdef __cplusplus
}
#endif

namespace esp {
class es_frame_pool {
public:
  es_frame_pool(int capacity = 16);

  ~es_frame_pool();

  AVFrame *acquire();

  void recycle(AVFrame *frm);

private:
  int m_capacity;
  std::deque<AVFrame *> m_q;
  std::mutex m_mtx;
  std::condition_variable m_cv;
};

class es_frame_queue {
public:
  es_frame_queue();
  ~es_frame_queue();

  AVFrame *dequeue();
  AVFrame *try_dequeue();
  void enqueue(AVFrame *frm);

private:
  bool m_abandon;
  std::deque<AVFrame *> m_q;
  std::mutex m_mtx;
  std::condition_variable m_cv;
};

class es_player {
public:
  es_player();

  ~es_player();

  bool open_video_decoder(const uint8_t *config, int len);

  void close_video_decoder();

  void open_video_renderer();

  void close_video_renderer();

  bool feed_video(uint8_t *data, int len);

  bool open_audio_decoder(const uint8_t *config, int len);

  void close_audio_decoder();

  void open_audio_renderer();

  void close_audio_renderer();

  bool feed_audio(uint8_t *data, int len);

private:
  AVCodecContext *m_video_codec_ctx = nullptr;
  bool create_video_decoder_resource(const uint8_t *config, int len);
  void destroy_video_decoder_resource();

  AVCodecContext *m_audio_codec_ctx = nullptr;
  bool create_audio_decoder_resource(const uint8_t *config, int len);
  void destroy_audio_decoder_resource();

private:
  // frame pool used by video decoder
  es_frame_pool m_video_frm_pool;

  // decoded video frame queue
  es_frame_queue m_video_render_frm_queue;

  SDL_Window *m_sdl_window = nullptr;
  SDL_Renderer *m_sdl_renderer = nullptr;
  SDL_Texture *m_sdl_texture = nullptr;
  AVFrame *m_yuv_frame = nullptr;
  SwsContext *m_sws_ctx = nullptr;

  int m_last_source_width = 0;
  int m_last_source_height = 0;
  int m_last_texture_widht = 0;
  int m_last_texture_height = 0;
  std::mutex m_video_resource_update_lock;
  std::shared_ptr<std::thread> m_window_thread;
  std::shared_ptr<std::thread> m_worker_thread;

  bool create_video_renderer_resource();
  void destroy_video_renderer_resource();

  void scale_frame(const AVFrame *frame);
  void present_frame();
  void update_video_render_resource();

  void video_renderer_window_loop();
  void video_renderer_worker_loop();

  // frame pool used by audio decoder
  es_frame_pool m_audio_frm_pool;

  // decoded audio frame queue
  es_frame_queue m_audio_render_frm_queue;
};
} // namespace esp

#endif