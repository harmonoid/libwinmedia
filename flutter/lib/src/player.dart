import 'dart:io';
import 'dart:ffi';
import 'dart:async';
import 'package:ffi/ffi.dart';
import 'package:libwinmedia/src/internal.dart';
import 'package:libwinmedia/src/media.dart';
import 'package:libwinmedia/src/utils.dart';

/// Various [Player] instances created.
/// Stored here for sending events to the [StreamController]s inside the [Player] class.
Map<int, Player> players = {};

/// Current playback status of native control buttons.
enum NativeControlsStatus {
  /// [Media] has completed playback.
  closed,

  /// Changing [Media].
  changing,

  /// Stopped playback.
  stopped,

  /// Playing mode.
  playing,

  /// Paused mode.
  paused,
}

/// Player Native Controls
/// ----------------------
/// These media controls show near the volume slider & on the lock screen in Windows 10.
/// a.k.a System Media Transport Controls.
///
/// This class offers various methods to control its state, style & displayed info.
///
/// For more raw control, use [NativeControls] instead.
///
/// ```dart
/// Player player = Player(id: 0);
/// player.nativeControls.status = NativeControlsStatus.paused;
/// player.nativeControls.update(
///   albumArtist: 'Porter Robinson',
///   album: 'Divinity',
///   thumbnail: File('album_art.png'),
/// );
/// ```
///
class PlayerNativeControls {
  /// Player's unique [id].
  final int id;

  PlayerNativeControls({required this.id});

  /// Current status of [PlayerNativeControls].
  NativeControlsStatus get status {
    return _status;
  }

  /// Sets current status of [PlayerNativeControls].
  set status(NativeControlsStatus status) {
    LWM.bindings.PlayerNativeControlsSetStatus(
      id,
      status.index,
    );
    _status = status;
  }

  /// Updates currently playing [Media]'s displayed info in the native controls.
  ///
  /// In case no [thumbnail] is passed, no image will be displayed.
  /// This method should be only called when the [Media] has started playing, otherwise the
  /// info retrieved automatically from [Media] will overrite the info set by [update].
  ///
  Future<void> update(
      {String? albumArtist,
      String? album,
      int? trackCount,
      String? artist,
      String? title,
      int? trackNumber,
      Uri? thumbnail}) async {
    var info = <String>[
      albumArtist ?? '',
      album ?? '',
      (trackCount != null) ? trackCount.toString() : '',
      artist ?? '',
      title ?? '',
      (trackNumber != null) ? trackNumber.toString() : '',
    ].toNativeUtf8Array();

    /// TODO (alexmercerind): Support video data.
    /// TODO (alexmercerind): Add [Stream] to listen to events performed by user on the native controls.

    LWM.bindings.PlayerNativeControlsUpdate(
      id,
      1,
      info,
      (thumbnail != null)
          ? 'file://${thumbnail.toString()}'.toNativeUtf8()
          : ''.toNativeUtf8(),
    );
    calloc.free(info);
  }

  /// Clears the native controls associated with the [Player].
  void clear() {
    LWM.bindings.PlayerNativeControlsClear(
      id,
    );
  }

  /// Disposes the native controls associated with the [Player].
  void dispose() {
    LWM.bindings.PlayerNativeControlsDispose(
      id,
    );
  }

  /// private:

  /// Internally used private variable to avoid calling another native call from
  NativeControlsStatus _status = NativeControlsStatus.closed;
}

/// Player
/// ------
/// A [Player] object can be used for media playback.
/// A unique [id] must be passed for creating & controlling a [Player] instance.
///
/// ```dart
/// Player player = Player(id: 0);
/// player.open([
///   Media(uri: 'https://www.example.com/music.mp3'),
///   Media(uri: 'file://C:/documents/video.mp4'),
/// ]);
/// player.play();
/// ```
///
class Player {
  /// Player
  /// ------
  /// A [Player] object can be used for media playback.
  /// A unique [id] must be passed for creating & controlling a [Player] instance.
  ///
  /// When using on Flutter Linux, [startLoop] must be passed as `false`.
  ///
  /// ```dart
  /// Player player = Player(id: 0);
  /// player.open([
  ///   Media(uri: 'https://www.example.com/music.mp3'),
  ///   Media(uri: 'file://C:/documents/video.mp4'),
  /// ]);
  /// player.play();
  /// ```
  ///
  Player({
    required this.id,
    bool showWindow = false,
    String windowTitle = 'libwinmedia',
    bool startLoop = true,
  }) {
    nativeControls = PlayerNativeControls(id: id);
    players[id] = this;
    LWM.bindings.PlayerCreate(
      id,
      showWindow,
      windowTitle.toNativeUtf8(),
    );
    if (Platform.isLinux) {
      if (startLoop) {
        LWM.dynamicLibrary
            .lookupFunction<Void Function(), void Function()>('PlayerRun')();
      }
    }
    LWM.bindings.PlayerSetRateEventHandler(
      id,
      Pointer.fromFunction(doubleC),
    );
    LWM.bindings.PlayerSetVolumeEventHandler(
      id,
      Pointer.fromFunction(doubleC),
    );
    LWM.bindings.PlayerSetPositionEventHandler(
      id,
      Pointer.fromFunction(intC),
    );
    LWM.bindings.PlayerSetDurationEventHandler(
      id,
      Pointer.fromFunction(intC),
    );
    LWM.bindings.PlayerSetIndexEventHandler(
      id,
      Pointer.fromFunction(intC),
    );
    LWM.bindings.PlayerSetIsPlayingEventHandler(
      id,
      Pointer.fromFunction(intC),
    );
    LWM.bindings.PlayerSetIsBufferingEventHandler(
      id,
      Pointer.fromFunction(intC),
    );
    LWM.bindings.PlayerSetIsCompletedEventHandler(
      id,
      Pointer.fromFunction(intC),
    );
    LWM.bindings.PlayerSetDownloadProgressEventHandler(
      id,
      Pointer.fromFunction(doubleC),
    );
    LWM.bindings.PlayerSetErrorEventHandler(
      id,
      Pointer.fromFunction(errorC),
    );
  }

  /// Unique [id] associated with the [Player].
  final int id;

  /// Current [Media] playback [position] of the [Player].
  Duration get position {
    return Duration(
      milliseconds: LWM.bindings.PlayerGetPosition(
        id,
      ),
    );
  }

  /// Current [volume] of the [Player].
  double get volume {
    return LWM.bindings.PlayerGetVolume(
      id,
    );
  }

  /// Current [rate] of the [Player].
  double get rate {
    return LWM.bindings.PlayerGetRate(
      id,
    );
  }

  /// Current [audioBalance] of the [Player].
  double get audioBalance {
    return LWM.bindings.PlayerGetAudioBalance(
      id,
    );
  }

  /// Current [downloadProgress] of the [Player].
  double? get downloadProgress {
    return LWM.bindings.PlayerGetDownloadProgress(id);
  }

  /// Current [autoplay] mode of the [Player].
  bool get autoplay {
    return LWM.bindings.PlayerIsAutoplay(
      id,
    );
  }

  /// Current [isLooping] mode of the [Player].
  bool get isLooping {
    return LWM.bindings.PlayerIsLooping(
      id,
    );
  }

  /// If the auto repeat is enabled.
  bool get isAutoRepeat {
    return LWM.bindings.PlayerIsAutoRepeat(
      id,
    );
  }

  /// If the shuffle is enabled.
  bool get isShuffling {
    return LWM.bindings.PlayerIsShuffling(
      id,
    );
  }

  /// Player Native Controls
  /// ----------------------
  /// These media controls show near the volume slider & on the lock screen in Windows 10.
  /// a.k.a System Media Transport Controls.
  ///
  /// This class offers various methods to control its state, style & displayed info.
  ///
  /// For more raw control, use [NativeControls] instead.
  ///
  /// ```dart
  /// Player player = Player(id: 0);
  /// player.nativeControls.status = NativeControlsStatus.paused;
  /// player.nativeControls.update(
  ///   albumArtist: 'Porter Robinson',
  ///   album: 'Divinity',
  ///   thumbnail: File('album_art.png'),
  /// );
  /// ```
  ///
  late PlayerNativeControls nativeControls;

  /// Current state of the [Player]. For listening to these values and events, use [Player.streams] instead.
  _PlayerState state = _PlayerState();

  /// Various event streams to listen to events of a [Player].
  ///
  /// ```dart
  /// Player player = Player(id: 0);
  /// player.streams.position..listen((position) {
  ///   print(position.inMilliseconds);
  /// });
  /// ```
  ///
  /// There are a lot of events like [isPlaying], [medias], [index] etc. to listen to & cause UI re-build.
  ///
  _PlayerStreams streams = _PlayerStreams();

  /// Disposes the [Player] instance & releases the resources.
  void dispose() {
    LWM.bindings.PlayerDispose(
      id,
    );
  }

  /// Shows the video output window of the [Player].
  void showWindow(String windowTitle) {
    LWM.bindings.PlayerShowWindow(
      id,
      windowTitle.toNativeUtf8(),
    );
  }

  /// Closes the video output window of the [Player].
  void closeWindow() {
    LWM.bindings.PlayerCloseWindow(
      id,
    );
  }

  /// Opens a [List] of [Media]s into the [Player] as a queue.
  /// Previously opened, added or inserted [Media]s get removed.
  ///
  /// ```dart
  /// player.open([
  ///   Media(uri: 'https://www.example.com/music.mp3'),
  ///   Media(uri: 'file://C:/documents/video.mp4'),
  /// ]);
  /// ```
  void open(List<Media> medias) {
    var uris = medias.map((media) => media.uri).toList().toNativeUtf8Array();
    var ids = medias.map((media) => media.id).toList().toNativeArray();
    LWM.bindings.PlayerOpen(
      id,
      medias.length,
      uris,
      ids,
    );
    calloc.free(uris);
    calloc.free(ids);
  }

  /// Starts playing the [Player].
  void play() {
    LWM.bindings.PlayerPlay(
      id,
    );
  }

  /// Pauses the [Player].
  void pause() {
    LWM.bindings.PlayerPause(
      id,
    );
  }

  /// Appends a [Media] to the [Player]'s queue.
  void add(Media media) {
    LWM.bindings.PlayerAdd(
      id,
      media.uri.toNativeUtf8(),
      media.id,
    );
  }

  /// Removes the [Media] at specified index from the [Player]'s queue.
  void remove(int index) {
    LWM.bindings.PlayerRemove(
      id,
      index,
    );
  }

  /// Jumps to next [Media] in the [Player]'s queue.
  void next() {
    LWM.bindings.PlayerNext(
      id,
    );
  }

  /// Jumps to previous [Media] in the [Player]'s queue.
  void back() {
    LWM.bindings.PlayerBack(
      id,
    );
  }

  /// Jumps to specified [Media]'s index in the [Player]'s queue.
  void jump(int index) {
    LWM.bindings.PlayerJump(
      id,
      index,
    );
  }

  /// Seeks the currently playing [Media] in the [Player] by specified [Duration].
  void seek(Duration duration) {
    LWM.bindings.PlayerSeek(
      id,
      duration.inMilliseconds,
    );
  }

  /// Sets the playback [volume] of the [Player]. Defaults to `1.0`.
  set volume(double volume) {
    LWM.bindings.PlayerSetVolume(
      id,
      volume,
    );
  }

  /// Sets the playback [rate] of the [Player]. Defaults to `1.0`.
  set rate(double rate) {
    LWM.bindings.PlayerSetRate(
      id,
      rate,
    );
  }

  /// Sets the audio balance of the [Player]. Defaults to `1.0`.
  set audioBalance(double audioBalance) {
    LWM.bindings.PlayerSetAudioBalance(
      id,
      audioBalance,
    );
  }

  /// Sets [autoplay] mode of the [Player].
  set autoplay(bool autoplay) {
    LWM.bindings.PlayerSetAutoplay(
      id,
      autoplay,
    );
  }

  /// Sets [isLooping] of the [Player].
  set isLooping(bool isLooping) {
    LWM.bindings.PlayerSetIsLooping(
      id,
      isLooping,
    );
  }

  /// Sets [isAutoRepeat] of the [Player]
  set isAutoRepeat(bool autoRepeat) {
    LWM.bindings.PlayerSetIsAutoRepeat(
      id,
      autoRepeat,
    );
  }

  /// Sets [isShuffling] of the [Player]
  set isShuffling(bool shuffling) {
    LWM.bindings.PlayerSetIsShuffling(
      id,
      shuffling,
    );
  }
}

/// Private class to keep state of the [Player].
class _PlayerState {
  /// [List] of currently opened [Media]s.
  List<Media> medias = [];

  /// If the [Player] is playing.
  bool isPlaying = false;

  /// If the [Player] is buffering.
  bool isBuffering = false;

  /// If the [Player]'s playback is completed.
  bool isCompleted = false;

  /// Current playback position of the [Player].
  Duration position = Duration.zero;

  /// Duration of the currently playing [Media] in the [Player].
  Duration duration = Duration.zero;

  /// Index of the currently playing [Media] in the queue.
  int index = 0;

  /// Download progress of the currently playing [Media].
  double downloadProgress = 0.0;

  /// Occured error in the [Player].
  PlayerError? error;
}

/// Private class for event handling of [Player].
class _PlayerStreams {
  /// [List] of currently opened [Media]s.
  late Stream<List<Media>> medias;

  /// If the [Player] is playing.
  late Stream<bool> isPlaying;

  /// If the [Player] is buffering.
  late Stream<bool> isBuffering;

  /// If the [Player]'s playback is completed.
  late Stream<bool> isCompleted;

  /// Current playback position of the [Player].
  late Stream<Duration> position;

  /// Duration of the currently playing [Media] in the [Player].
  late Stream<Duration> duration;

  /// Index of the currently playing [Media] in the queue.
  late Stream<int> index;

  /// Download progress of the currently playing [Media].
  late Stream<double> downloadProgress;

  /// Occured error in the [Player].
  late Stream<PlayerError?> error;

  /// Closes all the stream controllers.
  void dispose() {
    mediasController.close();
    isPlayingController.close();
    isBufferingController.close();
    isCompletedController.close();
    positionController.close();
    durationController.close();
    indexController.close();
    downloadProgressController.close();
    errorController.close();
  }

  _PlayerStreams() {
    medias = mediasController.stream;
    isPlaying = isPlayingController.stream;
    isBuffering = isBufferingController.stream;
    isCompleted = isCompletedController.stream;
    position = positionController.stream;
    duration = durationController.stream;
    index = indexController.stream;
    downloadProgress = downloadProgressController.stream;
    error = errorController.stream;
  }

  /// Internally used [StreamController].
  final StreamController<List<Media>> mediasController =
      StreamController.broadcast();

  /// Internally used [StreamController].
  final StreamController<bool> isPlayingController =
      StreamController.broadcast();

  /// Internally used [StreamController].
  final StreamController<bool> isBufferingController =
      StreamController.broadcast();

  /// Internally used [StreamController].
  final StreamController<bool> isCompletedController =
      StreamController.broadcast();

  /// Internally used [StreamController].
  final StreamController<Duration> positionController =
      StreamController.broadcast();

  /// Internally used [StreamController].
  final StreamController<Duration> durationController =
      StreamController.broadcast();

  /// Internally used [StreamController].
  final StreamController<int> indexController = StreamController.broadcast();

  /// Internally used [StreamController].
  final StreamController<double> downloadProgressController =
      StreamController.broadcast();

  /// Internally used [StreamController].
  final StreamController<PlayerError?> errorController =
      StreamController.broadcast();
}

/// Various [Player] error codes.
enum PlayerErrorCode {
  unknown,
  aborted,
  networkError,
  decodingError,
  sourceNotSupported
}

/// Class representing a [Player] error.
class PlayerError {
  /// Error code.
  final PlayerErrorCode code;

  /// Error message.
  final String message;

  PlayerError(this.code, this.message);

  @override
  String toString() => 'PlayerError: ($code) $message';
}
