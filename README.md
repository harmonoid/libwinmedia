<h1 align="center"><a href="https://github.com/libwinmedia/libwinmedia">libwinmedia</a></h1>
<h4 align="center">A cross-platform media playback library for C/C++ & Flutter with good number of features.</h4>

## Example

A very simple example can be as follows.

### C++

```cpp
#include "libwinmedia/libwinmedia.hpp"

int32_t main(int32_t ac, const char** av) {
  using namespace std;
  using namespace lwm;
  if (ac < 2) {
    cout << "No URI provided.\n"
         << "Example Usage:\n" << av[0]
         << " file://C:/alexmercerind/music.mp3\n" << av[0]
         << " https://alexmercerind.github.io/video.mp4\n";
    return EXIT_FAILURE;
  }
  auto player = Player(0);
  auto media = Media(av[1]);
  player.Open(vector<Media>{media});
  player.Play();
  player.events()->Position([](int32_t position) -> void {
    cout << "Current playback position is " << position << " ms.\n";
  });
  cin.get();
  return EXIT_SUCCESS;
}
```

### Flutter

**More about Flutter [here](https://github.com/harmonoid/libwinmedia/tree/master/flutter/README.md) or on the [pub.dev](https://pub.dev/packages/libwinmedia).**

```yaml
dependencies:
  ...
  libwinmedia: ^0.0.1
```

```dart
import 'package:libwinmedia/libwinmedia.dart';

void main() {
  LWM.initialize();
  runApp(MyApp());
}

void demo() {
  var player = Player(id: 0);
  player.streams.medias.listen((List<Media> medias) {});
  player.streams.isPlaying.listen((bool isPlaying) {});
  player.streams.isBuffering.listen((bool isBuffering) {});
  player.streams.isCompleted.listen((bool isCompleted) {});
  player.streams.position.listen((Duration position) {});
  player.streams.duration.listen((Duration duration) {});
  player.streams.index.listen((int index) {});
  player.open([
    Media(uri: 'https://www.example.com/media/music.mp3'),
    Media(uri: 'file://C:/documents/video.mp4'),
  ]);
  player.play();
  player.seek(Duration(seconds: 20));
  player.nativeControls.update(
    album: 'Fine Line',
    albumArtist: 'Harry Styles',
    trackCount: 12,
    artist: 'Harry Styles',
    title: 'Lights Up',
    trackNumber: 1,
    thumbnail: File('album_art.png'),
  );
}
```

## Bindings

- [just_audio](https://pub.dev/packages/just_audio): Cross platform audio playback library for Flutter, working on Android, iOS, macOS, Windows, Linux & Web.
- [libwinmedia-py](https://github.com/libwinmedia/libwinmedia-py): libwinmedia bindings for Python.
- [libwinmedia.dart](https://pub.dev/packages/libwinmedia): libwinmedia bindings for Flutter.

## Support

Consider supporting the project by starring the repository or buying me a coffee.

<a href="https://www.buymeacoffee.com/alexmercerind"><img src="https://img.buymeacoffee.com/button-api/?text=Buy me a coffee&emoji=&slug=alexmercerind&button_colour=FFDD00&font_colour=000000&font_family=Cookie&outline_colour=000000&coffee_colour=ffffff"></a>

Thanks a lot for your support.

## Documentation

### Setup

For using the library on Windows, you can download the pre-compiled shared library from the [releases page](https://github.com/harmonoid/libwinmedia/releases) & headers can be found [here](https://github.com/harmonoid/libwinmedia/tree/master/include).

### Usage

**Create a new player.**

```cpp
Player player = Player(0);
```

**Create a media to open inside player.**

```cpp
Media media = Media("file://C:/alexmercerind/music.mp3");
int32_t duration = media.duration();
```

**Play the medias.**

```cpp
player.Open(std::vector<lwm::Media>{media});
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
    "album_artist",
    "album",
    "track_count",
    "artist",
    "title",
    "track_number"
  ),
  "file://C:/Users/Hitesh/Pictures/AlbumArt.PNG"
);
```

Clear the native system media controls.

```cpp
controls.Dispose();
```

**Extract metadata tags.**

```cpp
auto tags = lwm::Tags::FromMusic(std::wstring(music_file));
std::wcout << "album        : " << tags.album() << ".\n"
           << "album_artist : " << tags.album_artist() << ".\n"
           << "bitrate      : " << tags.bitrate() << ".\n";
           
// Other metadata tags.
```

**Extract album art.**

```cpp
lwm::Tags::ExtractThumbnail(
  music_file,
  TO_WIDESTRING(std::filesystem::current_path().string()),
  "ExtractedAlbumArt.PNG",
  lwm::ThumbnailMode::Music,
  400
);
```

**Show video in a window.**

For showing video, you must instantiate player as follows.

```cpp
Player player = Player(0, true);
```

**Control video output.**

```cpp
player.ShowWindow();

player.CloseWindow();
```

## Notes

### Windows

**For showing video**

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

## Aim

The main goals of creating [libwinmedia](https://github.com/libwinmedia/libwinmedia) are:
- Having high-level, user friendly library for media playback in C++.
- Having small footprint, by using already existing OS APIs.
- Having necessary things like network playback, event callbacks etc.
- Being able to build similar high-level bindings to other programming languages, by just looking up for methods inside a single shared library.
- Supporting multiple playback instances.
- Supporting media tag-parsing & other things like lockscreen/system notifications.
- Being permissively licensed.
- Being cross-platform **.

** Currently only working on Windows & Linux.


## License 

Copyright (c) 2021 Hitesh Kumar Saini <saini123hitesh@gmail.com>

This library & work under this repository is MIT licensed.

Contributions welcomed.
