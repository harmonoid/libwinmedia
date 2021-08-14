#include <memory>
#include <cstdint>

#ifdef _WIN32
#define DLLEXPORT __declspec(dllexport)
#else
#define EXPORT
#endif
#define VIDEO_WINDOW_CLASS L"libwinmedia"
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

DLLEXPORT void PlayerCreate(int32_t player_id, bool show_window = false,
                            const wchar_t* window_title = VIDEO_WINDOW_CLASS);

DLLEXPORT void PlayerDispose(int32_t player_id);

DLLEXPORT void PlayerCloseWindow(int32_t player_id);

DLLEXPORT void PlayerOpen(int32_t player_id, int32_t media_id);

DLLEXPORT void PlayerPlay(int32_t player_id);

DLLEXPORT void PlayerPause(int32_t player_id);

DLLEXPORT void PlayerSeek(int32_t player_id, int32_t position);

DLLEXPORT void PlayerSetVolume(int32_t player_id, float volume);

DLLEXPORT void PlayerSetRate(int32_t player_id, float rate);

DLLEXPORT void PlayerSetAudioBalance(int32_t player_id, float audio_balance);

DLLEXPORT void PlayerSetAutoplay(int32_t player_id, bool autoplay);

DLLEXPORT void PlayerSetIsLooping(int32_t player_id, bool looping);

DLLEXPORT int32_t PlayerGetPosition(int32_t player_id);

DLLEXPORT float PlayerGetVolume(int32_t player_id);

DLLEXPORT float PlayerGetRate(int32_t player_id);

DLLEXPORT float PlayerGetAudioBalance(int32_t player_id);

DLLEXPORT bool PlayerIsAutoplay(int32_t player_id);

DLLEXPORT bool PlayerIsLooping(int32_t player_id);

DLLEXPORT void PlayerSetIsPlayingEventHandler(
    int32_t player_id, void (*callback)(bool is_playing));

DLLEXPORT void PlayerSetIsCompletedEventHandler(
    int32_t player_id, void (*callback)(bool is_completed));

DLLEXPORT void PlayerSetIsBufferingEventHandler(
    int32_t player_id, void (*callback)(bool is_buffering));

DLLEXPORT void PlayerSetVolumeEventHandler(int32_t player_id,
                                           void (*callback)(float volume));

DLLEXPORT void PlayerSetRateEventHandler(int32_t player_id,
                                         void (*callback)(float rate));

DLLEXPORT void PlayerSetPositionEventHandler(
    int32_t player_id, void (*callback)(int32_t position));

DLLEXPORT void PlayerSetDurationEventHandler(
    int32_t player_id, void (*callback)(int32_t duration));

DLLEXPORT void PlayerNativeControlsCreate(int32_t player_id,
                                          void (*callback)(int32_t button));

DLLEXPORT void PlayerNativeControlsSetStatus(int32_t player_id, int32_t status);

DLLEXPORT void PlayerNativeControlsUpdate(int32_t player_id, int32_t type,
                                          wchar_t** data,
                                          const wchar_t* thumbnail);

DLLEXPORT void PlayerNativeControlsClear(int32_t player_id);

DLLEXPORT void PlayerNativeControlsDispose(int32_t player_id);

DLLEXPORT void MediaCreate(int32_t media_id, const wchar_t* uri,
                           bool parse = false);

DLLEXPORT void MediaDispose(int32_t media_id);

DLLEXPORT int32_t MediaGetDuration(int32_t media_id);

DLLEXPORT wchar_t** TagsFromMusic(const wchar_t* uri);

DLLEXPORT wchar_t** TagsFromVideo(const wchar_t* uri);

DLLEXPORT void TagsExtractThumbnail(const wchar_t* media, const wchar_t* folder,
                                    const wchar_t* file_name, int32_t mode,
                                    int32_t size);

DLLEXPORT void NativeControlsCreate(void (*callback)(int32_t button));

DLLEXPORT void NativeControlsSetStatus(int32_t status);

DLLEXPORT void NativeControlsUpdate(int32_t type, wchar_t** data,
                                    const wchar_t* thumbnail);

DLLEXPORT void NativeControlsClear();

DLLEXPORT void NativeControlsDispose();
}

#ifdef __cplusplus
}
#endif
#endif
