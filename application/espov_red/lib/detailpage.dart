import 'dart:typed_data';
import 'package:async/async.dart';
import 'package:dropdown_formfield/dropdown_formfield.dart';
import 'package:flutter/material.dart';
import 'package:flutter_bluetooth_serial/flutter_bluetooth_serial.dart';
import 'package:flutter_svprogresshud/flutter_svprogresshud.dart';
import 'package:photo_view/photo_view.dart';
import 'pickImage.dart';
import 'dart:math';
import 'home_page.dart';
import 'globals.dart' as globals;

class DetailPage extends StatefulWidget {
  final BluetoothDevice? server;

  const DetailPage({this.server});

  @override
  _DetailPageState createState() => _DetailPageState();
}

class _DetailPageState extends State<DetailPage> {
  bool isSending = true; // not original
  BluetoothConnection? connection;
  bool isConnecting = true;

  bool get isConnected => connection != null && connection!.isConnected;
  bool isDisconnecting = false;

  String? _selectedFrameSize;

  List<List<int>> chunks = <List<int>>[];
  int contentLength = 0;
  Uint8List? _bytes;

  RestartableTimer? _timer;

  @override
  void initState() {
    super.initState();
    _selectedFrameSize = '0';
    _getBTConnection();
    _timer = new RestartableTimer(Duration(seconds: 1), _drawImage);
  }

  @override
  void dispose() {
    if (isConnected) {
      isDisconnecting = true;
      connection?.dispose();
      connection = null;
    }
    _timer?.cancel();
    super.dispose();
  }

  _getBTConnection() {
    BluetoothConnection.toAddress(widget.server?.address).then((_connection) {
      connection = _connection;
      isConnecting = false;
      isDisconnecting = false;
      setState(() {});
      connection?.input?.listen(_onDataReceived).onDone(() {
        if (isDisconnecting) {
          print('Disconnecting locally');
        } else {
          print('Disconnecting remotely');
        }
        if (this.mounted) {
          setState(() {});
        }
        Navigator.of(context).pop();
      });
    }).catchError((error) {
      Navigator.of(context).pop();
    });
  }

  _drawImage() {
    if (chunks.length == 0 || contentLength == 0) return;

    _bytes = Uint8List(contentLength);
    int offset = 0;
    for (final List<int> chunk in chunks) {
      _bytes?.setRange(offset, offset + chunk.length, chunk);
      offset += chunk.length;
    }

    setState(() {});

    SVProgressHUD.showInfo(status: 'Downloaded...');
    //SVProgressHUD.dismissWithDelay(1000);
    SVProgressHUD.dismiss(delay: Duration(milliseconds: 1000));

    contentLength = 0;
    chunks.clear();
  }

  void _onDataReceived(Uint8List data) {
    if (data != null && data.length > 0) {
      chunks.add(data);
      contentLength += data.length;
      _timer?.reset();
    }

    print("Data Length: ${data.length}, chunks: ${chunks.length}");
  }

  Future<void> _sendMessage(List<int> list) async {
    for (int i = 0; i < list.length; i += 1) {
      print(list[i]);
    }
    const int BytesEachTime = 100;
    if (list.length > 0) {
      try {
        setState(() {
          isSending = true;
        });

        for (int i = 0; i < list.length; i += BytesEachTime) {
          connection!.output.add(Uint8List.fromList(
              list.sublist(i, min(i + BytesEachTime, list.length))));
          await connection!.output.allSent;
          await Future.delayed(const Duration(milliseconds: 10));
        }

        setState(() {
          isSending = false;
        });
      } catch (e) {
        setState(() {});
      }
    }
  }

  @override
  Widget build(BuildContext context) {
    return Scaffold(
        backgroundColor: Colors.white,
        appBar: AppBar(
          backgroundColor: Colors.redAccent,
          title: (isConnecting
              ? Text('Connecting to ${widget.server?.name} ...')
              : isConnected
                  ? Text('Connected with ${widget.server?.name}')
                  : Text('Disconnected with ${widget.server?.name}')),
        ),
        body: SafeArea(
          child: isConnected
              ? Column(
                  children: <Widget>[
                    // selectFrameSize(),
                    SizedBox(
                      height: 100,
                    ),
                    // shotButton(),
                    SizedBox(
                      height: 40,
                    ),
                    sendImageButton(),
                    SizedBox(
                      height: 40,
                    ),
                    sendAnotherImageButton(),
                    // shotButton(),
                    photoFrame(),
                  ],
                )
              : Center(
                  child: Text(
                    "Connecting...",
                    style: TextStyle(
                        fontSize: 24,
                        fontWeight: FontWeight.bold,
                        color: Colors.black87),
                  ),
                ),
        ));
  }

  Widget photoFrame() {
    return Expanded(
      child: Container(
        width: double.infinity,
        child: _bytes != null
            ? PhotoView(
                enableRotation: true,
                initialScale: PhotoViewComputedScale.covered,
                maxScale: PhotoViewComputedScale.covered * 2.0,
                minScale: PhotoViewComputedScale.contained * 0.8,
                imageProvider:
                    Image.memory(_bytes!, fit: BoxFit.fitWidth).image,
              )
            : Container(),
      ),
    );
  }

  Widget shotButton() {
    return Container(
      padding: const EdgeInsets.all(16),
      child: ElevatedButton(
        style: ElevatedButton.styleFrom(
          primary: Colors.redAccent,
        ),
        // shape: RoundedRectangleBorder(
        //     borderRadius: BorderRadius.circular(18),
        //     side: BorderSide(color: Colors.red)),
        onPressed: () {
          Navigator.push(
            context,
            MaterialPageRoute(builder: (context) => Image_Picker()),
          );
          //_sendMessage(_selectedFrameSize);
        },
        // color: Colors.red,
        //textColor: Colors.white,
        child: Padding(
          padding: const EdgeInsets.all(8.0),
          child: Text(
            'PICK IMAGE',
            style: TextStyle(fontSize: 24),
          ),
        ),
      ),
    );
  }

  Widget sendImageButton() {
    return Container(
      padding: const EdgeInsets.all(16),
      child: ElevatedButton(
        style: ElevatedButton.styleFrom(
          primary: Colors.redAccent,
        ),
        // shape: RoundedRectangleBorder(
        //     borderRadius: BorderRadius.circular(18),
        //     side: BorderSide(color: Colors.red)),
        onPressed: () {
          _sendMessage(globals.imagesList[0]).then((value) async => {
                await Future.delayed(const Duration(seconds: 2)),
                _sendMessage(globals.imagesList[0])
              });
          // _sendMessage(globals.imagesList[0]);
          // Navigator.push(
          //   context,
          //   MaterialPageRoute(builder: (context) => Image_Picker()),
          // );
          //_sendMessage(_selectedFrameSize);
        },
        // color: Colors.red,
        //textColor: Colors.white,
        child: Padding(
          padding: const EdgeInsets.all(8.0),
          child: Text(
            'SEND IMAGE',
            style: TextStyle(fontSize: 24),
          ),
        ),
      ),
    );
  }

  Widget sendAnotherImageButton() {
    return Container(
      padding: const EdgeInsets.all(16),
      child: ElevatedButton(
        style: ElevatedButton.styleFrom(
          primary: Colors.redAccent,
        ),
        onPressed: () {
          Navigator.push(
            context,
            MaterialPageRoute(builder: (context) => HomePage()),
          );
        },
        child: Padding(
          padding: const EdgeInsets.all(8.0),
          child: Text(
            'SEND Another IMAGE',
            style: TextStyle(fontSize: 24),
          ),
        ),
      ),
    );
  }

  Widget selectFrameSize() {
    return Container(
      color: Colors.white,
      padding: const EdgeInsets.all(16),
      child: DropDownFormField(
        titleText: 'FRAMESIZE',
        hintText: 'Please choose one',
        value: _selectedFrameSize,
        onSaved: (value) {
          _selectedFrameSize = value;
          setState(() {});
        },
        onChanged: (value) {
          _selectedFrameSize = value;
          setState(() {});
        },
        dataSource: [
          {"value": "4", "display": "1600x1200"},
          {"value": "3", "display": "1280x1024"},
          {"value": "2", "display": "1024x768"},
          {"value": "1", "display": "800x600"},
          {"value": "0", "display": "640x480"},
        ],
        textField: 'display',
        valueField: 'value',
      ),
    );
  }
}
