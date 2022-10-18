import 'package:flutter/material.dart';
//import 'home_page.dart';
import 'pickImage.dart';

class WelcomeScreen extends StatelessWidget {
  @override
  Widget build(BuildContext context) {
    return Scaffold(
      backgroundColor: Colors.redAccent,
      body: Center(
        child: SafeArea(
          child: Column(
            children: <Widget>[
              SizedBox(
                height: 150.0,
              ),
              Text(
                'ESPOV  RED',
                style: TextStyle(
                  fontFamily: 'Pacifico',
                  fontSize: 40.0,
                  color: Colors.white70,
                  fontWeight: FontWeight.bold,
                ),
              ),
              Text(
                ' IOT PROJECT APP',
                style: TextStyle(
                  color: Colors.white70,
                  fontSize: 20,
                  letterSpacing: 2.5,
                  fontWeight: FontWeight.bold,
                ),
              ),
              SizedBox(
                height: 40.0,
                width: 170.0,
                child: Divider(
                  color: Colors.teal.shade100,
                ),
              ),
              SizedBox(
                height: 150.0,
              ),
              ElevatedButton(
                style: ElevatedButton.styleFrom(
                  primary: Colors.white70,
                ),
                onPressed: () {
                  // Navigator.push(
                  //   context,
                  //   MaterialPageRoute(builder: (context) => HomePage()),
                  // );
                  Navigator.push(
                    context,
                    MaterialPageRoute(builder: (context) => Image_Picker()),
                  );
                },
                child: Padding(
                  padding: const EdgeInsets.all(8.0),
                  child: Text(
                    ' START ',
                    style: TextStyle(fontSize: 24),
                  ),
                ),
              ),
            ],
          ),
        ),
      ),
    );
  }
}

/*
body: Center(
child: ElevatedButton(
style: ElevatedButton.styleFrom(
primary: Colors.red,
),
child: Text('START'),
onPressed: () {
Navigator.push(
context,
MaterialPageRoute(builder: (context) => HomePage()),
);
},
),
),

 */
