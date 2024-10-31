#include <WiFi.h>
#include <WebServer.h>
#include <ArduinoJson.h>
#include <EEPROM.h>
#include <HTTPClient.h>
#include <PubSubClient.h>

const char* ssid = "POS_Server";
const char* password = "asdffdsa";

// const char* ssid = "Digital Engineering Tech";
// const char* password = "1313373290";

const char* mqtt_server = "192.168.0.100";
const char* mqtt_username = "detpos";
const char* mqtt_password = "asdffdsa";
const int mqtt_port = 1883;

WiFiClient espClient;
PubSubClient client(espClient);


unsigned char tank_1[8] = { 0x01, 0x69, 0x32, 0x30, 0x31, 0x30, 0x31, 0x03 };
unsigned char tank_2[8] = { 0x01, 0x69, 0x32, 0x30, 0x31, 0x30, 0x32, 0x03 };
unsigned char tank_3[8] = { 0x01, 0x69, 0x32, 0x30, 0x31, 0x30, 0x33, 0x03 };
unsigned char tank_4[8] = { 0x01, 0x69, 0x32, 0x30, 0x31, 0x30, 0x34, 0x03 };
unsigned char tank_5[8] = { 0x01, 0x69, 0x32, 0x30, 0x31, 0x30, 0x35, 0x03 };
unsigned char tank_6[8] = { 0x01, 0x69, 0x32, 0x30, 0x31, 0x30, 0x36, 0x03 };
unsigned char tank_7[8] = { 0x01, 0x69, 0x32, 0x30, 0x31, 0x30, 0x37, 0x03 };
unsigned char tank_8[8] = { 0x01, 0x69, 0x32, 0x30, 0x31, 0x30, 0x38, 0x03 };
unsigned char tank_9[8] = { 0x01, 0x69, 0x32, 0x30, 0x31, 0x30, 0x39, 0x03 };
unsigned char tank_10[8] = { 0x01, 0x69, 0x32, 0x30, 0x31, 0x31, 0x30, 0x03 };
unsigned char tank_11[8] = { 0x01, 0x69, 0x32, 0x30, 0x31, 0x31, 0x31, 0x03 };
unsigned char tank_12[8] = { 0x01, 0x69, 0x32, 0x30, 0x31, 0x31, 0x32, 0x03 };

unsigned char buffer[100];

char tank_product_ary[8];
char tank_volume_ary[8];
char remain_volume_ary[8];
char oil_height_ary[8];
char temperature_ary[8];
char water_level_ary[8];
char water_volume_ary[8];

float tank_volume_1;
float tank_product_1;
float tank_remain_1;
float oil_height_1;
float atg_temp_1;
float water_height_1;
float water_volume_1;

float tank_volume_2;
float tank_product_2;
float tank_remain_2;
float oil_height_2;
float atg_temp_2;
float water_height_2;
float water_volume_2;

float tank_volume_3;
float tank_product_3;
float tank_remain_3;
float oil_height_3;
float atg_temp_3;
float water_height_3;
float water_volume_3;

float tank_volume_4;
float tank_product_4;
float tank_remain_4;
float oil_height_4;
float atg_temp_4;
float water_height_4;
float water_volume_4;

float tank_volume_5;
float tank_product_5;
float tank_remain_5;
float oil_height_5;
float atg_temp_5;
float water_height_5;
float water_volume_5;

float tank_volume_6;
float tank_product_6;
float tank_remain_6;
float oil_height_6;
float atg_temp_6;
float water_height_6;
float water_volume_6;

float tank_volume_7;
float tank_product_7;
float tank_remain_7;
float oil_height_7;
float atg_temp_7;
float water_height_7;
float water_volume_7;

float tank_volume_8;
float tank_product_8;
float tank_remain_8;
float oil_height_8;
float atg_temp_8;
float water_height_8;
float water_volume_8;

float tank_volume_9;
float tank_product_9;
float tank_remain_9;
float oil_height_9;
float atg_temp_9;
float water_height_9;
float water_volume_9;

float tank_volume_10;
float tank_product_10;
float tank_remain_10;
float oil_height_10;
float atg_temp_10;
float water_height_10;
float water_volume_10;

float tank_volume_11;
float tank_product_11;
float tank_remain_11;
float oil_height_11;
float atg_temp_11;
float water_height_11;
float water_volume_11;

float tank_volume_12;
float tank_product_12;
float tank_remain_12;
float oil_height_12;
float atg_temp_12;
float water_height_12;
float water_volume_12;


#define wifiled 33
#define powerled 32
#define TXled 27
#define RXled 26

#define RXD2 16
#define TXD2 17

int count;
int state_count = 1;
int wifi_try;

int device_id = 0;
int tank_count = 0;
int sensor_id = 0;

int fuel_id_1 = 0;
int fuel_id_2 = 0;
int fuel_id_3 = 0;
int fuel_id_4 = 0;
int fuel_id_5 = 0;
int fuel_id_6 = 0;
int fuel_id_7 = 0;
int fuel_id_8 = 0;
int fuel_id_9 = 0;
int fuel_id_10 = 0;
int fuel_id_11 = 0;
int fuel_id_12 = 0;

String Fuel_type;
String Fuel_type_1 = "";
String Fuel_type_2 = "";
String Fuel_type_3 = "";
String Fuel_type_4 = "";
String Fuel_type_5 = "";
String Fuel_type_6 = "";
String Fuel_type_7 = "";
String Fuel_type_8 = "";
String Fuel_type_9 = "";
String Fuel_type_10 = "";
String Fuel_type_11 = "";
String Fuel_type_12 = "";

char atg_topic[20] = "detpos/atg/initial";
String incommingMessage;

bool atg_check;

unsigned long start_time;

WebServer server(8080);

void setup() {
  Serial.begin(115200);
  Serial2.begin(9600, SERIAL_8N1, RXD2, TXD2);

  EEPROM.begin(512);

  pinMode(wifiled, OUTPUT);
  pinMode(powerled, OUTPUT);
  pinMode(TXled, OUTPUT);
  pinMode(RXled, OUTPUT);


  // EEPROMWrite();   // to change Tank information

  wifi_init();

  EEPROM_init();

  digitalWrite(powerled, HIGH);

  server.on("/baseOilcan", handleJsonData);  // server api
  server.onNotFound(handleNotFound);

  server.begin();

  client.setServer(mqtt_server, mqtt_port);
  client.setCallback(callback);

  Serial2.write(tank_1, 8);
}


void loop() {

  if (WiFi.status() == WL_CONNECTED) {

    if (!client.connected()) {
      Serial.println("mqtt is connecting...");
      reconnect();

    } else {

      digitalWrite(wifiled, HIGH);

      main_fun();
    }

  } else wifi_init();

  client.loop();
  server.handleClient();
}


void main_fun() {

  if (Serial2.available()) {
    start_time = millis() / 1000;

    buffer[count] = Serial2.read();

    digitalWrite(RXled, HIGH);
    
    delay(1);
    digitalWrite(RXled, LOW);
    delay(1);

    Serial.print(count);
    Serial.print("/");
    Serial.print(buffer[count], HEX);
    Serial.print(" ");
    count++;

    atg_check = true;

    if (buffer[count - 7] == 0x26 && buffer[count - 6] == 0x26 && buffer[count - 1] == 0x03) {

      Serial.println();

      if (buffer[17] == 0x30 && buffer[18] == 0x31) tank_1_fun();
      else if (buffer[17] == 0x30 && buffer[18] == 0x32) tank_2_fun();
      else if (buffer[17] == 0x30 && buffer[18] == 0x33) tank_3_fun();
      else if (buffer[17] == 0x30 && buffer[18] == 0x34) tank_4_fun();
      else if (buffer[17] == 0x30 && buffer[18] == 0x35) tank_5_fun();
      else if (buffer[17] == 0x30 && buffer[18] == 0x36) tank_6_fun();
      else if (buffer[17] == 0x30 && buffer[18] == 0x37) tank_7_fun();
      else if (buffer[17] == 0x30 && buffer[18] == 0x38) tank_8_fun();
      else if (buffer[17] == 0x30 && buffer[18] == 0x39) tank_9_fun();
      else if (buffer[17] == 0x31 && buffer[18] == 0x30) tank_10_fun();
      else if (buffer[17] == 0x31 && buffer[18] == 0x31) tank_11_fun();
      else if (buffer[17] == 0x31 && buffer[18] == 0x32) tank_12_fun();

      Serial.print("State count is => ");
      Serial.println(count);

      // ++state_count;

      if (count == 89) state_count++;

      state_fun();

      count = 0;
    }


  } else {

    if (millis() / 1000 - start_time > 1) {
      Serial.println("Waiting reply from ATG");
      Serial2.write(tank_1, 8);
      state_count = 1;
      count = 0;
      start_time = millis() / 1000;
      atg_check = false;
    }
  }
}



void EEPROMWrite() {
  EEPROM.write(100, 1);  // Device Id
  EEPROM.commit();
  EEPROM.write(101, 5);  // Tank Count
  EEPROM.commit();
  EEPROM.write(102, 1);  // Tank 1 fuel type =>(1=92,2=95,3=97,4=PD,5=D)
  EEPROM.commit();
  EEPROM.write(103, 2);  // Tank 2 fuel type =>(1=92,2=95,3=97,4=PD,5=D)
  EEPROM.commit();
  EEPROM.write(104, 3);  // Tank 3 fuel type =>(1=92,2=95,3=97,4=PD,5=D)
  EEPROM.commit();
  EEPROM.write(105, 4);  // Tank 4 fuel type =>(1=92,2=95,3=97,4=PD,5=D)
  EEPROM.commit();
  EEPROM.write(106, 5);  // Tank 5 fuel type =>(1=92,2=95,3=97,4=PD,5=D)
  EEPROM.commit();
  EEPROM.write(107, 0);  // Tank 6 fuel type =>(1=92,2=95,3=97,4=PD,5=D)
  EEPROM.commit();
  EEPROM.write(108, 0);  // Tank 7 fuel type =>(1=92,2=95,3=97,4=PD,5=D)
  EEPROM.commit();
  EEPROM.write(109, 0);  // Tank 8 fuel type =>(1=92,2=95,3=97,4=PD,5=D)
  EEPROM.commit();
  EEPROM.write(110, 0);  // Tank 9 fuel type =>(1=92,2=95,3=97,4=PD,5=D)
  EEPROM.commit();
  EEPROM.write(111, 0);  // Tank 10 fuel type =>(1=92,2=95,3=97,4=PD,5=D)
  EEPROM.commit();
  EEPROM.write(112, 0);  // Tank 11 fuel type =>(1=92,2=95,3=97,4=PD,5=D)
  EEPROM.commit();
  EEPROM.write(113, 0);  // Tank 12 fuel type =>(1=92,2=95,3=97,4=PD,5=D)
  EEPROM.commit();

  Serial.println("Yepp Save#############################################################");
}


void state_fun() {

  digitalWrite(TXled, HIGH);
  delay(2);
  digitalWrite(TXled, LOW);
  delay(2);

  if (state_count == 1) Serial2.write(tank_1, 8);
  else if (state_count == 2) Serial2.write(tank_2, 8);
  else if (state_count == 3) Serial2.write(tank_3, 8);
  else if (state_count == 4) Serial2.write(tank_4, 8);
  else if (state_count == 5) Serial2.write(tank_5, 8);
  else if (state_count == 6) Serial2.write(tank_6, 8);
  else if (state_count == 7) Serial2.write(tank_7, 8);
  else if (state_count == 8) Serial2.write(tank_8, 8);
  else if (state_count == 9) Serial2.write(tank_9, 8);
  else if (state_count == 10) Serial2.write(tank_10, 8);
  else if (state_count == 11) Serial2.write(tank_11, 8);
  else if (state_count == 12) Serial2.write(tank_12, 8);

  if (state_count == tank_count) state_count = 0;
}


void led_blink() {
  digitalWrite(RXled, HIGH);
  delay(50);
  digitalWrite(RXled, LOW);
  delay(50);
  digitalWrite(TXled, HIGH);
  delay(50);
  digitalWrite(TXled, LOW);
  delay(50);
}

void EEPROM_init() {
  device_id = EEPROM.read(100);
  tank_count = EEPROM.read(101);
  fuel_id_1 = EEPROM.read(102);
  fuel_id_2 = EEPROM.read(103);
  fuel_id_3 = EEPROM.read(104);
  fuel_id_4 = EEPROM.read(105);
  fuel_id_5 = EEPROM.read(106);
  fuel_id_6 = EEPROM.read(107);
  fuel_id_7 = EEPROM.read(108);
  fuel_id_8 = EEPROM.read(109);
  fuel_id_9 = EEPROM.read(110);
  fuel_id_10 = EEPROM.read(111);
  fuel_id_11 = EEPROM.read(112);
  fuel_id_12 = EEPROM.read(113);

  Serial.printf("ATG Device is %d\n", device_id);
  Serial.printf("ATG tank count is %d\n", tank_count);
  Serial.printf("ATG Fuel type 1 id is %d\n", fuel_id_1);
  Serial.printf("ATG Fuel type 2 id is %d\n", fuel_id_2);
  Serial.printf("ATG Fuel type 3 id is %d\n", fuel_id_3);
  Serial.printf("ATG Fuel type 4 id is %d\n", fuel_id_4);
  Serial.printf("ATG Fuel type 5 id is %d\n", fuel_id_5);
  Serial.printf("ATG Fuel type 6 id is %d\n", fuel_id_6);
  Serial.printf("ATG Fuel type 7 id is %d\n", fuel_id_7);
  Serial.printf("ATG Fuel type 8 id is %d\n", fuel_id_8);
  Serial.printf("ATG Fuel type 9 id is %d\n", fuel_id_9);
  Serial.printf("ATG Fuel type 10 id is %d\n", fuel_id_10);
  Serial.printf("ATG Fuel type 11 id is %d\n", fuel_id_11);
  Serial.printf("ATG Fuel type 12 id is %d\n", fuel_id_12);

  Serial.println("EEPROM init Done");
}


void handleJsonData() {
  StaticJsonDocument<1024> jsonDoc;

  JsonArray data = jsonDoc.createNestedArray("data");

  if (atg_check) {

    if (tank_count > 0) {

      JsonObject entry1 = data.createNestedObject();
      entry1["stateInfo"] = "No alarm";
      entry1["oilType"] = Fuel_type_1;
      entry1["weight"] = 0;
      entry1["level"] = oil_height_1;
      entry1["oilRatio"] = 0;
      entry1["waterRatio"] = 0;
      entry1["canAddOilWeight"] = 0;
      entry1["temperature"] = atg_temp_1;
      entry1["volume"] = tank_volume_1;
      entry1["netVolume"] = tank_product_1;
      entry1["remainVolume"] = tank_remain_1;
      entry1["waterVolume"] = water_volume_1;
      entry1["waterHeight"] = water_height_1;
      entry1["connect"] = tank_count;
      entry1["id"] = 1;
    }
    if (tank_count > 1) {
      JsonObject entry2 = data.createNestedObject();
      entry2["stateInfo"] = "No alarm";
      entry2["oilType"] = Fuel_type_2;
      entry2["weight"] = 0;
      entry2["level"] = oil_height_2;
      entry2["oilRatio"] = 0;
      entry2["waterRatio"] = 0;
      entry2["canAddOilWeight"] = 0;
      entry2["temperature"] = atg_temp_2;
      entry2["volume"] = tank_volume_2;
      entry2["netVolume"] = tank_product_2;
      entry2["remainVolume"] = tank_remain_2;
      entry2["waterVolume"] = water_volume_2;
      entry2["waterHeight"] = water_height_2;
      entry2["connect"] = tank_count;
      entry2["id"] = 2;
    }

    if (tank_count > 2) {
      JsonObject entry3 = data.createNestedObject();
      entry3["stateInfo"] = "No alarm";
      entry3["oilType"] = Fuel_type_3;
      entry3["weight"] = 0;
      entry3["level"] = oil_height_3;
      entry3["oilRatio"] = 0;
      entry3["waterRatio"] = 0;
      entry3["canAddOilWeight"] = 0;
      entry3["temperature"] = atg_temp_3;
      entry3["volume"] = tank_volume_3;
      entry3["netVolume"] = tank_product_3;
      entry3["remainVolume"] = tank_remain_3;
      entry3["waterVolume"] = water_volume_3;
      entry3["waterHeight"] = water_height_3;
      entry3["connect"] = tank_count;
      entry3["id"] = 3;
    }
    if (tank_count > 3) {
      JsonObject entry4 = data.createNestedObject();
      entry4["stateInfo"] = "No alarm";
      entry4["oilType"] = Fuel_type_4;
      entry4["weight"] = 0;
      entry4["level"] = oil_height_4;
      entry4["oilRatio"] = 0;
      entry4["waterRatio"] = 0;
      entry4["canAddOilWeight"] = 0;
      entry4["temperature"] = atg_temp_4;
      entry4["volume"] = tank_volume_4;
      entry4["netVolume"] = tank_product_4;
      entry4["remainVolume"] = tank_remain_4;
      entry4["waterVolume"] = water_volume_4;
      entry4["waterHeight"] = water_height_4;
      entry4["connect"] = tank_count;
      entry4["id"] = 4;
    }
    if (tank_count > 4) {
      JsonObject entry5 = data.createNestedObject();
      entry5["stateInfo"] = "No alarm";
      entry5["oilType"] = Fuel_type_5;
      entry5["weight"] = 0;
      entry5["level"] = oil_height_5;
      entry5["oilRatio"] = 0;
      entry5["waterRatio"] = 0;
      entry5["canAddOilWeight"] = 0;
      entry5["temperature"] = atg_temp_5;
      entry5["volume"] = tank_volume_5;
      entry5["netVolume"] = tank_product_5;
      entry5["remainVolume"] = tank_remain_5;
      entry5["waterVolume"] = water_volume_5;
      entry5["waterHeight"] = water_height_5;
      entry5["connect"] = tank_count;
      entry5["id"] = 5;
    }
    if (tank_count > 5) {
      JsonObject entry6 = data.createNestedObject();
      entry6["stateInfo"] = "No alarm";
      entry6["oilType"] = Fuel_type_6;
      entry6["weight"] = 0;
      entry6["level"] = oil_height_6;
      entry6["oilRatio"] = 0;
      entry6["waterRatio"] = 0;
      entry6["canAddOilWeight"] = 0;
      entry6["temperature"] = atg_temp_6;
      entry6["volume"] = tank_volume_6;
      entry6["netVolume"] = tank_product_6;
      entry6["remainVolume"] = tank_remain_6;
      entry6["waterVolume"] = water_volume_6;
      entry6["waterHeight"] = water_height_6;
      entry6["connect"] = tank_count;
      entry6["id"] = 6;
    }
    if (tank_count > 6) {
      JsonObject entry7 = data.createNestedObject();
      entry7["stateInfo"] = "No alarm";
      entry7["oilType"] = Fuel_type_7;
      entry7["weight"] = 0;
      entry7["level"] = oil_height_7;
      entry7["oilRatio"] = 0;
      entry7["waterRatio"] = 0;
      entry7["canAddOilWeight"] = 0;
      entry7["temperature"] = atg_temp_7;
      entry7["volume"] = tank_volume_7;
      entry7["netVolume"] = tank_product_7;
      entry7["remainVolume"] = tank_remain_7;
      entry7["waterVolume"] = water_volume_7;
      entry7["waterHeight"] = water_height_7;
      entry7["connect"] = tank_count;
      entry7["id"] = 7;
    }
    if (tank_count > 7) {
      JsonObject entry8 = data.createNestedObject();
      entry8["stateInfo"] = "No alarm";
      entry8["oilType"] = Fuel_type_8;
      entry8["weight"] = 0;
      entry8["level"] = oil_height_8;
      entry8["oilRatio"] = 0;
      entry8["waterRatio"] = 0;
      entry8["canAddOilWeight"] = 0;
      entry8["temperature"] = atg_temp_8;
      entry8["volume"] = tank_volume_8;
      entry8["netVolume"] = tank_product_8;
      entry8["remainVolume"] = tank_remain_8;
      entry8["waterVolume"] = water_volume_8;
      entry8["waterHeight"] = water_height_8;
      entry8["connect"] = tank_count;
      entry8["id"] = 8;
    }

    if (tank_count > 8) {
      JsonObject entry8 = data.createNestedObject();
      entry8["stateInfo"] = "No alarm";
      entry8["oilType"] = Fuel_type_9;
      entry8["weight"] = 0;
      entry8["level"] = oil_height_9;
      entry8["oilRatio"] = 0;
      entry8["waterRatio"] = 0;
      entry8["canAddOilWeight"] = 0;
      entry8["temperature"] = atg_temp_9;
      entry8["volume"] = tank_volume_9;
      entry8["netVolume"] = tank_product_9;
      entry8["remainVolume"] = tank_remain_9;
      entry8["waterVolume"] = water_volume_9;
      entry8["waterHeight"] = water_height_9;
      entry8["connect"] = tank_count;
      entry8["id"] = 9;
    }

    if (tank_count > 9) {
      JsonObject entry8 = data.createNestedObject();
      entry8["stateInfo"] = "No alarm";
      entry8["oilType"] = Fuel_type_10;
      entry8["weight"] = 0;
      entry8["level"] = oil_height_10;
      entry8["oilRatio"] = 0;
      entry8["waterRatio"] = 0;
      entry8["canAddOilWeight"] = 0;
      entry8["temperature"] = atg_temp_10;
      entry8["volume"] = tank_volume_10;
      entry8["netVolume"] = tank_product_10;
      entry8["remainVolume"] = tank_remain_10;
      entry8["waterVolume"] = water_volume_10;
      entry8["waterHeight"] = water_height_10;
      entry8["connect"] = tank_count;
      entry8["id"] = 10;
    }

    if (tank_count > 10) {
      JsonObject entry8 = data.createNestedObject();
      entry8["stateInfo"] = "No alarm";
      entry8["oilType"] = Fuel_type_11;
      entry8["weight"] = 0;
      entry8["level"] = oil_height_11;
      entry8["oilRatio"] = 0;
      entry8["waterRatio"] = 0;
      entry8["canAddOilWeight"] = 0;
      entry8["temperature"] = atg_temp_11;
      entry8["volume"] = tank_volume_11;
      entry8["netVolume"] = tank_product_11;
      entry8["remainVolume"] = tank_remain_11;
      entry8["waterVolume"] = water_volume_11;
      entry8["waterHeight"] = water_height_11;
      entry8["connect"] = tank_count;
      entry8["id"] = 11;
    }

    if (tank_count > 11) {
      JsonObject entry8 = data.createNestedObject();
      entry8["stateInfo"] = "No alarm";
      entry8["oilType"] = Fuel_type_12;
      entry8["weight"] = 0;
      entry8["level"] = oil_height_12;
      entry8["oilRatio"] = 0;
      entry8["waterRatio"] = 0;
      entry8["canAddOilWeight"] = 0;
      entry8["temperature"] = atg_temp_12;
      entry8["volume"] = tank_volume_12;
      entry8["netVolume"] = tank_product_12;
      entry8["remainVolume"] = tank_remain_12;
      entry8["waterVolume"] = water_volume_12;
      entry8["waterHeight"] = water_height_12;
      entry8["connect"] = tank_count;
      entry8["id"] = 12;
    }

  } else {

    JsonObject entry1 = data.createNestedObject();
    entry1["condition"] = "ATG is not Active";
  }

  String jsonData;
  serializeJson(jsonDoc, jsonData);

  server.send(200, "application/json", jsonData);
}


void handleNotFound() {  // error route
  Serial.print("Unknown route requested: ");
  Serial.println(server.uri());

  String message = "{\"error\":\"ERROR 404\",\"message\":\"Route not found\"}";
  server.send(404, "application/json", message);
}

void tank_1_fun() {
  for (int i = 0; i < 8; i++) {

    tank_volume_ary[i] = buffer[26 + i];
    tank_product_ary[i] = buffer[34 + i];
    remain_volume_ary[i] = buffer[42 + i];
    oil_height_ary[i] = buffer[50 + i];
    water_level_ary[i] = buffer[58 + i];
    temperature_ary[i] = buffer[66 + i];
    water_volume_ary[i] = buffer[74 + i];
  }


  Serial.println("Tank 1");

  Fuel_type_1 = "";
  fuel_type_fun(fuel_id_1);
  Fuel_type_1 = Fuel_type;


  tank_volume_1 = hexToIEEEFloat(tank_volume_ary);
  tank_product_1 = hexToIEEEFloat(tank_product_ary);
  tank_remain_1 = hexToIEEEFloat(remain_volume_ary);
  oil_height_1 = hexToIEEEFloat(oil_height_ary);
  water_height_1 = hexToIEEEFloat(water_level_ary);
  atg_temp_1 = hexToIEEEFloat(temperature_ary);
  water_volume_1 = hexToIEEEFloat(water_volume_ary);

  common_print(tank_volume_1, tank_product_1, tank_remain_1, oil_height_1, water_height_1, atg_temp_1, water_volume_1);
}


void tank_2_fun() {
  for (int i = 0; i < 8; i++) {

    tank_volume_ary[i] = buffer[26 + i];
    tank_product_ary[i] = buffer[34 + i];
    remain_volume_ary[i] = buffer[42 + i];
    oil_height_ary[i] = buffer[50 + i];
    water_level_ary[i] = buffer[58 + i];
    temperature_ary[i] = buffer[66 + i];
    water_volume_ary[i] = buffer[74 + i];
  }

  Serial.println("Tank 2");

  Fuel_type_2 = "";
  fuel_type_fun(fuel_id_2);
  Fuel_type_2 = Fuel_type;

  tank_volume_2 = hexToIEEEFloat(tank_volume_ary);
  tank_product_2 = hexToIEEEFloat(tank_product_ary);
  tank_remain_2 = hexToIEEEFloat(remain_volume_ary);
  oil_height_2 = hexToIEEEFloat(oil_height_ary);
  water_height_2 = hexToIEEEFloat(water_level_ary);
  atg_temp_2 = hexToIEEEFloat(temperature_ary);
  water_volume_2 = hexToIEEEFloat(water_volume_ary);

  common_print(tank_volume_2, tank_product_2, tank_remain_2, oil_height_2, water_height_2, atg_temp_2, water_volume_2);


  // Serial.print("ATG tank volume is => ");
  // Serial.println(tank_volume_2, 4);
  // Serial.print("ATG tank product is => ");
  // Serial.println(tank_product_2, 4);
  // Serial.print("ATG tank remain is => ");
  // Serial.println(tank_remain_2, 4);
  // Serial.print("ATG tank oil height is => ");
  // Serial.println(oil_height_2, 4);
  // Serial.print("ATG water height is => ");
  // Serial.println(water_height_2, 4);
  // Serial.print("ATG Temperatur is => ");
  // Serial.println(atg_temp_2, 2);
  // Serial.print("ATG water volume is => ");
  // Serial.println(water_volume_2, 2);
}

void tank_3_fun() {
  for (int i = 0; i < 8; i++) {

    tank_volume_ary[i] = buffer[26 + i];
    tank_product_ary[i] = buffer[34 + i];
    remain_volume_ary[i] = buffer[42 + i];
    oil_height_ary[i] = buffer[50 + i];
    water_level_ary[i] = buffer[58 + i];
    temperature_ary[i] = buffer[66 + i];
    water_volume_ary[i] = buffer[74 + i];
  }

  Serial.println("Tank 3");

  Fuel_type_3 = "";
  fuel_type_fun(fuel_id_3);
  Fuel_type_3 = Fuel_type;

  tank_volume_3 = hexToIEEEFloat(tank_volume_ary);
  tank_product_3 = hexToIEEEFloat(tank_product_ary);
  tank_remain_3 = hexToIEEEFloat(remain_volume_ary);
  oil_height_3 = hexToIEEEFloat(oil_height_ary);
  water_height_3 = hexToIEEEFloat(water_level_ary);
  atg_temp_3 = hexToIEEEFloat(temperature_ary);
  water_volume_3 = hexToIEEEFloat(water_volume_ary);

  common_print(tank_volume_3, tank_product_3, tank_remain_3, oil_height_3, water_height_3, atg_temp_3, water_volume_3);
}

void tank_4_fun() {
  for (int i = 0; i < 8; i++) {

    tank_volume_ary[i] = buffer[26 + i];
    tank_product_ary[i] = buffer[34 + i];
    remain_volume_ary[i] = buffer[42 + i];
    oil_height_ary[i] = buffer[50 + i];
    water_level_ary[i] = buffer[58 + i];
    temperature_ary[i] = buffer[66 + i];
    water_volume_ary[i] = buffer[74 + i];
  }

  Serial.println("Tank 4");

  Fuel_type_4 = "";
  fuel_type_fun(fuel_id_4);
  Fuel_type_4 = Fuel_type;

  tank_volume_4 = hexToIEEEFloat(tank_volume_ary);
  tank_product_4 = hexToIEEEFloat(tank_product_ary);
  tank_remain_4 = hexToIEEEFloat(remain_volume_ary);
  oil_height_4 = hexToIEEEFloat(oil_height_ary);
  water_height_4 = hexToIEEEFloat(water_level_ary);
  atg_temp_4 = hexToIEEEFloat(temperature_ary);
  water_volume_4 = hexToIEEEFloat(water_volume_ary);

  common_print(tank_volume_4, tank_product_4, tank_remain_4, oil_height_4, water_height_4, atg_temp_4, water_volume_4);
}

void tank_5_fun() {
  for (int i = 0; i < 8; i++) {

    tank_volume_ary[i] = buffer[26 + i];
    tank_product_ary[i] = buffer[34 + i];
    remain_volume_ary[i] = buffer[42 + i];
    oil_height_ary[i] = buffer[50 + i];
    water_level_ary[i] = buffer[58 + i];
    temperature_ary[i] = buffer[66 + i];
    water_volume_ary[i] = buffer[74 + i];
  }

  Serial.println("Tank 5");

  Fuel_type_5 = "";
  fuel_type_fun(fuel_id_5);
  Fuel_type_5 = Fuel_type;

  tank_volume_5 = hexToIEEEFloat(tank_volume_ary);
  tank_product_5 = hexToIEEEFloat(tank_product_ary);
  tank_remain_5 = hexToIEEEFloat(remain_volume_ary);
  oil_height_5 = hexToIEEEFloat(oil_height_ary);
  water_height_5 = hexToIEEEFloat(water_level_ary);
  atg_temp_5 = hexToIEEEFloat(temperature_ary);
  water_volume_5 = hexToIEEEFloat(water_volume_ary);

  common_print(tank_volume_5, tank_product_5, tank_remain_5, oil_height_5, water_height_5, atg_temp_5, water_volume_5);
}

void tank_6_fun() {
  for (int i = 0; i < 8; i++) {

    tank_volume_ary[i] = buffer[26 + i];
    tank_product_ary[i] = buffer[34 + i];
    remain_volume_ary[i] = buffer[42 + i];
    oil_height_ary[i] = buffer[50 + i];
    water_level_ary[i] = buffer[58 + i];
    temperature_ary[i] = buffer[66 + i];
    water_volume_ary[i] = buffer[74 + i];
  }

  Serial.println("Tank 6");

  Fuel_type_6 = "";
  fuel_type_fun(fuel_id_6);
  Fuel_type_6 = Fuel_type;

  tank_volume_6 = hexToIEEEFloat(tank_volume_ary);
  tank_product_6 = hexToIEEEFloat(tank_product_ary);
  tank_remain_6 = hexToIEEEFloat(remain_volume_ary);
  oil_height_6 = hexToIEEEFloat(oil_height_ary);
  water_height_6 = hexToIEEEFloat(water_level_ary);
  atg_temp_6 = hexToIEEEFloat(temperature_ary);
  water_volume_6 = hexToIEEEFloat(water_volume_ary);

  common_print(tank_volume_6, tank_product_6, tank_remain_6, oil_height_6, water_height_6, atg_temp_6, water_volume_6);
}

void tank_7_fun() {
  for (int i = 0; i < 8; i++) {

    tank_volume_ary[i] = buffer[26 + i];
    tank_product_ary[i] = buffer[34 + i];
    remain_volume_ary[i] = buffer[42 + i];
    oil_height_ary[i] = buffer[50 + i];
    water_level_ary[i] = buffer[58 + i];
    temperature_ary[i] = buffer[66 + i];
    water_volume_ary[i] = buffer[74 + i];
  }

  Serial.println("Tank 7");

  Fuel_type_7 = "";
  fuel_type_fun(fuel_id_7);
  Fuel_type_7 = Fuel_type;

  tank_volume_7 = hexToIEEEFloat(tank_volume_ary);
  tank_product_7 = hexToIEEEFloat(tank_product_ary);
  tank_remain_7 = hexToIEEEFloat(remain_volume_ary);
  oil_height_7 = hexToIEEEFloat(oil_height_ary);
  water_height_7 = hexToIEEEFloat(water_level_ary);
  atg_temp_7 = hexToIEEEFloat(temperature_ary);
  water_volume_7 = hexToIEEEFloat(water_volume_ary);

  common_print(tank_volume_7, tank_product_7, tank_remain_7, oil_height_7, water_height_7, atg_temp_7, water_volume_7);
}

void tank_8_fun() {
  for (int i = 0; i < 8; i++) {

    tank_volume_ary[i] = buffer[26 + i];
    tank_product_ary[i] = buffer[34 + i];
    remain_volume_ary[i] = buffer[42 + i];
    oil_height_ary[i] = buffer[50 + i];
    water_level_ary[i] = buffer[58 + i];
    temperature_ary[i] = buffer[66 + i];
    water_volume_ary[i] = buffer[74 + i];
  }

  Serial.println("Tank 8");

  Fuel_type_8 = "";
  fuel_type_fun(fuel_id_8);
  Fuel_type_8 = Fuel_type;

  tank_volume_8 = hexToIEEEFloat(tank_volume_ary);
  tank_product_8 = hexToIEEEFloat(tank_product_ary);
  tank_remain_8 = hexToIEEEFloat(remain_volume_ary);
  oil_height_8 = hexToIEEEFloat(oil_height_ary);
  water_height_8 = hexToIEEEFloat(water_level_ary);
  atg_temp_8 = hexToIEEEFloat(temperature_ary);
  water_volume_8 = hexToIEEEFloat(water_volume_ary);

  common_print(tank_volume_8, tank_product_8, tank_remain_8, oil_height_8, water_height_8, atg_temp_8, water_volume_8);
}

void tank_9_fun() {
  for (int i = 0; i < 8; i++) {

    tank_volume_ary[i] = buffer[26 + i];
    tank_product_ary[i] = buffer[34 + i];
    remain_volume_ary[i] = buffer[42 + i];
    oil_height_ary[i] = buffer[50 + i];
    water_level_ary[i] = buffer[58 + i];
    temperature_ary[i] = buffer[66 + i];
    water_volume_ary[i] = buffer[74 + i];
  }

  Serial.println("Tank 9");

  Fuel_type_9 = "";
  fuel_type_fun(fuel_id_9);
  Fuel_type_9 = Fuel_type;

  tank_volume_9 = hexToIEEEFloat(tank_volume_ary);
  tank_product_9 = hexToIEEEFloat(tank_product_ary);
  tank_remain_9 = hexToIEEEFloat(remain_volume_ary);
  oil_height_9 = hexToIEEEFloat(oil_height_ary);
  water_height_9 = hexToIEEEFloat(water_level_ary);
  atg_temp_9 = hexToIEEEFloat(temperature_ary);
  water_volume_9 = hexToIEEEFloat(water_volume_ary);

  common_print(tank_volume_9, tank_product_9, tank_remain_9, oil_height_9, water_height_9, atg_temp_9, water_volume_9);
}

void tank_10_fun() {
  for (int i = 0; i < 8; i++) {

    tank_volume_ary[i] = buffer[26 + i];
    tank_product_ary[i] = buffer[34 + i];
    remain_volume_ary[i] = buffer[42 + i];
    oil_height_ary[i] = buffer[50 + i];
    water_level_ary[i] = buffer[58 + i];
    temperature_ary[i] = buffer[66 + i];
    water_volume_ary[i] = buffer[74 + i];
  }

  Serial.println("Tank 10");

  Fuel_type_10 = "";
  fuel_type_fun(fuel_id_10);
  Fuel_type_10 = Fuel_type;

  tank_volume_10 = hexToIEEEFloat(tank_volume_ary);
  tank_product_10 = hexToIEEEFloat(tank_product_ary);
  tank_remain_10 = hexToIEEEFloat(remain_volume_ary);
  oil_height_10 = hexToIEEEFloat(oil_height_ary);
  water_height_10 = hexToIEEEFloat(water_level_ary);
  atg_temp_10 = hexToIEEEFloat(temperature_ary);
  water_volume_10 = hexToIEEEFloat(water_volume_ary);

  common_print(tank_volume_10, tank_product_10, tank_remain_10, oil_height_10, water_height_10, atg_temp_10, water_volume_10);
}

void tank_11_fun() {
  for (int i = 0; i < 8; i++) {

    tank_volume_ary[i] = buffer[26 + i];
    tank_product_ary[i] = buffer[34 + i];
    remain_volume_ary[i] = buffer[42 + i];
    oil_height_ary[i] = buffer[50 + i];
    water_level_ary[i] = buffer[58 + i];
    temperature_ary[i] = buffer[66 + i];
    water_volume_ary[i] = buffer[74 + i];
  }

  Serial.println("Tank 11");

  Fuel_type_11 = "";
  fuel_type_fun(fuel_id_11);
  Fuel_type_11 = Fuel_type;

  tank_volume_11 = hexToIEEEFloat(tank_volume_ary);
  tank_product_11 = hexToIEEEFloat(tank_product_ary);
  tank_remain_11 = hexToIEEEFloat(remain_volume_ary);
  oil_height_11 = hexToIEEEFloat(oil_height_ary);
  water_height_11 = hexToIEEEFloat(water_level_ary);
  atg_temp_11 = hexToIEEEFloat(temperature_ary);
  water_volume_11 = hexToIEEEFloat(water_volume_ary);

  common_print(tank_volume_11, tank_product_11, tank_remain_11, oil_height_11, water_height_11, atg_temp_11, water_volume_11);
}

void tank_12_fun() {

  for (int i = 0; i < 8; i++) {

    tank_volume_ary[i] = buffer[26 + i];
    tank_product_ary[i] = buffer[34 + i];
    remain_volume_ary[i] = buffer[42 + i];
    oil_height_ary[i] = buffer[50 + i];
    water_level_ary[i] = buffer[58 + i];
    temperature_ary[i] = buffer[66 + i];
    water_volume_ary[i] = buffer[74 + i];
  }

  Serial.println("Tank 12");

  Fuel_type_12 = "";
  fuel_type_fun(fuel_id_12);
  Fuel_type_12 = Fuel_type;

  tank_volume_12 = hexToIEEEFloat(tank_volume_ary);
  tank_product_12 = hexToIEEEFloat(tank_product_ary);
  tank_remain_12 = hexToIEEEFloat(remain_volume_ary);
  oil_height_12 = hexToIEEEFloat(oil_height_ary);
  water_height_12 = hexToIEEEFloat(water_level_ary);
  atg_temp_12 = hexToIEEEFloat(temperature_ary);
  water_volume_12 = hexToIEEEFloat(water_volume_ary);

  common_print(tank_volume_12, tank_product_12, tank_remain_12, oil_height_12, water_height_12, atg_temp_12, water_volume_12);
}

void common_print(float data_1, float data_2, float data_3, float data_4, float data_5, float data_6, float data_7) {

  Serial.print("ATG tank volume is => ");
  Serial.println(data_1, 4);
  Serial.print("ATG tank product is => ");
  Serial.println(data_2, 4);
  Serial.print("ATG tank remain is => ");
  Serial.println(data_3, 4);
  Serial.print("ATG tank oil height is => ");
  Serial.println(data_4, 4);
  Serial.print("ATG water height is => ");
  Serial.println(data_5, 4);
  Serial.print("ATG Temperatur is => ");
  Serial.println(data_6, 2);
  Serial.print("ATG water volume is => ");
  Serial.println(data_7, 2);
}

float hexToIEEEFloat(char* hexArray) {
  uint32_t hexValue = 0;

  // Convert each hex character to a single 32-bit integer
  for (int i = 0; i < 8; i++) {
    hexValue <<= 4;
    if (hexArray[i] >= '0' && hexArray[i] <= '9')
      hexValue |= (hexArray[i] - '0');
    else if (hexArray[i] >= 'A' && hexArray[i] <= 'F')
      hexValue |= (hexArray[i] - 'A' + 10);
  }

  // Interpret the integer as a float
  float result;
  memcpy(&result, &hexValue, sizeof(result));
  return result;
}

void wifi_init() {

  Serial.println("wifi init function");
  wifi_try = 0;

  WiFi.begin(ssid, password);  ///akkkkk
  while (WiFi.status() != WL_CONNECTED && wifi_try++ != 15) {
    digitalWrite(wifiled, LOW);
    delay(500);
    Serial.print(".");
  }

  if (WiFi.status() == WL_CONNECTED) {
    Serial.println();
    Serial.print("Wifi is connected in ");
    Serial.println(ssid);

    Serial.println("IP Address: ");
    Serial.println(WiFi.localIP());
  }
}

void fuel_type_fun(int data) {
  switch (data) {
    case 1:
      Fuel_type = "Petrol 92";
      break;
    case 2:
      Fuel_type = "Petrol 95";
      break;
    case 3:
      Fuel_type = "Petrol 97";
      break;
    case 4:
      Fuel_type = "Super Diesel";
      break;
    case 5:
      Fuel_type = "Diesel";
      break;
    default:
      Fuel_type = "-";
      break;
  }
}

void reconnect() {
  while (!client.connected()) {

    Serial.println("MQtt is connecting ");
    String clientId = "Rfid_client-";
    clientId += String(random(0xffff), HEX);
    // Serial.println(clientId);
    if (client.connect(clientId.c_str(), mqtt_username, mqtt_password)) {
      Serial.println("Mqtt connected");

      client.subscribe("Connect");                    // subscribe the topics here
      client.publish("connect", "Device1Connected");  // subscribe the topics here
      client.subscribe("detpos/#");

    } else {
      Serial.print("mqtt connection failed, rc=");
      Serial.print(client.state());
      Serial.print(" try again in 2 seconds");
      delay(2000);
    }
  }
}

void callback(char* topic, byte* payload, unsigned int length) {
  incommingMessage = "";

  for (int i = 0; i < length; i++) {
    incommingMessage += (char)payload[i];
  }

  // Serial.println(topic);
  Serial.println(incommingMessage);

  if (String(topic) == String(atg_topic)) {
    DynamicJsonDocument doc(4096);
    DeserializationError error = deserializeJson(doc, incommingMessage);

    if (error) {
      Serial.print(F("JSON parsing failed: "));
      Serial.println(error.c_str());
      return;
    }

    int dev_id = 0;
    int tank_id = 0;
    int id_1 = 0;
    int id_2 = 0;
    int id_3 = 0;
    int id_4 = 0;
    int id_5 = 0;
    int id_6 = 0;
    int id_7 = 0;
    int id_8 = 0;
    int id_9 = 9;
    int id_10 = 10;
    int id_11 = 11;
    int id_12 = 12;

    dev_id = doc["deviceId"].as<const int>();
    tank_id = doc["tank_count"].as<const int>();
    id_1 = doc["fuelId_1"].as<const int>();
    id_2 = doc["fuelId_2"].as<const int>();
    id_3 = doc["fuelId_3"].as<const int>();
    id_4 = doc["fuelId_4"].as<const int>();
    id_5 = doc["fuelId_5"].as<const int>();
    id_6 = doc["fuelId_6"].as<const int>();
    id_7 = doc["fuelId_7"].as<const int>();
    id_8 = doc["fuelId_8"].as<const int>();
    id_9 = doc["fuelId_9"].as<const int>();
    id_10 = doc["fuelId_10"].as<const int>();
    id_11 = doc["fuelId_11"].as<const int>();
    id_12 = doc["fuelId_12"].as<const int>();

    EEPROM.write(100, dev_id);  // Device Id
    EEPROM.commit();
    EEPROM.write(101, tank_id);  // Tank Count
    EEPROM.commit();
    EEPROM.write(102, id_1);  // Tank 1 fuel type =>(1=92,2=95,3=97,4=PD,5=D)
    EEPROM.commit();
    EEPROM.write(103, id_2);  // Tank 2 fuel type =>(1=92,2=95,3=97,4=PD,5=D)
    EEPROM.commit();
    EEPROM.write(104, id_3);  // Tank 3 fuel type =>(1=92,2=95,3=97,4=PD,5=D)
    EEPROM.commit();
    EEPROM.write(105, id_4);  // Tank 4 fuel type =>(1=92,2=95,3=97,4=PD,5=D)
    EEPROM.commit();
    EEPROM.write(106, id_5);  // Tank 5 fuel type =>(1=92,2=95,3=97,4=PD,5=D)
    EEPROM.commit();
    EEPROM.write(107, id_6);  // Tank 6 fuel type =>(1=92,2=95,3=97,4=PD,5=D)
    EEPROM.commit();
    EEPROM.write(108, id_7);  // Tank 7 fuel type =>(1=92,2=95,3=97,4=PD,5=D)
    EEPROM.commit();
    EEPROM.write(109, id_8);  // Tank 8 fuel type =>(1=92,2=95,3=97,4=PD,5=D)
    EEPROM.commit();
    EEPROM.write(110, id_9);  // Tank 9 fuel type =>(1=92,2=95,3=97,4=PD,5=D)
    EEPROM.commit();
    EEPROM.write(111, id_10);  // Tank 10 fuel type =>(1=92,2=95,3=97,4=PD,5=D)
    EEPROM.commit();
    EEPROM.write(112, id_11);  // Tank 11 fuel type =>(1=92,2=95,3=97,4=PD,5=D)
    EEPROM.commit();
    EEPROM.write(113, id_12);  // Tank 12 fuel type =>(1=92,2=95,3=97,4=PD,5=D)
    EEPROM.commit();
    Serial.println("yep all save from mqtt broker");

    device_id = EEPROM.read(100);
    tank_count = EEPROM.read(101);
    fuel_id_1 = EEPROM.read(102);
    fuel_id_2 = EEPROM.read(103);
    fuel_id_3 = EEPROM.read(104);
    fuel_id_4 = EEPROM.read(105);
    fuel_id_5 = EEPROM.read(106);
    fuel_id_6 = EEPROM.read(107);
    fuel_id_7 = EEPROM.read(108);
    fuel_id_8 = EEPROM.read(109);
    fuel_id_9 = EEPROM.read(110);
    fuel_id_10 = EEPROM.read(111);
    fuel_id_11 = EEPROM.read(112);
    fuel_id_12 = EEPROM.read(113);

    Serial.printf("ATG Device is %d\n", device_id);
    Serial.printf("ATG tank count is %d\n", tank_count);
    Serial.printf("ATG Fuel type 1 id is %d\n", fuel_id_1);
    Serial.printf("ATG Fuel type 2 id is %d\n", fuel_id_2);
    Serial.printf("ATG Fuel type 3 id is %d\n", fuel_id_3);
    Serial.printf("ATG Fuel type 4 id is %d\n", fuel_id_4);
    Serial.printf("ATG Fuel type 5 id is %d\n", fuel_id_5);
    Serial.printf("ATG Fuel type 6 id is %d\n", fuel_id_6);
    Serial.printf("ATG Fuel type 7 id is %d\n", fuel_id_7);
    Serial.printf("ATG Fuel type 8 id is %d\n", fuel_id_8);
    Serial.printf("ATG Fuel type 9 id is %d\n", fuel_id_9);
    Serial.printf("ATG Fuel type 10 id is %d\n", fuel_id_10);
    Serial.printf("ATG Fuel type 11 id is %d\n", fuel_id_11);
    Serial.printf("ATG Fuel type 12 id is %d\n", fuel_id_12);
  }
}
