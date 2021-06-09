/***********************************************************************************
 * Project     : WinMediaLib                                                     *
 * Description : A media playback, metadata, recording & broadcast library in C++. *
 * License     : MIT                                                               *
 *                                                                                 *
 * Author      : Hitesh Kumar Saini                                                *
 * Email       : saini123hitesh@gmail.com; alexmercerind@gmail.com                 *
 * GitHub      : https://github.com/alexmercerind                                  *
 ***********************************************************************************/

#include <iostream>
#include <thread>
#include <vector>
#include <map>

#include <winrt/Windows.Foundation.h>
#include <winrt/Windows.Storage.Streams.h>
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
#ifndef WIN_MEDIA_LIB_DLL
#define WIN_MEDIA_LIB_DLL

using namespace winrt::Windows::Foundation;
using namespace winrt::Windows::Storage;
using namespace winrt::Windows::Media;
using namespace winrt::Windows::System;

std::vector<Playback::MediaPlayer> players;
std::vector<Core::MediaSource> medias;
bool systemMediaTransportControlsExist = false;

namespace Internal {
    
    /// Player

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

    EXPORT void Player_setPosition(int32_t id, int32_t position) {
        players[id].Position(
            TimeSpan(std::chrono::seconds(position))
        );
    }

    EXPORT void Player_setVolume(int32_t id, float volume) {
        players[id].Volume(volume);
    }
    
    EXPORT void Player_setRate(int32_t id, float rate) {
        players[id].PlaybackRate(rate);
    }

    EXPORT void Player_setAudioBalance(int32_t id, float audioBalance) {
        players[id].AudioBalance(audioBalance);
    }

    EXPORT void Player_setAutoplay(int32_t id, bool autoplay) {
        players[id].AutoPlay(autoplay);
    }

    EXPORT void Player_setIsLooping(int32_t id, bool looping) {
        players[id].IsLoopingEnabled(looping);
    }

    EXPORT int32_t Player_getPosition(int32_t id) {
        return players[id].Position().count();
    }

    EXPORT float Player_getVolume(int32_t id) {
        return players[id].Volume();
    }

    EXPORT float Player_getRate(int32_t id) {
        return players[id].PlaybackRate();
    }

    EXPORT float Player_getAudioBalance(int32_t id) {
        return players[id].AudioBalance();
    }

    EXPORT bool Player_getAutoplay(int32_t id) {
        return players[id].AutoPlay();
    }

    EXPORT bool Player_getIsLooping(int32_t id) {
        return players[id].IsLoopingEnabled();
    }

    EXPORT void Player_setVolumeEventHandler(int32_t id, void (*callback)(float volume)) {
        players[id].VolumeChanged(
            [=](auto, const auto& args) -> void {
                (*callback)(players[id].Volume());
            }
        );
    }

    EXPORT void Player_setRateEventHandler(int32_t id, void (*callback)(float rate)) {
        players[id].MediaPlayerRateChanged(
            [=](auto, const auto& args) -> void {
                (*callback)(players[id].PlaybackRate());
            }
        );
    }

    EXPORT void Player_SystemMediaTransportControls_create(int32_t id, void (*callback)(int32_t button)) {
        if (systemMediaTransportControlsExist) return;
        systemMediaTransportControlsExist = true;
        SystemMediaTransportControls controls = players[id].SystemMediaTransportControls();
        controls.IsEnabled(true);
        controls.IsPlayEnabled(true);
        controls.IsPauseEnabled(true);
        controls.IsNextEnabled(true);
        controls.IsPreviousEnabled(true);
        controls.ButtonPressed(
            [=](auto, const SystemMediaTransportControlsButtonPressedEventArgs& args) -> void {
                if (args.Button() == SystemMediaTransportControlsButton::Play) controls.PlaybackStatus(MediaPlaybackStatus::Playing);
                if (args.Button() == SystemMediaTransportControlsButton::Pause) controls.PlaybackStatus(MediaPlaybackStatus::Paused);
                (*callback)(static_cast<int>(args.Button()));
            }
        );
    }

    EXPORT void Player_SystemMediaTransportControls_setState(int32_t id, const bool* state) {
        SystemMediaTransportControls controls = players[id].SystemMediaTransportControls();
        controls.IsPlayEnabled(state[0]);
        controls.IsPauseEnabled(state[1]);
        controls.IsNextEnabled(state[2]);
        controls.IsPreviousEnabled(state[3]);
    }

    EXPORT void Player_SystemMediaTransportControls_update(int32_t id, int32_t type, const wchar_t** data, const wchar_t* thumbnail) {
        SystemMediaTransportControls controls = players[id].SystemMediaTransportControls();
        SystemMediaTransportControlsDisplayUpdater updater = controls.DisplayUpdater();
        updater.Type(static_cast<MediaPlaybackType>(type));
        if (type == 1) {
            MusicDisplayProperties properties = updater.MusicProperties();
            properties.AlbumArtist(data[0]);
            properties.AlbumTitle(data[1]);
            properties.AlbumTrackCount(std::stoi(data[2]));
            properties.Artist(data[3]);
            properties.Title(data[4]);
            properties.TrackNumber(std::stoi(data[5]));
        }
        else if (type == 2) {
            VideoDisplayProperties properties = updater.VideoProperties();
            properties.Title(data[0]);
            properties.Subtitle(data[1]);
        }
        if (std::wstring(thumbnail) != L"") {
            updater.Thumbnail(
                Streams::RandomAccessStreamReference::CreateFromFile(
                    StorageFile::GetFileFromPathAsync(thumbnail).get()
                )
            );
        }
        updater.Update();
    }

    EXPORT void Player_SystemMediaTransportControls_clear(int32_t id) {
        SystemMediaTransportControls controls = players[id].SystemMediaTransportControls();
        controls.DisplayUpdater().ClearAll();
    }

    EXPORT void Player_SystemMediaTransportControls_dispose(int32_t id) {
        players[id].SystemMediaTransportControls().IsEnabled(false);
    }

    /// Media

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

    EXPORT void Media_setParseEventHandler(int32_t id, void (*callback)(int32_t parse)) {
        medias[id].StateChanged(
            [=](auto, const Core::MediaSourceStateChangedEventArgs& args) -> void {
                (*callback)(static_cast<int32_t>(args.NewState()));
            }
        );
    }

    /// SystemMediaTransportControls

    EXPORT void SystemMediaTransportControls_create(void (*callback)(int32_t button)) {
        if (systemMediaTransportControlsExist) return;
        systemMediaTransportControlsExist = true;
        SystemMediaTransportControls controls = Playback::BackgroundMediaPlayer::Current().SystemMediaTransportControls();
        controls.IsEnabled(true);
        controls.IsPlayEnabled(true);
        controls.IsPauseEnabled(true);
        controls.IsNextEnabled(true);
        controls.IsPreviousEnabled(true);
        controls.ButtonPressed(
            [=](auto , const SystemMediaTransportControlsButtonPressedEventArgs& args) -> void {
                if (args.Button() == SystemMediaTransportControlsButton::Play) controls.PlaybackStatus(MediaPlaybackStatus::Playing);
                if (args.Button() == SystemMediaTransportControlsButton::Pause) controls.PlaybackStatus(MediaPlaybackStatus::Paused);
                (*callback)(static_cast<int>(args.Button()));
            }
        );
    }

    EXPORT void SystemMediaTransportControls_setState(const bool* state) {
        SystemMediaTransportControls controls = Playback::BackgroundMediaPlayer::Current().SystemMediaTransportControls();
        controls.IsPlayEnabled(state[0]);
        controls.IsPauseEnabled(state[1]);
        controls.IsNextEnabled(state[2]);
        controls.IsPreviousEnabled(state[3]);
    }

    EXPORT void SystemMediaTransportControls_update(int32_t type, const wchar_t** data, const wchar_t* thumbnail) {
        SystemMediaTransportControls controls = Playback::BackgroundMediaPlayer::Current().SystemMediaTransportControls();
        SystemMediaTransportControlsDisplayUpdater updater = controls.DisplayUpdater();
        updater.Type(static_cast<MediaPlaybackType>(type));
        if (type == 1) {
            MusicDisplayProperties properties = updater.MusicProperties();
            properties.AlbumArtist(data[0]);
            properties.AlbumTitle(data[1]);
            properties.AlbumTrackCount(std::stoi(data[2]));
            properties.Artist(data[3]);
            properties.Title(data[4]);
            properties.TrackNumber(std::stoi(data[5]));
        }
        else if (type == 2) {
            VideoDisplayProperties properties = updater.VideoProperties();
            properties.Title(data[0]);
            properties.Subtitle(data[1]);
        }
        if (std::wstring(thumbnail) != L"") {
            updater.Thumbnail(
                Streams::RandomAccessStreamReference::CreateFromFile(
                    StorageFile::GetFileFromPathAsync(thumbnail).get()
                )
            );
        }
        updater.Update();
    }

    EXPORT void SystemMediaTransportControls_clear() {
        SystemMediaTransportControls controls = Playback::BackgroundMediaPlayer::Current().SystemMediaTransportControls();
        controls.DisplayUpdater().ClearAll();
    }

    EXPORT void SystemMediaTransportControls_dispose() {
        Playback::BackgroundMediaPlayer::Current().SystemMediaTransportControls().IsEnabled(false);
    }
}

#endif
#ifdef __cplusplus
}
#endif
