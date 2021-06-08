/**********************************************************************
 * Project     : WinMediaLib                                          *
 * Description : A media playback library in C++.                     *
 * License     : MIT                                                  *
 *                                                                    *
 * Author      : Hitesh Kumar Saini                                   *
 * Email       : alexmercerind@gmail.com; saini123hitesh@gmail.com    *
 * GitHub      : https://github.com/alexmercerind                     *
 **********************************************************************/

#include <iostream>

#ifndef UNICODE
#define UNICODE
#define _UNICODE
#endif
#ifndef WIN_MEDIA_LIB
#define WIN_MEDIA_LIB
#ifdef __cplusplus
extern "C" {
#endif


namespace WinMediaLibInternal {
    int Player_create();

    void Player_dispose(int32_t id);

    void Player_open(int32_t id, int32_t mediaId);

    void Player_play(int32_t id);

    void Player_pause(int32_t id);

    void Player_setVolume(int32_t id, float volume);

    void Player_setRate(int32_t id, float rate);

    int32_t Player_getVolume(int32_t id);

    int32_t Player_getRate(int32_t id);

    void Player_setVolumeEventHandler(int32_t id, void (*callback)(float event));

    void Player_setRateEventHandler(int32_t id, void (*callback)(float event));

    int Media_create(const wchar_t* uri, bool parse = false);

    void Media_dispose(int32_t id);

    int32_t Media_getDuration(int32_t id);

    void Media_setEventHandler(int32_t id, void (*callback)(int32_t event));
}


#ifdef __cplusplus
}
#endif
#endif
