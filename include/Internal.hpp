#include <cstdint>

#ifndef UNICODE
#define UNICODE
#define _UNICODE
#endif
#ifndef INTERNAL_HEADER
#define INTERNAL_HEADER
#ifdef __cplusplus
extern "C" {
#endif


namespace Internal {

    int32_t Player_create();

    void Player_dispose(int32_t id);

    void Player_open(int32_t id, int32_t mediaId);

    void Player_play(int32_t id);

    void Player_pause(int32_t id);

    void Player_setVolume(int32_t id, float volume);

    void Player_setRate(int32_t id, float rate);

    int32_t Player_getRate(int32_t id);

    int32_t Player_getPosition(int32_t id);

    float Player_getVolume(int32_t id);

    bool Player_isAutoplay(int32_t id);

    bool Player_isLooping(int32_t id);

    void Player_setVolumeEventHandler(int32_t id, void (*callback)(float volume));

    void Player_setRateEventHandler(int32_t id, void (*callback)(float rate));

    void Player_NativeControls_create(int32_t id, void (*callback)(int32_t button));

    void Player_NativeControls_setStatus(int32_t id, int32_t status);

    void Player_NativeControls_update(int32_t id, int32_t type, const wchar_t** data, const wchar_t* thumbnail);

    void Player_NativeControls_clear(int32_t id);

    void Player_NativeControls_dispose(int32_t id);

    int32_t Media_create(const wchar_t* uri, bool parse = false);

    void Media_dispose(int32_t id);

    int32_t Media_getDuration(int32_t id);

    wchar_t** Tags_fromMusic(const wchar_t* uri);

    wchar_t** Tags_fromVideo(const wchar_t* uri);

    void Tags_extractThumbnail(const wchar_t* source, const wchar_t* save, const wchar_t* fileName, int32_t mode, int32_t size);

    void Media_setParseEventHandler(int32_t id, void (*callback)(int32_t parse));

    void NativeControls_create(void (*callback)(int32_t button));

    void NativeControls_setStatus(int32_t state);

    void NativeControls_update(int32_t type, const wchar_t** data, const wchar_t* thumbnail);

    void NativeControls_clear();

    void NativeControls_dispose();

}


#ifdef __cplusplus
}
#endif
#endif
