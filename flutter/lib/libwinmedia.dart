/// libwinmedia.dart
/// ----------------
///
/// A Dart library for media playback, tag parsing & controlling system media transport controls, MPRIS on Windows 10 & Linux.
///
/// *Minimal example*
/// ```
/// LWM.initialize();
///
/// Player player = Player(id: 0);
/// player.open([
///   Media(uri: 'https://www.example.com/music.mp3'),
///   Media(uri: 'file://C:/documents/video.mp4'),
/// ]);
/// player.play();
/// player.nativeControls.status = NativeControlsStatus.paused;
/// player.nativeControls.update(
///   albumArtist: 'Porter Robinson',
///   album: 'Divinity',
/// );
/// ```
///
/// The library provides performant & completely synchronous API for media playback.
///
library libwinmedia;

/// Exposed high-level object oriented & stream based APIs.
/// Low-level C calls can be made inside `LWM.bindings` object after calling `LWM.initialize`.
///
export 'package:libwinmedia/src/player.dart';
export 'package:libwinmedia/src/media.dart';
export 'package:libwinmedia/src/internal.dart';
