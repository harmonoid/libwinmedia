#include <iostream>
#include <thread>
#include <string>
#include <cwchar>
#include <vector>
#include <map>

#include <winrt/Windows.Foundation.h>
#include <winrt/Windows.Foundation.Collections.h>
#include <winrt/Windows.Storage.FileProperties.h>
#include <winrt/Windows.Storage.Streams.h>
#include <winrt/Windows.Media.Core.h>
#include <winrt/Windows.Media.Playback.h>
#include <winrt/Windows.Media.Playlists.h>
#include <winrt/Windows.Media.Audio.h>
#include <winrt/Windows.System.h>

#define TAG_SIZE 200
#define TO_MILLISECONDS(timespan) timespan.count() / 10000

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

static std::vector<Playback::MediaPlayer> players;
static std::vector<Core::MediaSource> medias;
static bool systemMediaTransportControlsExist;

namespace Internal {
    
    /* Player */

    EXPORT int32_t Player_create() {
        int32_t id = players.size();
        Playback::MediaPlayer player = Playback::MediaPlayer();
        players.emplace_back(player);
        players[id].SystemMediaTransportControls().IsEnabled(false);
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
        return TO_MILLISECONDS(players[id].Position());
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

    EXPORT bool Player_isAutoplay(int32_t id) {
        return players[id].AutoPlay();
    }

    EXPORT bool Player_isLooping(int32_t id) {
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

    EXPORT void Player_NativeControls_create(int32_t id, void (*callback)(int32_t button)) {
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

    EXPORT void Player_NativeControls_setStatus(int32_t id, int32_t status) {
        SystemMediaTransportControls controls = Playback::BackgroundMediaPlayer::Current().SystemMediaTransportControls();
        controls.PlaybackStatus(
            static_cast<MediaPlaybackStatus>(status)
        );
    }

    EXPORT void Player_NativeControls_update(int32_t id, int32_t type, const wchar_t** data, const wchar_t* thumbnail) {
        SystemMediaTransportControls controls = players[id].SystemMediaTransportControls();
        SystemMediaTransportControlsDisplayUpdater updater = controls.DisplayUpdater();
        updater.Type(static_cast<MediaPlaybackType>(type));
        if (type == 0) {
            MusicDisplayProperties properties = updater.MusicProperties();
            properties.AlbumArtist(data[0]);
            properties.AlbumTitle(data[1]);
            properties.AlbumTrackCount(std::stoi(data[2]));
            properties.Artist(data[3]);
            properties.Title(data[4]);
            properties.TrackNumber(std::stoi(data[5]));
        }
        else if (type == 1) {
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

    EXPORT void Player_NativeControls_clear(int32_t id) {
        SystemMediaTransportControls controls = players[id].SystemMediaTransportControls();
        controls.DisplayUpdater().ClearAll();
    }

    EXPORT void Player_NativeControls_dispose(int32_t id) {
        players[id].SystemMediaTransportControls().IsEnabled(false);
    }

    /* Media */

    EXPORT int32_t Media_create(const wchar_t* uri, bool parse = false) {
        int32_t id = medias.size();
        Core::MediaSource media = Core::MediaSource::CreateFromUri(
            Uri(uri)
        );
        if (parse) media.OpenAsync().get();
        medias.emplace_back(media);
        return id;
    }

    EXPORT void Media_dispose(int32_t id) {
        return medias[id].Close();
    }

    EXPORT int32_t Media_getDuration(int32_t id) {
        return TO_MILLISECONDS(medias[id].Duration().Value());
    }

    /// Tags

    EXPORT wchar_t** Tags_fromMusic(const wchar_t* uri) {
        FileProperties::StorageItemContentProperties properties = StorageFile::GetFileFromPathAsync(uri).get().Properties();
        FileProperties::MusicProperties music = properties.GetMusicPropertiesAsync().get();
        std::wstring string = L"";
        wchar_t** tags = new wchar_t*[15];
        for (int32_t index = 0; index < 15; index++) tags[index] = new wchar_t[TAG_SIZE];
        wcscpy_s(tags[0], TAG_SIZE, music.Album().data());
        wcscpy_s(tags[1], TAG_SIZE, music.AlbumArtist().data());
        wcscpy_s(tags[2], TAG_SIZE, std::to_wstring(music.Bitrate()).data());
        Collections::IVector<winrt::hstring> composers = music.Composers();
        for (int32_t index = 0; index < composers.Size(); index++) {
            string += composers.GetAt(index).data();
            string += L" ";
        }
        wcscpy_s(tags[3], TAG_SIZE, string.data());
        string.clear();
        Collections::IVector<winrt::hstring> conductors = music.Conductors();
        for (int32_t index = 0; index < conductors.Size(); index++) {
            string += conductors.GetAt(index).data();
            string += L" ";
        }
        wcscpy_s(tags[4], TAG_SIZE, string.data());
        string.clear();
        wcscpy_s(tags[5], TAG_SIZE, std::to_wstring(TO_MILLISECONDS(music.Duration())).data());
        Collections::IVector<winrt::hstring> genre = music.Genre();
        for (int32_t index = 0; index < genre.Size(); index++) {
            string += genre.GetAt(index).data();
            string += L" ";
        }
        wcscpy_s(tags[6], TAG_SIZE, string.c_str());
        string.clear();
        Collections::IVector<winrt::hstring> producers = music.Producers();
        for (int32_t index = 0; index < producers.Size(); index++) {
            string += producers.GetAt(index).data();
            string += L" ";
        }
        wcscpy_s(tags[7], TAG_SIZE, string.c_str());
        string.clear();
        wcscpy_s(tags[8], TAG_SIZE, music.Publisher().data());
        wcscpy_s(tags[9], TAG_SIZE, std::to_wstring(music.Rating()).data());
        wcscpy_s(tags[10], TAG_SIZE, music.Subtitle().data());
        wcscpy_s(tags[11], TAG_SIZE, music.Title().data());
        wcscpy_s(tags[12], TAG_SIZE, std::to_wstring(music.TrackNumber()).data());
        Collections::IVector<winrt::hstring> writers = music.Writers();
        for (int32_t index = 0; index < writers.Size(); index++) {
            string += writers.GetAt(index).data();
            string += L" ";
        }
        wcscpy_s(tags[13], TAG_SIZE, string.data());
        string.clear();
        wcscpy(tags[14], std::to_wstring(music.Year()).data());
        return tags;
    }

    EXPORT wchar_t** Tags_fromVideo(const wchar_t* uri) {
        FileProperties::StorageItemContentProperties properties = StorageFile::GetFileFromPathAsync(uri).get().Properties();
        FileProperties::VideoProperties video = properties.GetVideoPropertiesAsync().get();
        std::wstring string = L"";
        wchar_t** tags = new wchar_t* [16];
        for (int32_t index = 0; index < 16; index++) tags[index] = new wchar_t[TAG_SIZE];
        wcscpy_s(tags[0], TAG_SIZE, std::to_wstring(video.Bitrate()).data());
        Collections::IVector<winrt::hstring> directors = video.Directors();
        for (int32_t index = 0; index < directors.Size(); index++) {
            string += directors.GetAt(index).data();
            string += L" ";
        }
        wcscpy_s(tags[1], TAG_SIZE, string.data());
        string.clear();
        wcscpy_s(tags[2], TAG_SIZE, std::to_wstring(TO_MILLISECONDS(video.Duration())).data());
        wcscpy_s(tags[3], TAG_SIZE, std::to_wstring(video.Height()).data());
        Collections::IVector<winrt::hstring> keywords = video.Keywords();
        for (int32_t index = 0; index < keywords.Size(); index++) {
            string += keywords.GetAt(index).data();
            string += L" ";
        }
        wcscpy_s(tags[4], TAG_SIZE, string.data());
        string.clear();
        wcscpy_s(tags[5], TAG_SIZE, L"-1");
        wcscpy_s(tags[6], TAG_SIZE, L"-1");
        wcscpy_s(tags[7], TAG_SIZE, std::to_wstring(static_cast<uint32_t>(video.Orientation())).data());
        Collections::IVector<winrt::hstring> producers = video.Producers();
        for (int32_t index = 0; index < producers.Size(); index++) {
            string += producers.GetAt(index).data();
            string += L" ";
        }
        wcscpy_s(tags[8], TAG_SIZE, string.c_str());
        string.clear();
        wcscpy_s(tags[9], TAG_SIZE, video.Publisher().data());
        wcscpy_s(tags[10], TAG_SIZE, std::to_wstring(video.Rating()).data());
        wcscpy_s(tags[11], TAG_SIZE, video.Subtitle().data());
        wcscpy_s(tags[12], TAG_SIZE, video.Title().data());
        wcscpy_s(tags[13], TAG_SIZE, std::to_wstring(video.Width()).data());
        Collections::IVector<winrt::hstring> writers = video.Writers();
        for (int32_t index = 0; index < writers.Size(); index++) {
            string += writers.GetAt(index).data();
            string += L" ";
        }
        wcscpy_s(tags[14], TAG_SIZE, string.c_str());
        string.clear();
        wcscpy_s(tags[15], TAG_SIZE, std::to_wstring(video.Year()).data());
        return tags;
    }

    EXPORT void Tags_extractThumbnail(const wchar_t* source, const wchar_t* save, const wchar_t* fileName, int32_t mode, int32_t size) {
        StorageFile sourceFile = StorageFile::GetFileFromPathAsync(source).get();
        StorageFolder saveFolder = StorageFolder::GetFolderFromPathAsync(save).get();
        saveFolder.CreateFileAsync(fileName, CreationCollisionOption::ReplaceExisting).get();
        StorageFile saveFile = saveFolder.GetFileAsync(fileName).get();
        FileProperties::StorageItemThumbnail thumbnail = sourceFile.GetThumbnailAsync(
            static_cast<FileProperties::ThumbnailMode>(mode),
            size
        ).get();
        Streams::Buffer thumbnailBytes = Streams::Buffer(thumbnail.Size());
        thumbnail.ReadAsync(thumbnailBytes, thumbnail.Size(), Streams::InputStreamOptions::ReadAhead).get();
        FileIO::WriteBufferAsync(saveFile, thumbnailBytes).get();
    }

    /* NativeControls */

    EXPORT void NativeControls_create(void (*callback)(int32_t button)) {
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

    EXPORT void NativeControls_setStatus(int32_t status) {
        SystemMediaTransportControls controls = Playback::BackgroundMediaPlayer::Current().SystemMediaTransportControls();
        controls.PlaybackStatus(
            static_cast<MediaPlaybackStatus>(status)
        );
    }

    EXPORT void NativeControls_update(int32_t type, const wchar_t** data, const wchar_t* thumbnail) {
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

    EXPORT void NativeControls_clear() {
        SystemMediaTransportControls controls = Playback::BackgroundMediaPlayer::Current().SystemMediaTransportControls();
        controls.DisplayUpdater().ClearAll();
    }

    EXPORT void NativeControls_dispose() {
        Playback::BackgroundMediaPlayer::Current().SystemMediaTransportControls().IsEnabled(false);
    }
}

#endif
#ifdef __cplusplus
}
#endif
