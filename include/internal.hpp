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

    EXPORT void Player_create(int32_t id, bool showVideo = false);

    EXPORT void Player_dispose(int32_t id);

    EXPORT void Player_open(int32_t id, int32_t mediaId);

    EXPORT void Player_play(int32_t id);

    EXPORT void Player_pause(int32_t id);

    EXPORT void Player_setPosition(int32_t id, int32_t position);

    EXPORT void Player_setVolume(int32_t id, float volume);

    EXPORT void Player_setAudioBalance(int32_t id, float audioBalance);

    EXPORT void Player_setAutoplay(int32_t id, bool autoplay);

    EXPORT void Player_setIsLooping(int32_t id, bool looping);

    EXPORT void Player_setRate(int32_t id, float rate);

    EXPORT int32_t Player_getPosition(int32_t id);

    EXPORT float Player_getVolume(int32_t id);

    EXPORT int32_t Player_getRate(int32_t id);

    EXPORT float Player_getAudioBalance(int32_t id);

    EXPORT bool Player_isAutoplay(int32_t id);

    EXPORT bool Player_isLooping(int32_t id);

    EXPORT void Player_setIsPlayingEventHandler(int32_t id, void (*callback)(bool isPlaying));

    EXPORT void Player_setIsCompletedEventHandler(int32_t id, void (*callback)(bool isCompleted));

    EXPORT void Player_setIsBufferingEventHandler(int32_t id, void (*callback)(bool isBuffering));

    EXPORT void Player_setVolumeEventHandler(int32_t id, void (*callback)(float volume));

    EXPORT void Player_setRateEventHandler(int32_t id, void (*callback)(float rate));

    EXPORT void Player_setPositionEventHandler(int32_t id, void (*callback)(int32_t position));

    EXPORT void Player_setDurationEventHandler(int32_t id, void (*callback)(int32_t duration));
    
    EXPORT void Player_NativeControls_create(int32_t id, void (*callback)(int32_t button));

    EXPORT void Player_NativeControls_setStatus(int32_t id, int32_t status);

    EXPORT void Player_NativeControls_update(int32_t id, int32_t type, wchar_t** data, const wchar_t* thumbnail);

    EXPORT void Player_NativeControls_clear(int32_t id);

    EXPORT void Player_NativeControls_dispose(int32_t id);

    EXPORT void Media_create(int32_t id, const wchar_t* uri, bool parse = false);

    EXPORT void Media_dispose(int32_t id);

    EXPORT int32_t Media_getDuration(int32_t id);

    EXPORT wchar_t** Tags_fromMusic(const wchar_t* uri);

    EXPORT wchar_t** Tags_fromVideo(const wchar_t* uri);

    EXPORT void Tags_extractThumbnail(const wchar_t* source, const wchar_t* save, const wchar_t* fileName, int32_t mode, int32_t size);

    EXPORT void NativeControls_create(void (*callback)(int32_t button));

    EXPORT void NativeControls_setStatus(int32_t state);

    EXPORT void NativeControls_update(int32_t type, wchar_t** data, const wchar_t* thumbnail);

    EXPORT void NativeControls_clear();

    EXPORT void NativeControls_dispose();

}


#ifdef __cplusplus
}
#endif
#endif
