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
 * The above copyright notice and this permission notice shall be included in
 * all
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
#include <thread>
#include <cwchar>
#include <memory>
#include <unordered_map>
#include <Windows.h>
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
#define DLLEXPORT __declspec(dllexport)
#else
#define DLLEXPORT
#endif
#ifdef __cplusplus
extern "C" {
#endif
#ifndef LIBWINMEDIA
#define LIBWINMEDIA
#define VIDEO_WINDOW_CLASS L"libwinmedia"
#define TAG_SIZE 200
#define TO_MILLISECONDS(timespan) timespan.count() / 10000

using namespace winrt::Windows::Foundation;
using namespace winrt::Windows::Storage;
using namespace winrt::Windows::Media;
using namespace winrt::Windows::System;
using namespace winrt::Windows::UI::Xaml;
using namespace winrt::Windows::Graphics::Imaging;
using namespace winrt::Windows::Graphics::DirectX::Direct3D11;

static std::unordered_map<int32_t, Playback::MediaPlayer> g_players;
static std::unordered_map<int32_t, std::unique_ptr<std::thread>>
    g_players_window_threads;
static std::unordered_map<int32_t, HWND> g_players_window_handles;
static std::unordered_map<int32_t, HWND> g_players_xaml_handles;
static std::unordered_map<int32_t, Core::MediaSource> g_medias;
static bool g_smtc_exist;

LRESULT CALLBACK VideoWindowProc(HWND, UINT, WPARAM, LPARAM);

namespace {

DLLEXPORT void PlayerCreate(int32_t player_id, bool show_window = false,
                            wchar_t* window_title = VIDEO_WINDOW_CLASS) {
  g_players.insert(std::make_pair(player_id, Playback::MediaPlayer()));
  g_players.at(player_id).SystemMediaTransportControls().IsEnabled(false);
  if (show_window) {
    g_players_window_threads[player_id] =
        std::make_unique<std::thread>([=]() -> void {
          STARTUPINFO startup_info;
          GetStartupInfo(&startup_info);
          WNDCLASSEXW window_class;
          SecureZeroMemory(&window_class, sizeof(window_class));
          window_class.cbSize = sizeof(window_class);
          window_class.style = CS_HREDRAW | CS_VREDRAW;
          window_class.lpfnWndProc = VideoWindowProc;
          window_class.hInstance = 0;
          window_class.lpszClassName = VIDEO_WINDOW_CLASS;
          window_class.hCursor = ::LoadCursorW(nullptr, IDC_ARROW);
          window_class.hbrBackground =
              reinterpret_cast<HBRUSH>(COLOR_WINDOW + 1);
          ::RegisterClassExW(&window_class);
          g_players_window_handles[player_id] = ::CreateWindowExW(
              0L, VIDEO_WINDOW_CLASS, window_title,
              WS_OVERLAPPEDWINDOW | WS_VISIBLE, CW_USEDEFAULT, CW_USEDEFAULT,
              CW_USEDEFAULT, CW_USEDEFAULT, nullptr, nullptr, 0, nullptr);
          if (!g_players_window_handles[player_id]) {
            throw std::exception("COULD_NOT_CREATE_WIN32_WINDOW", -1);
          }
          winrt::init_apartment(winrt::apartment_type::multi_threaded);
          const auto window_xaml_manager =
              Hosting::WindowsXamlManager::InitializeForCurrentThread();
          Hosting::DesktopWindowXamlSource desktop_window_xaml_source = {};
          const auto interop =
              desktop_window_xaml_source.as<IDesktopWindowXamlSourceNative>();
          winrt::check_hresult(
              interop->AttachToWindow(g_players_window_handles[player_id]));
          interop->get_WindowHandle(&g_players_xaml_handles[player_id]);
          if (!g_players_xaml_handles[player_id]) {
            throw std::exception("COULD_NOT_CREATE_XAML_SOURCE", -2);
          }
          SetWindowLongPtr(
              g_players_window_handles[player_id], GWLP_USERDATA,
              reinterpret_cast<LONG_PTR>(g_players_xaml_handles[player_id]));
          RECT rect = {0, 0, 0, 0};
          ::GetClientRect(g_players_window_handles[player_id], &rect);
          ::SetWindowPos(g_players_xaml_handles[player_id], nullptr, 0, 0,
                         rect.right, rect.bottom, SWP_SHOWWINDOW);
          Controls::MediaPlayerElement player_element =
              Controls::MediaPlayerElement();
          player_element.SetMediaPlayer(g_players.at(player_id));
          desktop_window_xaml_source.Content(player_element);
          ::ShowWindow(g_players_xaml_handles[player_id],
                       startup_info.wShowWindow);
          ::UpdateWindow(g_players_xaml_handles[player_id]);
          MSG msg = {};
          while (::GetMessageW(&msg, nullptr, 0, 0)) {
            ::TranslateMessage(&msg);
            ::DispatchMessageW(&msg);
          }
        });
  }
}

DLLEXPORT void PlayerDispose(int32_t player_id) {
  g_players.at(player_id).Close();
  ::CloseWindow(g_players_window_handles.at(player_id));
  g_players_window_handles.erase(player_id);
  g_players_xaml_handles.erase(player_id);
  g_players_window_threads.erase(player_id);
  g_players.erase(player_id);
}

DLLEXPORT void PlayerCloseWindow(int32_t player_id) {
  ::CloseWindow(g_players_window_handles.at(player_id));
  g_players_window_handles.erase(player_id);
  g_players_xaml_handles.erase(player_id);
  g_players_window_threads.erase(player_id);
}

DLLEXPORT void PlayerOpen(int32_t player_id, int32_t media_id) {
  g_players.at(player_id)
      .Source(Playback::MediaPlaybackItem(g_medias.at(media_id)));
}

DLLEXPORT void PlayerPlay(int32_t player_id) { g_players.at(player_id).Play(); }

DLLEXPORT void PlayerPause(int32_t player_id) {
  g_players.at(player_id).Pause();
}

DLLEXPORT void PlayerSeek(int32_t player_id, int32_t position) {
  g_players.at(player_id)
      .Position(TimeSpan(std::chrono::milliseconds(position)));
}

DLLEXPORT void PlayerSetVolume(int32_t player_id, float volume) {
  g_players.at(player_id).Volume(volume);
}

DLLEXPORT void PlayerSetRate(int32_t player_id, float rate) {
  g_players.at(player_id).PlaybackRate(rate);
}

DLLEXPORT void PlayerSetAudioBalance(int32_t player_id, float audio_balance) {
  g_players.at(player_id).AudioBalance(audio_balance);
}

DLLEXPORT void PlayerSetAutoplay(int32_t player_id, bool autoplay) {
  g_players.at(player_id).AutoPlay(autoplay);
}

DLLEXPORT void PlayerSetIsLooping(int32_t player_id, bool looping) {
  g_players.at(player_id).IsLoopingEnabled(looping);
}

DLLEXPORT int32_t PlayerGetPosition(int32_t player_id) {
  return TO_MILLISECONDS(g_players.at(player_id).Position());
}

DLLEXPORT float PlayerGetVolume(int32_t player_id) {
  return g_players.at(player_id).Volume();
}

DLLEXPORT float PlayerGetRate(int32_t player_id) {
  return g_players.at(player_id).PlaybackRate();
}

DLLEXPORT float PlayerGetAudioBalance(int32_t player_id) {
  return g_players.at(player_id).AudioBalance();
}

DLLEXPORT bool PlayerIsAutoplay(int32_t player_id) {
  return g_players.at(player_id).AutoPlay();
}

DLLEXPORT bool PlayerIsLooping(int32_t player_id) {
  return g_players.at(player_id).IsLoopingEnabled();
}

// TODO (alexmercerind): Implement PlayerSetFrameEventHandler.
// DLLEXPORT void PlayerSetFrameEventHandler(int32_t player_id, int32_t width,
//                                           int32_t height,
//                                           void (*callback)(uint8_t* frame)) {
//   IDirect3DSurface surface = IDirect3DSurface();
//   Streams::IBuffer buffer = Streams::IBuffer();
//   g_players.at(player_id).IsVideoFrameServerEnabled(true);
//   g_players.at(player_id)
//       .VideoFrameAvailable([=](auto, const auto& args) -> void {
//         g_players.at(player_id).CopyFrameToVideoSurface(surface);
//         SoftwareBitmap bitmap =
//             SoftwareBitmap::CreateCopyFromSurfaceAsync(surface).get();
//         bitmap.CopyToBuffer(buffer);
//         (*callback)(buffer.Data());
//       });
// }

DLLEXPORT void PlayerSetIsPlayingEventHandler(
    int32_t player_id, void (*callback)(bool is_playing)) {
  g_players.at(player_id).PlaybackSession().PlaybackStateChanged(
      [=](auto, const auto& args) -> void {
        if (g_players.at(player_id).PlaybackSession().PlaybackState() ==
            Playback::MediaPlaybackState::Playing) {
          (*callback)(true);
        }
        if (g_players.at(player_id).PlaybackSession().PlaybackState() ==
            Playback::MediaPlaybackState::Paused) {
          (*callback)(false);
        }
      });
}

DLLEXPORT void PlayerSetIsCompletedEventHandler(
    int32_t player_id, void (*callback)(bool is_completed)) {
  g_players.at(player_id)
      .MediaEnded([=](auto, const auto& args) -> void { (*callback)(true); });
  g_players.at(player_id).PlaybackSession().PlaybackStateChanged(
      [=](auto, const auto& args) -> void { (*callback)(false); });
}

DLLEXPORT void PlayerSetIsBufferingEventHandler(
    int32_t player_id, void (*callback)(bool is_buffering)) {
  g_players.at(player_id).BufferingStarted(
      [=](auto, const auto& args) -> void { (*callback)(true); });
  g_players.at(player_id).BufferingEnded(
      [=](auto, const auto& args) -> void { (*callback)(false); });
}

DLLEXPORT void PlayerSetVolumeEventHandler(int32_t player_id,
                                           void (*callback)(float volume)) {
  g_players.at(player_id).VolumeChanged([=](auto, const auto& args) -> void {
    (*callback)(g_players.at(player_id).Volume());
  });
}

DLLEXPORT void PlayerSetRateEventHandler(int32_t player_id,
                                         void (*callback)(float rate)) {
  g_players.at(player_id)
      .MediaPlayerRateChanged([=](auto, const auto& args) -> void {
        (*callback)(g_players.at(player_id).PlaybackRate());
      });
}

DLLEXPORT void PlayerSetPositionEventHandler(
    int32_t player_id, void (*callback)(int32_t position)) {
  g_players.at(player_id).PlaybackSession().PositionChanged(
      [=](auto, const auto& args) -> void {
        (*callback)(TO_MILLISECONDS(
            g_players.at(player_id).PlaybackSession().Position()));
      });
}

DLLEXPORT void PlayerSetDurationEventHandler(
    int32_t player_id, void (*callback)(int32_t duration)) {
  g_players.at(player_id).PlaybackSession().NaturalDurationChanged(
      [=](auto, const auto& args) -> void {
        (*callback)(TO_MILLISECONDS(
            g_players.at(player_id).PlaybackSession().NaturalDuration()));
      });
}

DLLEXPORT void PlayerNativeControlsCreate(int32_t player_id,
                                          void (*callback)(int32_t button)) {
  if (g_smtc_exist) return;
  g_smtc_exist = true;
  SystemMediaTransportControls controls =
      g_players.at(player_id).SystemMediaTransportControls();
  controls.IsEnabled(true);
  controls.IsPlayEnabled(true);
  controls.IsPauseEnabled(true);
  controls.IsNextEnabled(true);
  controls.IsPreviousEnabled(true);
  controls.ButtonPressed(
      [=](auto, const SystemMediaTransportControlsButtonPressedEventArgs& args)
          -> void {
            if (args.Button() == SystemMediaTransportControlsButton::Play)
              controls.PlaybackStatus(MediaPlaybackStatus::Playing);
            if (args.Button() == SystemMediaTransportControlsButton::Pause)
              controls.PlaybackStatus(MediaPlaybackStatus::Paused);
            (*callback)(static_cast<int>(args.Button()));
          });
}

DLLEXPORT void PlayerNativeControlsSetStatus(int32_t player_id,
                                             int32_t status) {
  SystemMediaTransportControls controls =
      Playback::BackgroundMediaPlayer::Current().SystemMediaTransportControls();
  controls.PlaybackStatus(static_cast<MediaPlaybackStatus>(status));
}

DLLEXPORT void PlayerNativeControlsUpdate(int32_t player_id, int32_t type,
                                          wchar_t** data,
                                          const wchar_t* thumbnail) {
  SystemMediaTransportControls controls =
      g_players.at(player_id).SystemMediaTransportControls();
  SystemMediaTransportControlsDisplayUpdater updater =
      controls.DisplayUpdater();
  updater.Type(static_cast<MediaPlaybackType>(type));
  if (type == 0) {
    MusicDisplayProperties properties = updater.MusicProperties();
    properties.AlbumArtist(data[0]);
    properties.AlbumTitle(data[1]);
    try {
      properties.AlbumTrackCount(std::stoi(data[2]));
    } catch (...) {
    }
    properties.Artist(data[3]);
    properties.Title(data[4]);
    try {
      properties.TrackNumber(std::stoi(data[5]));
    } catch (...) {
    }
  } else if (type == 1) {
    VideoDisplayProperties properties = updater.VideoProperties();
    properties.Title(data[0]);
    properties.Subtitle(data[1]);
  }
  if (std::wstring(thumbnail) != L"") {
    updater.Thumbnail(Streams::RandomAccessStreamReference::CreateFromFile(
        StorageFile::GetFileFromPathAsync(thumbnail).get()));
  }
  updater.Update();
}

DLLEXPORT void PlayerNativeControlsClear(int32_t player_id) {
  SystemMediaTransportControls controls =
      g_players.at(player_id).SystemMediaTransportControls();
  controls.DisplayUpdater().ClearAll();
}

DLLEXPORT void PlayerNativeControlsDispose(int32_t player_id) {
  g_players.at(player_id).SystemMediaTransportControls().IsEnabled(false);
}

/* Media */

DLLEXPORT void MediaCreate(int32_t media_id, const wchar_t* uri,
                           bool parse = false) {
  g_medias.insert(
      std::make_pair(media_id, Core::MediaSource::CreateFromUri(Uri(uri))));
  if (parse) g_medias.at(media_id).OpenAsync().get();
}

DLLEXPORT void MediaDispose(int32_t media_id) {
  g_medias.at(media_id).Close();
  g_medias.erase(media_id);
}

DLLEXPORT int32_t MediaGetDuration(int32_t media_id) {
  return TO_MILLISECONDS(g_medias.at(media_id).Duration().Value());
}

/* Tags */

DLLEXPORT wchar_t** TagsFromMusic(const wchar_t* uri) {
  FileProperties::StorageItemContentProperties properties =
      StorageFile::GetFileFromPathAsync(uri).get().Properties();
  FileProperties::MusicProperties music =
      properties.GetMusicPropertiesAsync().get();
  std::wstring string = L"";
  wchar_t** tags = new wchar_t*[15];
  for (int32_t index = 0; index < 15; index++)
    tags[index] = new wchar_t[TAG_SIZE];
  wcscpy_s(tags[0], TAG_SIZE, music.Album().c_str());
  wcscpy_s(tags[1], TAG_SIZE, music.AlbumArtist().c_str());
  wcscpy_s(tags[2], TAG_SIZE, std::to_wstring(music.Bitrate()).c_str());
  Collections::IVector<winrt::hstring> composers = music.Composers();
  for (int32_t index = 0; index < composers.Size(); index++) {
    string += composers.GetAt(index).c_str();
    string += L" ";
  }
  wcscpy_s(tags[3], TAG_SIZE, string.c_str());
  string.clear();
  Collections::IVector<winrt::hstring> conductors = music.Conductors();
  for (int32_t index = 0; index < conductors.Size(); index++) {
    string += conductors.GetAt(index).c_str();
    string += L" ";
  }
  wcscpy_s(tags[4], TAG_SIZE, string.c_str());
  string.clear();
  wcscpy_s(tags[5], TAG_SIZE,
           std::to_wstring(TO_MILLISECONDS(music.Duration())).c_str());
  Collections::IVector<winrt::hstring> genre = music.Genre();
  for (int32_t index = 0; index < genre.Size(); index++) {
    string += genre.GetAt(index).c_str();
    string += L" ";
  }
  wcscpy_s(tags[6], TAG_SIZE, string.c_str());
  string.clear();
  Collections::IVector<winrt::hstring> producers = music.Producers();
  for (int32_t index = 0; index < producers.Size(); index++) {
    string += producers.GetAt(index).c_str();
    string += L" ";
  }
  wcscpy_s(tags[7], TAG_SIZE, string.c_str());
  string.clear();
  wcscpy_s(tags[8], TAG_SIZE, music.Publisher().c_str());
  wcscpy_s(tags[9], TAG_SIZE, std::to_wstring(music.Rating()).c_str());
  wcscpy_s(tags[10], TAG_SIZE, music.Subtitle().c_str());
  wcscpy_s(tags[11], TAG_SIZE, music.Title().c_str());
  wcscpy_s(tags[12], TAG_SIZE, std::to_wstring(music.TrackNumber()).c_str());
  Collections::IVector<winrt::hstring> writers = music.Writers();
  for (int32_t index = 0; index < writers.Size(); index++) {
    string += writers.GetAt(index).c_str();
    string += L" ";
  }
  wcscpy_s(tags[13], TAG_SIZE, string.c_str());
  string.clear();
  wcscpy(tags[14], std::to_wstring(music.Year()).c_str());
  return tags;
}

DLLEXPORT wchar_t** TagsFromVideo(const wchar_t* uri) {
  FileProperties::StorageItemContentProperties properties =
      StorageFile::GetFileFromPathAsync(uri).get().Properties();
  FileProperties::VideoProperties video =
      properties.GetVideoPropertiesAsync().get();
  std::wstring string = L"";
  wchar_t** tags = new wchar_t*[16];
  for (int32_t index = 0; index < 16; index++)
    tags[index] = new wchar_t[TAG_SIZE];
  wcscpy_s(tags[0], TAG_SIZE, std::to_wstring(video.Bitrate()).c_str());
  Collections::IVector<winrt::hstring> directors = video.Directors();
  for (int32_t index = 0; index < directors.Size(); index++) {
    string += directors.GetAt(index).c_str();
    string += L" ";
  }
  wcscpy_s(tags[1], TAG_SIZE, string.c_str());
  string.clear();
  wcscpy_s(tags[2], TAG_SIZE,
           std::to_wstring(TO_MILLISECONDS(video.Duration())).c_str());
  wcscpy_s(tags[3], TAG_SIZE, std::to_wstring(video.Height()).c_str());
  Collections::IVector<winrt::hstring> keywords = video.Keywords();
  for (int32_t index = 0; index < keywords.Size(); index++) {
    string += keywords.GetAt(index).c_str();
    string += L" ";
  }
  wcscpy_s(tags[4], TAG_SIZE, string.c_str());
  string.clear();
  // TODO (alexmercerind): Handle VideoProperties::Latitude &
  // VideoProperties::Longitude.
  wcscpy_s(tags[5], TAG_SIZE, L"-1");
  wcscpy_s(tags[6], TAG_SIZE, L"-1");
  wcscpy_s(tags[7], TAG_SIZE,
           std::to_wstring(static_cast<uint32_t>(video.Orientation())).c_str());
  Collections::IVector<winrt::hstring> producers = video.Producers();
  for (int32_t index = 0; index < producers.Size(); index++) {
    string += producers.GetAt(index).c_str();
    string += L" ";
  }
  wcscpy_s(tags[8], TAG_SIZE, string.c_str());
  string.clear();
  wcscpy_s(tags[9], TAG_SIZE, video.Publisher().c_str());
  wcscpy_s(tags[10], TAG_SIZE, std::to_wstring(video.Rating()).c_str());
  wcscpy_s(tags[11], TAG_SIZE, video.Subtitle().c_str());
  wcscpy_s(tags[12], TAG_SIZE, video.Title().c_str());
  wcscpy_s(tags[13], TAG_SIZE, std::to_wstring(video.Width()).c_str());
  Collections::IVector<winrt::hstring> writers = video.Writers();
  for (int32_t index = 0; index < writers.Size(); index++) {
    string += writers.GetAt(index).c_str();
    string += L" ";
  }
  wcscpy_s(tags[14], TAG_SIZE, string.c_str());
  string.clear();
  wcscpy_s(tags[15], TAG_SIZE, std::to_wstring(video.Year()).c_str());
  return tags;
}

DLLEXPORT void TagsExtractThumbnail(const wchar_t* media, const wchar_t* folder,
                                    const wchar_t* file_name, int32_t mode,
                                    int32_t size) {
  StorageFile source_file = StorageFile::GetFileFromPathAsync(media).get();
  StorageFolder storage_folder =
      StorageFolder::GetFolderFromPathAsync(folder).get();
  storage_folder.CreateFileAsync(file_name,
                                 CreationCollisionOption::ReplaceExisting)
      .get();
  StorageFile storage_file = storage_folder.GetFileAsync(file_name).get();
  FileProperties::StorageItemThumbnail thumbnail =
      source_file.GetThumbnailAsync(
                     static_cast<FileProperties::ThumbnailMode>(mode), size)
          .get();
  Streams::Buffer buffer = Streams::Buffer(thumbnail.Size());
  thumbnail.ReadAsync(buffer, thumbnail.Size(),
                      Streams::InputStreamOptions::ReadAhead)
      .get();
  FileIO::WriteBufferAsync(storage_file, buffer).get();
}

/* NativeControls */

DLLEXPORT void NativeControlsCreate(void (*callback)(int32_t button)) {
  if (g_smtc_exist) return;
  g_smtc_exist = true;
  SystemMediaTransportControls controls =
      Playback::BackgroundMediaPlayer::Current().SystemMediaTransportControls();
  controls.IsEnabled(true);
  controls.IsPlayEnabled(true);
  controls.IsPauseEnabled(true);
  controls.IsNextEnabled(true);
  controls.IsPreviousEnabled(true);
  controls.ButtonPressed(
      [=](auto, const SystemMediaTransportControlsButtonPressedEventArgs& args)
          -> void {
            if (args.Button() == SystemMediaTransportControlsButton::Play)
              controls.PlaybackStatus(MediaPlaybackStatus::Playing);
            if (args.Button() == SystemMediaTransportControlsButton::Pause)
              controls.PlaybackStatus(MediaPlaybackStatus::Paused);
            (*callback)(static_cast<int>(args.Button()));
          });
}

DLLEXPORT void NativeControlsSetStatus(int32_t status) {
  SystemMediaTransportControls controls =
      Playback::BackgroundMediaPlayer::Current().SystemMediaTransportControls();
  controls.PlaybackStatus(static_cast<MediaPlaybackStatus>(status));
}

DLLEXPORT void NativeControlsUpdate(int32_t type, wchar_t** data,
                                    const wchar_t* thumbnail) {
  SystemMediaTransportControls controls =
      Playback::BackgroundMediaPlayer::Current().SystemMediaTransportControls();
  SystemMediaTransportControlsDisplayUpdater updater =
      controls.DisplayUpdater();
  updater.Type(static_cast<MediaPlaybackType>(type));
  if (type == 1) {
    MusicDisplayProperties properties = updater.MusicProperties();
    properties.AlbumArtist(data[0]);
    properties.AlbumTitle(data[1]);
    try {
      properties.AlbumTrackCount(std::stoi(data[2]));
    } catch (...) {
    }
    properties.Artist(data[3]);
    properties.Title(data[4]);
    try {
      properties.TrackNumber(std::stoi(data[5]));
    } catch (...) {
    }
  } else if (type == 2) {
    VideoDisplayProperties properties = updater.VideoProperties();
    properties.Title(data[0]);
    properties.Subtitle(data[1]);
  }
  if (std::wstring(thumbnail) != L"") {
    updater.Thumbnail(Streams::RandomAccessStreamReference::CreateFromFile(
        StorageFile::GetFileFromPathAsync(thumbnail).get()));
  }
  updater.Update();
}

DLLEXPORT void NativeControlsClear() {
  SystemMediaTransportControls controls =
      Playback::BackgroundMediaPlayer::Current().SystemMediaTransportControls();
  controls.DisplayUpdater().ClearAll();
}

DLLEXPORT void NativeControlsDispose() {
  Playback::BackgroundMediaPlayer::Current()
      .SystemMediaTransportControls()
      .IsEnabled(false);
}
}

LRESULT CALLBACK VideoWindowProc(HWND window, UINT code, WPARAM wparam,
                                 LPARAM lparam) {
  switch (code) {
    case WM_DESTROY: {
      PostQuitMessage(0);
      return 0;
    }
    case WM_SIZE: {
      RECT rect = {0, 0, 0, 0};
      GetClientRect(window, &rect);
      MoveWindow(
          reinterpret_cast<HWND>(GetWindowLongPtr(window, GWLP_USERDATA)), 0, 0,
          rect.right, rect.bottom, TRUE);
      break;
    }
    default: { break; }
  }
  return DefWindowProcW(window, code, wparam, lparam);
}

#endif
#ifdef __cplusplus
}
#endif
