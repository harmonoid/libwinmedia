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


static bool nativeControlsExist;


enum NativeControlsButton {
    play,
    pause,
    stop,
    record,
    fastForward,
    rewind,
    next,
    previous,
    channelUp,
    channelDown
};


enum NativeControlsStatus {
    closed,
    changing,
    stopped,
    playing,
    paused,
};


class NativeControlsState {
public:
    NativeControlsState() {}

    wchar_t** data = nullptr;
    virtual int32_t type() = 0;
};


class MusicNativeControls: public NativeControlsState {
public:
    MusicNativeControls(
        std::wstring albumArtist,
        std::wstring album,
        std::wstring trackCount,
        std::wstring artist,
        std::wstring title,
        std::wstring trackNumber
    ) {
        this->data = new wchar_t*[6];
        for (int32_t i = 0; i < 6; i++) this->data[i] = new wchar_t[200];
        wcscpy_s(this->data[0], 200, albumArtist.data());
        wcscpy_s(this->data[1], 200, album.data());
        wcscpy_s(this->data[2], 200, trackCount.data());
        wcscpy_s(this->data[3], 200, artist.data());
        wcscpy_s(this->data[4], 200, title.data());
        wcscpy_s(this->data[5], 200, trackNumber.data());
    }

    ~MusicNativeControls() {
        for (int32_t i = 0; i < 6; i++) delete this->data[i];
        delete this->data;
    }

    int32_t type() override {
        return 1;
    }
};


class VideoNativeControls: public NativeControlsState {
public:
    VideoNativeControls(
        std::wstring title,
        std::wstring subtitle
    ) {
        this->data = new wchar_t*[2];
        for (int32_t i = 0; i < 2; i++) this->data[i] = new wchar_t[200];
        wcscpy_s(this->data[0], 200, title.data());
        wcscpy_s(this->data[1], 200, subtitle.data());
    }

    ~VideoNativeControls() {
        for (int32_t i = 0; i < 2; i++) delete this->data[i];
        delete this->data;
    }

    int32_t type() override {
        return 2;
    }
};


class NativeControls {
public:

    NativeControls(std::function<void(NativeControlsButton)> handler) {
        if (!nativeControlsExist) {
            NativeControls::handler = handler;
            Internal::NativeControls_create(
                &NativeControls::_handler
            );
            nativeControlsExist = true;
        }
    }

    void update(NativeControlsState* state, std::wstring thumbnail = L"") {
        Internal::NativeControls_update(
            state->type(),
            state->data,
            thumbnail.data()
        );
        delete state;
    }

    void setStatus(NativeControlsStatus status) {
        Internal::NativeControls_setStatus(
            static_cast<int>(status)
        );
    }

    void clear() {
        Internal::NativeControls_clear();
    }

    void dispose() {
        Internal::NativeControls_dispose();
    }

private:
    static inline std::function<void(NativeControlsButton)> handler;

    static inline void _handler(int32_t button) {
        NativeControls::handler(
            static_cast<NativeControlsButton>(button)
        );
    }
};


}


#ifdef __cplusplus
}
#endif
#endif
