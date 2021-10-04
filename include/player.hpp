#include <functional>
#include <thread>

#include "internal.hpp"
#include "media.hpp"

#ifndef PLAYER_HEADER
#define PLAYER_HEADER
#ifdef __cplusplus
extern "C" {
#endif

namespace lwm {

enum PlayerErrorCode {
  unknown,
  aborted,
  networkError,
  decodingError,
  sourceNotSupported
};

class Player;

class PlayerEvents {
  // TODO (alexmercerind): Make event handling multiple instance friendly & make
  // improvements to this class in specific.
 public:
  PlayerEvents(){};

  PlayerEvents(int32_t id) : id_(id) {}

  // void Frame(std::function<void(uint8_t* data, int32_t size, int32_t width,
  //                               int32_t height)> listener) {
  //   PlayerEvents::frame_ = listener;
  //   Internal::PlayerSetFrameEventHandler(id_, &PlayerEvents::OnFrame);
  // }

  void IsPlaying(std::function<void(bool)> listener) {
    PlayerEvents::is_playing_ = listener;
    Internal::PlayerSetIsPlayingEventHandler(id_, &PlayerEvents::OnIsPlaying);
  }

  void IsCompleted(std::function<void(bool)> listener) {
    PlayerEvents::is_completed_ = listener;
    Internal::PlayerSetIsCompletedEventHandler(id_,
                                               &PlayerEvents::OnIsCompleted);
  }

  void IsBuffering(std::function<void(bool)> listener) {
    PlayerEvents::is_buffering_ = listener;
    Internal::PlayerSetIsBufferingEventHandler(id_,
                                               &PlayerEvents::OnIsCompleted);
  }

  void Volume(std::function<void(float)> listener) {
    PlayerEvents::volume_ = listener;
    Internal::PlayerSetVolumeEventHandler(id_, &PlayerEvents::OnVolume);
  }

  void Rate(std::function<void(float)> listener) {
    PlayerEvents::rate_ = listener;
    Internal::PlayerSetRateEventHandler(id_, &PlayerEvents::OnRate);
  }

  void Position(std::function<void(int32_t)> listener) {
    PlayerEvents::position_ = listener;
    Internal::PlayerSetPositionEventHandler(id_, &PlayerEvents::OnPosition);
  }

  void Duration(std::function<void(int32_t)> listener) {
    PlayerEvents::duration_ = listener;
    Internal::PlayerSetDurationEventHandler(id_, &PlayerEvents::OnDuration);
  }

  void Index(std::function<void(int32_t)> listener) {
    PlayerEvents::index_ = listener;
    Internal::PlayerSetIndexEventHandler(id_, &PlayerEvents::OnIndex);
  }

  void DownloadProgress(std::function<void(float)> listener) {
    PlayerEvents::download_progress_ = listener;
    Internal::PlayerSetDownloadProgressEventHandler(
        id_, &PlayerEvents::OnDownloadProgress);
  }

  void Error(std::function<void(PlayerErrorCode, std::string)> listener) {
    PlayerEvents::error_ = listener;
    Internal::PlayerSetErrorEventHandler(id_, &PlayerEvents::OnError);
  }

 private:
  int32_t id_;

  // static inline std::function<void(uint8_t*, int32_t, int32_t, int32_t)>
  // frame_;

  // static inline void OnFrame(uint8_t* data, int32_t size, int32_t width,
  //                            int32_t height) {
  //   PlayerEvents::frame_(data, size, width, height);
  // }

  static inline std::function<void(bool)> is_playing_;

  static inline void OnIsPlaying(bool value) {
    PlayerEvents::is_playing_(value);
  }

  static inline std::function<void(bool)> is_completed_;

  static inline void OnIsCompleted(bool value) {
    PlayerEvents::is_completed_(value);
  }

  static inline std::function<void(bool)> is_buffering_;

  static inline void OnIsBuffering(bool value) {
    PlayerEvents::is_buffering_(value);
  }

  static inline std::function<void(float)> volume_;

  static inline void OnVolume(float value) { PlayerEvents::volume_(value); }

  static inline std::function<void(float)> rate_;

  static inline void OnRate(float value) { PlayerEvents::rate_(value); }

  static inline std::function<void(int32_t)> position_;

  static inline void OnPosition(int32_t value) {
    PlayerEvents::position_(value);
  }

  static inline std::function<void(int32_t)> duration_;

  static inline void OnDuration(int32_t value) {
    PlayerEvents::duration_(value);
  }

  static inline std::function<void(int32_t)> index_;

  static inline void OnIndex(int32_t value) { PlayerEvents::index_(value); }

  static inline std::function<void(float)> download_progress_;

  static inline void OnDownloadProgress(float value) {
    PlayerEvents::download_progress_(value);
  }

  static inline std::function<void(PlayerErrorCode, std::string)> error_;

  static inline void OnError(int32_t code, char* error) {
    PlayerEvents::error_(static_cast<PlayerErrorCode>(code),
                         std::string(error));
  }

  friend class Player;
};

class Player {
 public:
  Player(int32_t id, bool show_window = false,
         std::string window_title = VIDEO_WINDOW_CLASS, bool start_loop = true)
      : id_(id) {
    Internal::PlayerCreate(id, show_window, window_title.c_str());
    events_ = std::make_unique<PlayerEvents>(id_);
#ifdef __linux__
    if (start_loop) {
      new std::thread([=]() -> void { Internal::PlayerRun(); });
    }
#endif
  }

  PlayerEvents* events() const { return events_.get(); }

  void Open(std::vector<Media> medias) {
    std::vector<const char*> uris;
    uris.reserve(medias.size());
    std::vector<int32_t> ids;
    ids.reserve(medias.size());
    for (auto& media : medias) {
      uris.emplace_back(media.uri().c_str());
      ids.emplace_back(media.id());
    }
    Internal::PlayerOpen(id_, uris.size(), uris.data(), ids.data());
  }

  void Play() { Internal::PlayerPlay(id_); }

  void Pause() { Internal::PlayerPause(id_); }

  void Next() { Internal::PlayerNext(id_); }

  void Back() { Internal::PlayerBack(id_); }

  void Jump(int32_t index) { Internal::PlayerJump(id_, index); }

  void Add(Media media) {
    Internal::PlayerAdd(id_, media.uri().c_str(), media.id());
  }

  void Remove(int32_t index) { Internal::PlayerRemove(id_, index); }

  void ShowWindow(std::string window_title = VIDEO_WINDOW_CLASS) {
    Internal::PlayerShowWindow(id_, window_title.c_str());
  }

  void CloseWindow() { Internal::PlayerCloseWindow(id_); }

  void Seek(int32_t position) { Internal::PlayerSeek(id_, position); }

  void SetVolume(float volume) { Internal::PlayerSetVolume(id_, volume); }

  void SetRate(float rate) { Internal::PlayerSetRate(id_, rate); }

  void SetAudioBalance(float audio_balance) {
    Internal::PlayerSetAudioBalance(id_, audio_balance);
  }

  void SetAutoplay(bool autoplay) {
    Internal::PlayerSetAutoplay(id_, autoplay);
  }

  void SetShuffling(bool shuffling) {
    Internal::PlayerSetIsShuffling(id_, shuffling);
  }

  void SetIsLooping(bool looping) { Internal::PlayerSetAutoplay(id_, looping); }

  void SetIsAutoRepeat(bool autorepeat) {
    Internal::PlayerSetIsAutoRepeat(id_, autorepeat);
  }

  int32_t GetPosition() { return Internal::PlayerGetPosition(id_); }

  float GetVolume() { return Internal::PlayerGetVolume(id_); }

  float GetRate() { return Internal::PlayerGetRate(id_); }

  float GetAudioBalance() { return Internal::PlayerGetAudioBalance(id_); }

  float PlayerGetDownloadProgress() {
    return Internal::PlayerGetDownloadProgress(id_);
  }

  bool IsAutoplay() { return Internal::PlayerIsAutoplay(id_); }

  bool IsLooping() { return Internal::PlayerIsLooping(id_); }

  bool IsAutoRepeat() { return Internal::PlayerIsAutoRepeat(id_); }

  bool IsShuffling() { return Internal::PlayerIsShuffling(id_); }

  void Dispose() { Internal::PlayerDispose(id_); }

 private:
  int32_t id_;
  std::unique_ptr<PlayerEvents> events_;
};
}  // namespace lwm

#ifdef __cplusplus
}
#endif
#endif
