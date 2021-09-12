/// Various [Media] instances created.
/// Stored here for persistence of [Media.extras].
Map<int, Media> medias = {};

/// Media
/// -----
/// A [Media] object to open inside a [Player] instance using [Player.open] method for playback.
///
/// ```dart
/// var media = Media(uri: 'https://www.example.com/music.mp3');
/// ```
///
class Media {
  /// Unique ID associated with the [Media].
  late int id;

  /// The URI of the [Media].
  final String uri;

  /// Additional optional user data.
  final dynamic extras;

  /// Media
  /// -----
  /// A [Media] object to open inside a [Player] instance using [Player.open] method for playback.
  ///
  /// ```dart
  /// var media = Media(uri: 'https://www.example.com/music.mp3');
  /// ```
  ///
  Media({required this.uri, this.extras}) {
    id = medias.length;
    medias[id] = this;
  }

  @override
  String toString() {
    return 'Media(uri: $uri, extras: $extras, id: $id)';
  }
}
