#include "mqtt.h"
#include <iostream>
#include <string>
#include <string.h>
#include <sstream>

#define PUBLISH_TOPIC "EXAMPLE_TOPIC"

#ifdef DEBUG
#include <iostream>
#endif

using namespace std;

mqtt_client::mqtt_client(const char *id, const char *host, int port) : mosquittopp(id)
{
	int ret = connect(host, port, DEFAULT_KEEP_ALIVE);
	if (ret == 0) {
		cout << "Connected" << endl;
	}
	else {
		cout << "Connection fail: " << to_string(ret) << endl;
	}
}
mqtt_client::~mqtt_client()
{
}
void mqtt_client::on_connect(int rc)
{
	Connected = true;
	cout << "On_connect" << endl;
	if (!rc)
	{
#ifdef DEBUG
		std::cout << "Connected - code " << rc << std::endl;
#endif
	}
}
void mqtt_client::on_subscribe(int mid, int qos_count, const int *granted_qos)
{

	cout << "on_subscribe" << endl;

#ifdef DEBUG
	std::cout << "Subscription succeeded." << std::endl;
#endif
}
void mqtt_client::on_disconnect(int rc)
{
	Connected = false;
	cout << "disconnected" << endl;
	this->reconnect();
	
}
int mqtt_client::szam(string szo)
{
	stringstream s;
	int x;

	s << szo;
	s >> x;

	return x;
};
int mqtt_client::cmd(string mes)
{
	static const size_t npos = -1;

	if (mes.find("x")!= npos) {
		setX(szam(mes.substr(mes.find("=")+1)));
		cout << "cmd(), X= " << getX() << endl;
		return 0;
	}
	else if (mes.find("y") != npos) {
		setY(szam(mes.substr(mes.find("=") + 1)));
		cout << "cmd(), Y= " << getY() << endl;
		return 0;
	}else if (mes.find("z") != npos) {
		setZ(szam(mes.substr(mes.find("=") + 1)));
		cout << "cmd(), Z= " << getZ() << endl;
		return 0;
	}
	else if (mes.find("recon") != npos) {
		this->reconnect();
	}
	else if (mes.find("exit") != npos) {
		O = false;
	}
	return 1;
}
int mqtt_client::getX()
{
	return x;
}
int mqtt_client::getY()
{
	return y;
}
int mqtt_client::getZ()
{
	return z;
}
void mqtt_client::setX(int newx)
{
	x = newx;
}
void mqtt_client::setY(int newy)
{
	y = newy;
}
void mqtt_client::setZ(int newz)
{
	z = newz;
}
int mqtt_client::send(const struct mosquitto_message *msg, string newPl)
{
	int result;
	char *buf;
	buf = new char[newPl.length() + 1];
	strcpy(buf, newPl.c_str());
	result = this->publish(NULL, msg->topic, strlen(buf), buf, 2, false);
	cout << "send(), publish: " << result << endl;

	return result;
}
void mqtt_client::on_message(const struct mosquitto_message *message)
{	
	if (message->payloadlen >= 1) {
		string strPayload = (char*)message->payload;
		static const size_t npos = -1;

		cout << "Got Message" << endl;
		cout << "mid: " << message->mid << endl;
		cout << "payload: " << (char*)message->payload << endl;
		cout << "payloadlen: " << message->payloadlen << endl;
		cout << "qos: " << message->qos << endl;
		cout << "retain: " << message->retain << endl;
		cout << "topic: " << message->topic << endl;
		if (strPayload == "disc") {
			int ri = this->disconnect();
			if (ri != 0) cout << "disconnect: " << ri << endl;
			else
			{
				cout << "Disconnected!" << endl;
				this->reconnect();
			}
		}
		if (strPayload.find("cmd") != npos) {
			int ret = cmd(strPayload);
			if (ret == 0) send(message, "Data refreshed!\n");
		}
	}
	else{
		cout << "Got message, but it was empty." << endl;
	}

}
void mqtt_client::reconnect() {

	this->reinitialise(CLIENT_ID, true);
	
	int x = this->connect(BROKER_ADDRESS, MQTT_PORT, DEFAULT_KEEP_ALIVE);
	int y = this->subscribe(i, MQTT_TOPIC, 0);

	if (x == 0 && y == 0) cout << "reconnected" << endl;
	else {
		cout << "x: " << to_string(x) << ", y: " << to_string(y) << endl;
	}
}