/*
 * MIT License
 * 
 * Copyright (c) 2021 Hitesh Kumar Saini <saini123hitesh@gmail.com>
 * 
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

#define UNICODE
#define _UNICODE
/* Standard headers. */
#include <iostream>
#include <thread>
#include <cwchar>
#include <unordered_map>
/* win32 headers. */
#include <Windows.h>
/* C++/WinRT headers. */
#include <winrt/Windows.Foundation.h>
#include <winrt/Windows.Foundation.Collections.h>
#include <winrt/Windows.Storage.FileProperties.h>
#include <winrt/Windows.Storage.Streams.h>
#include <winrt/Windows.Media.Core.h>
#include <winrt/Windows.Media.Playback.h>
#include <winrt/Windows.Media.Playlists.h>
#include <winrt/Windows.Media.Audio.h>
#include <winrt/Windows.System.h>
#include <winrt/Windows.UI.Xaml.Hosting.h>
#include <winrt/Windows.UI.Xaml.Controls.h>
#include <winrt/Windows.Graphics.DirectX.h>
#include <winrt/Windows.Graphics.Imaging.h>
#include <winrt/Windows.Graphics.DirectX.Direct3D11.h>
#include <windows.ui.xaml.hosting.desktopwindowxamlsource.h>

#ifdef _WIN32
#define EXPORT __declspec(dllexport)
#else
#define EXPORT
#endif
#ifdef __cplusplus
extern "C" {
#endif
#ifndef LIBWINMEDIA
#define LIBWINMEDIA
/* TODO: Add ability to define lpClassName & lpWindowName. */
#define VIDEO_WINDOW_TITLE L"libwinmedia"
#define VIDEO_WINDOW_CLASS VIDEO_WINDOW_TITLE
/* Project specific macros. */
#define TAG_SIZE 200
#define TO_MILLISECONDS(timespan) timespan.count() / 10000

using namespace winrt::Windows::Foundation;
using namespace winrt::Windows::Storage;
using namespace winrt::Windows::Media;
using namespace winrt::Windows::System;
using namespace winrt::Windows::UI::Xaml;
using namespace winrt::Windows::Graphics::Imaging;
using namespace winrt::Windows::Graphics::DirectX::Direct3D11;

std::unordered_map<int32_t, Core::MediaSource> medias = {};
std::unordered_map<int32_t, Playback::MediaPlayer> players = {};

static bool SMTCExist = false;
/* Host win32 video window handle. */
static HWND window = nullptr;
/* XAML content video window handle. */
static HWND xaml = nullptr;

/* Video window procedure. */
LRESULT CALLBACK videoWindowProc(HWND, UINT, WPARAM, LPARAM);


namespace Internal {

    /* Player */

    EXPORT void Player_create(int32_t id, bool showVideo = false) {
        Playback::MediaPlayer player = Playback::MediaPlayer();
        players.insert(std::make_pair(id, player));
        players.at(id).SystemMediaTransportControls().IsEnabled(false);
        if (showVideo) {
            /* Thread creating HWND & XAML source for showing corresponding MediaPlayerElement. */
            /* TODO: Delete thread object if window is closed. */
            new std::thread(
                [=]() -> void {
                    STARTUPINFO startupInfo;
                    GetStartupInfo(&startupInfo);
                    WNDCLASSEXW windowClass;
                    SecureZeroMemory(&windowClass, sizeof(windowClass));
                    windowClass.cbSize = sizeof(windowClass);
                    windowClass.style = CS_HREDRAW | CS_VREDRAW;
                    windowClass.lpfnWndProc = videoWindowProc;
                    windowClass.hInstance = 0;
                    windowClass.lpszClassName = VIDEO_WINDOW_CLASS;
                    windowClass.hCursor = LoadCursorW(nullptr, IDC_ARROW);
                    windowClass.hbrBackground = reinterpret_cast<HBRUSH>(COLOR_WINDOW + 1);
                    RegisterClassExW(&windowClass);
                    window = CreateWindowExW(
                        0L,
                        VIDEO_WINDOW_CLASS,
                        VIDEO_WINDOW_TITLE,
                        WS_OVERLAPPEDWINDOW | WS_VISIBLE,
                        CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT,
                        nullptr,
                        nullptr,
                        0,
                        nullptr
                    );
                    if (!window) {
                        throw std::exception("COULD_NOT_CREATE_WIN32_WINDOW", -1);
                    }
                    winrt::init_apartment(winrt::apartment_type::multi_threaded);
                    const auto windowsXamlManager = Hosting::WindowsXamlManager::InitializeForCurrentThread();
                    Hosting::DesktopWindowXamlSource desktopWindowXamlSource = {};
                    const auto interop = desktopWindowXamlSource.as<IDesktopWindowXamlSourceNative>();
                    winrt::check_hresult(interop->AttachToWindow(window));
                    interop->get_WindowHandle(&xaml);
                    if (!xaml) {
                        throw std::exception("COULD_NOT_CREATE_XAML_SOURCE", -2);
                    }
                    RECT rect = { 0, 0, 0, 0 };
                    GetClientRect(window, &rect);
                    SetWindowPos(xaml, nullptr, 0, 0, rect.right, rect.bottom, SWP_SHOWWINDOW);
                    Controls::MediaPlayerElement playerElement = Controls::MediaPlayerElement();
                    playerElement.SetMediaPlayer(players.at(id));
                    desktopWindowXamlSource.Content(playerElement);
                    ShowWindow(xaml, startupInfo.wShowWindow);
                    UpdateWindow(xaml);
                    MSG msg = {};
                    while (GetMessageW(&msg, nullptr, 0, 0)) {
                        TranslateMessage(&msg);
                        DispatchMessageW(&msg);
                    }
                    /* TODO: Add ability to close the window. */
                }
            );
        }
    }

    EXPORT void Player_dispose(int32_t id) {
        players.at(id).Close();
    }

    EXPORT void Player_open(int32_t id, int32_t mediaId) {
        players.at(id).Source(
            Playback::MediaPlaybackItem(medias.at(mediaId))
        );
    }

    EXPORT void Player_play(int32_t id) {
        players.at(id).Play();
    }

    EXPORT void Player_pause(int32_t id) {
        players.at(id).Pause();
    }

    EXPORT void Player_setPosition(int32_t id, int32_t position) {
        players.at(id).Position(
            TimeSpan(std::chrono::seconds(position))
        );
    }

    EXPORT void Player_setVolume(int32_t id, float volume) {
        players.at(id).Volume(volume);
    }
    
    EXPORT void Player_setRate(int32_t id, float rate) {
        players.at(id).PlaybackRate(rate);
    }

    EXPORT void Player_setAudioBalance(int32_t id, float audioBalance) {
        players.at(id).AudioBalance(audioBalance);
    }

    EXPORT void Player_setAutoplay(int32_t id, bool autoplay) {
        players.at(id).AutoPlay(autoplay);
    }

    EXPORT void Player_setIsLooping(int32_t id, bool looping) {
        players.at(id).IsLoopingEnabled(looping);
    }

    EXPORT int32_t Player_getPosition(int32_t id) {
        return TO_MILLISECONDS(players.at(id).Position());
    }

    EXPORT float Player_getVolume(int32_t id) {
        return players.at(id).Volume();
    }

    EXPORT float Player_getRate(int32_t id) {
        return players.at(id).PlaybackRate();
    }

    EXPORT float Player_getAudioBalance(int32_t id) {
        return players.at(id).AudioBalance();
    }

    EXPORT bool Player_isAutoplay(int32_t id) {
        return players.at(id).AutoPlay();
    }

    EXPORT bool Player_isLooping(int32_t id) {
        return players.at(id).IsLoopingEnabled();
    }

    EXPORT void Player_setFrameEventHandler(int32_t id, int32_t width, int32_t height, void (*callback)(uint8_t* frame)) {
        IDirect3DSurface surface = IDirect3DSurface();
        Streams::IBuffer buffer = Streams::IBuffer();
        players.at(id).IsVideoFrameServerEnabled(true);
        players.at(id).VideoFrameAvailable(
            [=](auto, const auto& args) -> void {
                players.at(id).CopyFrameToVideoSurface(surface);
                SoftwareBitmap bitmap = SoftwareBitmap::CreateCopyFromSurfaceAsync(surface).get();
                bitmap.CopyToBuffer(buffer);
                (*callback)(buffer.data());
            }
        );
    }

    EXPORT void Player_setIsPlayingEventHandler(int32_t id, void (*callback)(bool isPlaying)) {
        players.at(id).PlaybackSession().PlaybackStateChanged(
            [=](auto, const auto& args) -> void {
                if (players.at(id).PlaybackSession().PlaybackState() == Playback::MediaPlaybackState::Playing) {
                    (*callback)(true);
                }
                if (players.at(id).PlaybackSession().PlaybackState() == Playback::MediaPlaybackState::Paused) {
                    (*callback)(false);
                }
            }
        );
    }

    EXPORT void Player_setIsCompletedEventHandler(int32_t id, void (*callback)(bool isCompleted)) {
        players.at(id).MediaEnded(
            [=](auto, const auto& args) -> void {
                (*callback)(true);
            }
        );
        players.at(id).PlaybackSession().PlaybackStateChanged(
            [=](auto, const auto& args) -> void {
                (*callback)(false);
            }
        );
    }

    EXPORT void Player_setIsBufferingEventHandler(int32_t id, void (*callback)(bool isBuffering)) {
        players.at(id).BufferingStarted(
            [=](auto, const auto& args) -> void {
                (*callback)(true);
            }
        );
        players.at(id).BufferingEnded(
            [=](auto, const auto& args) -> void {
                (*callback)(false);
            }
        );
    }

    EXPORT void Player_setVolumeEventHandler(int32_t id, void (*callback)(float volume)) {
        players.at(id).VolumeChanged(
            [=](auto, const auto& args) -> void {
                (*callback)(players.at(id).Volume());
            }
        );
    }

    EXPORT void Player_setRateEventHandler(int32_t id, void (*callback)(float rate)) {
        players.at(id).MediaPlayerRateChanged(
            [=](auto, const auto& args) -> void {
                (*callback)(players.at(id).PlaybackRate());
            }
        );
    }

    EXPORT void Player_setPositionEventHandler(int32_t id, void (*callback)(int32_t position)) {
        players.at(id).PlaybackSession().PositionChanged(
            [=](auto, const auto& args) -> void {
                (*callback)(TO_MILLISECONDS(players.at(id).PlaybackSession().Position()));
            }
        );
    }

    EXPORT void Player_setDurationEventHandler(int32_t id, void (*callback)(int32_t duration)) {
        players.at(id).PlaybackSession().NaturalDurationChanged(
            [=](auto, const auto& args) -> void {
                (*callback)(TO_MILLISECONDS(players.at(id).PlaybackSession().NaturalDuration()));
            }
        );
    }

    EXPORT void Player_NativeControls_create(int32_t id, void (*callback)(int32_t button)) {
        if (SMTCExist) return;
        SMTCExist = true;
        SystemMediaTransportControls controls = players.at(id).SystemMediaTransportControls();
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

    EXPORT void Player_NativeControls_update(int32_t id, int32_t type, wchar_t** data, const wchar_t* thumbnail) {
        SystemMediaTransportControls controls = players.at(id).SystemMediaTransportControls();
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
        SystemMediaTransportControls controls = players.at(id).SystemMediaTransportControls();
        controls.DisplayUpdater().ClearAll();
    }

    EXPORT void Player_NativeControls_dispose(int32_t id) {
        players.at(id).SystemMediaTransportControls().IsEnabled(false);
    }

    /* Media */

    EXPORT void Media_create(int32_t id, const wchar_t* uri, bool parse = false) {
        Core::MediaSource media = Core::MediaSource::CreateFromUri(
            Uri(uri)
        );
        medias.insert(
            std::make_pair(id, media)
        );
        if (parse) medias.at(id).OpenAsync().get();
    }

    EXPORT void Media_dispose(int32_t id) {
        return medias.at(id).Close();
    }

    EXPORT int32_t Media_getDuration(int32_t id) {
        return TO_MILLISECONDS(medias.at(id).Duration().Value());
    }

    /* Tags */

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
        /* TODO: Handle VideoProperties::Latitude & VideoProperties::Longitude. */
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
        if (SMTCExist) return;
        SMTCExist = true;
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

    EXPORT void NativeControls_update(int32_t type, wchar_t** data, const wchar_t* thumbnail) {
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


LRESULT CALLBACK videoWindowProc(HWND hwnd, UINT code, WPARAM wparam, LPARAM lparam) {
    switch (code) {
        case WM_DESTROY: {
            PostQuitMessage(0);
            return 0;
        }
        case WM_SIZE: {
            RECT rect = { 0, 0, 0, 0 };
            GetClientRect(window, &rect);
            MoveWindow(xaml, 0, 0, rect.right, rect.bottom, TRUE);
            break;
        }
        default: {
            break;
        }
    }
    return DefWindowProcW(hwnd, code, wparam, lparam);
}

#endif
#ifdef __cplusplus
}
#endif
