#include "BluetoothSerial.h"
#include "ESP32_Servo.h"
#include <Arduino.h>
#include <M5Stack.h>

BluetoothSerial serialBT;

class M5_FS90 {
  Servo servo;
  int pin;
  int target = 0;

public:
  void init(int att_pin) {
    pin = att_pin;
    servo.attach(pin, 500, 2400);
  };
  void write() {
    target = constrain(target, 0, 180);
    servo.write(target);
  };
  void up(void) {
    target += 10;
    target = constrain(target, 0, 180);
  };
  void up(int val) {
    target += val;
    target = constrain(target, 0, 180);
  };
  void down(void) {
    target -= 10;
    target = constrain(target, 0, 180);
  };
  void down(int val) {
    target -= val;
    target = constrain(target, 0, 180);
  };

} yaw, pit;

uint8_t retByte = 0;

void setup() {
  serialBT.begin("ESP32test");
  Serial.begin(115200);
  Serial.println("Booting");
  yaw.init(22);
  pit.init(19);

  M5.begin();
  M5.Lcd.setTextSize(2);
  M5.Lcd.print("Start.");
}

void loop() {
  if (serialBT.available() > 0) { // 受信したデータが存在する
    retByte = serialBT.read();    // 受信データを読み込む
    if (bitRead(retByte, 0) || retByte == '0') {
      yaw.up();

      serialBT.print("yaw++");
      serialBT.print("\t");
    }
    if (bitRead(retByte, 1) || retByte == '1') {
      yaw.down();

      serialBT.print("yaw--");
      serialBT.print("\t");
    }
    if (bitRead(retByte, 2) || retByte == '2') {
      pit.up();
      serialBT.print("pit++");
      serialBT.print("\t");
    }
    if (bitRead(retByte, 3) || retByte == '3') {
      pit.down();

      serialBT.print("pit--");
      serialBT.print("\t");
    }
    serialBT.println(BIN, retByte);
  }
  yaw.write();
  pit.write();
}
