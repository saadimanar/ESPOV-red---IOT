/*
import 'package:flutter/material.dart';
import 'package:flutter_blue/flutter_blue.dart';

class Screen2 extends StatelessWidget {
  @override
  Widget build(BuildContext context) {
    return Scaffold(
      appBar: AppBar(
        backgroundColor: Colors.blue,
        title: Text('Screen 2'),
      ),
      body: Center(
        child: ElevatedButton(
          style: ElevatedButton.styleFrom(
            primary: Colors.blue,
          ),
          //color: Colors.blue,
          child: Text('Go Back To Screen 1'),
          onPressed: () {
            Navigator.pop(context);
          },
        ),
      ),
    );
  }
}

*/

/////////////////////////////////////////
import 'dart:io';
import 'dart:math';

import 'package:flutter/material.dart';
import 'package:flutter/services.dart';
import 'package:image_picker/image_picker.dart';
import 'package:flutter_native_image/flutter_native_image.dart';
import 'package:image/image.dart' as Imagi;

var imagesList = List<List<int>>.empty(growable: true);

class Image_Picker extends StatelessWidget {
  const Image_Picker({Key? key}) : super(key: key);

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
        primarySwatch: Colors.red,
      ),
      home: const MyHomePage(title: 'Flutter Demo Home Page'),
    );
  }
}

class MyHomePage extends StatefulWidget {
  const MyHomePage({Key? key, required this.title}) : super(key: key);

  // This widget is the home page of your application. It is stateful, meaning
  // that it has a State object (defined below) that contains fields that affect
  // how it looks.

  // This class is the configuration for the state. It holds the values (in this
  // case the title) provided by the parent (in this case the App widget) and
  // used by the build method of the State. Fields in a Widget subclass are
  // always marked "final".

  final String title;

  @override
  State<MyHomePage> createState() => _MyHomePageState();
}

class _MyHomePageState extends State<MyHomePage> {
  final ImagePicker imagePicker = ImagePicker();

  // List<XFile>? imageFileList = [];

  //////////////////////image decode ///////////////////////
/*
import 'package:image/image.dart' as img;
final xFile = await _controller.takePicture();
final path = xFile.path;
final bytes = await File(path).readAsBytes();
final img.Image image = img.decodeImage(bytes);
*/
  File selectedimage = File('');
  void selectImage() async {
    final selectedimage =
        await ImagePicker().pickImage(source: ImageSource.gallery);

    if (selectedimage != null) {
      final image = File(selectedimage.path);
      setState(() => this.selectedimage = image);
      File compressedImage = await FlutterNativeImage.compressImage(image.path,
          quality: 100, targetWidth: 55, targetHeight: 55);
      final compressedImageFinal = compressedImage;
      final binaryImage = await compressedImageFinal.readAsBytes();
      final image_decoded = Imagi.JpegDecoder().decodeImage(binaryImage);
      final image_to_bytes = image_decoded!.getBytes(format: Imagi.Format.rgb);
      imagesList.add(image_to_bytes); // check !
      setState(() => this.selectedimage = compressedImageFinal);
    }
  }

  //to do
  void sendImage() async {



  }

  @override
  Widget build(BuildContext context) {
    return Scaffold(
        appBar: AppBar(
          title: const Text("Image Picker"),
        ),
        body: Center(
          child: Column(
            children: [
              /*
              ElevatedButton(
                style: ElevatedButton.styleFrom(
                  primary: Colors.blue,
                ),
                //color: Colors.blue,
                child: Text('Go Back To Screen 1'),
                onPressed: () {
                  Navigator.pop(context);
                },
              ),*/
              SizedBox(
                height: 40,
              ),
              MaterialButton(
                  color: Colors.red,
                  child: const Text("Pick Images from Gallery",
                      style: TextStyle(
                          color: Colors.white70, fontWeight: FontWeight.bold)),
                  onPressed: () {
                    selectImage();
                  }),
              SizedBox(
                height: 20,
              ),
              MaterialButton(
                  color: Colors.red,
                  child: const Text(" Send Image To esp32",
                      style: TextStyle(
                          color: Colors.white70, fontWeight: FontWeight.bold)),
                  onPressed: () {
                    sendImage();
                  }),
              // Expanded(
              //     child: Padding(
              //   padding: const EdgeInsets.all(8.0),
              //   child: GridView.builder(
              //       itemCount: imageFileList!.length,
              //       gridDelegate: SliverGridDelegateWithFixedCrossAxisCount(
              //           crossAxisCount: 3),
              //       itemBuilder: (BuildContext context, int index) {
              //         return Image.file(File(imageFileList![index].path),
              //             fit: BoxFit.cover);
              //       }),
              // )),
            ],
          ),
        ));
  }
}



