<h1 align="center"><a href="https://github.com/libwinmedia/libwinmedia">libwinmedia</a></h1>
<h4 align="center">A tiny high-level media playback library for C++.</h4>


<p align="center"><img height="400" src="https://github.com/libwinmedia/libwinmedia/blob/assets/screenshot.jpg?raw=true"></img></p>

## Example

A very simple example can be as follows.

```cpp
#include "libwinmedia/libwinmedia.hpp"

auto TO_WIDESTRING = [](const char* array) -> std::wstring {
  std::string string = std::string(array);
  return std::wstring(string.begin(), string.end());
};

int main(int ac, const char** av) {
  if (ac < 2) {
    std::wcout << L"No URI provided.\n" << L"Example Usage:\n" << av[0]
               << L" file://C:/alexmercerind/music.mp3\n" << av[0]
               << L" https://alexmercerind.github.io/video.mp4\n";
    return EXIT_FAILURE;
  }
  auto player = lwm::Player(0);
  auto media = std::make_shared<lwm::Media>(0, TO_WIDESTRING(av[1]));
  player.Open(media);
  player.Play();
  player.events()->Position([](int position) -> void {
    std::wcout << "Position changed to " << position << " milliseconds.\n";
  });
  std::cin.get();
  return EXIT_SUCCESS;
}

```

## Documentation

**Create a new player.**

```cpp
Player player = Player(0);
```

**Create a media to open inside player.**

```cpp
Media media = Media(0, true);
int32_t duration = media.duration();
```

**Play the media.**

```cpp
player.Open(media);
```

**Control playback.**

```cpp
player.Play();

player.Pause();

player.SetVolume(0.69);

player.SetRate(1.2);

player.Seek(10000);

player.SetIsLooping(false);

player.SetAudioBalance(1.0);
```

**Listen to playback events.**

```cpp
player.events()->Position(
  [=](int position) -> void {}
);

player.events()->Duration(
  [=](int duration) -> void {}
);

player.events()->Rate(
  [=](float rate) -> void {}
);

player.events()->Volume(
  [=](float volume) -> void {}
);

player.events()->IsPlaying(
  [=](bool isPlaying) -> void {}
);

// Other events.
```

**Create native system media controls.**

![](https://github.com/libwinmedia/libwinmedia/blob/assets/native_controls.jpg)

Pass function as argument to receive event callbacks.

```cpp
auto controls = lwm::NativeControls(
  [](auto button) -> void {
    if (button == NativeControlsButton::Play) std::cout << "Play clicked.\n";
    if (button == NativeControlsButton::Pause) std::cout << "Pause clicked.\n";
  }
);
```

Update the native system media controls.

```cpp
controls.Update(
  std::make_shared<lwm::MusicNativeControlState>(
    L"album_artist",
    L"album",
    L"track_count",
    L"artist",
    L"title",
    L"track_number"
  ),
  L"file://C:/Users/Hitesh/Pictures/AlbumArt.PNG"
);
```

Clear the native system media controls.

```cpp
controls.Dispose();
```

**Extract metadata tags.**

```cpp
auto tags = lwm::Tags::FromMusic(std::wstring(music_file));
std::wcout << L"album        : " << tags.album() << ".\n"
           << L"album_artist : " << tags.album_artist() << ".\n"
           << L"bitrate      : " << tags.bitrate() << ".\n";
// Other metadata tags.
```

**Extract album art.**

```cpp
lwm::Tags::ExtractThumbnail(
  music_file,
  TO_WIDESTRING(std::filesystem::current_path().string()),
  L"ExtractedAlbumArt.PNG",
  lwm::ThumbnailMode::Music,
  400
);
```

**Show video in a window.**

For showing video, you must instantiate player as follows.

```cpp
Player player = Player(0, true);
```

You need to embed a manifest with `maxversiontested` property to the generated executable. The library creates a separate win32 window on another thread & uses XAML islands to render the `MediaPlayerElement` in it (for showing video). Learn more [here](https://docs.microsoft.com/en-us/windows/apps/desktop/modernize/host-standard-control-with-xaml-islands-cpp) & [here](https://docs.microsoft.com/en-us/cpp/build/how-to-embed-a-manifest-inside-a-c-cpp-application?view=msvc-160).

```manifest
<?xml version="1.0" encoding="UTF-8"?>
<assembly xmlns="urn:schemas-microsoft-com:asm.v1" manifestVersion="1.0">
    <compatibility xmlns="urn:schemas-microsoft-com:compatibility.v1">
        <application>
            <maxversiontested Id="10.0.18362.0"/>
            <supportedOS Id="{8e0f7a12-bfb3-4fe8-b9a5-48fd50a15a9a}" />
        </application>
    </compatibility>
</assembly>
```

## Upcoming Features

- Fixes to multiple video windows.
- Improving memory usage.
- Adding video frame callbacks.

## Aim

The main goals of creating [libwinmedia](https://github.com/libwinmedia/libwinmedia) are:
- Having high-level, user friendly library for media playback in C++.
- Having small footprint, by using already existing OS APIs.
- Having necessary things like network playback, event callbacks etc.
- Being able to build similar high-level bindings to other programming languages, by just looking up for methods inside a single shared library.
- Supporting multiple playback instances.
- Supporting media tag-parsing & other things like lockscreen/system notifications.
- Being permissively licensed.
- Being cross-platform. **

** Currently only working on Windows.

## License 

Copyright (c) 2021 Hitesh Kumar Saini <saini123hitesh@gmail.com>

This library & work under this repository is MIT licensed.

Contributions welcomed.
