// ignore_for_file: unused_element
// ignore_for_file: unused_field
// ignore_for_file: non_constant_identifier_names
// ignore_for_file: camel_case_types
import 'dart:ffi' as ffi;
import 'package:ffi/ffi.dart' as ffi;

class LWM {
  final ffi.Pointer<T> Function<T extends ffi.NativeType>(String symbolName)
      _lookup;

  LWM(ffi.DynamicLibrary dynamicLibrary) : _lookup = dynamicLibrary.lookup;

  LWM.fromLookup(
      ffi.Pointer<T> Function<T extends ffi.NativeType>(String symbolName)
          lookup)
      : _lookup = lookup;

  void InitializeDartApi(
      ffi.Pointer<
              ffi.NativeFunction<
                  ffi.Int8 Function(ffi.Int64, ffi.Pointer<ffi.Dart_CObject>)>>
          post_C_object,
      int dart_port) {
    return _InitializeDartApi(
      post_C_object,
      dart_port,
    );
  }

  late final _InitializeDartApi_ptr =
      _lookup<ffi.NativeFunction<_c_InitializeDartApi>>('InitializeDartApi');
  late final _dart_InitializeDartApi _InitializeDartApi =
      _InitializeDartApi_ptr.asFunction<_dart_InitializeDartApi>();

  void PlayerShowWindow(
    int player_id,
    ffi.Pointer<ffi.Utf8> window_title,
  ) {
    return _PlayerShowWindow(
      player_id,
      window_title,
    );
  }

  late final _PlayerShowWindow_ptr =
      _lookup<ffi.NativeFunction<_c_PlayerShowWindow>>('PlayerShowWindow');
  late final _dart_PlayerShowWindow _PlayerShowWindow =
      _PlayerShowWindow_ptr.asFunction<_dart_PlayerShowWindow>();

  void PlayerCloseWindow(
    int player_id,
  ) {
    return _PlayerCloseWindow(
      player_id,
    );
  }

  late final _PlayerCloseWindow_ptr =
      _lookup<ffi.NativeFunction<_c_PlayerCloseWindow>>('PlayerCloseWindow');
  late final _dart_PlayerCloseWindow _PlayerCloseWindow =
      _PlayerCloseWindow_ptr.asFunction<_dart_PlayerCloseWindow>();

  void PlayerCreate(
    int player_id,
    bool show_window,
    ffi.Pointer<ffi.Utf8> window_title,
  ) {
    return _PlayerCreate(
      player_id,
      show_window ? 1 : 0,
      window_title,
    );
  }

  late final _PlayerCreate_ptr =
      _lookup<ffi.NativeFunction<_c_PlayerCreate>>('PlayerCreate');
  late final _dart_PlayerCreate _PlayerCreate =
      _PlayerCreate_ptr.asFunction<_dart_PlayerCreate>();

  void PlayerDispose(
    int player_id,
  ) {
    return _PlayerDispose(
      player_id,
    );
  }

  late final _PlayerDispose_ptr =
      _lookup<ffi.NativeFunction<_c_PlayerDispose>>('PlayerDispose');
  late final _dart_PlayerDispose _PlayerDispose =
      _PlayerDispose_ptr.asFunction<_dart_PlayerDispose>();

  void PlayerOpen(
    int player_id,
    int size,
    ffi.Pointer<ffi.Pointer<ffi.Utf8>> uris,
    ffi.Pointer<ffi.Int32> ids,
  ) {
    return _PlayerOpen(
      player_id,
      size,
      uris,
      ids,
    );
  }

  late final _PlayerOpen_ptr =
      _lookup<ffi.NativeFunction<_c_PlayerOpen>>('PlayerOpen');
  late final _dart_PlayerOpen _PlayerOpen =
      _PlayerOpen_ptr.asFunction<_dart_PlayerOpen>();

  void PlayerPlay(
    int player_id,
  ) {
    return _PlayerPlay(
      player_id,
    );
  }

  late final _PlayerPlay_ptr =
      _lookup<ffi.NativeFunction<_c_PlayerPlay>>('PlayerPlay');
  late final _dart_PlayerPlay _PlayerPlay =
      _PlayerPlay_ptr.asFunction<_dart_PlayerPlay>();

  void PlayerPause(
    int player_id,
  ) {
    return _PlayerPause(
      player_id,
    );
  }

  late final _PlayerPause_ptr =
      _lookup<ffi.NativeFunction<_c_PlayerPause>>('PlayerPause');
  late final _dart_PlayerPause _PlayerPause =
      _PlayerPause_ptr.asFunction<_dart_PlayerPause>();

  void PlayerAdd(
    int player_id,
    ffi.Pointer<ffi.Utf8> uri,
    int id,
  ) {
    return _PlayerAdd(
      player_id,
      uri,
      id,
    );
  }

  late final _PlayerAdd_ptr =
      _lookup<ffi.NativeFunction<_c_PlayerAdd>>('PlayerAdd');
  late final _dart_PlayerAdd _PlayerAdd =
      _PlayerAdd_ptr.asFunction<_dart_PlayerAdd>();

  void PlayerRemove(
    int player_id,
    int index,
  ) {
    return _PlayerRemove(
      player_id,
      index,
    );
  }

  late final _PlayerRemove_ptr =
      _lookup<ffi.NativeFunction<_c_PlayerRemove>>('PlayerRemove');
  late final _dart_PlayerRemove _PlayerRemove =
      _PlayerRemove_ptr.asFunction<_dart_PlayerRemove>();

  void PlayerNext(
    int player_id,
  ) {
    return _PlayerNext(
      player_id,
    );
  }

  late final _PlayerNext_ptr =
      _lookup<ffi.NativeFunction<_c_PlayerNext>>('PlayerNext');
  late final _dart_PlayerNext _PlayerNext =
      _PlayerNext_ptr.asFunction<_dart_PlayerNext>();

  void PlayerBack(
    int player_id,
  ) {
    return _PlayerBack(
      player_id,
    );
  }

  late final _PlayerBack_ptr =
      _lookup<ffi.NativeFunction<_c_PlayerBack>>('PlayerBack');
  late final _dart_PlayerBack _PlayerBack =
      _PlayerBack_ptr.asFunction<_dart_PlayerBack>();

  void PlayerJump(
    int player_id,
    int index,
  ) {
    return _PlayerJump(
      player_id,
      index,
    );
  }

  late final _PlayerJump_ptr =
      _lookup<ffi.NativeFunction<_c_PlayerJump>>('PlayerJump');
  late final _dart_PlayerJump _PlayerJump =
      _PlayerJump_ptr.asFunction<_dart_PlayerJump>();

  void PlayerSeek(
    int player_id,
    int position,
  ) {
    return _PlayerSeek(
      player_id,
      position,
    );
  }

  late final _PlayerSeek_ptr =
      _lookup<ffi.NativeFunction<_c_PlayerSeek>>('PlayerSeek');
  late final _dart_PlayerSeek _PlayerSeek =
      _PlayerSeek_ptr.asFunction<_dart_PlayerSeek>();

  void PlayerSetVolume(
    int player_id,
    double volume,
  ) {
    return _PlayerSetVolume(
      player_id,
      volume,
    );
  }

  late final _PlayerSetVolume_ptr =
      _lookup<ffi.NativeFunction<_c_PlayerSetVolume>>('PlayerSetVolume');
  late final _dart_PlayerSetVolume _PlayerSetVolume =
      _PlayerSetVolume_ptr.asFunction<_dart_PlayerSetVolume>();

  void PlayerSetRate(
    int player_id,
    double rate,
  ) {
    return _PlayerSetRate(
      player_id,
      rate,
    );
  }

  late final _PlayerSetRate_ptr =
      _lookup<ffi.NativeFunction<_c_PlayerSetRate>>('PlayerSetRate');
  late final _dart_PlayerSetRate _PlayerSetRate =
      _PlayerSetRate_ptr.asFunction<_dart_PlayerSetRate>();

  void PlayerSetAudioBalance(
    int player_id,
    double audio_balance,
  ) {
    return _PlayerSetAudioBalance(
      player_id,
      audio_balance,
    );
  }

  late final _PlayerSetAudioBalance_ptr =
      _lookup<ffi.NativeFunction<_c_PlayerSetAudioBalance>>(
          'PlayerSetAudioBalance');
  late final _dart_PlayerSetAudioBalance _PlayerSetAudioBalance =
      _PlayerSetAudioBalance_ptr.asFunction<_dart_PlayerSetAudioBalance>();

  void PlayerSetAutoplay(
    int player_id,
    bool autoplay,
  ) {
    return _PlayerSetAutoplay(
      player_id,
      autoplay ? 1 : 0,
    );
  }

  late final _PlayerSetAutoplay_ptr =
      _lookup<ffi.NativeFunction<_c_PlayerSetAutoplay>>('PlayerSetAutoplay');
  late final _dart_PlayerSetAutoplay _PlayerSetAutoplay =
      _PlayerSetAutoplay_ptr.asFunction<_dart_PlayerSetAutoplay>();

  void PlayerSetIsLooping(
    int player_id,
    bool looping,
  ) {
    return _PlayerSetIsLooping(
      player_id,
      looping ? 1 : 0,
    );
  }

  late final _PlayerSetIsLooping_ptr =
      _lookup<ffi.NativeFunction<_c_PlayerSetIsLooping>>('PlayerSetIsLooping');
  late final _dart_PlayerSetIsLooping _PlayerSetIsLooping =
      _PlayerSetIsLooping_ptr.asFunction<_dart_PlayerSetIsLooping>();

  void PlayerSetIsShuffling(
    int player_id,
    bool shuffling,
  ) {
    return _PlayerSetIsShuffling(
      player_id,
      shuffling ? 1 : 0,
    );
  }

  late final _PlayerSetIsShuffling_ptr =
      _lookup<ffi.NativeFunction<_c_PlayerSetIsShuffling>>(
          'PlayerSetIsShuffling');
  late final _dart_PlayerSetIsShuffling _PlayerSetIsShuffling =
      _PlayerSetIsShuffling_ptr.asFunction<_dart_PlayerSetIsShuffling>();

  void PlayerSetIsAutoRepeat(
    int player_id,
    bool enabled,
  ) {
    return _PlayerSetIsAutoRepeat(
      player_id,
      enabled ? 1 : 0,
    );
  }

  late final _PlayerSetIsAutoRepeat_ptr =
      _lookup<ffi.NativeFunction<_c_PlayerSetIsLooping>>(
          'PlayerSetIsAutoRepeat');
  late final _dart_PlayerSetIsLooping _PlayerSetIsAutoRepeat =
      _PlayerSetIsAutoRepeat_ptr.asFunction<_dart_PlayerSetIsAutoRepeat>();

  int PlayerGetPosition(
    int player_id,
  ) {
    return _PlayerGetPosition(
      player_id,
    );
  }

  late final _PlayerGetPosition_ptr =
      _lookup<ffi.NativeFunction<_c_PlayerGetPosition>>('PlayerGetPosition');
  late final _dart_PlayerGetPosition _PlayerGetPosition =
      _PlayerGetPosition_ptr.asFunction<_dart_PlayerGetPosition>();

  double PlayerGetVolume(
    int player_id,
  ) {
    return _PlayerGetVolume(
      player_id,
    );
  }

  late final _PlayerGetVolume_ptr =
      _lookup<ffi.NativeFunction<_c_PlayerGetVolume>>('PlayerGetVolume');
  late final _dart_PlayerGetVolume _PlayerGetVolume =
      _PlayerGetVolume_ptr.asFunction<_dart_PlayerGetVolume>();

  double PlayerGetRate(
    int player_id,
  ) {
    return _PlayerGetRate(
      player_id,
    );
  }

  late final _PlayerGetRate_ptr =
      _lookup<ffi.NativeFunction<_c_PlayerGetRate>>('PlayerGetRate');
  late final _dart_PlayerGetRate _PlayerGetRate =
      _PlayerGetRate_ptr.asFunction<_dart_PlayerGetRate>();

  double PlayerGetAudioBalance(
    int player_id,
  ) {
    return _PlayerGetAudioBalance(
      player_id,
    );
  }

  late final _PlayerGetAudioBalance_ptr =
      _lookup<ffi.NativeFunction<_c_PlayerGetAudioBalance>>(
          'PlayerGetAudioBalance');
  late final _dart_PlayerGetAudioBalance _PlayerGetAudioBalance =
      _PlayerGetAudioBalance_ptr.asFunction<_dart_PlayerGetAudioBalance>();

  double? PlayerGetDownloadProgress(
    int player_id,
  ) {
    return _PlayerGetDownloadProgress(
      player_id,
    );
  }

  late final _PlayerGetDownloadProgress_ptr =
      _lookup<ffi.NativeFunction<_c_PlayerGetDownloadProgress>>(
          'PlayerGetDownloadProgress');
  late final _dart_PlayerGetDownloadProgress _PlayerGetDownloadProgress =
      _PlayerGetDownloadProgress_ptr.asFunction<
          _dart_PlayerGetDownloadProgress>();

  bool PlayerIsAutoplay(
    int player_id,
  ) {
    return _PlayerIsAutoplay(
          player_id,
        ) !=
        0;
  }

  late final _PlayerIsAutoplay_ptr =
      _lookup<ffi.NativeFunction<_c_PlayerIsAutoplay>>('PlayerIsAutoplay');
  late final _dart_PlayerIsAutoplay _PlayerIsAutoplay =
      _PlayerIsAutoplay_ptr.asFunction<_dart_PlayerIsAutoplay>();

  bool PlayerIsLooping(
    int player_id,
  ) {
    return _PlayerIsLooping(
          player_id,
        ) !=
        0;
  }

  late final _PlayerIsLooping_ptr =
      _lookup<ffi.NativeFunction<_c_PlayerIsLooping>>('PlayerIsLooping');
  late final _dart_PlayerIsLooping _PlayerIsLooping =
      _PlayerIsLooping_ptr.asFunction<_dart_PlayerIsLooping>();

  bool PlayerIsAutoRepeat(
    int player_id,
  ) {
    return _PlayerIsAutoRepeat(
          player_id,
        ) !=
        0;
  }

  late final _PlayerIsAutoRepeat_ptr =
      _lookup<ffi.NativeFunction<_c_PlayerIsAutoRepeat>>('PlayerIsAutoRepeat');
  late final _dart_PlayerIsAutoRepeat _PlayerIsAutoRepeat =
      _PlayerIsLooping_ptr.asFunction<_dart_PlayerIsAutoRepeat>();

  bool PlayerIsShuffling(
    int player_id,
  ) {
    return _PlayerIsShuffling(
          player_id,
        ) !=
        0;
  }

  late final _PlayerIsShuffling_ptr =
      _lookup<ffi.NativeFunction<_c_PlayerIsShuffling>>('PlayerIsShuffling');
  late final _dart_PlayerIsShuffling _PlayerIsShuffling =
      _PlayerIsLooping_ptr.asFunction<_dart_PlayerIsShuffling>();

  void PlayerSetIsPlayingEventHandler(
    int player_id,
    ffi.Pointer<ffi.NativeFunction<_typedefC_1>> callback,
  ) {
    return _PlayerSetIsPlayingEventHandler(
      player_id,
      callback,
    );
  }

  late final _PlayerSetIsPlayingEventHandler_ptr =
      _lookup<ffi.NativeFunction<_c_PlayerSetIsPlayingEventHandler>>(
          'PlayerSetIsPlayingEventHandler');
  late final _dart_PlayerSetIsPlayingEventHandler
      _PlayerSetIsPlayingEventHandler = _PlayerSetIsPlayingEventHandler_ptr
          .asFunction<_dart_PlayerSetIsPlayingEventHandler>();

  void PlayerSetIsCompletedEventHandler(
    int player_id,
    ffi.Pointer<ffi.NativeFunction<_typedefC_2>> callback,
  ) {
    return _PlayerSetIsCompletedEventHandler(
      player_id,
      callback,
    );
  }

  late final _PlayerSetIsCompletedEventHandler_ptr =
      _lookup<ffi.NativeFunction<_c_PlayerSetIsCompletedEventHandler>>(
          'PlayerSetIsCompletedEventHandler');
  late final _dart_PlayerSetIsCompletedEventHandler
      _PlayerSetIsCompletedEventHandler = _PlayerSetIsCompletedEventHandler_ptr
          .asFunction<_dart_PlayerSetIsCompletedEventHandler>();

  void PlayerSetIsBufferingEventHandler(
    int player_id,
    ffi.Pointer<ffi.NativeFunction<_typedefC_3>> callback,
  ) {
    return _PlayerSetIsBufferingEventHandler(
      player_id,
      callback,
    );
  }

  late final _PlayerSetIsBufferingEventHandler_ptr =
      _lookup<ffi.NativeFunction<_c_PlayerSetIsBufferingEventHandler>>(
          'PlayerSetIsBufferingEventHandler');
  late final _dart_PlayerSetIsBufferingEventHandler
      _PlayerSetIsBufferingEventHandler = _PlayerSetIsBufferingEventHandler_ptr
          .asFunction<_dart_PlayerSetIsBufferingEventHandler>();

  void PlayerSetVolumeEventHandler(
    int player_id,
    ffi.Pointer<ffi.NativeFunction<_typedefC_4>> callback,
  ) {
    return _PlayerSetVolumeEventHandler(
      player_id,
      callback,
    );
  }

  late final _PlayerSetVolumeEventHandler_ptr =
      _lookup<ffi.NativeFunction<_c_PlayerSetVolumeEventHandler>>(
          'PlayerSetVolumeEventHandler');
  late final _dart_PlayerSetVolumeEventHandler _PlayerSetVolumeEventHandler =
      _PlayerSetVolumeEventHandler_ptr.asFunction<
          _dart_PlayerSetVolumeEventHandler>();

  void PlayerSetRateEventHandler(
    int player_id,
    ffi.Pointer<ffi.NativeFunction<_typedefC_5>> callback,
  ) {
    return _PlayerSetRateEventHandler(
      player_id,
      callback,
    );
  }

  late final _PlayerSetRateEventHandler_ptr =
      _lookup<ffi.NativeFunction<_c_PlayerSetRateEventHandler>>(
          'PlayerSetRateEventHandler');
  late final _dart_PlayerSetRateEventHandler _PlayerSetRateEventHandler =
      _PlayerSetRateEventHandler_ptr.asFunction<
          _dart_PlayerSetRateEventHandler>();

  void PlayerSetPositionEventHandler(
    int player_id,
    ffi.Pointer<ffi.NativeFunction<_typedefC_6>> callback,
  ) {
    return _PlayerSetPositionEventHandler(
      player_id,
      callback,
    );
  }

  late final _PlayerSetPositionEventHandler_ptr =
      _lookup<ffi.NativeFunction<_c_PlayerSetPositionEventHandler>>(
          'PlayerSetPositionEventHandler');
  late final _dart_PlayerSetPositionEventHandler
      _PlayerSetPositionEventHandler = _PlayerSetPositionEventHandler_ptr
          .asFunction<_dart_PlayerSetPositionEventHandler>();

  void PlayerSetDurationEventHandler(
    int player_id,
    ffi.Pointer<ffi.NativeFunction<_typedefC_7>> callback,
  ) {
    return _PlayerSetDurationEventHandler(
      player_id,
      callback,
    );
  }

  late final _PlayerSetDurationEventHandler_ptr =
      _lookup<ffi.NativeFunction<_c_PlayerSetDurationEventHandler>>(
          'PlayerSetDurationEventHandler');
  late final _dart_PlayerSetDurationEventHandler
      _PlayerSetDurationEventHandler = _PlayerSetDurationEventHandler_ptr
          .asFunction<_dart_PlayerSetDurationEventHandler>();

  void PlayerSetIndexEventHandler(
    int player_id,
    ffi.Pointer<ffi.NativeFunction<_typedefC_8>> callback,
  ) {
    return _PlayerSetIndexEventHandler(
      player_id,
      callback,
    );
  }

  late final _PlayerSetIndexEventHandler_ptr =
      _lookup<ffi.NativeFunction<_c_PlayerSetIndexEventHandler>>(
          'PlayerSetIndexEventHandler');
  late final _dart_PlayerSetIndexEventHandler _PlayerSetIndexEventHandler =
      _PlayerSetIndexEventHandler_ptr.asFunction<
          _dart_PlayerSetIndexEventHandler>();

  void PlayerSetDownloadProgressEventHandler(
    int player_id,
    ffi.Pointer<ffi.NativeFunction<_typedefC_5>> callback,
  ) {
    return _PlayerSetDownloadProgressEventHandler(
      player_id,
      callback,
    );
  }

  late final _PlayerSetDownloadProgressEventHandler_ptr =
      _lookup<ffi.NativeFunction<_c_PlayerSetDownloadProgressEventHandler>>(
          'PlayerSetDownloadProgressEventHandler');
  late final _dart_PlayerSetDownloadProgressEventHandler
      _PlayerSetDownloadProgressEventHandler =
      _PlayerSetDownloadProgressEventHandler_ptr.asFunction<
          _dart_PlayerSetDownloadProgressEventHandler>();

  void PlayerSetErrorEventHandler(
    int player_id,
    ffi.Pointer<ffi.NativeFunction<_typedefC_11>> callback,
  ) {
    return _PlayerSetErrorEventHandler(
      player_id,
      callback,
    );
  }

  late final _PlayerSetErrorEventHandler_ptr =
      _lookup<ffi.NativeFunction<_c_PlayerSetErrorEventHandler>>(
          'PlayerSetErrorEventHandler');
  late final _dart_PlayerSetErrorEventHandler _PlayerSetErrorEventHandler =
      _PlayerSetErrorEventHandler_ptr.asFunction<
          _dart_PlayerSetErrorEventHandler>();

  void PlayerNativeControlsCreate(
    int player_id,
    ffi.Pointer<ffi.NativeFunction<_typedefC_9>> callback,
  ) {
    return _PlayerNativeControlsCreate(
      player_id,
      callback,
    );
  }

  late final _PlayerNativeControlsCreate_ptr =
      _lookup<ffi.NativeFunction<_c_PlayerNativeControlsCreate>>(
          'PlayerNativeControlsCreate');
  late final _dart_PlayerNativeControlsCreate _PlayerNativeControlsCreate =
      _PlayerNativeControlsCreate_ptr.asFunction<
          _dart_PlayerNativeControlsCreate>();

  void PlayerNativeControlsSetStatus(
    int player_id,
    int status,
  ) {
    return _PlayerNativeControlsSetStatus(
      player_id,
      status,
    );
  }

  late final _PlayerNativeControlsSetStatus_ptr =
      _lookup<ffi.NativeFunction<_c_PlayerNativeControlsSetStatus>>(
          'PlayerNativeControlsSetStatus');
  late final _dart_PlayerNativeControlsSetStatus
      _PlayerNativeControlsSetStatus = _PlayerNativeControlsSetStatus_ptr
          .asFunction<_dart_PlayerNativeControlsSetStatus>();

  void PlayerNativeControlsUpdate(
    int player_id,
    int type,
    ffi.Pointer<ffi.Pointer<ffi.Utf8>> data,
    ffi.Pointer<ffi.Utf8> thumbnail,
  ) {
    return _PlayerNativeControlsUpdate(
      player_id,
      type,
      data,
      thumbnail,
    );
  }

  late final _PlayerNativeControlsUpdate_ptr =
      _lookup<ffi.NativeFunction<_c_PlayerNativeControlsUpdate>>(
          'PlayerNativeControlsUpdate');
  late final _dart_PlayerNativeControlsUpdate _PlayerNativeControlsUpdate =
      _PlayerNativeControlsUpdate_ptr.asFunction<
          _dart_PlayerNativeControlsUpdate>();

  void PlayerNativeControlsClear(
    int player_id,
  ) {
    return _PlayerNativeControlsClear(
      player_id,
    );
  }

  late final _PlayerNativeControlsClear_ptr =
      _lookup<ffi.NativeFunction<_c_PlayerNativeControlsClear>>(
          'PlayerNativeControlsClear');
  late final _dart_PlayerNativeControlsClear _PlayerNativeControlsClear =
      _PlayerNativeControlsClear_ptr.asFunction<
          _dart_PlayerNativeControlsClear>();

  void PlayerNativeControlsDispose(
    int player_id,
  ) {
    return _PlayerNativeControlsDispose(
      player_id,
    );
  }

  late final _PlayerNativeControlsDispose_ptr =
      _lookup<ffi.NativeFunction<_c_PlayerNativeControlsDispose>>(
          'PlayerNativeControlsDispose');
  late final _dart_PlayerNativeControlsDispose _PlayerNativeControlsDispose =
      _PlayerNativeControlsDispose_ptr.asFunction<
          _dart_PlayerNativeControlsDispose>();

  void MediaCreate(
    int media_id,
    ffi.Pointer<ffi.Utf8> uri,
    bool parse,
  ) {
    return _MediaCreate(
      media_id,
      uri,
      parse ? 1 : 0,
    );
  }

  late final _MediaCreate_ptr =
      _lookup<ffi.NativeFunction<_c_MediaCreate>>('MediaCreate');
  late final _dart_MediaCreate _MediaCreate =
      _MediaCreate_ptr.asFunction<_dart_MediaCreate>();

  void MediaDispose(
    int media_id,
  ) {
    return _MediaDispose(
      media_id,
    );
  }

  late final _MediaDispose_ptr =
      _lookup<ffi.NativeFunction<_c_MediaDispose>>('MediaDispose');
  late final _dart_MediaDispose _MediaDispose =
      _MediaDispose_ptr.asFunction<_dart_MediaDispose>();

  int MediaGetDuration(
    int media_id,
  ) {
    return _MediaGetDuration(
      media_id,
    );
  }

  late final _MediaGetDuration_ptr =
      _lookup<ffi.NativeFunction<_c_MediaGetDuration>>('MediaGetDuration');
  late final _dart_MediaGetDuration _MediaGetDuration =
      _MediaGetDuration_ptr.asFunction<_dart_MediaGetDuration>();

  ffi.Pointer<ffi.Pointer<ffi.Int32>> TagsFromMusic(
    ffi.Pointer<ffi.Int32> uri,
  ) {
    return _TagsFromMusic(
      uri,
    );
  }

  late final _TagsFromMusic_ptr =
      _lookup<ffi.NativeFunction<_c_TagsFromMusic>>('TagsFromMusic');
  late final _dart_TagsFromMusic _TagsFromMusic =
      _TagsFromMusic_ptr.asFunction<_dart_TagsFromMusic>();

  ffi.Pointer<ffi.Pointer<ffi.Int32>> TagsFromVideo(
    ffi.Pointer<ffi.Int32> uri,
  ) {
    return _TagsFromVideo(
      uri,
    );
  }

  late final _TagsFromVideo_ptr =
      _lookup<ffi.NativeFunction<_c_TagsFromVideo>>('TagsFromVideo');
  late final _dart_TagsFromVideo _TagsFromVideo =
      _TagsFromVideo_ptr.asFunction<_dart_TagsFromVideo>();

  void TagsExtractThumbnail(
    ffi.Pointer<ffi.Int32> media,
    ffi.Pointer<ffi.Int32> folder,
    ffi.Pointer<ffi.Int32> file_name,
    int mode,
    int size,
  ) {
    return _TagsExtractThumbnail(
      media,
      folder,
      file_name,
      mode,
      size,
    );
  }

  late final _TagsExtractThumbnail_ptr =
      _lookup<ffi.NativeFunction<_c_TagsExtractThumbnail>>(
          'TagsExtractThumbnail');
  late final _dart_TagsExtractThumbnail _TagsExtractThumbnail =
      _TagsExtractThumbnail_ptr.asFunction<_dart_TagsExtractThumbnail>();

  void NativeControlsCreate(
    ffi.Pointer<ffi.NativeFunction<_typedefC_10>> callback,
  ) {
    return _NativeControlsCreate(
      callback,
    );
  }

  late final _NativeControlsCreate_ptr =
      _lookup<ffi.NativeFunction<_c_NativeControlsCreate>>(
          'NativeControlsCreate');
  late final _dart_NativeControlsCreate _NativeControlsCreate =
      _NativeControlsCreate_ptr.asFunction<_dart_NativeControlsCreate>();

  void NativeControlsSetStatus(
    int status,
  ) {
    return _NativeControlsSetStatus(
      status,
    );
  }

  late final _NativeControlsSetStatus_ptr =
      _lookup<ffi.NativeFunction<_c_NativeControlsSetStatus>>(
          'NativeControlsSetStatus');
  late final _dart_NativeControlsSetStatus _NativeControlsSetStatus =
      _NativeControlsSetStatus_ptr.asFunction<_dart_NativeControlsSetStatus>();

  void NativeControlsUpdate(
    int type,
    ffi.Pointer<ffi.Pointer<ffi.Utf8>> data,
    ffi.Pointer<ffi.Utf8> thumbnail,
  ) {
    return _NativeControlsUpdate(
      type,
      data,
      thumbnail,
    );
  }

  late final _NativeControlsUpdate_ptr =
      _lookup<ffi.NativeFunction<_c_NativeControlsUpdate>>(
          'NativeControlsUpdate');
  late final _dart_NativeControlsUpdate _NativeControlsUpdate =
      _NativeControlsUpdate_ptr.asFunction<_dart_NativeControlsUpdate>();

  void NativeControlsClear() {
    return _NativeControlsClear();
  }

  late final _NativeControlsClear_ptr =
      _lookup<ffi.NativeFunction<_c_NativeControlsClear>>(
          'NativeControlsClear');
  late final _dart_NativeControlsClear _NativeControlsClear =
      _NativeControlsClear_ptr.asFunction<_dart_NativeControlsClear>();

  void NativeControlsDispose() {
    return _NativeControlsDispose();
  }

  late final _NativeControlsDispose_ptr =
      _lookup<ffi.NativeFunction<_c_NativeControlsDispose>>(
          'NativeControlsDispose');
  late final _dart_NativeControlsDispose _NativeControlsDispose =
      _NativeControlsDispose_ptr.asFunction<_dart_NativeControlsDispose>();
}

class __fsid_t extends ffi.Struct {
  @ffi.Array.multi([2])
  external ffi.Array<ffi.Int32> __val;
}

class __mbstate_t extends ffi.Opaque {}

class _IO_FILE extends ffi.Opaque {}

class __locale_data extends ffi.Opaque {}

class __locale_struct extends ffi.Struct {
  @ffi.Array.multi([13])
  external ffi.Array<ffi.Pointer<__locale_data>> __locales;

  external ffi.Pointer<ffi.Uint16> __ctype_b;

  external ffi.Pointer<ffi.Int32> __ctype_tolower;

  external ffi.Pointer<ffi.Int32> __ctype_toupper;

  @ffi.Array.multi([13])
  external ffi.Array<ffi.Pointer<ffi.Int8>> __names;
}

class tm extends ffi.Opaque {}

class __va_list_tag extends ffi.Struct {
  @ffi.Uint32()
  external int gp_offset;

  @ffi.Uint32()
  external int fp_offset;

  external ffi.Pointer<ffi.Void> overflow_arg_area;

  external ffi.Pointer<ffi.Void> reg_save_area;
}

typedef _c_InitializeDartApi = ffi.Void Function(
  ffi.Pointer<
          ffi.NativeFunction<
              ffi.Int8 Function(ffi.Int64, ffi.Pointer<ffi.Dart_CObject>)>>
      post_C_object,
  ffi.Int64 dart_port,
);

typedef _dart_InitializeDartApi = void Function(
  ffi.Pointer<
          ffi.NativeFunction<
              ffi.Int8 Function(ffi.Int64, ffi.Pointer<ffi.Dart_CObject>)>>
      post_C_object,
  int dart_port,
);

typedef _c_PlayerShowWindow = ffi.Void Function(
  ffi.Int32 player_id,
  ffi.Pointer<ffi.Utf8> window_title,
);

typedef _dart_PlayerShowWindow = void Function(
  int player_id,
  ffi.Pointer<ffi.Utf8> window_title,
);

typedef _c_PlayerCloseWindow = ffi.Void Function(
  ffi.Int32 player_id,
);

typedef _dart_PlayerCloseWindow = void Function(
  int player_id,
);

typedef _c_PlayerCreate = ffi.Void Function(
  ffi.Int32 player_id,
  ffi.Uint8 show_window,
  ffi.Pointer<ffi.Utf8> window_title,
);

typedef _dart_PlayerCreate = void Function(
  int player_id,
  int show_window,
  ffi.Pointer<ffi.Utf8> window_title,
);

typedef _c_PlayerDispose = ffi.Void Function(
  ffi.Int32 player_id,
);

typedef _dart_PlayerDispose = void Function(
  int player_id,
);

typedef _c_PlayerOpen = ffi.Void Function(
  ffi.Int32 player_id,
  ffi.Int32 size,
  ffi.Pointer<ffi.Pointer<ffi.Utf8>> uris,
  ffi.Pointer<ffi.Int32> ids,
);

typedef _dart_PlayerOpen = void Function(
  int player_id,
  int size,
  ffi.Pointer<ffi.Pointer<ffi.Utf8>> uris,
  ffi.Pointer<ffi.Int32> ids,
);

typedef _c_PlayerPlay = ffi.Void Function(
  ffi.Int32 player_id,
);

typedef _dart_PlayerPlay = void Function(
  int player_id,
);

typedef _c_PlayerPause = ffi.Void Function(
  ffi.Int32 player_id,
);

typedef _dart_PlayerPause = void Function(
  int player_id,
);

typedef _c_PlayerAdd = ffi.Void Function(
  ffi.Int32 player_id,
  ffi.Pointer<ffi.Utf8> uri,
  ffi.Int32 id,
);

typedef _dart_PlayerAdd = void Function(
  int player_id,
  ffi.Pointer<ffi.Utf8> uri,
  int id,
);

typedef _c_PlayerRemove = ffi.Void Function(
  ffi.Int32 player_id,
  ffi.Int32 index,
);

typedef _dart_PlayerRemove = void Function(
  int player_id,
  int index,
);

typedef _c_PlayerNext = ffi.Void Function(
  ffi.Int32 player_id,
);

typedef _dart_PlayerNext = void Function(
  int player_id,
);

typedef _c_PlayerBack = ffi.Void Function(
  ffi.Int32 player_id,
);

typedef _dart_PlayerBack = void Function(
  int player_id,
);

typedef _c_PlayerJump = ffi.Void Function(
  ffi.Int32 player_id,
  ffi.Int32 index,
);

typedef _dart_PlayerJump = void Function(
  int player_id,
  int index,
);

typedef _c_PlayerSeek = ffi.Void Function(
  ffi.Int32 player_id,
  ffi.Int32 position,
);

typedef _dart_PlayerSeek = void Function(
  int player_id,
  int position,
);

typedef _c_PlayerSetVolume = ffi.Void Function(
  ffi.Int32 player_id,
  ffi.Float volume,
);

typedef _dart_PlayerSetVolume = void Function(
  int player_id,
  double volume,
);

typedef _c_PlayerSetRate = ffi.Void Function(
  ffi.Int32 player_id,
  ffi.Float rate,
);

typedef _dart_PlayerSetRate = void Function(
  int player_id,
  double rate,
);

typedef _c_PlayerSetAudioBalance = ffi.Void Function(
  ffi.Int32 player_id,
  ffi.Float audio_balance,
);

typedef _dart_PlayerSetAudioBalance = void Function(
  int player_id,
  double audio_balance,
);

typedef _c_PlayerSetAutoplay = ffi.Void Function(
  ffi.Int32 player_id,
  ffi.Uint8 autoplay,
);

typedef _dart_PlayerSetAutoplay = void Function(
  int player_id,
  int autoplay,
);

typedef _c_PlayerSetIsLooping = ffi.Void Function(
  ffi.Int32 player_id,
  ffi.Uint8 looping,
);

typedef _dart_PlayerSetIsLooping = void Function(
  int player_id,
  int looping,
);

typedef _c_PlayerSetIsShuffling = ffi.Void Function(
  ffi.Int32 player_id,
  ffi.Uint8 shuffling,
);

typedef _dart_PlayerSetIsShuffling = void Function(
  int player_id,
  int shuffling,
);

typedef _c_PlayerSetIsAutoRepeat = ffi.Void Function(
  ffi.Int32 player_id,
  ffi.Uint8 enabled,
);

typedef _dart_PlayerSetIsAutoRepeat = void Function(
  int player_id,
  int enabled,
);

typedef _c_PlayerGetPosition = ffi.Int32 Function(
  ffi.Int32 player_id,
);

typedef _dart_PlayerGetPosition = int Function(
  int player_id,
);

typedef _c_PlayerGetVolume = ffi.Float Function(
  ffi.Int32 player_id,
);

typedef _dart_PlayerGetVolume = double Function(
  int player_id,
);

typedef _c_PlayerGetRate = ffi.Float Function(
  ffi.Int32 player_id,
);

typedef _dart_PlayerGetRate = double Function(
  int player_id,
);

typedef _c_PlayerGetAudioBalance = ffi.Float Function(
  ffi.Int32 player_id,
);

typedef _dart_PlayerGetAudioBalance = double Function(
  int player_id,
);

typedef _c_PlayerGetDownloadProgress = ffi.Float Function(
  ffi.Int32 player_id,
);

typedef _dart_PlayerGetDownloadProgress = double? Function(
  int player_id,
);

typedef _c_PlayerIsAutoplay = ffi.Uint8 Function(
  ffi.Int32 player_id,
);

typedef _dart_PlayerIsAutoplay = int Function(
  int player_id,
);

typedef _c_PlayerIsLooping = ffi.Uint8 Function(
  ffi.Int32 player_id,
);

typedef _dart_PlayerIsLooping = int Function(
  int player_id,
);

typedef _c_PlayerIsAutoRepeat = ffi.Uint8 Function(
  ffi.Int32 player_id,
);

typedef _dart_PlayerIsAutoRepeat = int Function(
  int player_id,
);

typedef _c_PlayerIsShuffling = ffi.Uint8 Function(
  ffi.Int32 player_id,
);

typedef _dart_PlayerIsShuffling = int Function(
  int player_id,
);

typedef _typedefC_1 = ffi.Void Function(
  ffi.Uint8,
);

typedef _c_PlayerSetIsPlayingEventHandler = ffi.Void Function(
  ffi.Int32 player_id,
  ffi.Pointer<ffi.NativeFunction<_typedefC_1>> callback,
);

typedef _dart_PlayerSetIsPlayingEventHandler = void Function(
  int player_id,
  ffi.Pointer<ffi.NativeFunction<_typedefC_1>> callback,
);

typedef _typedefC_2 = ffi.Void Function(
  ffi.Uint8,
);

typedef _c_PlayerSetIsCompletedEventHandler = ffi.Void Function(
  ffi.Int32 player_id,
  ffi.Pointer<ffi.NativeFunction<_typedefC_2>> callback,
);

typedef _dart_PlayerSetIsCompletedEventHandler = void Function(
  int player_id,
  ffi.Pointer<ffi.NativeFunction<_typedefC_2>> callback,
);

typedef _typedefC_3 = ffi.Void Function(
  ffi.Uint8,
);

typedef _c_PlayerSetIsBufferingEventHandler = ffi.Void Function(
  ffi.Int32 player_id,
  ffi.Pointer<ffi.NativeFunction<_typedefC_3>> callback,
);

typedef _dart_PlayerSetIsBufferingEventHandler = void Function(
  int player_id,
  ffi.Pointer<ffi.NativeFunction<_typedefC_3>> callback,
);

typedef _typedefC_4 = ffi.Void Function(
  ffi.Float,
);

typedef _c_PlayerSetVolumeEventHandler = ffi.Void Function(
  ffi.Int32 player_id,
  ffi.Pointer<ffi.NativeFunction<_typedefC_4>> callback,
);

typedef _dart_PlayerSetVolumeEventHandler = void Function(
  int player_id,
  ffi.Pointer<ffi.NativeFunction<_typedefC_4>> callback,
);

typedef _typedefC_5 = ffi.Void Function(
  ffi.Float,
);

typedef _c_PlayerSetRateEventHandler = ffi.Void Function(
  ffi.Int32 player_id,
  ffi.Pointer<ffi.NativeFunction<_typedefC_5>> callback,
);

typedef _dart_PlayerSetRateEventHandler = void Function(
  int player_id,
  ffi.Pointer<ffi.NativeFunction<_typedefC_5>> callback,
);

typedef _typedefC_6 = ffi.Void Function(
  ffi.Int32,
);

typedef _c_PlayerSetPositionEventHandler = ffi.Void Function(
  ffi.Int32 player_id,
  ffi.Pointer<ffi.NativeFunction<_typedefC_6>> callback,
);

typedef _dart_PlayerSetPositionEventHandler = void Function(
  int player_id,
  ffi.Pointer<ffi.NativeFunction<_typedefC_6>> callback,
);

typedef _typedefC_7 = ffi.Void Function(
  ffi.Int32,
);

typedef _c_PlayerSetDurationEventHandler = ffi.Void Function(
  ffi.Int32 player_id,
  ffi.Pointer<ffi.NativeFunction<_typedefC_7>> callback,
);

typedef _dart_PlayerSetDurationEventHandler = void Function(
  int player_id,
  ffi.Pointer<ffi.NativeFunction<_typedefC_7>> callback,
);

typedef _typedefC_8 = ffi.Void Function(
  ffi.Int32,
);

typedef _c_PlayerSetIndexEventHandler = ffi.Void Function(
  ffi.Int32 player_id,
  ffi.Pointer<ffi.NativeFunction<_typedefC_8>> callback,
);

typedef _dart_PlayerSetIndexEventHandler = void Function(
  int player_id,
  ffi.Pointer<ffi.NativeFunction<_typedefC_8>> callback,
);

typedef _c_PlayerSetDownloadProgressEventHandler = ffi.Void Function(
  ffi.Int32 player_id,
  ffi.Pointer<ffi.NativeFunction<_typedefC_5>> callback,
);

typedef _dart_PlayerSetDownloadProgressEventHandler = void Function(
  int player_id,
  ffi.Pointer<ffi.NativeFunction<_typedefC_5>> callback,
);

typedef _c_PlayerSetErrorEventHandler = ffi.Void Function(
  ffi.Int32 player_id,
  ffi.Pointer<ffi.NativeFunction<_typedefC_11>> callback,
);

typedef _dart_PlayerSetErrorEventHandler = void Function(
  int player_id,
  ffi.Pointer<ffi.NativeFunction<_typedefC_11>> callback,
);

typedef _typedefC_9 = ffi.Void Function(
  ffi.Int32,
);

typedef _c_PlayerNativeControlsCreate = ffi.Void Function(
  ffi.Int32 player_id,
  ffi.Pointer<ffi.NativeFunction<_typedefC_9>> callback,
);

typedef _dart_PlayerNativeControlsCreate = void Function(
  int player_id,
  ffi.Pointer<ffi.NativeFunction<_typedefC_9>> callback,
);

typedef _c_PlayerNativeControlsSetStatus = ffi.Void Function(
  ffi.Int32 player_id,
  ffi.Int32 status,
);

typedef _dart_PlayerNativeControlsSetStatus = void Function(
  int player_id,
  int status,
);

typedef _c_PlayerNativeControlsUpdate = ffi.Void Function(
  ffi.Int32 player_id,
  ffi.Int32 type,
  ffi.Pointer<ffi.Pointer<ffi.Utf8>> data,
  ffi.Pointer<ffi.Utf8> thumbnail,
);

typedef _dart_PlayerNativeControlsUpdate = void Function(
  int player_id,
  int type,
  ffi.Pointer<ffi.Pointer<ffi.Utf8>> data,
  ffi.Pointer<ffi.Utf8> thumbnail,
);

typedef _c_PlayerNativeControlsClear = ffi.Void Function(
  ffi.Int32 player_id,
);

typedef _dart_PlayerNativeControlsClear = void Function(
  int player_id,
);

typedef _c_PlayerNativeControlsDispose = ffi.Void Function(
  ffi.Int32 player_id,
);

typedef _dart_PlayerNativeControlsDispose = void Function(
  int player_id,
);

typedef _c_MediaCreate = ffi.Void Function(
  ffi.Int32 media_id,
  ffi.Pointer<ffi.Utf8> uri,
  ffi.Uint8 parse,
);

typedef _dart_MediaCreate = void Function(
  int media_id,
  ffi.Pointer<ffi.Utf8> uri,
  int parse,
);

typedef _c_MediaDispose = ffi.Void Function(
  ffi.Int32 media_id,
);

typedef _dart_MediaDispose = void Function(
  int media_id,
);

typedef _c_MediaGetDuration = ffi.Int32 Function(
  ffi.Int32 media_id,
);

typedef _dart_MediaGetDuration = int Function(
  int media_id,
);

typedef _c_TagsFromMusic = ffi.Pointer<ffi.Pointer<ffi.Int32>> Function(
  ffi.Pointer<ffi.Int32> uri,
);

typedef _dart_TagsFromMusic = ffi.Pointer<ffi.Pointer<ffi.Int32>> Function(
  ffi.Pointer<ffi.Int32> uri,
);

typedef _c_TagsFromVideo = ffi.Pointer<ffi.Pointer<ffi.Int32>> Function(
  ffi.Pointer<ffi.Int32> uri,
);

typedef _dart_TagsFromVideo = ffi.Pointer<ffi.Pointer<ffi.Int32>> Function(
  ffi.Pointer<ffi.Int32> uri,
);

typedef _c_TagsExtractThumbnail = ffi.Void Function(
  ffi.Pointer<ffi.Int32> media,
  ffi.Pointer<ffi.Int32> folder,
  ffi.Pointer<ffi.Int32> file_name,
  ffi.Int32 mode,
  ffi.Int32 size,
);

typedef _dart_TagsExtractThumbnail = void Function(
  ffi.Pointer<ffi.Int32> media,
  ffi.Pointer<ffi.Int32> folder,
  ffi.Pointer<ffi.Int32> file_name,
  int mode,
  int size,
);

typedef _typedefC_10 = ffi.Void Function(
  ffi.Int32,
);

typedef _typedefC_11 = ffi.Void Function(
  ffi.Int32,
  ffi.Pointer<ffi.Utf8>,
);

typedef _c_NativeControlsCreate = ffi.Void Function(
  ffi.Pointer<ffi.NativeFunction<_typedefC_10>> callback,
);

typedef _dart_NativeControlsCreate = void Function(
  ffi.Pointer<ffi.NativeFunction<_typedefC_10>> callback,
);

typedef _c_NativeControlsSetStatus = ffi.Void Function(
  ffi.Int32 status,
);

typedef _dart_NativeControlsSetStatus = void Function(
  int status,
);

typedef _c_NativeControlsUpdate = ffi.Void Function(
  ffi.Int32 type,
  ffi.Pointer<ffi.Pointer<ffi.Utf8>> data,
  ffi.Pointer<ffi.Utf8> thumbnail,
);

typedef _dart_NativeControlsUpdate = void Function(
  int type,
  ffi.Pointer<ffi.Pointer<ffi.Utf8>> data,
  ffi.Pointer<ffi.Utf8> thumbnail,
);

typedef _c_NativeControlsClear = ffi.Void Function();

typedef _dart_NativeControlsClear = void Function();

typedef _c_NativeControlsDispose = ffi.Void Function();

typedef _dart_NativeControlsDispose = void Function();
