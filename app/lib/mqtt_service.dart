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

  void prepareMqttClient() async {
    await _connectClient();
  }

  Future<void> _connectClient() async {
    try {
      print('client connecting....');
      await client.connect(user, password);
    } on Exception catch (e) {
      print('client exception - $e');
      client.disconnect();
    }

    // when connected, print a confirmation, else print an error
    if (client.connectionStatus?.state == MqttConnectionState.connected) {
      print('client connected');
    } else {
      print(
          'ERROR client connection failed - disconnecting, status is ${client.connectionStatus}');
      client.disconnect();
    }
  }

  Future<void> send(int code) async {
    final builder = MqttClientPayloadBuilder();
    builder.addString(code.toRadixString(16));

    while (client.connectionStatus?.state != MqttConnectionState.connected) {
      await _connectClient();
    }

    client.publishMessage(topic, MqttQos.atMostOnce, builder.payload!);
  }

  static void _onDisconnected() {
    print('OnDisconnected client callback - Client disconnected');
  }

  static void _onConnected() {
    print('OnConnected client callback - Client connection was sucessful');
  }
}
