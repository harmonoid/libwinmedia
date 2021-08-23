#include <functional>
#include <cwchar>

#include "Internal.hpp"

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

  wchar_t** data() const { return data_.get(); }

  virtual int32_t Type() const = 0;

 protected:
  std::unique_ptr<wchar_t* []> data_;
};

class MusicNativeControlState : public NativeControlsState {
 public:
  MusicNativeControlState(std::wstring album_artist, std::wstring album,
                          std::wstring track_count, std::wstring artist,
                          std::wstring title, std::wstring track_number) {
    data_ = std::unique_ptr<wchar_t* []>(new wchar_t*[6]);
    for (int32_t i = 0; i < 6; i++) data_[i] = new wchar_t[200];
    wcscpy_s(data_[0], 200, album_artist.c_str());
    wcscpy_s(data_[1], 200, album.c_str());
    wcscpy_s(data_[2], 200, track_count.c_str());
    wcscpy_s(data_[3], 200, artist.c_str());
    wcscpy_s(data_[4], 200, title.c_str());
    wcscpy_s(data_[5], 200, track_number.c_str());
  }

  int32_t Type() const override { return 1; }

  MusicNativeControlState() {
    for (int32_t i = 0; i < 6; i++) delete data_[i];
  }
};

class VideoNativeControlState : public NativeControlsState {
 public:
  VideoNativeControlState(std::wstring title, std::wstring subtitle) {
    data_ = std::unique_ptr<wchar_t* []>(new wchar_t*[2]);
    for (int32_t i = 0; i < 2; i++) data_[i] = new wchar_t[200];
    wcscpy_s(data_[0], 200, title.c_str());
    wcscpy_s(data_[1], 200, subtitle.c_str());
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
              std::wstring thumbnail = L"") {
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
}

#ifdef __cplusplus
}
#endif
#endif
