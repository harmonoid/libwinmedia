import 'package:flutter/material.dart';
import 'package:libwinmedia/libwinmedia.dart';

void main() {
  LWM.initialize();
  runApp(MyApp());
}

class MyApp extends StatefulWidget {
  @override
  _MyAppState createState() => _MyAppState();
}

class _MyAppState extends State<MyApp> {
  final player = Player(id: 0);
  int index = 0;
  List<Media> medias = [];
  double volume = 1.0;
  double rate = 1.0;
  bool isPlaying = false;
  bool isBuffering = false;
  bool isCompleted = false;
  Duration duration = Duration.zero;
  Duration position = Duration.zero;
  String uri = '';
  double downloadProgress = 0.0;
  @override
  void initState() {
    super.initState();
    player.streams.index.listen((event) {
      index = event;
      setState(() {});
    });
    player.streams.medias.listen((event) {
      medias = event;
      setState(() {});
    });
    player.streams.isPlaying.listen((event) {
      isPlaying = event;
      setState(() {});
    });
    player.streams.isBuffering.listen((event) {
      isBuffering = event;
      setState(() {});
    });
    player.streams.isCompleted.listen((event) {
      isCompleted = event;
      setState(() {});
    });
    player.streams.duration.listen((event) {
      duration = event;
      setState(() {});
    });
    player.streams.position.listen((event) {
      position = event;
      setState(() {});
    });
    player.streams.downloadProgress.listen((event) {
      downloadProgress = event;
      setState(() {});
    });
    player.streams.error.listen((event) {
      print(event);
    });
  }

  void open() {
    player.open(
      [
        Media(uri: uri),
      ],
    );
  }

  @override
  Widget build(BuildContext context) {
    return MaterialApp(
      home: Scaffold(
        appBar: AppBar(
          title: const Text('libwinmedia'),
        ),
        body: ListView(
          padding: EdgeInsets.symmetric(
            vertical: 4.0,
          ),
          children: [
            Padding(
              padding: EdgeInsets.symmetric(
                horizontal: 8.0,
                vertical: 4.0,
              ),
              child: Card(
                elevation: 2.0,
                child: Column(
                  crossAxisAlignment: CrossAxisAlignment.start,
                  children: [
                    Padding(
                      padding:
                          EdgeInsets.only(top: 16.0, left: 16.0, bottom: 8.0),
                      child: Text('Open URI into Player'),
                    ),
                    Padding(
                      padding: EdgeInsets.only(
                          top: 8.0, left: 16.0, right: 16.0, bottom: 16.0),
                      child: Row(
                        children: [
                          Expanded(
                            child: TextField(
                              onChanged: (value) => uri = value,
                              cursorWidth: 1.0,
                              style: TextStyle(
                                fontSize: 14.0,
                              ),
                              onSubmitted: (_) => open(),
                              decoration: InputDecoration(
                                hintText:
                                    'Enter a URI like file:///home/alexmercerind/music.mp3 or https://alexmercerind.github.io/music.ogg',
                                hintStyle: TextStyle(
                                  fontSize: 14.0,
                                ),
                                border: UnderlineInputBorder(
                                  borderSide: BorderSide(width: 2.0),
                                ),
                                focusedBorder: UnderlineInputBorder(
                                  borderSide: BorderSide(
                                    width: 2.0,
                                    color: Theme.of(context).primaryColor,
                                  ),
                                ),
                              ),
                            ),
                          ),
                          SizedBox(
                            width: 12.0,
                          ),
                          ElevatedButton(
                            onPressed: open,
                            child: Text('Open'),
                          ),
                        ],
                      ),
                    ),
                  ],
                ),
              ),
            ),
            Padding(
              padding: EdgeInsets.symmetric(
                horizontal: 8.0,
                vertical: 4.0,
              ),
              child: Card(
                elevation: 2.0,
                child: Column(
                  crossAxisAlignment: CrossAxisAlignment.start,
                  children: [
                    Padding(
                      padding:
                          EdgeInsets.only(top: 16.0, left: 16.0, bottom: 8.0),
                      child: Text('Player controls'),
                    ),
                    Padding(
                      padding: EdgeInsets.only(
                          top: 4.0, left: 16.0, right: 16.0, bottom: 8.0),
                      child: Row(
                        children: [
                          ElevatedButton(
                            onPressed: player.play,
                            child: Text('Play'),
                          ),
                          SizedBox(
                            width: 12.0,
                          ),
                          ElevatedButton(
                            onPressed: player.pause,
                            child: Text('Pause'),
                          ),
                        ],
                      ),
                    ),
                    Padding(
                      padding: EdgeInsets.only(
                          top: 4.0, left: 16.0, right: 16.0, bottom: 4.0),
                      child: Row(
                        children: [
                          Text('Player volume'),
                          Slider(
                            value: volume,
                            min: 0.0,
                            max: 1.0,
                            onChanged: (value) {
                              player.volume = value;
                              volume = value;
                              setState(() {});
                            },
                          ),
                          Text('Player rate'),
                          Slider(
                            value: rate,
                            min: 0.0,
                            max: 2.0,
                            onChanged: (value) {
                              player.rate = value;
                              rate = value;
                              setState(() {});
                            },
                          ),
                        ],
                      ),
                    ),
                    Padding(
                      padding: EdgeInsets.only(
                          top: 4.0, left: 16.0, right: 16.0, bottom: 4.0),
                      child: Text('Player position'),
                    ),
                    Padding(
                      padding: EdgeInsets.only(
                          top: 4.0, left: 16.0, right: 16.0, bottom: 16.0),
                      child: Row(
                        children: [
                          Text(position.toString()),
                          Expanded(
                            child: Slider(
                              value: position.inMilliseconds.toDouble(),
                              min: 0.0,
                              max: duration.inMilliseconds.toDouble(),
                              onChanged: (value) {
                                player.seek(
                                  Duration(
                                    milliseconds: value ~/ 1,
                                  ),
                                );
                                setState(() {});
                              },
                            ),
                          ),
                          Text(duration.toString()),
                        ],
                      ),
                    ),
                    Padding(
                      padding: EdgeInsets.all(8.0),
                      child: Text(
                        'Player has playlists, which are not shown in the example.',
                        style: TextStyle(color: Colors.black.withOpacity(0.67)),
                      ),
                    ),
                  ],
                ),
              ),
            ),
            Padding(
              padding: EdgeInsets.symmetric(
                horizontal: 8.0,
                vertical: 4.0,
              ),
              child: Card(
                elevation: 2.0,
                child: Column(
                  crossAxisAlignment: CrossAxisAlignment.start,
                  children: [
                    Padding(
                      padding:
                          EdgeInsets.only(top: 16.0, left: 16.0, bottom: 0.0),
                      child: Text('Player state'),
                    ),
                    Padding(
                      padding: EdgeInsets.only(
                          top: 4.0, left: 16.0, right: 16.0, bottom: 16.0),
                      child: DataTable(
                        columns: [
                          DataColumn(label: Text('attribute')),
                          DataColumn(label: Text('value')),
                        ],
                        rows: [
                          DataRow(cells: [
                            DataCell(Text('player.state.index')),
                            DataCell(Text('$index')),
                          ]),
                          DataRow(cells: [
                            DataCell(Text('player.state.medias')),
                            DataCell(Text(
                              '$medias',
                              maxLines: 1,
                              overflow: TextOverflow.ellipsis,
                            )),
                          ]),
                          DataRow(cells: [
                            DataCell(Text('player.state.isPlaying')),
                            DataCell(Text('$isPlaying')),
                          ]),
                          DataRow(cells: [
                            DataCell(Text('player.state.isBuffering')),
                            DataCell(Text('$isBuffering')),
                          ]),
                          DataRow(cells: [
                            DataCell(Text('player.state.isCompleted')),
                            DataCell(Text('$isCompleted')),
                          ]),
                          DataRow(cells: [
                            DataCell(Text('player.state.downloadProgress')),
                            DataCell(Text('$downloadProgress')),
                          ]),
                          DataRow(cells: [
                            DataCell(Text('player.volume')),
                            DataCell(Text('$volume')),
                          ]),
                          DataRow(cells: [
                            DataCell(Text('player.rate')),
                            DataCell(Text('$rate')),
                          ]),
                        ],
                      ),
                    ),
                    Padding(
                      padding: EdgeInsets.all(8.0),
                      child: Text(
                        'Other event streams have not been shown in the example.',
                        style: TextStyle(color: Colors.black.withOpacity(0.67)),
                      ),
                    ),
                  ],
                ),
              ),
            ),
            Padding(
              padding: EdgeInsets.all(16.0),
              child: Text(
                'MIT\nhttps://github.com/harmonoid/libwinmedia\nCopyright © 2020 Hitesh Kumar Saini <saini123hitesh@gmail.com>',
                style: TextStyle(color: Colors.black.withOpacity(0.67)),
              ),
            ),
          ],
        ),
      ),
    );
  }
}
