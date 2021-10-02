#include <cstdint>
#include <memory>

#define VIDEO_WINDOW_CLASS "libwinmedia"
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

void PlayerShowWindow(int32_t player_id,
                      const char* window_title = VIDEO_WINDOW_CLASS);

void PlayerCloseWindow(int32_t player_id);

void PlayerCreate(int32_t player_id, bool show_window = false,
                  const char* window_title = VIDEO_WINDOW_CLASS);

void PlayerDispose(int32_t player_id);

void PlayerOpen(int32_t player_id, int32_t size, const char** uris,
                const int32_t* ids);

void PlayerPlay(int32_t player_id);

void PlayerPause(int32_t player_id);

void PlayerAdd(int32_t player_id, const char* uri, int32_t id);

void PlayerRemove(int32_t player_id, int32_t index);

void PlayerNext(int32_t player_id);

void PlayerBack(int32_t player_id);

void PlayerJump(int32_t player_id, int32_t index);

void PlayerSeek(int32_t player_id, int32_t position);

void PlayerSetVolume(int32_t player_id, float volume);

void PlayerSetRate(int32_t player_id, float rate);

void PlayerSetAudioBalance(int32_t player_id, float audio_balance);

void PlayerSetAutoplay(int32_t player_id, bool autoplay);

void PlayerSetIsLooping(int32_t player_id, bool looping);

void PlayerSetIsAutoRepeat(int32_t player_id, bool enabled);

void PlayerSetIsShuffling(int32_t player_id, bool shuffling);

int32_t PlayerGetPosition(int32_t player_id);

float PlayerGetVolume(int32_t player_id);

float PlayerGetRate(int32_t player_id);

float PlayerGetAudioBalance(int32_t player_id);

float PlayerGetDownloadProgress(int32_t player_id);

bool PlayerIsAutoplay(int32_t player_id);

bool PlayerIsLooping(int32_t player_id);

bool PlayerIsAutoRepeat(int32_t player_id);

bool PlayerIsShuffling(int32_t player_id);

void PlayerSetIsPlayingEventHandler(int32_t player_id,
                                    void (*callback)(bool is_playing));

void PlayerSetIsCompletedEventHandler(int32_t player_id,
                                      void (*callback)(bool is_completed));

void PlayerSetIsBufferingEventHandler(int32_t player_id,
                                      void (*callback)(bool is_buffering));

void PlayerSetVolumeEventHandler(int32_t player_id,
                                 void (*callback)(float volume));

void PlayerSetRateEventHandler(int32_t player_id, void (*callback)(float rate));

void PlayerSetPositionEventHandler(int32_t player_id,
                                   void (*callback)(int32_t position));

void PlayerSetDurationEventHandler(int32_t player_id,
                                   void (*callback)(int32_t duration));

void PlayerSetIndexEventHandler(int32_t player_id,
                                void (*callback)(int32_t index));

void PlayerSetDownloadProgressEventHandler(
    int32_t player_id, void (*callback)(float download_progress));

void PlayerSetErrorEventHandler(int32_t player_id,
                                void (*callback)(int32_t code, char* message));

void PlayerNativeControlsCreate(int32_t player_id,
                                void (*callback)(int32_t button));

void PlayerNativeControlsSetStatus(int32_t player_id, int32_t status);

void PlayerNativeControlsUpdate(int32_t player_id, int32_t type, char** data,
                                const char* thumbnail);

void PlayerNativeControlsClear(int32_t player_id);

void PlayerNativeControlsDispose(int32_t player_id);

void MediaCreate(int32_t media_id, const char* uri, bool parse = false);

void MediaDispose(int32_t media_id);

int32_t MediaGetDuration(int32_t media_id);

char** TagsFromMusic(const char* uri);

char** TagsFromVideo(const char* uri);

void TagsExtractThumbnail(const char* media, const char* folder,
                          const char* file_name, int32_t mode, int32_t size);

void NativeControlsCreate(void (*callback)(int32_t button));

void NativeControlsSetStatus(int32_t status);

void NativeControlsUpdate(int32_t type, char** data, const char* thumbnail);

void NativeControlsClear();

void NativeControlsDispose();

#ifdef __linux__
void PlayerRun();
#endif
}  // namespace Internal

#ifdef __cplusplus
}
#endif
#endif
