<h1 align="center"><a href="https://github.com/libwinmedia/libwinmedia">libwinmedia</a></h1>
<h4 align="center">🎞 A tiny high-level media playback library for C++.</h4>


<p align="center"><img src="https://github.com/libwinmedia/libwinmedia/blob/assets/screenshot.jpg?raw=true"></img></p>

## Example

A very simple example can be as follows:

```cpp
#include "libwinmedia/libwinmedia.hpp"

auto TO_WIDESTRING = [](const char* array) -> std::wstring {
    std::string string = std::string(array);
    return std::wstring(string.begin(), string.end());
};

int main(int ac, const char** av) {
    if (ac < 2) {
        std::wcout << L"No URI provided.\n";
        std::wcout << L"Example Usage:\n";
        std::wcout << av[0] << L" file://C:/alexmercerind/music.mp3\n";
        std::wcout << av[0] << L" https://alexmercerind.github.io/video.mp4\n";
        return 1;
    }
    lwm::Player player = lwm::Player(0);
    lwm::Media media = lwm::Media(0, TO_WIDESTRING(av[1]));
    player.open(media);
    player.play();
    player.events.position(
        [](int position) -> void {
            std::wcout << "Position changed to " << position << " milliseconds.\n";
        }
    );
    std::cin.get();
    return 0;
}
```

## Documentation

Create a new player.

```cpp
Player player = Player(0);
```

Create a media to open inside player.

```cpp
Media media = Media(0, true);
int duration = media.duration;
```

Play the media.

```cpp
player.open(media);
```

Control playback.

```cpp
player.play();

player.pause();

player.setVolume(0.69);

player.setRate(1.2);

player.setPosition(10000);

player.setIsLooping(false);

player.setAudioBalance(1.0);
```

Listen to playback events.

```cpp
player.events.position(
  [=](int position) -> void {}
);

player.events.duration(
  [=](int duration) -> void {}
);

player.events.rate(
  [=](float rate) -> void {}
);

player.events.volume(
  [=](float volume) -> void {}
);

player.events.isPlaying(
  [=](bool isPlaying) -> void {}
);

// Other events.
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
- Being cross-platform. **

** Currently only working on Windows.
