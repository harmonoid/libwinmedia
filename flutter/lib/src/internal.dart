import 'dart:io';
import 'dart:ffi';
import 'dart:isolate';
import 'package:libwinmedia/generated/bindings.dart' as ffi;
import 'package:libwinmedia/libwinmedia.dart';
import 'package:path/path.dart';

/// libwinmedia.dart
/// ----------------
///
/// Initializer class for libwinmedia.dart.
///
/// ```dart
/// void main() {
///   LWM.initialize(DynamicLibrary.open('C:/path/to/libwinmedia.dll'));
///   runApp(MyApp());
/// }
/// ```
///
/// Calling this before any other API from the library is necessary to make event callbacks & prevent crashes.
///
abstract class LWM {
  static late ffi.LWM bindings;
  static late DynamicLibrary dynamicLibrary;
  static var receivePort = ReceivePort()
    ..listen((event) {
      if (event is List) {
        int id = event[0];
        String type = event[1];
        assert(players.containsKey(id));
        switch (type) {
          case 'Open':
            {
              // No OP for `events[2]` which is `List<String>` of URIs.
              players[id]!.streams.mediasController.add(
                    event[3]
                        .map((mediaId) => medias[mediaId]!)
                        .toList()
                        .cast<Media>(),
                  );
              players[id]!.state.medias = event[3]
                  .map((mediaId) => medias[mediaId]!)
                  .toList()
                  .cast<Media>();
              break;
            }
          case 'IsPlaying':
            {
              players[id]!.streams.isPlayingController.add(
                    event[2],
                  );
              players[id]!.state.isPlaying = event[2];
              break;
            }
          case 'IsBuffering':
            {
              players[id]!.streams.isBufferingController.add(
                    event[2],
                  );
              players[id]!.state.isBuffering = event[2];
              break;
            }
          case 'IsCompleted':
            {
              players[id]!.streams.isCompletedController.add(
                    event[2],
                  );
              players[id]!.state.isCompleted = event[2];
              break;
            }
          case 'Position':
            {
              players[id]!.streams.positionController.add(
                    Duration(
                      milliseconds: event[2],
                    ),
                  );
              players[id]!.state.position = Duration(
                milliseconds: event[2],
              );
              break;
            }
          case 'Duration':
            {
              players[id]!.streams.durationController.add(
                    Duration(
                      milliseconds: event[2],
                    ),
                  );
              players[id]!.state.duration = Duration(
                milliseconds: event[2],
              );
              break;
            }
          case 'Index':
            {
              players[id]!.streams.indexController.add(
                    event[2],
                  );
              players[id]!.state.index = event[2];
              break;
            }
          case 'DownloadProgress':
            {
              players[id]!.streams.downloadProgressController.add(
                    event[2],
                  );
              players[id]!.state.downloadProgress = event[2];
              break;
            }
          case 'Error':
            {
              var error = PlayerError(event[2], event[3]);
              players[id]!.streams.errorController.add(
                    error,
                  );
              players[id]!.state.error = error;
              break;
            }
          default:
            break;
        }
      }
    });

  static void initialize() {
    if (Platform.isLinux) {
      LWM.dynamicLibrary = DynamicLibrary.open(
        join(
          dirname(
            Platform.resolvedExecutable,
          ),
          'lib',
          'libwinmedia.so',
        ),
      );
    }
    if (Platform.isWindows) {
      LWM.dynamicLibrary = DynamicLibrary.open(
        join(
          dirname(
            Platform.resolvedExecutable,
          ),
          'libwinmedia.dll',
        ),
      );
    }
    LWM.bindings = ffi.LWM(dynamicLibrary);
    LWM.bindings.InitializeDartApi(
      NativeApi.postCObject,
      LWM.receivePort.sendPort.nativePort,
    );
  }
}
