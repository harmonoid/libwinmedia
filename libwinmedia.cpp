/*
 * libwinmedia
 * -----------
 * v0.0.3
 * A cross-platform media playback library for C/C++ with good number of
 * features.
 *
 * Works on Windows & Linux.
 *
 * https://github.com/harmonoid/libwinmedia
 *
 *
 * Minimal Example
 * ```cpp
 * #include "libwinmedia/libwinmedia.hpp"
 *
 * int32_t main(int32_t ac, const char** av) {
 *   using namespace std;
 *   using namespace lwm;
 *   if (ac < 2) {
 *     cout << "No URI provided.\n"
 *         << "Example Usage:\n" << av[0]
 *         << " file://C:/alexmercerind/music.mp3\n" << av[0]
 *         << " https://alexmercerind.github.io/video.mp4\n";
 *     return EXIT_FAILURE;
 *   }
 *   auto player = Player(0);
 *   auto media = Media(av[1]);
 *   player.Open(vector<Media>{media});
 *   player.Play();
 *   player.events()->Position([](int32_t position) -> void {
 *     cout << "Current playback position is " << position << " ms.\n";
 *   });
 *   cin.get();
 *   return EXIT_SUCCESS;
 * }
 * ```
 *
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
#ifndef UNICODE
#define UNICODE
#define _UNICODE
#endif
#include <cwchar>
#include <iostream>
#include <memory>
#include <thread>
#include <unordered_map>
#ifdef _WIN32
#include <Windows.h>
#include <windows.ui.xaml.hosting.desktopwindowxamlsource.h>
#include <winrt/Windows.Foundation.Collections.h>
#include <winrt/Windows.Foundation.h>
#include <winrt/Windows.Graphics.DirectX.Direct3D11.h>
#include <winrt/Windows.Graphics.DirectX.h>
#include <winrt/Windows.Graphics.Imaging.h>
#include <winrt/Windows.Media.Audio.h>
#include <winrt/Windows.Media.Core.h>
#include <winrt/Windows.Media.Playback.h>
#include <winrt/Windows.Storage.FileProperties.h>
#include <winrt/Windows.Storage.Streams.h>
#include <winrt/Windows.System.h>
#include <winrt/Windows.UI.Xaml.Controls.h>
#include <winrt/Windows.UI.Xaml.Hosting.h>
#elif __linux__
#include "linux/player.hpp"
#endif
#ifdef _WIN32
#define DLLEXPORT __declspec(dllexport)
#else
#define DLLEXPORT
#endif
#ifdef DART_VM
#include "external/dart_api/dart_api_dl.h"
#endif
#ifdef __cplusplus
extern "C" {
#endif
#ifndef LIBWINMEDIA
#define LIBWINMEDIA
#define VIDEO_WINDOW_CLASS "libwinmedia"
#define TAG_SIZE 200
#define TO_MILLISECONDS(timespan) timespan.count() / 10000

#ifdef _WIN32
auto TO_STRING = [](std::wstring wide_string) -> std::string {
  if (wide_string.empty()) {
    return std::string();
  }
  int32_t target_length = ::WideCharToMultiByte(
      CP_UTF8, WC_ERR_INVALID_CHARS, wide_string.data(),
      static_cast<int32_t>(wide_string.length()), nullptr, 0, nullptr, nullptr);
  if (target_length == 0) {
    return std::string();
  }
  std::string utf8_string;
  utf8_string.resize(target_length);
  int32_t converted_length = ::WideCharToMultiByte(
      CP_UTF8, WC_ERR_INVALID_CHARS, wide_string.data(),
      static_cast<int32_t>(wide_string.length()), utf8_string.data(),
      target_length, nullptr, nullptr);
  if (converted_length == 0) {
    return std::string();
  }
  return utf8_string;
};

auto TO_WIDESTRING = [](std::string string) -> std::wstring {
  if (string.empty()) {
    return std::wstring();
  }
  int32_t target_length =
      ::MultiByteToWideChar(CP_UTF8, MB_ERR_INVALID_CHARS, string.data(),
                            static_cast<int32_t>(string.length()), nullptr, 0);
  if (target_length == 0) {
    return std::wstring();
  }
  std::wstring utf16_string;
  utf16_string.resize(target_length);
  int32_t converted_length =
      ::MultiByteToWideChar(CP_UTF8, MB_ERR_INVALID_CHARS, string.data(),
                            static_cast<int32_t>(string.length()),
                            utf16_string.data(), target_length);
  if (converted_length == 0) {
    return std::wstring();
  }
  return utf16_string;
};

using namespace winrt::Windows::Foundation;
using namespace winrt::Windows::Storage;
using namespace winrt::Windows::Media;
using namespace winrt::Windows::System;
using namespace winrt::Windows::UI::Xaml;
using namespace winrt::Windows::Graphics::Imaging;
using namespace winrt::Windows::Graphics::DirectX::Direct3D11;

static std::unordered_map<int32_t, Playback::MediaPlayer> g_media_players;
static std::unordered_map<int32_t, Playback::MediaPlaybackList>
    g_media_playback_lists;
static std::unordered_map<int32_t, std::vector<int32_t>> g_media_ids_lists;
static std::unordered_map<int32_t, std::unique_ptr<std::thread>>
    g_media_players_window_threads;
static std::unordered_map<int32_t, HWND> g_media_players_window_handles;
static std::unordered_map<int32_t, HWND> g_media_players_xaml_handles;
static std::unordered_map<int32_t, Core::MediaSource> g_medias;
static bool g_smtc_exist;

LRESULT CALLBACK VideoWindowProc(HWND, UINT, WPARAM, LPARAM);
#elif __linux__

static std::unordered_map<int32_t, std::unique_ptr<Player>> g_media_players;
static std::unordered_map<int32_t, bool> g_media_players_initialization;
static std::unordered_map<int32_t, std::promise<void>> g_media_players_promises;

void Dispatch(std::function<void()> f) {
  g_idle_add_full(G_PRIORITY_HIGH_IDLE, (GSourceFunc)([](void* f) -> int32_t {
                    (*static_cast<std::function<void()>*>(f))();
                    return G_SOURCE_REMOVE;
                  }),
                  new std::function<void()>(f), [](void* f) {
                    delete static_cast<std::function<void()>*>(f);
                  });
}

void EnsureFuture(int32_t player_id) {
  if (!g_media_players_initialization.at(player_id)) {
    g_media_players_promises.at(player_id).get_future().wait();
    g_media_players_initialization[player_id] = true;
  }
}

#endif

namespace {

#ifdef DART_VM
typedef bool (*Dart_PostCObjectType)(Dart_Port port_id, Dart_CObject* message);

Dart_PostCObjectType g_dart_post_C_object;
Dart_Port g_callback_port;

DLLEXPORT void InitializeDartApi(Dart_PostCObjectType dart_post_C_object,
                                 Dart_Port callback_port) {
  g_dart_post_C_object = dart_post_C_object;
  g_callback_port = callback_port;
}
#endif

DLLEXPORT void PlayerShowWindow(int32_t player_id,
                                const char* window_title = VIDEO_WINDOW_CLASS) {
#ifdef _WIN32
  g_media_players_window_threads[player_id] =
      std::make_unique<std::thread>([=]() -> void {
        try {
          STARTUPINFO startup_info;
          GetStartupInfo(&startup_info);
          WNDCLASSEXW window_class;
          SecureZeroMemory(&window_class, sizeof(window_class));
          window_class.cbSize = sizeof(window_class);
          window_class.style = CS_HREDRAW | CS_VREDRAW;
          window_class.lpfnWndProc = VideoWindowProc;
          window_class.hInstance = 0;
          window_class.lpszClassName =
              TO_WIDESTRING(VIDEO_WINDOW_CLASS).c_str();
          window_class.hCursor = ::LoadCursorW(nullptr, IDC_ARROW);
          window_class.hbrBackground =
              reinterpret_cast<HBRUSH>(COLOR_WINDOW + 1);
          ::RegisterClassExW(&window_class);
          g_media_players_window_handles[player_id] = ::CreateWindowEx(
              0L, TO_WIDESTRING(VIDEO_WINDOW_CLASS).c_str(),
              TO_WIDESTRING(window_title).c_str(),
              WS_OVERLAPPEDWINDOW | WS_VISIBLE, CW_USEDEFAULT, CW_USEDEFAULT,
              CW_USEDEFAULT, CW_USEDEFAULT, nullptr, nullptr, 0, nullptr);
          if (!g_media_players_window_handles[player_id]) {
            throw std::exception("COULD_NOT_CREATE_WIN32_WINDOW", -1);
          }
          winrt::init_apartment(winrt::apartment_type::multi_threaded);
          const auto window_xaml_manager =
              Hosting::WindowsXamlManager::InitializeForCurrentThread();
          Hosting::DesktopWindowXamlSource desktop_window_xaml_source = {};
          const auto interop =
              desktop_window_xaml_source.as<IDesktopWindowXamlSourceNative>();
          winrt::check_hresult(interop->AttachToWindow(
              g_media_players_window_handles[player_id]));
          interop->get_WindowHandle(&g_media_players_xaml_handles[player_id]);
          if (!g_media_players_xaml_handles[player_id]) {
            throw std::exception("COULD_NOT_CREATE_XAML_SOURCE", -2);
          }
          SetWindowLongPtr(g_media_players_window_handles[player_id],
                           GWLP_USERDATA,
                           reinterpret_cast<LONG_PTR>(
                               g_media_players_xaml_handles[player_id]));
          RECT rect = {0, 0, 0, 0};
          ::GetClientRect(g_media_players_window_handles[player_id], &rect);
          ::SetWindowPos(g_media_players_xaml_handles[player_id], nullptr, 0, 0,
                         rect.right, rect.bottom, SWP_SHOWWINDOW);
          Controls::MediaPlayerElement player_element =
              Controls::MediaPlayerElement();
          player_element.SetMediaPlayer(g_media_players.at(player_id));
          desktop_window_xaml_source.Content(player_element);
          ::ShowWindow(g_media_players_xaml_handles[player_id],
                       startup_info.wShowWindow);
          ::UpdateWindow(g_media_players_xaml_handles[player_id]);
          MSG msg = {};
          while (::GetMessageW(&msg, nullptr, 0, 0)) {
            ::TranslateMessage(&msg);
            ::DispatchMessageW(&msg);
          }
        } catch (...) {
          std::wcerr << "PlayerShowWindow cannot be called.\n"
                     << "Executable must embed a manifest with "
                        "maxversiontexted property in it.\n"
                     << "Learn more: "
                        "https://docs.microsoft.com/en-us/cpp/build/"
                        "how-to-embed-a-manifest-inside-a-c-cpp-application\n";
        }
      });
#elif __linux__
  g_media_players.at(player_id)->ShowWindow();
#endif
}

DLLEXPORT void PlayerCloseWindow(int32_t player_id) {
#ifdef _WIN32
  if (g_media_players_window_handles.find(player_id) !=
      g_media_players_window_handles.end()) {
    // ::DestroyWindow will not work since window exists on another thread.
    ::SendMessage(g_media_players_window_handles.at(player_id), WM_CLOSE, NULL,
                  NULL);
    g_media_players_window_handles.erase(player_id);
  }
  if (g_media_players_xaml_handles.find(player_id) !=
      g_media_players_xaml_handles.end()) {
    g_media_players_xaml_handles.erase(player_id);
  }
  if (g_media_players_window_threads.find(player_id) !=
      g_media_players_window_threads.end()) {
    g_media_players_window_threads.at(player_id)->detach();
    g_media_players_window_threads.erase(player_id);
  }
#elif __linux__
  g_media_players.at(player_id)->CloseWindow();
#endif
}

DLLEXPORT void PlayerCreate(int32_t player_id, bool show_window = false,
                            const char* window_title = VIDEO_WINDOW_CLASS) {
  if (g_media_players.find(player_id) != g_media_players.end()) {
    return;
  }
#ifdef _WIN32
  g_media_players.insert(std::make_pair(player_id, Playback::MediaPlayer()));
  g_media_playback_lists.insert(
      std::make_pair(player_id, Playback::MediaPlaybackList()));
  g_media_ids_lists.insert(std::make_pair(player_id, std::vector<int32_t>{}));
  g_media_players.at(player_id).SystemMediaTransportControls().IsEnabled(false);
  if (show_window) PlayerShowWindow(player_id, window_title);
#elif __linux__
  g_media_players_initialization.insert(std::make_pair(player_id, false));
  g_media_players_promises.insert(
      std::make_pair(player_id, std::promise<void>()));
  Dispatch([=]() -> void {
    g_media_players.insert(std::make_pair(
        player_id,
        std::make_unique<Player>(player_id, show_window, window_title)));
    g_media_players_promises.at(player_id).set_value();
  });
#endif
}

DLLEXPORT void PlayerDispose(int32_t player_id) {
#ifdef _WIN32
  g_media_players.at(player_id).Close();
  PlayerCloseWindow(player_id);
  if (g_media_playback_lists.find(player_id) != g_media_playback_lists.end()) {
    g_media_playback_lists.erase(player_id);
  }
  if (g_media_ids_lists.find(player_id) != g_media_ids_lists.end()) {
    g_media_ids_lists.erase(player_id);
  }
  if (g_media_players.find(player_id) != g_media_players.end()) {
    g_media_players.erase(player_id);
  }
#elif __linux__
  EnsureFuture(player_id);
  if (g_media_players.find(player_id) != g_media_players.end()) {
    g_media_players.erase(player_id);
  }
  if (g_media_players_initialization.find(player_id) !=
      g_media_players_initialization.end()) {
    g_media_players_initialization.erase(player_id);
  }
  if (g_media_players_promises.find(player_id) !=
      g_media_players_promises.end()) {
    g_media_players_promises.erase(player_id);
  }
#endif
}

DLLEXPORT void PlayerOpen(int32_t player_id, int32_t size, const char** uris,
                          const int32_t* ids) {
#ifdef _WIN32
  g_media_players.at(player_id).Pause();
  g_media_playback_lists.at(player_id).Items().Clear();
  g_media_ids_lists.at(player_id).clear();
  for (int32_t index = 0; index < size; index++) {
    g_media_playback_lists.at(player_id).Items().Append(
        Playback::MediaPlaybackItem(
            Core::MediaSource::CreateFromUri(Uri(TO_WIDESTRING(uris[index])))));
    g_media_ids_lists.at(player_id).emplace_back(ids[index]);
  }
  g_media_players.at(player_id).Source(g_media_playback_lists.at(player_id));
#ifdef DART_VM
  Dart_CObject player_id_object;
  player_id_object.type = Dart_CObject_kInt32;
  player_id_object.value.as_int32 = player_id;
  Dart_CObject type_object;
  type_object.type = Dart_CObject_kString;
  type_object.value.as_string = "Open";
  auto uri_objects = std::unique_ptr<Dart_CObject[]>(
      new Dart_CObject[g_media_playback_lists.at(player_id).Items().Size()]);
  auto uri_object_refs = std::unique_ptr<Dart_CObject*[]>(
      new Dart_CObject*[g_media_playback_lists.at(player_id).Items().Size()]);
  std::vector<std::string> uris_str(
      g_media_playback_lists.at(player_id).Items().Size());
  std::vector<char*> uris_ptr(
      g_media_playback_lists.at(player_id).Items().Size());
  for (int32_t i = 0; i < g_media_playback_lists.at(player_id).Items().Size();
       i++) {
    uris_str[i] = TO_STRING(g_media_playback_lists.at(player_id)
                                .Items()
                                .GetAt(i)
                                .Source()
                                .Uri()
                                .ToString()
                                .c_str());
    uris_ptr[i] = uris_str[i].data();
    Dart_CObject* value_object = &uri_objects[i];
    value_object->type = Dart_CObject_kString;
    value_object->value.as_string = uris_ptr[i];
    uri_object_refs[i] = value_object;
  }
  Dart_CObject uris_object;
  uris_object.type = Dart_CObject_kArray;
  uris_object.value.as_array.length =
      g_media_playback_lists.at(player_id).Items().Size();
  uris_object.value.as_array.values = uri_object_refs.get();
  auto id_objects = std::unique_ptr<Dart_CObject[]>(
      new Dart_CObject[g_media_playback_lists.at(player_id).Items().Size()]);
  auto id_object_refs = std::unique_ptr<Dart_CObject*[]>(
      new Dart_CObject*[g_media_playback_lists.at(player_id).Items().Size()]);
  for (int32_t i = 0; i < g_media_ids_lists.at(player_id).size(); i++) {
    Dart_CObject* value_object = &id_objects[i];
    value_object->type = Dart_CObject_kInt32;
    value_object->value.as_int32 = g_media_ids_lists.at(player_id)[i];
    id_object_refs[i] = value_object;
  }
  Dart_CObject ids_object;
  ids_object.type = Dart_CObject_kArray;
  ids_object.value.as_array.length = g_media_ids_lists.at(player_id).size();
  ids_object.value.as_array.values = id_object_refs.get();
  Dart_CObject* value_objects[] = {&player_id_object, &type_object,
                                   &uris_object, &ids_object};
  Dart_CObject return_object;
  return_object.type = Dart_CObject_kArray;
  return_object.value.as_array.length = 4;
  return_object.value.as_array.values = value_objects;
  g_dart_post_C_object(g_callback_port, &return_object);
#endif
#elif __linux__
  EnsureFuture(player_id);
  std::vector<std::string> uris_vector;
  uris_vector.reserve(size);
  std::vector<int32_t> ids_vector;
  ids_vector.reserve(size);
  for (int32_t i = 0; i < size; i++) {
    uris_vector.emplace_back(uris[i]);
    ids_vector.emplace_back(ids[i]);
  }
  g_media_players.at(player_id)->Pause();
  g_media_players.at(player_id)->Open(uris_vector, ids_vector);
#ifdef DART_VM
  Dart_CObject player_id_object;
  player_id_object.type = Dart_CObject_kInt32;
  player_id_object.value.as_int32 = player_id;
  Dart_CObject type_object;
  type_object.type = Dart_CObject_kString;
  type_object.value.as_string = const_cast<char*>("Open");
  auto uri_objects = std::unique_ptr<Dart_CObject[]>(
      new Dart_CObject[g_media_players.at(player_id)->media_uris().size()]);
  auto uri_object_refs = std::unique_ptr<Dart_CObject*[]>(
      new Dart_CObject*[g_media_players.at(player_id)->media_uris().size()]);
  std::vector<std::string> uris_str(
      g_media_players.at(player_id)->media_uris().size());
  std::vector<char*> uris_ptr(
      g_media_players.at(player_id)->media_uris().size());
  for (int32_t i = 0; i < g_media_players.at(player_id)->media_uris().size();
       i++) {
    uris_str[i] = g_media_players.at(player_id)->media_uris()[i];
    uris_ptr[i] = uris_str[i].data();
    Dart_CObject* value_object = &uri_objects[i];
    value_object->type = Dart_CObject_kString;
    value_object->value.as_string = uris_ptr[i];
    uri_object_refs[i] = value_object;
  }
  Dart_CObject uris_object;
  uris_object.type = Dart_CObject_kArray;
  uris_object.value.as_array.length =
      g_media_players.at(player_id)->media_uris().size();
  uris_object.value.as_array.values = uri_object_refs.get();
  auto id_objects = std::unique_ptr<Dart_CObject[]>(
      new Dart_CObject[g_media_players.at(player_id)->media_ids().size()]);
  auto id_object_refs = std::unique_ptr<Dart_CObject*[]>(
      new Dart_CObject*[g_media_players.at(player_id)->media_ids().size()]);
  for (int32_t i = 0; i < g_media_players.at(player_id)->media_ids().size();
       i++) {
    Dart_CObject* value_object = &id_objects[i];
    value_object->type = Dart_CObject_kInt32;
    value_object->value.as_int32 =
        g_media_players.at(player_id)->media_ids()[i];
    id_object_refs[i] = value_object;
  }
  Dart_CObject ids_object;
  ids_object.type = Dart_CObject_kArray;
  ids_object.value.as_array.length =
      g_media_players.at(player_id)->media_ids().size();
  ids_object.value.as_array.values = id_object_refs.get();
  Dart_CObject* value_objects[] = {&player_id_object, &type_object,
                                   &uris_object, &ids_object};
  Dart_CObject return_object;
  return_object.type = Dart_CObject_kArray;
  return_object.value.as_array.length = 4;
  return_object.value.as_array.values = value_objects;
  g_dart_post_C_object(g_callback_port, &return_object);
#endif
#endif
}

DLLEXPORT void PlayerPlay(int32_t player_id) {
#ifdef _WIN32
  g_media_players.at(player_id).Play();
#elif __linux__
  EnsureFuture(player_id);
  g_media_players.at(player_id)->Play();
#endif
}

DLLEXPORT void PlayerPause(int32_t player_id) {
#ifdef _WIN32
  g_media_players.at(player_id).Pause();
#elif __linux__
  EnsureFuture(player_id);
  g_media_players.at(player_id)->Pause();
#endif
}

DLLEXPORT void PlayerAdd(int32_t player_id, const char* uri, int32_t id) {
#ifdef _WIN32
  g_media_playback_lists.at(player_id).Items().Append(
      Playback::MediaPlaybackItem(
          Core::MediaSource::CreateFromUri(Uri(TO_WIDESTRING(uri)))));
  g_media_ids_lists.at(player_id).emplace_back(id);
#ifdef DART_VM
  Dart_CObject player_id_object;
  player_id_object.type = Dart_CObject_kInt32;
  player_id_object.value.as_int32 = player_id;
  Dart_CObject type_object;
  type_object.type = Dart_CObject_kString;
  type_object.value.as_string = "Open";
  auto uri_objects = std::unique_ptr<Dart_CObject[]>(
      new Dart_CObject[g_media_playback_lists.at(player_id).Items().Size()]);
  auto uri_object_refs = std::unique_ptr<Dart_CObject*[]>(
      new Dart_CObject*[g_media_playback_lists.at(player_id).Items().Size()]);
  std::vector<std::string> uris_str(
      g_media_playback_lists.at(player_id).Items().Size());
  std::vector<char*> uris_ptr(
      g_media_playback_lists.at(player_id).Items().Size());
  for (int32_t i = 0; i < g_media_playback_lists.at(player_id).Items().Size();
       i++) {
    uris_str[i] = TO_STRING(g_media_playback_lists.at(player_id)
                                .Items()
                                .GetAt(i)
                                .Source()
                                .Uri()
                                .ToString()
                                .c_str());
    uris_ptr[i] = uris_str[i].data();
    Dart_CObject* value_object = &uri_objects[i];
    value_object->type = Dart_CObject_kString;
    value_object->value.as_string = uris_ptr[i];
    uri_object_refs[i] = value_object;
  }
  Dart_CObject uris_object;
  uris_object.type = Dart_CObject_kArray;
  uris_object.value.as_array.length =
      g_media_playback_lists.at(player_id).Items().Size();
  uris_object.value.as_array.values = uri_object_refs.get();
  auto id_objects = std::unique_ptr<Dart_CObject[]>(
      new Dart_CObject[g_media_playback_lists.at(player_id).Items().Size()]);
  auto id_object_refs = std::unique_ptr<Dart_CObject*[]>(
      new Dart_CObject*[g_media_playback_lists.at(player_id).Items().Size()]);
  for (int32_t i = 0; i < g_media_ids_lists.at(player_id).size(); i++) {
    Dart_CObject* value_object = &id_objects[i];
    value_object->type = Dart_CObject_kInt32;
    value_object->value.as_int32 = g_media_ids_lists.at(player_id)[i];
    id_object_refs[i] = value_object;
  }
  Dart_CObject ids_object;
  ids_object.type = Dart_CObject_kArray;
  ids_object.value.as_array.length = g_media_ids_lists.at(player_id).size();
  ids_object.value.as_array.values = id_object_refs.get();
  Dart_CObject* value_objects[] = {&player_id_object, &type_object,
                                   &uris_object, &ids_object};
  Dart_CObject return_object;
  return_object.type = Dart_CObject_kArray;
  return_object.value.as_array.length = 4;
  return_object.value.as_array.values = value_objects;
  g_dart_post_C_object(g_callback_port, &return_object);
#endif
#elif __linux__
  EnsureFuture(player_id);
  g_media_players.at(player_id)->Pause();
  g_media_players.at(player_id)->Add(uri, id);
#ifdef DART_VM
  Dart_CObject player_id_object;
  player_id_object.type = Dart_CObject_kInt32;
  player_id_object.value.as_int32 = player_id;
  Dart_CObject type_object;
  type_object.type = Dart_CObject_kString;
  type_object.value.as_string = const_cast<char*>("Open");
  auto uri_objects = std::unique_ptr<Dart_CObject[]>(
      new Dart_CObject[g_media_players.at(player_id)->media_uris().size()]);
  auto uri_object_refs = std::unique_ptr<Dart_CObject*[]>(
      new Dart_CObject*[g_media_players.at(player_id)->media_uris().size()]);
  std::vector<std::string> uris_str(
      g_media_players.at(player_id)->media_uris().size());
  std::vector<char*> uris_ptr(
      g_media_players.at(player_id)->media_uris().size());
  for (int32_t i = 0; i < g_media_players.at(player_id)->media_uris().size();
       i++) {
    uris_str[i] = g_media_players.at(player_id)->media_uris()[i];
    uris_ptr[i] = uris_str[i].data();
    Dart_CObject* value_object = &uri_objects[i];
    value_object->type = Dart_CObject_kString;
    value_object->value.as_string = uris_ptr[i];
    uri_object_refs[i] = value_object;
  }
  Dart_CObject uris_object;
  uris_object.type = Dart_CObject_kArray;
  uris_object.value.as_array.length =
      g_media_players.at(player_id)->media_uris().size();
  uris_object.value.as_array.values = uri_object_refs.get();
  auto id_objects = std::unique_ptr<Dart_CObject[]>(
      new Dart_CObject[g_media_players.at(player_id)->media_ids().size()]);
  auto id_object_refs = std::unique_ptr<Dart_CObject*[]>(
      new Dart_CObject*[g_media_players.at(player_id)->media_ids().size()]);
  for (int32_t i = 0; i < g_media_players.at(player_id)->media_ids().size();
       i++) {
    Dart_CObject* value_object = &id_objects[i];
    value_object->type = Dart_CObject_kInt32;
    value_object->value.as_int32 =
        g_media_players.at(player_id)->media_ids()[i];
    id_object_refs[i] = value_object;
  }
  Dart_CObject ids_object;
  ids_object.type = Dart_CObject_kArray;
  ids_object.value.as_array.length =
      g_media_players.at(player_id)->media_ids().size();
  ids_object.value.as_array.values = id_object_refs.get();
  Dart_CObject* value_objects[] = {&player_id_object, &type_object,
                                   &uris_object, &ids_object};
  Dart_CObject return_object;
  return_object.type = Dart_CObject_kArray;
  return_object.value.as_array.length = 4;
  return_object.value.as_array.values = value_objects;
  g_dart_post_C_object(g_callback_port, &return_object);
#endif
#endif
}

DLLEXPORT void PlayerRemove(int32_t player_id, int32_t index) {
#ifdef _WIN32
  g_media_playback_lists.at(player_id).Items().RemoveAt(index);
  g_media_ids_lists.at(player_id).erase(
      g_media_ids_lists.at(player_id).begin() + index);
#ifdef DART_VM
  Dart_CObject player_id_object;
  player_id_object.type = Dart_CObject_kInt32;
  player_id_object.value.as_int32 = player_id;
  Dart_CObject type_object;
  type_object.type = Dart_CObject_kString;
  type_object.value.as_string = "Open";
  auto uri_objects = std::unique_ptr<Dart_CObject[]>(
      new Dart_CObject[g_media_playback_lists.at(player_id).Items().Size()]);
  auto uri_object_refs = std::unique_ptr<Dart_CObject*[]>(
      new Dart_CObject*[g_media_playback_lists.at(player_id).Items().Size()]);
  std::vector<std::string> uris_str(
      g_media_playback_lists.at(player_id).Items().Size());
  std::vector<char*> uris_ptr(
      g_media_playback_lists.at(player_id).Items().Size());
  for (int32_t i = 0; i < g_media_playback_lists.at(player_id).Items().Size();
       i++) {
    uris_str[i] = TO_STRING(g_media_playback_lists.at(player_id)
                                .Items()
                                .GetAt(i)
                                .Source()
                                .Uri()
                                .ToString()
                                .c_str());
    uris_ptr[i] = uris_str[i].data();
    Dart_CObject* value_object = &uri_objects[i];
    value_object->type = Dart_CObject_kString;
    value_object->value.as_string = uris_ptr[i];
    uri_object_refs[i] = value_object;
  }
  Dart_CObject uris_object;
  uris_object.type = Dart_CObject_kArray;
  uris_object.value.as_array.length =
      g_media_playback_lists.at(player_id).Items().Size();
  uris_object.value.as_array.values = uri_object_refs.get();
  auto id_objects = std::unique_ptr<Dart_CObject[]>(
      new Dart_CObject[g_media_playback_lists.at(player_id).Items().Size()]);
  auto id_object_refs = std::unique_ptr<Dart_CObject*[]>(
      new Dart_CObject*[g_media_playback_lists.at(player_id).Items().Size()]);
  for (int32_t i = 0; i < g_media_ids_lists.at(player_id).size(); i++) {
    Dart_CObject* value_object = &id_objects[i];
    value_object->type = Dart_CObject_kInt32;
    value_object->value.as_int32 = g_media_ids_lists.at(player_id)[i];
    id_object_refs[i] = value_object;
  }
  Dart_CObject ids_object;
  ids_object.type = Dart_CObject_kArray;
  ids_object.value.as_array.length = g_media_ids_lists.at(player_id).size();
  ids_object.value.as_array.values = id_object_refs.get();
  Dart_CObject* value_objects[] = {&player_id_object, &type_object,
                                   &uris_object, &ids_object};
  Dart_CObject return_object;
  return_object.type = Dart_CObject_kArray;
  return_object.value.as_array.length = 4;
  return_object.value.as_array.values = value_objects;
  g_dart_post_C_object(g_callback_port, &return_object);
#endif
#elif __linux__
// TODO (alexmercerind): Add Linux support.
#endif
}

DLLEXPORT void PlayerNext(int32_t player_id) {
#ifdef _WIN32
  if (g_media_playback_lists.at(player_id).Items().Size() == 0) return;
  g_media_playback_lists.at(player_id).MoveNext();
#elif __linux__
  EnsureFuture(player_id);
  g_media_players.at(player_id)->Next();
#endif
}

DLLEXPORT void PlayerBack(int32_t player_id) {
#ifdef _WIN32
  if (g_media_playback_lists.at(player_id).Items().Size() == 0) return;
  g_media_playback_lists.at(player_id).MovePrevious();
#elif __linux__
  EnsureFuture(player_id);
  g_media_players.at(player_id)->Back();
#endif
}

DLLEXPORT void PlayerJump(int32_t player_id, int32_t index) {
#ifdef _WIN32
  if (g_media_playback_lists.at(player_id).Items().Size() == 0) return;
  g_media_playback_lists.at(player_id).MoveTo(index);
#elif __linux__
  EnsureFuture(player_id);
  g_media_players.at(player_id)->Jump(index);
#endif
}

DLLEXPORT void PlayerSeek(int32_t player_id, int32_t position) {
#ifdef _WIN32
  g_media_players.at(player_id).Position(
      TimeSpan(std::chrono::milliseconds(position)));
#elif __linux__
  g_media_players.at(player_id)->Seek(position);
#endif
}

DLLEXPORT void PlayerSetVolume(int32_t player_id, float volume) {
#ifdef _WIN32
  g_media_players.at(player_id).Volume(volume);
#elif __linux__
  EnsureFuture(player_id);
  g_media_players.at(player_id)->SetVolume(volume);
#endif
}

DLLEXPORT void PlayerSetRate(int32_t player_id, float rate) {
#ifdef _WIN32
  g_media_players.at(player_id).PlaybackRate(rate);
#elif __linux__
  g_media_players.at(player_id)->SetRate(rate);
#endif
}

DLLEXPORT void PlayerSetAudioBalance(int32_t player_id, float audio_balance) {
#ifdef _WIN32
  g_media_players.at(player_id).AudioBalance(audio_balance);
#elif __linux__
// TODO (alexmercerind): Add Linux support.
#endif
}

DLLEXPORT void PlayerSetAutoplay(int32_t player_id, bool autoplay) {
#ifdef _WIN32
  g_media_players.at(player_id).AutoPlay(autoplay);
#elif __linux__
// TODO (alexmercerind): Add Linux support.
#endif
}

DLLEXPORT void PlayerSetIsLooping(int32_t player_id, bool looping) {
#ifdef _WIN32
  g_media_players.at(player_id).IsLoopingEnabled(looping);
#elif __linux__
  EnsureFuture(player_id);
  g_media_players.at(player_id)->SetIsLooping(looping);
#endif
}

DLLEXPORT void PlayerSetIsAutoRepeat(int32_t player_id, bool autorepeat) {
#ifdef _WIN32
  g_media_playback_lists.at(player_id).AutoRepeatEnabled(autorepeat);
#elif __linux__
  EnsureFuture(player_id);
  g_media_players.at(player_id)->SetIsAutoRepeat(autorepeat);
#endif
}

DLLEXPORT void PlayerSetIsShuffling(int32_t player_id, bool shuffling) {
#ifdef _WIN32
  g_media_playback_lists.at(player_id).ShuffleEnabled(shuffling);
#elif __linux__
  EnsureFuture(player_id);
  g_media_players.at(player_id)->SetIsShuffling(shuffling);
#endif
}

DLLEXPORT int32_t PlayerGetPosition(int32_t player_id) {
#ifdef _WIN32
  return TO_MILLISECONDS(g_media_players.at(player_id).Position());
#elif __linux__
  EnsureFuture(player_id);
  return g_media_players.at(player_id)->position();
#endif
}

DLLEXPORT float PlayerGetVolume(int32_t player_id) {
#ifdef _WIN32
  return g_media_players.at(player_id).Volume();
#elif __linux__
  EnsureFuture(player_id);
  return g_media_players.at(player_id)->volume();
#endif
}

DLLEXPORT float PlayerGetRate(int32_t player_id) {
#ifdef _WIN32
  return g_media_players.at(player_id).PlaybackRate();
#elif __linux__
  EnsureFuture(player_id);
  return g_media_players.at(player_id)->rate();
#endif
}

DLLEXPORT float PlayerGetAudioBalance(int32_t player_id) {
#ifdef _WIN32
  return g_media_players.at(player_id).AudioBalance();
#elif __linux__
  return 1.0f;
#endif
}

DLLEXPORT float PlayerGetDownloadProgress(int32_t player_id) {
#ifdef _WIN32
  return g_media_players.at(player_id).PlaybackSession().DownloadProgress();
#elif __linux__
  EnsureFuture(player_id);
  return g_media_players.at(player_id)->download_progress();
#endif
}

DLLEXPORT bool PlayerIsAutoplay(int32_t player_id) {
#ifdef _WIN32
  return g_media_players.at(player_id).AutoPlay();
#elif __linux__
  return false;
#endif
}

DLLEXPORT bool PlayerIsLooping(int32_t player_id) {
#ifdef _WIN32
  return g_media_players.at(player_id).IsLoopingEnabled();
#elif __linux__
  EnsureFuture(player_id);
  return g_media_players.at(player_id)->is_looping();
#endif
}

DLLEXPORT bool PlayerIsAutoRepeat(int32_t player_id) {
#ifdef _WIN32
  return g_media_playback_lists.at(player_id).AutoRepeatEnabled();
#elif __linux__
  EnsureFuture(player_id);
  return g_media_players.at(player_id)->is_autorepeat();
#endif
}

DLLEXPORT bool PlayerIsShuffling(int32_t player_id) {
#ifdef _WIN32
  return g_media_playback_lists.at(player_id).ShuffleEnabled();
#elif __linux__
  EnsureFuture(player_id);
  return g_media_players.at(player_id)->is_shuffling();
#endif
}

// TODO (alexmercerind): Implement frame callbacks.
// Windows:
// Current summary:
// Tried calling `CopyFrameToVideoSurface` directly upon IDirect3DSurface, which
// results in accessing invalid memory location.
// Later on tried using win2D from NuGet, but it results in class not
// registered.
//
// DLLEXPORT void PlayerSetFrameEventHandler(
//     int32_t player_id, void (*callback)(uint8_t* buffer, int32_t size,
//                                         int32_t width, int32_t height)) {
//   g_media_players.at(player_id).IsVideoFrameServerEnabled(true);
//   winrt::Microsoft::Graphics::Canvas::CanvasDevice canvasDevice =
//       winrt::Microsoft::Graphics::Canvas::CanvasDevice::GetSharedDevice();
//   SoftwareBitmap bitmap = SoftwareBitmap(BitmapPixelFormat::Rgba8, 480, 360,
//                                          BitmapAlphaMode::Ignore);
//   winrt::Microsoft::Graphics::Canvas::CanvasBitmap surface =
//       winrt::Microsoft::Graphics::Canvas::CanvasBitmap::
//           CreateFromSoftwareBitmap(canvasDevice, bitmap);
//   g_media_players.at(player_id).VideoFrameAvailable([=](auto,
//                                                   const auto& args) -> void {
//     g_media_players.at(player_id).CopyFrameToVideoSurface(surface);
//     (*callback)(surface.GetPixelBytes().data(),
//     surface.GetPixelBytes().size(),
//                 bitmap.PixelWidth(), bitmap.PixelHeight());
//   });
// }

DLLEXPORT void PlayerSetIsPlayingEventHandler(
    int32_t player_id, void (*callback)(bool is_playing)) {
#ifdef _WIN32
  g_media_players.at(player_id).PlaybackSession().PlaybackStateChanged(
      [=](auto, const auto& args) -> void {
        if (g_media_players.at(player_id).PlaybackSession().PlaybackState() ==
            Playback::MediaPlaybackState::Playing) {
#ifdef DART_VM
          Dart_CObject player_id_object;
          player_id_object.type = Dart_CObject_kInt32;
          player_id_object.value.as_int32 = player_id;
          Dart_CObject type_object;
          type_object.type = Dart_CObject_kString;
          type_object.value.as_string = "IsPlaying";
          Dart_CObject is_playing_object;
          is_playing_object.type = Dart_CObject_kBool;
          is_playing_object.value.as_bool = true;
          Dart_CObject* value_objects[] = {&player_id_object, &type_object,
                                           &is_playing_object};
          Dart_CObject return_object;
          return_object.type = Dart_CObject_kArray;
          return_object.value.as_array.length = 3;
          return_object.value.as_array.values = value_objects;
          g_dart_post_C_object(g_callback_port, &return_object);
#else
          (*callback)(true);
#endif
        }
        if (g_media_players.at(player_id).PlaybackSession().PlaybackState() ==
            Playback::MediaPlaybackState::Paused) {
#ifdef DART_VM
          Dart_CObject player_id_object;
          player_id_object.type = Dart_CObject_kInt32;
          player_id_object.value.as_int32 = player_id;
          Dart_CObject type_object;
          type_object.type = Dart_CObject_kString;
          type_object.value.as_string = "IsPlaying";
          Dart_CObject is_playing_object;
          is_playing_object.type = Dart_CObject_kBool;
          is_playing_object.value.as_bool = false;
          Dart_CObject* value_objects[] = {&player_id_object, &type_object,
                                           &is_playing_object};
          Dart_CObject return_object;
          return_object.type = Dart_CObject_kArray;
          return_object.value.as_array.length = 3;
          return_object.value.as_array.values = value_objects;
          g_dart_post_C_object(g_callback_port, &return_object);
#else
          (*callback)(false);
#endif
        }
      });
#elif __linux__
  EnsureFuture(player_id);
  g_media_players.at(player_id)->SetIsPlayingEventHandler(
      [=](bool is_playing) -> void {
#ifdef DART_VM
        Dart_CObject player_id_object;
        player_id_object.type = Dart_CObject_kInt32;
        player_id_object.value.as_int32 = player_id;
        Dart_CObject type_object;
        type_object.type = Dart_CObject_kString;
        type_object.value.as_string = const_cast<char*>("IsPlaying");
        Dart_CObject is_playing_object;
        is_playing_object.type = Dart_CObject_kBool;
        is_playing_object.value.as_bool = is_playing;
        Dart_CObject* value_objects[] = {&player_id_object, &type_object,
                                         &is_playing_object};
        Dart_CObject return_object;
        return_object.type = Dart_CObject_kArray;
        return_object.value.as_array.length = 3;
        return_object.value.as_array.values = value_objects;
        g_dart_post_C_object(g_callback_port, &return_object);
#else
        (*callback)(is_playing);
#endif
      });
#endif
}

DLLEXPORT void PlayerSetIsCompletedEventHandler(
    int32_t player_id, void (*callback)(bool is_completed)) {
#ifdef _WIN32
  g_media_players.at(player_id).MediaEnded([=](auto, const auto& args) -> void {
#ifdef DART_VM
    Dart_CObject player_id_object;
    player_id_object.type = Dart_CObject_kInt32;
    player_id_object.value.as_int32 = player_id;
    Dart_CObject type_object;
    type_object.type = Dart_CObject_kString;
    type_object.value.as_string = "IsCompleted";
    Dart_CObject is_completed_object;
    is_completed_object.type = Dart_CObject_kBool;
    is_completed_object.value.as_bool = true;
    Dart_CObject* value_objects[] = {&player_id_object, &type_object,
                                     &is_completed_object};
    Dart_CObject return_object;
    return_object.type = Dart_CObject_kArray;
    return_object.value.as_array.length = 3;
    return_object.value.as_array.values = value_objects;
    g_dart_post_C_object(g_callback_port, &return_object);
#else
    (*callback)(true);
#endif
  });
  g_media_players.at(player_id).PlaybackSession().PlaybackStateChanged(
      [=](auto, const auto& args) -> void {
#ifdef DART_VM
        Dart_CObject player_id_object;
        player_id_object.type = Dart_CObject_kInt32;
        player_id_object.value.as_int32 = player_id;
        Dart_CObject type_object;
        type_object.type = Dart_CObject_kString;
        type_object.value.as_string = "IsCompleted";
        Dart_CObject is_completed_object;
        is_completed_object.type = Dart_CObject_kBool;
        is_completed_object.value.as_bool = false;
        Dart_CObject* value_objects[] = {&player_id_object, &type_object,
                                         &is_completed_object};
        Dart_CObject return_object;
        return_object.type = Dart_CObject_kArray;
        return_object.value.as_array.length = 3;
        return_object.value.as_array.values = value_objects;
        g_dart_post_C_object(g_callback_port, &return_object);
#else
        (*callback)(false);
#endif
      });
#elif __linux__
  EnsureFuture(player_id);
  g_media_players.at(player_id)->SetIsCompletedEventHandler(
      [=](bool is_completed) -> void {
#ifdef DART_VM
        Dart_CObject player_id_object;
        player_id_object.type = Dart_CObject_kInt32;
        player_id_object.value.as_int32 = player_id;
        Dart_CObject type_object;
        type_object.type = Dart_CObject_kString;
        type_object.value.as_string = const_cast<char*>("IsCompleted");
        Dart_CObject is_completed_object;
        is_completed_object.type = Dart_CObject_kBool;
        is_completed_object.value.as_bool = is_completed;
        Dart_CObject* value_objects[] = {&player_id_object, &type_object,
                                         &is_completed_object};
        Dart_CObject return_object;
        return_object.type = Dart_CObject_kArray;
        return_object.value.as_array.length = 3;
        return_object.value.as_array.values = value_objects;
        g_dart_post_C_object(g_callback_port, &return_object);
#else
        (*callback)(is_completed);
#endif
      });
#endif
}

DLLEXPORT void PlayerSetIsBufferingEventHandler(
    int32_t player_id, void (*callback)(bool is_buffering)) {
#ifdef _WIN32
  g_media_players.at(player_id).BufferingStarted(
      [=](auto, const auto& args) -> void {
#ifdef DART_VM
        Dart_CObject player_id_object;
        player_id_object.type = Dart_CObject_kInt32;
        player_id_object.value.as_int32 = player_id;
        Dart_CObject type_object;
        type_object.type = Dart_CObject_kString;
        type_object.value.as_string = "IsBuffering";
        Dart_CObject is_buffering_object;
        is_buffering_object.type = Dart_CObject_kBool;
        is_buffering_object.value.as_bool = true;
        Dart_CObject* value_objects[] = {&player_id_object, &type_object,
                                         &is_buffering_object};
        Dart_CObject return_object;
        return_object.type = Dart_CObject_kArray;
        return_object.value.as_array.length = 3;
        return_object.value.as_array.values = value_objects;
        g_dart_post_C_object(g_callback_port, &return_object);
#else
        (*callback)(true);
#endif
      });
  g_media_players.at(player_id).BufferingEnded(
      [=](auto, const auto& args) -> void {
#ifdef DART_VM
        Dart_CObject player_id_object;
        player_id_object.type = Dart_CObject_kInt32;
        player_id_object.value.as_int32 = player_id;
        Dart_CObject type_object;
        type_object.type = Dart_CObject_kString;
        type_object.value.as_string = "IsBuffering";
        Dart_CObject is_buffering_object;
        is_buffering_object.type = Dart_CObject_kBool;
        is_buffering_object.value.as_bool = false;
        Dart_CObject* value_objects[] = {&player_id_object, &type_object,
                                         &is_buffering_object};
        Dart_CObject return_object;
        return_object.type = Dart_CObject_kArray;
        return_object.value.as_array.length = 3;
        return_object.value.as_array.values = value_objects;
        g_dart_post_C_object(g_callback_port, &return_object);
#else
        (*callback)(false);
#endif
      });
#elif __linux__
  EnsureFuture(player_id);
  g_media_players.at(player_id)->SetIsBufferingEventHandler(
      [=](bool is_buffering) -> void {
#ifdef DART_VM
        Dart_CObject player_id_object;
        player_id_object.type = Dart_CObject_kInt32;
        player_id_object.value.as_int32 = player_id;
        Dart_CObject type_object;
        type_object.type = Dart_CObject_kString;
        type_object.value.as_string = const_cast<char*>("IsBuffering");
        Dart_CObject is_buffering_object;
        is_buffering_object.type = Dart_CObject_kBool;
        is_buffering_object.value.as_bool = is_buffering;
        Dart_CObject* value_objects[] = {&player_id_object, &type_object,
                                         &is_buffering_object};
        Dart_CObject return_object;
        return_object.type = Dart_CObject_kArray;
        return_object.value.as_array.length = 3;
        return_object.value.as_array.values = value_objects;
        g_dart_post_C_object(g_callback_port, &return_object);
#else
        (*callback)(is_buffering);
#endif
      });
#endif
}

DLLEXPORT void PlayerSetVolumeEventHandler(int32_t player_id,
                                           void (*callback)(float volume)) {
#ifdef _WIN32
  g_media_players.at(player_id).VolumeChanged(
      [=](auto, const auto& args) -> void {
#ifdef DART_VM
        Dart_CObject player_id_object;
        player_id_object.type = Dart_CObject_kInt32;
        player_id_object.value.as_int32 = player_id;
        Dart_CObject type_object;
        type_object.type = Dart_CObject_kString;
        type_object.value.as_string = "Volume";
        Dart_CObject volume_object;
        volume_object.type = Dart_CObject_kDouble;
        volume_object.value.as_double = g_media_players.at(player_id).Volume();
        Dart_CObject* value_objects[] = {&player_id_object, &type_object,
                                         &volume_object};
        Dart_CObject return_object;
        return_object.type = Dart_CObject_kArray;
        return_object.value.as_array.length = 3;
        return_object.value.as_array.values = value_objects;
        g_dart_post_C_object(g_callback_port, &return_object);
#else
        (*callback)(g_media_players.at(player_id).Volume());
#endif
      });
#elif __linux__
  EnsureFuture(player_id);
  g_media_players.at(player_id)->SetVolumeEventHandler(
      [=](float volume) -> void {
#ifdef DART_VM
        Dart_CObject player_id_object;
        player_id_object.type = Dart_CObject_kInt32;
        player_id_object.value.as_int32 = player_id;
        Dart_CObject type_object;
        type_object.type = Dart_CObject_kString;
        type_object.value.as_string = const_cast<char*>("Volume");
        Dart_CObject volume_object;
        volume_object.type = Dart_CObject_kDouble;
        volume_object.value.as_double = volume;
        Dart_CObject* value_objects[] = {&player_id_object, &type_object,
                                         &volume_object};
        Dart_CObject return_object;
        return_object.type = Dart_CObject_kArray;
        return_object.value.as_array.length = 3;
        return_object.value.as_array.values = value_objects;
        g_dart_post_C_object(g_callback_port, &return_object);
#else
        (*callback)(volume);
#endif
      });
#endif
}

DLLEXPORT void PlayerSetRateEventHandler(int32_t player_id,
                                         void (*callback)(float rate)) {
#ifdef _WIN32
  g_media_players.at(player_id).MediaPlayerRateChanged(
      [=](auto, const auto& args) -> void {
#ifdef DART_VM
        Dart_CObject player_id_object;
        player_id_object.type = Dart_CObject_kInt32;
        player_id_object.value.as_int32 = player_id;
        Dart_CObject type_object;
        type_object.type = Dart_CObject_kString;
        type_object.value.as_string = "Rate";
        Dart_CObject rate_object;
        rate_object.type = Dart_CObject_kDouble;
        rate_object.value.as_double =
            g_media_players.at(player_id).PlaybackRate();
        Dart_CObject* value_objects[] = {&player_id_object, &type_object,
                                         &rate_object};
        Dart_CObject return_object;
        return_object.type = Dart_CObject_kArray;
        return_object.value.as_array.length = 3;
        return_object.value.as_array.values = value_objects;
        g_dart_post_C_object(g_callback_port, &return_object);
#else
        (*callback)(g_media_players.at(player_id).PlaybackRate());
#endif
      });
#elif __linux__
  EnsureFuture(player_id);
  g_media_players.at(player_id)->SetRateEventHandler([=](float rate) -> void {
#ifdef DART_VM
    Dart_CObject player_id_object;
    player_id_object.type = Dart_CObject_kInt32;
    player_id_object.value.as_int32 = player_id;
    Dart_CObject type_object;
    type_object.type = Dart_CObject_kString;
    type_object.value.as_string = const_cast<char*>("Rate");
    Dart_CObject rate_object;
    rate_object.type = Dart_CObject_kDouble;
    rate_object.value.as_double = rate;
    Dart_CObject* value_objects[] = {&player_id_object, &type_object,
                                     &rate_object};
    Dart_CObject return_object;
    return_object.type = Dart_CObject_kArray;
    return_object.value.as_array.length = 3;
    return_object.value.as_array.values = value_objects;
    g_dart_post_C_object(g_callback_port, &return_object);
#else
    (*callback)(rate);
#endif
  });
#endif
}

DLLEXPORT void PlayerSetPositionEventHandler(
    int32_t player_id, void (*callback)(int32_t position)) {
#ifdef _WIN32
  g_media_players.at(player_id).PlaybackSession().PositionChanged(
      [=](auto, const auto& args) -> void {
#ifdef DART_VM
        Dart_CObject player_id_object;
        player_id_object.type = Dart_CObject_kInt32;
        player_id_object.value.as_int32 = player_id;
        Dart_CObject type_object;
        type_object.type = Dart_CObject_kString;
        type_object.value.as_string = "Position";
        Dart_CObject position_object;
        position_object.type = Dart_CObject_kInt32;
        position_object.value.as_int32 = TO_MILLISECONDS(
            g_media_players.at(player_id).PlaybackSession().Position());
        Dart_CObject* value_objects[] = {&player_id_object, &type_object,
                                         &position_object};
        Dart_CObject return_object;
        return_object.type = Dart_CObject_kArray;
        return_object.value.as_array.length = 3;
        return_object.value.as_array.values = value_objects;
        g_dart_post_C_object(g_callback_port, &return_object);
#else
        (*callback)(TO_MILLISECONDS(
            g_media_players.at(player_id).PlaybackSession().Position()));
#endif
      });
#elif __linux__
  EnsureFuture(player_id);
  g_media_players.at(player_id)->SetPositionEventHandler(
      [=](int32_t position) -> void {
#ifdef DART_VM
        Dart_CObject player_id_object;
        player_id_object.type = Dart_CObject_kInt32;
        player_id_object.value.as_int32 = player_id;
        Dart_CObject type_object;
        type_object.type = Dart_CObject_kString;
        type_object.value.as_string = const_cast<char*>("Position");
        Dart_CObject position_object;
        position_object.type = Dart_CObject_kInt32;
        position_object.value.as_int32 = position;
        Dart_CObject* value_objects[] = {&player_id_object, &type_object,
                                         &position_object};
        Dart_CObject return_object;
        return_object.type = Dart_CObject_kArray;
        return_object.value.as_array.length = 3;
        return_object.value.as_array.values = value_objects;
        g_dart_post_C_object(g_callback_port, &return_object);
#else
        (*callback)(position);
#endif
      });
#endif
}

DLLEXPORT void PlayerSetDurationEventHandler(
    int32_t player_id, void (*callback)(int32_t duration)) {
#ifdef _WIN32
  g_media_players.at(player_id).PlaybackSession().NaturalDurationChanged(
      [=](auto, const auto& args) -> void {
#ifdef DART_VM
        Dart_CObject player_id_object;
        player_id_object.type = Dart_CObject_kInt32;
        player_id_object.value.as_int32 = player_id;
        Dart_CObject type_object;
        type_object.type = Dart_CObject_kString;
        type_object.value.as_string = "Duration";
        Dart_CObject duration_object;
        duration_object.type = Dart_CObject_kInt32;
        duration_object.value.as_int32 = TO_MILLISECONDS(
            g_media_players.at(player_id).PlaybackSession().NaturalDuration());
        Dart_CObject* value_objects[] = {&player_id_object, &type_object,
                                         &duration_object};
        Dart_CObject return_object;
        return_object.type = Dart_CObject_kArray;
        return_object.value.as_array.length = 3;
        return_object.value.as_array.values = value_objects;
        g_dart_post_C_object(g_callback_port, &return_object);
#else
        (*callback)(TO_MILLISECONDS(
            g_media_players.at(player_id).PlaybackSession().NaturalDuration()));
#endif
      });
#elif __linux__
  EnsureFuture(player_id);
  g_media_players.at(player_id)->SetDurationEventHandler(
      [=](int32_t duration) -> void {
#ifdef DART_VM
        Dart_CObject player_id_object;
        player_id_object.type = Dart_CObject_kInt32;
        player_id_object.value.as_int32 = player_id;
        Dart_CObject type_object;
        type_object.type = Dart_CObject_kString;
        type_object.value.as_string = const_cast<char*>("Duration");
        Dart_CObject duration_object;
        duration_object.type = Dart_CObject_kInt32;
        duration_object.value.as_int32 = duration;
        Dart_CObject* value_objects[] = {&player_id_object, &type_object,
                                         &duration_object};
        Dart_CObject return_object;
        return_object.type = Dart_CObject_kArray;
        return_object.value.as_array.length = 3;
        return_object.value.as_array.values = value_objects;
        g_dart_post_C_object(g_callback_port, &return_object);
#else
        (*callback)(duration);
#endif
      });
#endif
}

DLLEXPORT void PlayerSetIndexEventHandler(int32_t player_id,
                                          void (*callback)(int32_t index)) {
#ifdef _WIN32
  g_media_playback_lists.at(player_id).CurrentItemChanged(
      [=](auto, const auto& args) -> void {
#ifdef DART_VM
        Dart_CObject player_id_object;
        player_id_object.type = Dart_CObject_kInt32;
        player_id_object.value.as_int32 = player_id;
        Dart_CObject type_object;
        type_object.type = Dart_CObject_kString;
        type_object.value.as_string = "Index";
        Dart_CObject index_object;
        index_object.type = Dart_CObject_kInt32;
        index_object.value.as_int32 =
            g_media_playback_lists.at(player_id).CurrentItemIndex();
        Dart_CObject* value_objects[] = {&player_id_object, &type_object,
                                         &index_object};
        Dart_CObject return_object;
        return_object.type = Dart_CObject_kArray;
        return_object.value.as_array.length = 3;
        return_object.value.as_array.values = value_objects;
        g_dart_post_C_object(g_callback_port, &return_object);
#else
        (*callback)(g_media_playback_lists.at(player_id).CurrentItemIndex());
#endif
      });
#elif __linux__
  EnsureFuture(player_id);
  g_media_players.at(player_id)->SetIndexEventHandler(
      [=](int32_t index) -> void {
#ifdef DART_VM
        Dart_CObject player_id_object;
        player_id_object.type = Dart_CObject_kInt32;
        player_id_object.value.as_int32 = player_id;
        Dart_CObject type_object;
        type_object.type = Dart_CObject_kString;
        type_object.value.as_string = const_cast<char*>("Index");
        Dart_CObject index_object;
        index_object.type = Dart_CObject_kInt32;
        index_object.value.as_int32 = index;
        Dart_CObject* value_objects[] = {&player_id_object, &type_object,
                                         &index_object};
        Dart_CObject return_object;
        return_object.type = Dart_CObject_kArray;
        return_object.value.as_array.length = 3;
        return_object.value.as_array.values = value_objects;
        g_dart_post_C_object(g_callback_port, &return_object);
#else
        (*callback)(index);
#endif
      });
#endif
}

DLLEXPORT void PlayerSetDownloadProgressEventHandler(
    int32_t player_id, void (*callback)(float download_progress)) {
#ifdef _WIN32
  g_media_players.at(player_id).PlaybackSession().DownloadProgressChanged(
      [=](auto, const auto& args) -> void {
#ifdef DART_VM
        Dart_CObject player_id_object;
        player_id_object.type = Dart_CObject_kInt32;
        player_id_object.value.as_int32 = player_id;
        Dart_CObject type_object;
        type_object.type = Dart_CObject_kString;
        type_object.value.as_string = "DownloadProgress";
        Dart_CObject download_progress_object;
        download_progress_object.type = Dart_CObject_kDouble;
        download_progress_object.value.as_double = static_cast<double>(
            g_media_players.at(player_id).PlaybackSession().DownloadProgress());
        Dart_CObject* value_objects[] = {&player_id_object, &type_object,
                                         &download_progress_object};
        Dart_CObject return_object;
        return_object.type = Dart_CObject_kArray;
        return_object.value.as_array.length = 3;
        return_object.value.as_array.values = value_objects;
        g_dart_post_C_object(g_callback_port, &return_object);
#else
        (*callback)(
            g_media_players.at(player_id).PlaybackSession().DownloadProgress());

#endif
      });
#elif __linux__
  g_media_players.at(player_id)->SetDownloadProgressEventHandler(
      [=](float download_progress) {
#ifdef DART_VM
        Dart_CObject player_id_object;
        player_id_object.type = Dart_CObject_kInt32;
        player_id_object.value.as_int32 = player_id;
        Dart_CObject type_object;
        type_object.type = Dart_CObject_kString;
        type_object.value.as_string = const_cast<char*>("DownloadProgress");
        Dart_CObject download_progress_object;
        download_progress_object.type = Dart_CObject_kDouble;
        download_progress_object.value.as_double =
            static_cast<double>(download_progress);
        Dart_CObject* value_objects[] = {&player_id_object, &type_object,
                                         &download_progress_object};
        Dart_CObject return_object;
        return_object.type = Dart_CObject_kArray;
        return_object.value.as_array.length = 3;
        return_object.value.as_array.values = value_objects;
        g_dart_post_C_object(g_callback_port, &return_object);
#else
        (*callback)(download_progress);

#endif
      });
#endif
}

DLLEXPORT void PlayerSetErrorEventHandler(int32_t player_id,
                                          void (*callback)(int32_t code,
                                                           char* message)) {
#ifdef _WIN32
  g_media_players.at(player_id).MediaFailed(
      [=](auto, const Playback::MediaPlayerFailedEventArgs& args) -> void {
        winrt::hstring error = args.ErrorMessage();
        std::string error_str =
            TO_STRING(std::wstring(error.begin(), error.end()));
        char* error_ptr = const_cast<char*>(error_str.c_str());
#ifdef DART_VM
        Dart_CObject player_id_object;
        player_id_object.type = Dart_CObject_kInt32;
        player_id_object.value.as_int32 = player_id;
        Dart_CObject type_object;
        type_object.type = Dart_CObject_kString;
        type_object.value.as_string = "Error";
        Dart_CObject error_code_object;
        error_code_object.type = Dart_CObject_kInt32;
        error_code_object.value.as_int32 = static_cast<int32_t>(args.Error());
        Dart_CObject error_message_object;
        error_message_object.type = Dart_CObject_kString;
        error_message_object.value.as_string = error_ptr;
        Dart_CObject* value_objects[] = {&player_id_object, &type_object,
                                         &error_code_object,
                                         &error_message_object};
        Dart_CObject return_object;
        return_object.type = Dart_CObject_kArray;
        return_object.value.as_array.length = 4;
        return_object.value.as_array.values = value_objects;
        g_dart_post_C_object(g_callback_port, &return_object);
#else
        (*callback)(static_cast<int32_t>(args.Error()), error_ptr);

#endif
      });
#elif __linux__
  g_media_players.at(player_id)->SetErrorEventHandler([=](std::string error) {
#ifdef DART_VM
    Dart_CObject player_id_object;
    player_id_object.type = Dart_CObject_kInt32;
    player_id_object.value.as_int32 = player_id;
    Dart_CObject type_object;
    type_object.type = Dart_CObject_kString;
    type_object.value.as_string = const_cast<char*>("Error");
    Dart_CObject error_code_object;
    error_code_object.type = Dart_CObject_kInt32;
    error_code_object.value.as_int32 = 0;
    Dart_CObject error_message_object;
    error_message_object.type = Dart_CObject_kString;
    error_message_object.value.as_string = const_cast<char*>(error.c_str());
    Dart_CObject* value_objects[] = {&player_id_object, &type_object,
                                     &error_code_object, &error_message_object};
    Dart_CObject return_object;
    return_object.type = Dart_CObject_kArray;
    return_object.value.as_array.length = 4;
    return_object.value.as_array.values = value_objects;
    g_dart_post_C_object(g_callback_port, &return_object);
#else
    (*callback)(0, const_cast<char*>(error.c_str()));
#endif
  });
#endif
}

// TODO (alexmercerind): Add native controls, tag & media parsing on Linux.

#ifdef _WIN32

DLLEXPORT void PlayerNativeControlsCreate(int32_t player_id,
                                          void (*callback)(int32_t button)) {
  if (g_smtc_exist) return;
  g_smtc_exist = true;
  SystemMediaTransportControls controls =
      g_media_players.at(player_id).SystemMediaTransportControls();
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
        (*callback)(static_cast<int32_t>(args.Button()));
      });
}

DLLEXPORT void PlayerNativeControlsSetStatus(int32_t player_id,
                                             int32_t status) {
  SystemMediaTransportControls controls =
      Playback::BackgroundMediaPlayer::Current().SystemMediaTransportControls();
  controls.PlaybackStatus(static_cast<MediaPlaybackStatus>(status));
}

DLLEXPORT void PlayerNativeControlsUpdate(int32_t player_id, int32_t type,
                                          char** data, const char* thumbnail) {
  SystemMediaTransportControls controls =
      g_media_players.at(player_id).SystemMediaTransportControls();
  SystemMediaTransportControlsDisplayUpdater updater =
      controls.DisplayUpdater();
  updater.Type(static_cast<MediaPlaybackType>(type));
  if (type == 1) {
    MusicDisplayProperties properties = updater.MusicProperties();
    properties.AlbumArtist(TO_WIDESTRING(data[0]));
    properties.AlbumTitle(TO_WIDESTRING(data[1]));
    try {
      properties.AlbumTrackCount(std::stoi(data[2]));
    } catch (...) {
    }
    properties.Artist(TO_WIDESTRING(data[3]));
    properties.Title(TO_WIDESTRING(data[4]));
    try {
      properties.TrackNumber(std::stoi(TO_WIDESTRING(data[5])));
    } catch (...) {
    }
  } else if (type == 2) {
    VideoDisplayProperties properties = updater.VideoProperties();
    properties.Title(TO_WIDESTRING(data[0]));
    properties.Subtitle(TO_WIDESTRING(data[1]));
  }
  if (thumbnail != "") {
    updater.Thumbnail(Streams::RandomAccessStreamReference::CreateFromUri(
        Uri(TO_WIDESTRING(thumbnail))));
  }
  updater.Update();
}

DLLEXPORT void PlayerNativeControlsClear(int32_t player_id) {
  SystemMediaTransportControls controls =
      g_media_players.at(player_id).SystemMediaTransportControls();
  controls.DisplayUpdater().ClearAll();
}

DLLEXPORT void PlayerNativeControlsDispose(int32_t player_id) {
  g_media_players.at(player_id).SystemMediaTransportControls().IsEnabled(false);
}

/* Media */

DLLEXPORT void MediaCreate(int32_t media_id, const char* uri,
                           bool parse = false) {
  g_medias.insert(std::make_pair(
      media_id, Core::MediaSource::CreateFromUri(Uri(TO_WIDESTRING(uri)))));
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

DLLEXPORT char** TagsFromMusic(const char* uri) {
  FileProperties::StorageItemContentProperties properties =
      StorageFile::GetFileFromPathAsync(TO_WIDESTRING(uri)).get().Properties();
  FileProperties::MusicProperties music =
      properties.GetMusicPropertiesAsync().get();
  std::string string = "";
  char** tags = new char*[15];
  for (int32_t index = 0; index < 15; index++) tags[index] = new char[TAG_SIZE];
  strncpy(tags[0], TO_STRING(music.Album().c_str()).c_str(), TAG_SIZE);
  strncpy(tags[1], TO_STRING(music.AlbumArtist().c_str()).c_str(), TAG_SIZE);
  strncpy(tags[2], std::to_string(music.Bitrate()).c_str(), TAG_SIZE);
  Collections::IVector<winrt::hstring> composers = music.Composers();
  for (int32_t index = 0; index < composers.Size(); index++) {
    string += TO_STRING(composers.GetAt(index).c_str());
    string += " ";
  }
  strncpy(tags[3], string.c_str(), TAG_SIZE);
  string.clear();
  Collections::IVector<winrt::hstring> conductors = music.Conductors();
  for (int32_t index = 0; index < conductors.Size(); index++) {
    string += TO_STRING(conductors.GetAt(index).c_str());
    string += " ";
  }
  strncpy(tags[4], string.c_str(), TAG_SIZE);
  string.clear();
  strncpy(tags[5], std::to_string(TO_MILLISECONDS(music.Duration())).c_str(),
          TAG_SIZE);
  Collections::IVector<winrt::hstring> genre = music.Genre();
  for (int32_t index = 0; index < genre.Size(); index++) {
    string += TO_STRING(genre.GetAt(index).c_str());
    string += " ";
  }
  strncpy(tags[6], string.c_str(), TAG_SIZE);
  string.clear();
  Collections::IVector<winrt::hstring> producers = music.Producers();
  for (int32_t index = 0; index < producers.Size(); index++) {
    string = TO_STRING(producers.GetAt(index).c_str());
    string = " ";
  }
  strncpy(tags[7], string.c_str(), TAG_SIZE);
  string.clear();
  strncpy(tags[8], TO_STRING(music.Publisher().c_str()).c_str(), TAG_SIZE);
  strncpy(tags[9], std::to_string(music.Rating()).c_str(), TAG_SIZE);
  strncpy(tags[10], TO_STRING(music.Subtitle().c_str()).c_str(), TAG_SIZE);
  strncpy(tags[11], TO_STRING(music.Title().c_str()).c_str(), TAG_SIZE);
  strncpy(tags[12], std::to_string(music.TrackNumber()).c_str(), TAG_SIZE);
  Collections::IVector<winrt::hstring> writers = music.Writers();
  for (int32_t index = 0; index < writers.Size(); index++) {
    string += TO_STRING(writers.GetAt(index).c_str());
    string += " ";
  }
  strncpy(tags[13], string.c_str(), TAG_SIZE);
  string.clear();
  strncpy(tags[14], std::to_string(music.Year()).c_str(), TAG_SIZE);
  return tags;
}

DLLEXPORT char** TagsFromVideo(const char* uri) {
  FileProperties::StorageItemContentProperties properties =
      StorageFile::GetFileFromPathAsync(TO_WIDESTRING(uri)).get().Properties();
  FileProperties::VideoProperties video =
      properties.GetVideoPropertiesAsync().get();
  std::string string = "";
  char** tags = new char*[16];
  for (int32_t index = 0; index < 16; index++) tags[index] = new char[TAG_SIZE];
  strncpy(tags[0], std::to_string(video.Bitrate()).c_str(), TAG_SIZE);
  Collections::IVector<winrt::hstring> directors = video.Directors();
  for (int32_t index = 0; index < directors.Size(); index++) {
    string += TO_STRING(directors.GetAt(index).c_str());
    string += " ";
  }
  strncpy(tags[1], string.c_str(), TAG_SIZE);
  string.clear();
  strncpy(tags[2], std::to_string(TO_MILLISECONDS(video.Duration())).c_str(),
          TAG_SIZE);
  strncpy(tags[3], std::to_string(video.Height()).c_str(), TAG_SIZE);
  Collections::IVector<winrt::hstring> keywords = video.Keywords();
  for (int32_t index = 0; index < keywords.Size(); index++) {
    string += TO_STRING(keywords.GetAt(index).c_str());
    string += " ";
  }
  strncpy(tags[4], string.c_str(), TAG_SIZE);
  string.clear();
  // TODO (alexmercerind): Handle VideoProperties::Latitude &
  // VideoProperties::Longitude.
  strncpy(tags[5], "-1", TAG_SIZE);
  strncpy(tags[6], "-1", TAG_SIZE);
  strncpy(tags[7],
          std::to_string(static_cast<uint32_t>(video.Orientation())).c_str(),
          TAG_SIZE);
  Collections::IVector<winrt::hstring> producers = video.Producers();
  for (int32_t index = 0; index < producers.Size(); index++) {
    string += TO_STRING(producers.GetAt(index).c_str());
    string += " ";
  }
  strncpy(tags[8], string.c_str(), TAG_SIZE);
  string.clear();
  strncpy(tags[9], TO_STRING(video.Publisher().c_str()).c_str(), TAG_SIZE);
  strncpy(tags[10], std::to_string(video.Rating()).c_str(), TAG_SIZE);
  strncpy(tags[11], TO_STRING(video.Subtitle().c_str()).c_str(), TAG_SIZE);
  strncpy(tags[12], TO_STRING(video.Title().c_str()).c_str(), TAG_SIZE);
  strncpy(tags[13], std::to_string(video.Width()).c_str(), TAG_SIZE);
  Collections::IVector<winrt::hstring> writers = video.Writers();
  for (int32_t index = 0; index < writers.Size(); index++) {
    string += TO_STRING(writers.GetAt(index).c_str());
    string += " ";
  }
  strncpy(tags[14], string.c_str(), TAG_SIZE);
  string.clear();
  strncpy(tags[15], std::to_string(video.Year()).c_str(), TAG_SIZE);
  return tags;
}

DLLEXPORT void TagsExtractThumbnail(const char* media, const char* folder,
                                    const char* file_name, int32_t mode,
                                    int32_t size) {
  StorageFile source_file =
      StorageFile::GetFileFromPathAsync(TO_WIDESTRING(media)).get();
  StorageFolder storage_folder =
      StorageFolder::GetFolderFromPathAsync(TO_WIDESTRING(folder)).get();
  storage_folder
      .CreateFileAsync(TO_WIDESTRING(file_name),
                       CreationCollisionOption::ReplaceExisting)
      .get();
  StorageFile storage_file =
      storage_folder.GetFileAsync(TO_WIDESTRING(file_name)).get();
  FileProperties::StorageItemThumbnail thumbnail =
      source_file
          .GetThumbnailAsync(static_cast<FileProperties::ThumbnailMode>(mode),
                             size)
          .get();
  Streams::Buffer buffer = Streams::Buffer(thumbnail.Size());
  thumbnail
      .ReadAsync(buffer, thumbnail.Size(),
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
#ifdef DART_VM
        Dart_CObject player_id_object;
        player_id_object.type = Dart_CObject_kInt32;
        player_id_object.value.as_int32 = 0;
        Dart_CObject type_object;
        type_object.type = Dart_CObject_kString;
        type_object.value.as_string = "NativeControls";
        Dart_CObject native_controls_object;
        native_controls_object.type = Dart_CObject_kInt32;
        native_controls_object.value.as_int32 =
            static_cast<int32_t>(args.Button());
        Dart_CObject* value_objects[] = {&player_id_object, &type_object,
                                         &native_controls_object};
        Dart_CObject return_object;
        return_object.type = Dart_CObject_kArray;
        return_object.value.as_array.length = 3;
        return_object.value.as_array.values = value_objects;
        g_dart_post_C_object(g_callback_port, &return_object);
#else
        (*callback)(static_cast<int32_t>(args.Button()));
#endif
      });
}

DLLEXPORT void NativeControlsSetStatus(int32_t status) {
  SystemMediaTransportControls controls =
      Playback::BackgroundMediaPlayer::Current().SystemMediaTransportControls();
  controls.PlaybackStatus(static_cast<MediaPlaybackStatus>(status));
}

DLLEXPORT void NativeControlsUpdate(int32_t type, char** data,
                                    const char* thumbnail) {
  SystemMediaTransportControls controls =
      Playback::BackgroundMediaPlayer::Current().SystemMediaTransportControls();
  SystemMediaTransportControlsDisplayUpdater updater =
      controls.DisplayUpdater();
  updater.Type(static_cast<MediaPlaybackType>(type));
  if (type == 1) {
    MusicDisplayProperties properties = updater.MusicProperties();
    properties.AlbumArtist(TO_WIDESTRING(data[0]));
    properties.AlbumTitle(TO_WIDESTRING(data[1]));
    try {
      properties.AlbumTrackCount(std::stoi(data[2]));
    } catch (...) {
    }
    properties.Artist(TO_WIDESTRING(data[3]));
    properties.Title(TO_WIDESTRING(data[4]));
    try {
      properties.TrackNumber(std::stoi(data[5]));
    } catch (...) {
    }
  } else if (type == 2) {
    VideoDisplayProperties properties = updater.VideoProperties();
    properties.Title(TO_WIDESTRING(data[0]));
    properties.Subtitle(TO_WIDESTRING(data[1]));
  }
  if (thumbnail != "") {
    updater.Thumbnail(Streams::RandomAccessStreamReference::CreateFromUri(
        Uri(TO_WIDESTRING(thumbnail))));
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

#endif

}  // namespace

#ifdef _WIN32

LRESULT CALLBACK VideoWindowProc(HWND window, UINT code, WPARAM wparam,
                                 LPARAM lparam) {
  switch (code) {
    case WM_CLOSE: {
      DestroyWindow(window);
      return 0;
    }
    case WM_DESTROY: {
      PostQuitMessage(0);
      return 0;
    }
    case WM_SIZE: {
      RECT rect = {0, 0, 0, 0};
      GetClientRect(window, &rect);
      // Accessing saved XAML handle as user data on parent HWND.
      MoveWindow(
          reinterpret_cast<HWND>(GetWindowLongPtr(window, GWLP_USERDATA)), 0, 0,
          rect.right, rect.bottom, TRUE);
      break;
    }
    default: {
      break;
    }
  }
  return DefWindowProcW(window, code, wparam, lparam);
}

#endif

#ifdef __linux__

DLLEXPORT void PlayerRun() { new std::thread(gtk_main); }

#endif

#endif
#ifdef __cplusplus
}
#endif
