import 'package:app/mqtt_service.dart';
import 'package:flutter/material.dart';

class SendButton extends StatelessWidget {
  final int sendCode;
  final String buttonFunction;
  final MqttService mqttclient;

  const SendButton(
      {super.key, required this.sendCode, required this.buttonFunction, required this.mqttclient});

  void onClick() {
    mqttclient.send(sendCode);
  }

  @override
  Widget build(BuildContext context) {
    Size size = MediaQuery.of(context).size;

    return Container(
        width: size.width * 0.5,
        height: size.width * 0.10,
        padding: EdgeInsets.all(size.width * 0.05),
        child: OutlinedButton(onPressed: onClick, child: Text(buttonFunction)));
  }
}
