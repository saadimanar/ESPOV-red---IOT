import 'package:flutter/material.dart';
import 'package:image_picker/image_picker.dart';

void main() => runApp(MyApp());
class MyApp extends StatefulWidget {
  @override
  State<StatefulWidget> createState() {
    return _MyAppState();
  }
}

class _MyAppState extends State<MyApp> {
  XFile? _image;

  Future getImage(bool isCamera) async {
    XFile? image;
    if (isCamera) {
      image = await ImagePicker().pickImage(source: ImageSource.camera);
    } else {
      image = await ImagePicker().pickImage(source: ImageSource.gallery);
    }
    setState(() {
      _image = image;
    });
  }

  Widget showImage(){
    if(_image != null){
      return Image.file(_image, height: 300.0, width: 300.0,);
    }
    return Container(color: Colors.red, height: 100, width: 100);
  }

  @override
  Widget build(BuildContext context) {
    return MaterialApp(
      home: Scaffold(
          appBar: AppBar(
            title: Text('image pick'),
          ),
          body: Center(
            child: Column(
            children: <Widget>[
            IconButton(
            icon: Icon(Icons.insert_drive_file),
            onPressed: () {
              getImage(false);
            },

          ),
          SizedBox(height: 10.0,),
          IconButton(
            icon: Icon(Icons.camera_alt),
            onPressed: () {
              getImage(true);
            },

          ),

          showImage(),

          ]
      ),
    ),)
    ,
    );
  }
}
  // @override
  // void debugFillProperties(DiagnosticPropertiesBuilder properties) {
  //   super.debugFillProperties(properties);
  //   properties.add(DiagnosticsProperty<File>('_image', _image));
  // }