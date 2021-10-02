import 'dart:ffi';
import 'package:ffi/ffi.dart';

/// For converting `List<String>` into `wchar_t*[]`.
/// Requires calling `calloc.free` on the returned value after usage.
///
extension NativeTypesString on List<String> {
  Pointer<Pointer<Utf8>> toNativeUtf8Array() {
    final list =
        map((string) => string.toNativeUtf8()).toList().cast<Pointer<Utf8>>();
    final pointer = calloc.allocate<Pointer<Utf8>>(list.length * 8);
    for (var index = 0; index < length; index++) {
      pointer[index] = list[index];
    }
    return pointer;
  }
}

/// For converting `List<int>` into `int[]`.
/// Requires calling `calloc.free` on the returned value after usage.
///
extension NativeTypesInt on List<int> {
  Pointer<Int32> toNativeArray() {
    final pointer = calloc.allocate<Int32>(length * 4);
    for (var index = 0; index < length; index++) {
      pointer[index] = this[index];
    }
    return pointer;
  }
}

/// A template function to call few functions exposed by the `libwinmedia.dll`, never gets invoked.
void intC(int _) {}

/// A template function to call few functions exposed by the `libwinmedia.dll`, never gets invoked.
void doubleC(double _) {}

/// A template function to call few functions exposed by the `libwinmedia.dll`, never gets invoked.
void errorC(int _, Pointer<Utf8> __) {}
