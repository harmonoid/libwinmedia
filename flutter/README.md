<h1 align="center"><a href="https://github.com/libwinmedia/libwinmedia.dart">libwinmedia.dart</a></h1>
<h4 align="center">Flutter bindings to <a href='https://github.com/harmonoid/libwinmedia'>libwinmedia</a>.</h4>

<br></br>

![](https://user-images.githubusercontent.com/28951144/132977554-f755e62c-0e22-4ef9-9b22-7bc45cf93e4a.png)

## Install

```yaml
dependencies:
  ...
  libwinmedia: ^0.0.1
```

## Example

A very simple example can be as follows.

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

Checkout the other APIs & docstrings within the project, for more detailed information. Those are the most updated source of documentation.

## Support

Consider supporting the project by starring the repository or buying me a coffee.

<a href="https://www.buymeacoffee.com/alexmercerind"><img src="https://img.buymeacoffee.com/button-api/?text=Buy me a coffee&emoji=&slug=alexmercerind&button_colour=FFDD00&font_colour=000000&font_family=Cookie&outline_colour=000000&coffee_colour=ffffff"></a>

Thanks a lot for your support.


## Contributions

Contributions to the project are welcomed, either it be API improvements or documentation changes. Let's make it better.

## License

This library & work under this repository is MIT licensed, just like [libwinmedia](https://github.com/harmonoid/libwinmedia).

Copyright (C) 2021 libwinmedia.dart contributors.
