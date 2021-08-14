/* TODO: Update `Player` class with event handlers. */

#include <functional>

#include "Internal.hpp"
#include "Media.hpp"

#ifndef PLAYER_HEADER
#define PLAYER_HEADER
#ifdef __cplusplus
extern "C" {
#endif

namespace lwm {

class Player;

class PlayerEvents {
 public:
  PlayerEvents(){};

  PlayerEvents(int32_t id) : id_(id) {}

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

 private:
  int32_t id_;

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

  friend class Player;
};

class Player {
 public:
  Player(int32_t id, bool show_window = false,
         std::wstring window_title = VIDEO_WINDOW_CLASS)
      : id_(id) {
    Internal::PlayerCreate(id, show_window, window_title.c_str());
    events_ = std::make_unique<PlayerEvents>(id_);
  }

  PlayerEvents* events() const { return events_.get(); }

  void Open(std::shared_ptr<Media> media) {
    Internal::PlayerOpen(id_, media->id());
  }

  void Play() { Internal::PlayerPlay(id_); }

  void Pause() { Internal::PlayerPause(id_); }

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

  void SetIsLooping(bool looping) { Internal::PlayerSetAutoplay(id_, looping); }

  int32_t GetPosition() { return Internal::PlayerGetPosition(id_); }

  float GetVolume() { return Internal::PlayerGetVolume(id_); }

  int32_t GetRate() { return Internal::PlayerGetRate(id_); }

  float GetAudioBalance() { return Internal::PlayerGetAudioBalance(id_); }

  bool IsAutoplay() { return Internal::PlayerIsAutoplay(id_); }

  float IsLooping() { return Internal::PlayerIsLooping(id_); }

  void Dispose() { Internal::PlayerDispose(id_); }

 private:
  int32_t id_;
  std::unique_ptr<PlayerEvents> events_;
};
}

#ifdef __cplusplus
}
#endif
#endif
