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
import 'package:androidesp32cambtapp/home_page.dart';
import 'package:flutter/material.dart';
import 'package:flutter/services.dart';
import 'package:image_picker/image_picker.dart';
import 'package:flutter_native_image/flutter_native_image.dart';
import 'package:image/image.dart' as Imagi;
import 'globals.dart' as globals;

class Image_Picker extends StatelessWidget {
  const Image_Picker({Key? key}) : super(key: key);

  // This widget is the root of your application.
  @override
  Widget build(BuildContext context) {
    return MaterialApp(
      title: 'Flutter Demo',
      theme: ThemeData(
        primarySwatch: Colors.red,
      ),
      home: const MyHomePage(title: 'Flutter Demo Home Page'),
    );
  }
}

class MyHomePage extends StatefulWidget {
  const MyHomePage({Key? key, required this.title}) : super(key: key);

  final String title;

  @override
  State<MyHomePage> createState() => _MyHomePageState();
}

//////////////////////image decode ///////////////////////
/*
import 'package:image/image.dart' as img;
final xFile = await _controller.takePicture();
final path = xFile.path;
final bytes = await File(path).readAsBytes();
final img.Image image = img.decodeImage(bytes);
*/

class _MyHomePageState extends State<MyHomePage> {
  File image = File('');

  Future pickImage() async {
    try {
      /*check*/
      globals.fileslist = List<File>.empty(growable: true);
      globals.imagesList = List<List<int>>.empty(growable: true);
      List<int> red_list = [];
      List<int> green_list = [];
      List<int> blue_list = [];
      ///////////////

      final image = await ImagePicker().pickImage(source: ImageSource.gallery);

      if (image == null) return;

      final imageTemp = File(image.path);

      setState(() => this.image = imageTemp);

      File compressedFile = await FlutterNativeImage.compressImage(image.path,
          quality: 100, targetWidth: 55, targetHeight: 55);
      final compressed_image_temp = compressedFile;
      final inputImg = await compressed_image_temp.readAsBytes();
      final decoder = Imagi.JpegDecoder();
      final decodedImg = decoder.decodeImage(inputImg);
      final decodedBytes = decodedImg!.getBytes(format: Imagi.Format.rgb);

      assert(decodedImg != null);
      for (int i = 0; i < decodedImg!.height; i++) {
        for (int j = 0; j < decodedImg!.width; j++) {
          int red = decodedBytes[i * decodedImg.width * 3 + j * 3];
          int green = decodedBytes[i * decodedImg.width * 3 + j * 3 + 1];
          int blue = decodedBytes[i * decodedImg.width * 3 + j * 3 + 2];
          red_list.add(red);
          green_list.add(green);
          blue_list.add(blue);
        }
      }

      List<int> rgb_list = []
        ..addAll(red_list)
        ..addAll(green_list)
        ..addAll(blue_list);

      globals.imagesList.add(rgb_list);

      setState(() => this.image = compressed_image_temp);
    } on PlatformException catch (e) {
      print('Failed to pick image: $e');
    }
    globals.fileslist.add(this.image);
  }

  void initState() {
    globals.fileslist = List<File>.empty(growable: true);
    globals.imagesList = List<List<int>>.empty(growable: true);
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
                height: 60,
              ),
              MaterialButton(
                  color: Colors.red,
                  child: const Text("Pick Images from Gallery",
                      style: TextStyle(
                          color: Colors.white70, fontWeight: FontWeight.bold)),
                  onPressed: () {
                    pickImage();
                  }),
              SizedBox(
                height: 40,
              ),
              MaterialButton(
                  color: Colors.red,
                  child: const Text("Done",
                      style: TextStyle(
                          color: Colors.white70, fontWeight: FontWeight.bold)),
                  // onPressed: () {
                  //   Navigator.pop(context);
                  //   Navigator.push(
                  //     context,
                  //     MaterialPageRoute(
                  //         builder: (context) =>
                  //             DetailPage(server: globals.global_server)),
                  //   );
                  // }),
                  onPressed: () {
                    //Navigator.pop(context);
                    Navigator.push(
                      context,
                      MaterialPageRoute(builder: (context) => HomePage()),
                    );
                  }),
              Expanded(
                  child: Padding(
                padding: const EdgeInsets.all(8.0),
                child: GridView.builder(
                    itemCount: globals.fileslist!.length,
                    gridDelegate: SliverGridDelegateWithFixedCrossAxisCount(
                        crossAxisCount: 3),
                    itemBuilder: (BuildContext context, int index) {
                      return Image.file(File(globals.fileslist![index].path),
                          fit: BoxFit.cover);
                    }),
              )),
            ],
          ),
        ));
  }
}
