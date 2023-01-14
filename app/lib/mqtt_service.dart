import 'package:mqtt_client/mqtt_client.dart';
import 'package:mqtt_client/mqtt_server_client.dart';
import 'dart:io';

class MqttService {
  final String host;
  final String user;
  final String password;
  final int port;
  final String topic;
  final MqttServerClient client;

  MqttService._(
      {required this.host,
      required this.user,
      required this.password,
      required this.port,
      required this.topic,
      required this.client});

  factory MqttService.initWith(
      String host, String user, String password, int port, String topic) {
    MqttServerClient client =
        MqttServerClient.withPort(host, 'flutter_app', port);
    client.secure = true;
    client.securityContext = SecurityContext.defaultContext;
    client.onDisconnected = _onDisconnected;
    client.onConnected = _onConnected;

    client.connect(user, password);

    return MqttService._(
        host: host,
        user: user,
        password: password,
        port: port,
        topic: topic,
        client: client);
  }

  void send(int code) {
    final builder = MqttClientPayloadBuilder();
    builder.addString(code.toRadixString(16));
    client.publishMessage(topic, MqttQos.exactlyOnce, builder.payload!);
  }

  static void _onDisconnected() {
    print('OnDisconnected client callback - Client disconnection');
  }

  static void _onConnected() {
    print('OnConnected client callback - Client connection was sucessful');
  }
}
