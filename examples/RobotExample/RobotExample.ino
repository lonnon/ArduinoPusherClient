#include <Servo.h>
#include <WiFlyHQ.h>
#include <SoftwareSerial.h>
#include <PusherClient.h>

const char mySSID[] = "my$ssid"; //use $ to represent spaces in your ssid or passphrase
const char myPassword[] = "my$pass$phrase";
const char pusherKey[] = "abc123";

PusherClient client;

SoftwareSerial wifiSerial(2,3); //for debugging

WiFly wifly;

Servo leftServo; 
Servo rightServo; 

void setup() {
  pinMode(2,OUTPUT);
  leftServo.attach(2);
  
  pinMode(3, OUTPUT);
  rightServo.attach(3);

  leftServo.write(95);
  rightServo.write(95);

  wifiSerial.begin(57600);
  Serial.begin(9600);
  if (!wifly.begin(&Serial, &wifiSerial)) {
    wifiSerial.println(F("Failed to start wifly"));
    wifly.terminal();
  }

  if (!wifly.isAssociated()) {
    wifiSerial.println(F("Joining network"));
    if (wifly.join(mySSID, myPassword, true)) {
      wifly.save();
      wifiSerial.println(F("Joined wifi network"));
    } 
    else {
      wifiSerial.println(F("Failed to join wifi network"));
      wifly.terminal();
    }
  } 
  else {
    wifiSerial.println(F("Already joined network"));
  }
  client.setClient(wifly);
  
  if(client.connect("your-api-key-here")) {
    client.bind("forward", moveForward);
    client.bind("backward", moveBackward);
    client.bind("turn_left", turnLeft);
    client.bind("turn_right", turnRight);
    client.bind("stop", stopMoving);
    client.subscribe("robot_channel");
  }
  else {
    while(1) {}
  }
}

void loop() {
  if (client.connected()) {
    client.monitor();
  }
  else {
    leftServo.write(95);
    rightServo.write(95);
  }
}

void moveForward(String data) {
  leftServo.write(0);
  rightServo.write(180);
}

void moveBackward(String data) {
  leftServo.write(180);
  rightServo.write(0);
}

void turnLeft(String data) {
  leftServo.write(0);
  rightServo.write(0);
}

void turnRight(String data) {
  leftServo.write(180);
  rightServo.write(180);
}

void stopMoving(String data) {
  leftServo.write(95);
  rightServo.write(95);
}
