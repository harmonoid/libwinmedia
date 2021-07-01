#include <cstdint>

#ifdef _WIN32
#define EXPORT __declspec(dllexport)
#else
#define EXPORT
#endif

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

    EXPORT int32_t Player_create();

    EXPORT void Player_dispose(int32_t id);

    EXPORT void Player_open(int32_t id, int32_t mediaId);

    EXPORT void Player_play(int32_t id);

    EXPORT void Player_pause(int32_t id);

    EXPORT void Player_setVolume(int32_t id, float volume);

    EXPORT void Player_setRate(int32_t id, float rate);

    EXPORT int32_t Player_getRate(int32_t id);

    EXPORT int32_t Player_getPosition(int32_t id);

    EXPORT float Player_getVolume(int32_t id);

    EXPORT bool Player_isAutoplay(int32_t id);

    EXPORT bool Player_isLooping(int32_t id);

    EXPORT void Player_setVolumeEventHandler(int32_t id, void (*callback)(float volume));

    EXPORT void Player_setRateEventHandler(int32_t id, void (*callback)(float rate));

    EXPORT void Player_NativeControls_create(int32_t id, void (*callback)(int32_t button));

    EXPORT void Player_NativeControls_setStatus(int32_t id, int32_t status);

    EXPORT void Player_NativeControls_update(int32_t id, int32_t type, const wchar_t** data, const wchar_t* thumbnail);

    EXPORT void Player_NativeControls_clear(int32_t id);

    EXPORT void Player_NativeControls_dispose(int32_t id);

    EXPORT int32_t Media_create(const wchar_t* uri, bool parse = false);

    EXPORT void Media_dispose(int32_t id);

    EXPORT int32_t Media_getDuration(int32_t id);

    EXPORT wchar_t** Tags_fromMusic(const wchar_t* uri);

    EXPORT wchar_t** Tags_fromVideo(const wchar_t* uri);

    EXPORT void Tags_extractThumbnail(const wchar_t* source, const wchar_t* save, const wchar_t* fileName, int32_t mode, int32_t size);

    EXPORT void Media_setParseEventHandler(int32_t id, void (*callback)(int32_t parse));

    EXPORT void NativeControls_create(void (*callback)(int32_t button));

    EXPORT void NativeControls_setStatus(int32_t state);

    EXPORT void NativeControls_update(int32_t type, const wchar_t** data, const wchar_t* thumbnail);

    EXPORT void NativeControls_clear();

    EXPORT void NativeControls_dispose();

}


#ifdef __cplusplus
}
#endif
#endif
