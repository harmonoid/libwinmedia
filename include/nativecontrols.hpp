#include <cstring>
#include <functional>

#include "internal.hpp"

#ifndef NATIVECONTROLS_HEADER
#define NATIVECONTROLS_HEADER
#ifdef __cplusplus
extern "C" {
#endif

namespace lwm {

class NativeControls;

static bool g_native_controls_exist;

enum NativeControlsButton {
  Play,
  Pause,
  Stop,
  Record,
  FastForward,
  Rewind,
  Next,
  Previous,
  ChannelUp,
  ChannelDown
};

enum NativeControlsStatus {
  Closed,
  Changing,
  Stopped,
  Playing,
  Paused,
};

class NativeControlsState {
 public:
  NativeControlsState() {}

  char** data() const { return data_.get(); }

  virtual int32_t Type() const = 0;

 protected:
  std::unique_ptr<char*[]> data_;
};

class MusicNativeControlState : public NativeControlsState {
 public:
  MusicNativeControlState(std::string album_artist, std::string album,
                          std::string track_count, std::string artist,
                          std::string title, std::string track_number) {
    data_ = std::unique_ptr<char*[]>(new char*[6]);
    for (int32_t i = 0; i < 6; i++) data_[i] = new char[200];
    strncpy(data_[0], album_artist.c_str(), 200);
    strncpy(data_[1], album.c_str(), 200);
    strncpy(data_[2], track_count.c_str(), 200);
    strncpy(data_[3], artist.c_str(), 200);
    strncpy(data_[4], title.c_str(), 200);
    strncpy(data_[5], track_number.c_str(), 200);
  }

  int32_t Type() const override { return 1; }

  MusicNativeControlState() {
    for (int32_t i = 0; i < 6; i++) delete data_[i];
  }
};

class VideoNativeControlState : public NativeControlsState {
 public:
  VideoNativeControlState(std::string title, std::string subtitle) {
    data_ = std::unique_ptr<char*[]>(new char*[2]);
    for (int32_t i = 0; i < 2; i++) data_[i] = new char[200];
    strncpy(data_[0], title.c_str(), 200);
    strncpy(data_[1], subtitle.c_str(), 200);
  }

  int32_t Type() const override { return 2; }

  VideoNativeControlState() {
    for (int32_t i = 0; i < 2; i++) delete data_[i];
  }
};

class NativeControls {
 public:
  NativeControls(std::function<void(NativeControlsButton)> listener) {
    if (!g_native_controls_exist) {
      NativeControls::handler = listener;
      Internal::NativeControlsCreate(&NativeControls::_handler);
      g_native_controls_exist = true;
    }
  }

  void Update(std::shared_ptr<NativeControlsState> state,
              std::string thumbnail = "") {
    Internal::NativeControlsUpdate(state->Type(), state->data(),
                                   thumbnail.c_str());
  }

  void SetStatus(NativeControlsStatus status) {
    Internal::NativeControlsSetStatus(static_cast<int>(status));
  }

  void Clear() { Internal::NativeControlsClear(); }

  void Dispose() { Internal::NativeControlsDispose(); }

 private:
  static inline std::function<void(NativeControlsButton)> handler;

  static inline void _handler(int32_t button) {
    NativeControls::handler(static_cast<NativeControlsButton>(button));
  }
};
}  // namespace lwm

#ifdef __cplusplus
}
#endif
#endif
