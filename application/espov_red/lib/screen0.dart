import 'package:flutter/material.dart';

// change it to be home welcome screen

class Screen0 extends StatelessWidget {
  @override
  Widget build(BuildContext context) {
    return Scaffold(
      appBar: AppBar(
        backgroundColor: Colors.purple,
        title: Text('Screen 0'),
      ),
      body: Center(
        child: Column(
          children: <Widget>[
            ElevatedButton(
              style: ElevatedButton.styleFrom(
                primary: Colors.red,
              ),
              //color: Colors.red,
              child: Text('Go To Screen 1'),
              onPressed: () {
                //Navigate to Screen 1
              },
            ),
            // ElevatedButton(
            //   style: ElevatedButton.styleFrom(
            //     primary: Colors.blue,
            //   ),
            //   // color: Colors.blue,
            //   child: Text('Go To Screen 2'),
            //   onPressed: () {
            //     //Navigate to Screen 2
            //   },
            // ),
          ],
        ),
      ),
    );
  }
}
