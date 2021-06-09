/***********************************************************************************
 * Project     : WinMediaLib                                                       *
 * Description : A media playback, metadata, recording & broadcast library in C++. *
 * License     : MIT                                                               *
 *                                                                                 *
 * Author      : Hitesh Kumar Saini                                                *
 * Email       : saini123hitesh@gmail.com; alexmercerind@gmail.com                 *
 * GitHub      : https://github.com/alexmercerind                                  *
 ***********************************************************************************/

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

    int Player_create();

    void Player_dispose(int32_t id);

    void Player_open(int32_t id, int32_t mediaId);

    void Player_play(int32_t id);

    void Player_pause(int32_t id);

    void Player_setVolume(int32_t id, float volume);

    void Player_setRate(int32_t id, float rate);

    int32_t Player_getVolume(int32_t id);

    int32_t Player_getRate(int32_t id);

    int32_t Player_getPosition(int32_t id);

    int32_t Player_getVolume(int32_t id);

    bool Player_getAutoplay(int32_t id);

    bool Player_getIsLooping(int32_t id);

    void Player_setVolumeEventHandler(int32_t id, void (*callback)(float volume));

    void Player_setRateEventHandler(int32_t id, void (*callback)(float rate));

    void Player_SystemMediaTransportControls_create(int32_t id, void (*callback)(int32_t button));

    void Player_SystemMediaTransportControls_setState(int32_t id, const bool* state);

    void Player_SystemMediaTransportControls_update(int32_t id, int32_t type, const wchar_t** data, const wchar_t* thumbnail);

    void Player_SystemMediaTransportControls_clear(int32_t id);

    void Player_SystemMediaTransportControls_dispose(int32_t id);

    int32_t Media_create(const wchar_t* uri, bool parse = false);

    void Media_dispose(int32_t id);

    int32_t Media_getDuration(int32_t id);

    void Media_setParseEventHandler(int32_t id, void (*callback)(int32_t parse));

    void SystemMediaTransportControls_create(void (*callback)(int32_t button));

    void SystemMediaTransportControls_setState(const bool* state);

    void SystemMediaTransportControls_update(int32_t type, const wchar_t** data, const wchar_t* thumbnail);

    void SystemMediaTransportControls_clear();

    void SystemMediaTransportControls_dispose();
}


#ifdef __cplusplus
}
#endif
#endif
