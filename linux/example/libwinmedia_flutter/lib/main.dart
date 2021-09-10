import 'dart:isolate';

import 'package:ffi/ffi.dart';
import 'package:flutter/material.dart';
import 'dart:ffi';

import 'package:libwinmedia/generated/bindings.dart';

var path = '/home/alexmercerind/Documents/libwinmedia/libwinmedia.so';
var lwm = LWM(DynamicLibrary.open(path));
var receiver = ReceivePort()
  ..listen(
    (message) {
      print(message);
    },
  );

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

extension NativeTypesInt on List<int> {
  Pointer<Int32> toNativeArray() {
    final pointer = calloc.allocate<Int32>(length * 4);
    for (var index = 0; index < length; index++) {
      pointer[index] = this[index];
    }
    return pointer;
  }
}

void intC(int _) {}
void doubleC(double _) {}

void main() {
  lwm.InitializeDartApi(NativeApi.postCObject, receiver.sendPort.nativePort);
  lwm.PlayerCreate(0, false, ''.toNativeUtf8());
  runApp(MyApp());
}

class MyApp extends StatelessWidget {
  // This widget is the root of your application.
  @override
  Widget build(BuildContext context) {
    return MaterialApp(
      title: 'Flutter Demo',
      theme: ThemeData(
        // This is the theme of your application.
        //
        // Try running your application with "flutter run". You'll see the
        // application has a blue toolbar. Then, without quitting the app, try
        // changing the primarySwatch below to Colors.green and then invoke
        // "hot reload" (press "r" in the console where you ran "flutter run",
        // or simply save your changes to "hot reload" in a Flutter IDE).
        // Notice that the counter didn't reset back to zero; the application
        // is not restarted.
        primarySwatch: Colors.blue,
      ),
      home: MyHomePage(title: 'Flutter Demo Home Page'),
    );
  }
}

class MyHomePage extends StatefulWidget {
  MyHomePage({Key? key, required this.title}) : super(key: key);

  // This widget is the home page of your application. It is stateful, meaning
  // that it has a State object (defined below) that contains fields that affect
  // how it looks.

  // This class is the configuration for the state. It holds the values (in this
  // case the title) provided by the parent (in this case the App widget) and
  // used by the build method of the State. Fields in a Widget subclass are
  // always marked "final".

  final String title;

  @override
  _MyHomePageState createState() => _MyHomePageState();
}

class _MyHomePageState extends State<MyHomePage> {
  int _counter = 0;

  @override
  void initState() {
    super.initState();
  }

  void _incrementCounter() {
    lwm.PlayerSetRateEventHandler(0, Pointer.fromFunction(doubleC));
    lwm.PlayerSetVolumeEventHandler(0, Pointer.fromFunction(doubleC));
    lwm.PlayerSetPositionEventHandler(0, Pointer.fromFunction(intC));
    lwm.PlayerSetDurationEventHandler(0, Pointer.fromFunction(intC));
    lwm.PlayerSetIndexEventHandler(0, Pointer.fromFunction(intC));
    lwm.PlayerSetIsPlayingEventHandler(0, Pointer.fromFunction(intC));
    lwm.PlayerSetIsBufferingEventHandler(0, Pointer.fromFunction(intC));
    lwm.PlayerSetIsCompletedEventHandler(0, Pointer.fromFunction(intC));
    lwm.PlayerSetVolume(0, 0.5);
    lwm.PlayerSetRate(0, 1.2);
    lwm.PlayerOpen(
      0,
      2,
      [
        'https://p.scdn.co/mp3-preview/18f50618e8737c8a1f3b50a653023c5576af8955?cid=774b29d4f13844c495f206cafdad9c86',
        'https://p.scdn.co/mp3-preview/669eef4c25c47eb54c8c0bceee55b94519f3b0c1?cid=774b29d4f13844c495f206cafdad9c86',
      ].toNativeUtf8Array(),
      [
        0,
        1,
      ].toNativeArray(),
    );
    lwm.PlayerPlay(0);
    setState(() {
      // This call to setState tells the Flutter framework that something has
      // changed in this State, which causes it to rerun the build method below
      // so that the display can reflect the updated values. If we changed
      // _counter without calling setState(), then the build method would not be
      // called again, and so nothing would appear to happen.
      _counter++;
    });
  }

  @override
  Widget build(BuildContext context) {
    // This method is rerun every time setState is called, for instance as done
    // by the _incrementCounter method above.
    //
    // The Flutter framework has been optimized to make rerunning build methods
    // fast, so that you can just rebuild anything that needs updating rather
    // than having to individually change instances of widgets.
    return Scaffold(
      appBar: AppBar(
        // Here we take the value from the MyHomePage object that was created by
        // the App.build method, and use it to set our appbar title.
        title: Text(widget.title),
      ),
      body: Center(
        // Center is a layout widget. It takes a single child and positions it
        // in the middle of the parent.
        child: Column(
          // Column is also a layout widget. It takes a list of children and
          // arranges them vertically. By default, it sizes itself to fit its
          // children horizontally, and tries to be as tall as its parent.
          //
          // Invoke "debug painting" (press "p" in the console, choose the
          // "Toggle Debug Paint" action from the Flutter Inspector in Android
          // Studio, or the "Toggle Debug Paint" command in Visual Studio Code)
          // to see the wireframe for each widget.
          //
          // Column has various properties to control how it sizes itself and
          // how it positions its children. Here we use mainAxisAlignment to
          // center the children vertically; the main axis here is the vertical
          // axis because Columns are vertical (the cross axis would be
          // horizontal).
          mainAxisAlignment: MainAxisAlignment.center,
          children: <Widget>[
            Text(
              'You have pushed the button this many times:',
            ),
            Text(
              '$_counter',
              style: Theme.of(context).textTheme.headline4,
            ),
          ],
        ),
      ),
      floatingActionButton: FloatingActionButton(
        onPressed: _incrementCounter,
        tooltip: 'Increment',
        child: Icon(Icons.add),
      ), // This trailing comma makes auto-formatting nicer for build methods.
    );
  }
}
