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
#include <vector>
#include <map>

#include <winrt/Windows.Foundation.h>
#include <winrt/Windows.Media.Core.h>
#include <winrt/Windows.Media.Playback.h>
#include <winrt/Windows.Media.Playlists.h>
#include <winrt/Windows.Media.Audio.h>
#include <winrt/Windows.System.h>

#ifdef _WIN32
#define EXPORT __declspec(dllexport)
#else
#define EXPORT
#endif
#ifdef __cplusplus
extern "C" {
#endif
#ifndef WIN_MEDIA_LIB
#define WIN_MEDIA_LIB

using namespace winrt::Windows::Foundation;
using namespace winrt::Windows::Media;
using namespace winrt::Windows::System;

std::vector<Playback::MediaPlayer> players;
std::vector<Core::MediaSource> medias;


namespace WinMediaLibInternal {
    
    EXPORT int32_t Player_create() {
        int32_t id = players.size();
        Playback::MediaPlayer player = Playback::MediaPlayer();
        players.emplace_back(player);
        return id;
    }

    EXPORT void Player_dispose(int32_t id) {
        players[id].Close();
    }

    EXPORT void Player_open(int32_t id, int32_t mediaId) {
        players[id].Source(
            Playback::MediaPlaybackItem(medias[mediaId])
        );
    }

    EXPORT void Player_play(int32_t id) {
        players[id].Play();
    }

    EXPORT void Player_pause(int32_t id) {
        players[id].Pause();
    }

    EXPORT void Player_setVolume(int32_t id, float volume) {
        players[id].Volume(volume);
    }
    
    EXPORT void Player_setRate(int32_t id, float rate) {
        players[id].PlaybackRate(rate);
    }

    EXPORT int32_t Player_getVolume(int32_t id) {
        return players[id].Volume();
    }

    EXPORT int32_t Player_getRate(int32_t id) {
        return players[id].PlaybackRate();
    }

    EXPORT void Player_setVolumeEventHandler(int32_t id, void (*callback)(float event)) {
        players[id].VolumeChanged(
            [=](auto, const auto& args) -> void {
                (*callback)(players[id].Volume());
            }
        );
    }

    EXPORT void Player_setRateEventHandler(int32_t id, void (*callback)(float event)) {
        players[id].MediaPlayerRateChanged(
            [=](auto, const auto& args) -> void {
                (*callback)(players[id].PlaybackRate());
            }
        );
    }

    EXPORT int32_t Media_create(const wchar_t* uri, bool parse = false) {
        int32_t id = medias.size();
        Core::MediaSource media = Core::MediaSource::CreateFromUri(
            Uri(uri)
        );
        if (parse) media.OpenAsync();
        medias.emplace_back(media);
        return id;
    }

    EXPORT void Media_dispose(int32_t id) {
        return medias[id].Close();
    }

    EXPORT int32_t Media_getDuration(int32_t id) {
        return medias[id].Duration().GetInt32();
    }

    EXPORT void Media_setEventHandler(int32_t id, void (*callback)(int32_t event)) {
        medias[id].StateChanged(
            [=](auto, const Core::MediaSourceStateChangedEventArgs& args) -> void {
                (*callback)(static_cast<int32_t>(args.NewState()));
            }
        );
    }
}

#endif
#ifdef __cplusplus
}
#endif
