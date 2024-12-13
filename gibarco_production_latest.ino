#include <WiFi.h>
#include <PubSubClient.h>
#include <WiFiClientSecure.h>
#include <EEPROM.h>
#include <ArduinoJson.h>

// const char* mqtt_server = "192.168.0.100";
// const char* mqtt_username = "detpos";
// const char* mqtt_password = "asdffdsa";
// const int mqtt_port = 1883;


/******* MQTT Broker Connection Details *******/
const char* mqtt_server = "ee57fed264c64c2ea4a4777ba708f972.s2.eu.hivemq.cloud";
const char* mqtt_username = "detpos";
const char* mqtt_password = "Asdffdsa1";
const int mqtt_port = 8883;

// WiFiClient espClient;
// PubSubClient client(espClient);

WiFiClientSecure espClient;  ///////
PubSubClient client(espClient);

static const char* root_ca PROGMEM = R"EOF(
-----BEGIN CERTIFICATE-----
MIIFazCCA1OgAwIBAgIRAIIQz7DSQONZRGPgu2OCiwAwDQYJKoZIhvcNAQELBQAw
TzELMAkGA1UEBhMCVVMxKTAnBgNVBAoTIEludGVybmV0IFNlY3VyaXR5IFJlc2Vh
cmNoIEdyb3VwMRUwEwYDVQQDEwxJU1JHIFJvb3QgWDEwHhcNMTUwNjA0MTEwNDM4
WhcNMzUwNjA0MTEwNDM4WjBPMQswCQYDVQQGEwJVUzEpMCcGA1UEChMgSW50ZXJu
ZXQgU2VjdXJpdHkgUmVzZWFyY2ggR3JvdXAxFTATBgNVBAMTDElTUkcgUm9vdCBY
MTCCAiIwDQYJKoZIhvcNAQEBBQADggIPADCCAgoCggIBAK3oJHP0FDfzm54rVygc
h77ct984kIxuPOZXoHj3dcKi/vVqbvYATyjb3miGbESTtrFj/RQSa78f0uoxmyF+
0TM8ukj13Xnfs7j/EvEhmkvBioZxaUpmZmyPfjxwv60pIgbz5MDmgK7iS4+3mX6U
A5/TR5d8mUgjU+g4rk8Kb4Mu0UlXjIB0ttov0DiNewNwIRt18jA8+o+u3dpjq+sW
T8KOEUt+zwvo/7V3LvSye0rgTBIlDHCNAymg4VMk7BPZ7hm/ELNKjD+Jo2FR3qyH
B5T0Y3HsLuJvW5iB4YlcNHlsdu87kGJ55tukmi8mxdAQ4Q7e2RCOFvu396j3x+UC
B5iPNgiV5+I3lg02dZ77DnKxHZu8A/lJBdiB3QW0KtZB6awBdpUKD9jf1b0SHzUv
KBds0pjBqAlkd25HN7rOrFleaJ1/ctaJxQZBKT5ZPt0m9STJEadao0xAH0ahmbWn
OlFuhjuefXKnEgV4We0+UXgVCwOPjdAvBbI+e0ocS3MFEvzG6uBQE3xDk3SzynTn
jh8BCNAw1FtxNrQHusEwMFxIt4I7mKZ9YIqioymCzLq9gwQbooMDQaHWBfEbwrbw
qHyGO0aoSCqI3Haadr8faqU9GY/rOPNk3sgrDQoo//fb4hVC1CLQJ13hef4Y53CI
rU7m2Ys6xt0nUW7/vGT1M0NPAgMBAAGjQjBAMA4GA1UdDwEB/wQEAwIBBjAPBgNV
HRMBAf8EBTADAQH/MB0GA1UdDgQWBBR5tFnme7bl5AFzgAiIyBpY9umbbjANBgkq
hkiG9w0BAQsFAAOCAgEAVR9YqbyyqFDQDLHYGmkgJykIrGF1XIpu+ILlaS/V9lZL
ubhzEFnTIZd+50xx+7LSYK05qAvqFyFWhfFQDlnrzuBZ6brJFe+GnY+EgPbk6ZGQ
3BebYhtF8GaV0nxvwuo77x/Py9auJ/GpsMiu/X1+mvoiBOv/2X/qkSsisRcOj/KK
NFtY2PwByVS5uCbMiogziUwthDyC3+6WVwW6LLv3xLfHTjuCvjHIInNzktHCgKQ5
ORAzI4JMPJ+GslWYHb4phowim57iaztXOoJwTdwJx4nLCgdNbOhdjsnvzqvHu7Ur
TkXWStAmzOVyyghqpZXjFaH3pO3JLF+l+/+sKAIuvtd7u+Nxe5AW0wdeRlN8NwdC
jNPElpzVmbUq4JUagEiuTDkHzsxHpFKVK7q4+63SM1N95R1NbdWhscdCb+ZAJzVc
oyi3B43njTOQ5yOf+1CceWxG1bQVs5ZufpsMljq4Ui0/1lvh+wjChP4kqKOJ2qxq
4RgqsahDYVvTH9w7jXbyLeiNdd8XM2w9U/t7y0Ff/9yi0GE44Za4rF2LN9d11TPA
mRGunUHBcnWEvgJBQl9nJEiU0Zsnvgc/ubhPgXRR4Xq37Z0j4r7g1SgEEzwxA57d
emyPxgcYxn/eR44/KJ4EBs+lVDR3veyJm+kXQ99b21/+jh5Xos1AnX5iItreGCc=
-----END CERTIFICATE-----
)EOF";

String incommingMessage;

// mqtt topic
char pumpapprobuf[22] = "detpos/local_server/1";
char pumpreqbuf[23] = "detpos/device/permit/1";
char pumppresetbuf[28] = "detpos/local_server/preset";
char pplive[25] = "detpos/device/livedata/1";
char ppfinal[22] = "detpos/device/Final/1";
char pricechange[26] = "detpos/local_server/price";
char pricereqbuf[25] = "detpos/device/pricereq/1";
char activebuf[23] = "detpos/device/active/1";
char devicebuf[2] = "1";
char device_Id_topic[40] = "detpos/local_server/initial1/det/0A0000";
char Reset_topic[17] = "detpos/hmi/reset";
char server_rpy[20] = "detpos/local_server";      //ak
char server_err[25] = "detpos/local_server/err";  //ak
char myreload[29] = "detpos/local_server/reload/1";
char pumpfinalreloadbuf[23] = "detpos/device/Reload/1";  // reload add

bool pumpapprocount1 = false;
bool pumpapprocount2 = false;

bool zerocount = true;

#define wifiled 33
#define powerled 32
#define TXled 27
#define RXled 26
#define RXD2 16  // Set your RX pin
#define TXD2 17  // Set your TX pin


/// Pump Protocol
unsigned char state_req_1[1] = { 0x01 };
unsigned char state_req_2[1] = { 0x02 };

unsigned char pump_stop_1[1] = { 0x31 };
unsigned char pump_stop_2[1] = { 0x32 };

unsigned char tran_data_1[1] = { 0x41 };
unsigned char tran_data_2[1] = { 0x42 };

unsigned char auth_1[1] = { 0x11 };
unsigned char auth_2[1] = { 0x12 };

unsigned char live_1[1] = { 0x61 };
unsigned char live_2[1] = { 0x62 };

unsigned char final_data_1[1] = { 0x51 };
unsigned char final_data_2[1] = { 0x52 };


bool active_check = false;
bool presetcount = false;
bool lastpresetsend = false;
bool id_1 = false;
bool id_2 = false;

char Buffer[150];
char mqttdatabuf[50];
char presetmqttary[11];
char pricechangeary[7];
char charArray[4];

char pp_live[6];
char pp_live_2[6];

char pump_price_ary[4];
char tran_price_ary[6];
char tran_volume_ary[6];
char total_volume_ary[8];
char total_amount_ary[8];


String price;
String liter;
String price_state;
String total_state;
String total_amount;

String mqtt_price;
String mqtt_liter;


int wait_count = 0;

int pumpid1 = 1;
int pumpid2 = 2;
int pumpid3;
int pumpid4;
int pumpid5;
int pumpid6;
int pumpid7;
int pumpid8;
int nozzlenum;
int devicenum;

int pumpmqttnum;
int pumpmqttnum2;
int pumpmqttnum3;

int count;
int add_count;

unsigned long start_time;

void setup() {

  Serial.begin(115200);
  Serial2.begin(5787, SERIAL_8E1, 16, 17);

  EEPROM.begin(512);

  pinMode(15, OUTPUT);
  pinMode(wifiled, OUTPUT);
  pinMode(powerled, OUTPUT);
  pinMode(TXled, OUTPUT);
  pinMode(RXled, OUTPUT);

  // EEPROMWrite();   // to change

  espClient.setCACert(root_ca);

  initEEPROMdatashow();
  EEPROMinit();

  // initWiFi();

  client.setServer(mqtt_server, mqtt_port);
  client.setCallback(callback);


  delay(2000);

  // digitalWrite(15, HIGH);

  // Serial2.write(state_req_1, 1);
  // delay(2);
  // digitalWrite(15, LOW);
}

void EEPROMWrite() {
  EEPROM.write(109, 1);  //device id
  EEPROM.commit();
  EEPROM.write(110, 2);  // nozzle count
  EEPROM.commit();
  EEPROM.write(101, 1);  // nozzle id
  EEPROM.commit();
  EEPROM.write(102, 2);
  EEPROM.commit();
  EEPROM.write(103, 0);
  EEPROM.commit();
  EEPROM.write(104, 0);
  EEPROM.commit();
  EEPROM.write(105, 0);
  EEPROM.commit();
  EEPROM.write(106, 0);
  EEPROM.commit();
  EEPROM.write(107, 0);
  EEPROM.commit();
  EEPROM.write(108, 0);
  EEPROM.commit();

  Serial.println("yep all save $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$");
}


void loop() {

  // if (WiFi.status() == WL_CONNECTED) {

  //   if (!client.connected()) {
  //     Serial.println("Mqtt is connecting");
  //     reconnect();

  //   } else {

  //     main_fun();
  //   }

  // } else initWiFi();
  // client.loop();

  main_fun();
}


void main_fun() {

  delay(1);

  if (Serial2.available() > 0) {

    start_time = millis() / 1000;

    Buffer[count] = Serial2.read();
    delay(2);
    // delay(2);

    Serial.print(count);
    Serial.print("/");
    Serial.print(Buffer[count], HEX);
    Serial.print(" ");

    count++;

    // if (Buffer[count - 1] == 0x71 || Buffer[count - 1] == 0x72) count = 0;

    if (Buffer[count - 1] == 0xF0) count = 0;

    if (Buffer[count - 1] == 0x61 || Buffer[count - 1] == 0x62) count = 0;

    if (Buffer[count - 1] == 0x91) {  // live one

      Serial.println();

      // for (int i = 0; i < 150; i++) {
      //   Buffer[i] = 0;
      // }

      count = 0;

      digitalWrite(15, HIGH);

      // Serial.println(" live 1");
      // id_1 = true;
      Serial2.write(live_1, 1);
      delay(2);
      digitalWrite(15, LOW);


      while (!Serial2.available() && wait_count++ < 10) {
        delay(500);
        Serial.println("..");
      }
      wait_count = 0;

      count++;

      while (Serial2.available()) {
        Buffer[count] = Serial2.read();
        delay(2);

        Serial.print(count);
        Serial.print("/");
        Serial.print(Buffer[count], HEX);
        Serial.print(" ");

        count++;

        // if (Buffer[count - 1] == 0xF0) count = 0;

      }

      count = 0;

      for (int i = 1; i < 7; i++) {
        pp_live[i - 1] = Buffer[i];
      }

      Serial.println();

      mqttpplive();
      // id_1 = false;
      // id_2 = false;
    }

    if (Buffer[count - 1] == 0x92) {  // live two

      Serial.println();

      // for (int i = 0; i < 151; i++) {
      //   Buffer[i] = 0;
      // }

      count = 0;

      digitalWrite(15, HIGH);
      Serial2.write(live_2, 1);
      delay(2);
      digitalWrite(15, LOW);

      while (!Serial2.available() > 0 && wait_count++ < 10) {
        delay(500);
        Serial.println("..");
      }


      count++;
      while (Serial2.available() > 0) {
        Buffer[count] = Serial2.read();
        delay(2);

        Serial.print(count);
        Serial.print("//");
        Serial.print(Buffer[count], HEX);
        Serial.print(" ");

        count++;

        // if (Buffer[count - 1] == 0xF0) count = 0;
      }

      count = 0;

      for (int i = 1; i < 7; i++) {
        pp_live_2[i -1] = Buffer[i];
      }

      mqttpplive();
    }

    manage_msg();

    if (Buffer[count - 1] == 0xB1 || Buffer[count - 1] == 0xA1) {
      id_1 = true;
      read_fueled_data();
      read_totalizer();
      mqttfinal();

      id_1 = false;
    }

    if (Buffer[count - 1] == 0xB2 || Buffer[count - 1] == 0xA2) {

      id_2 = true;
      read_fueled_data();
      read_totalizer();
      mqttfinal();

      id_2 = false;
    }

    add_count++;
    pump_state();

  } else {

    if (millis() / 1000 - start_time > 1) {

      if (active_check) {
        Serial.println("Waiting reply from dispenser");

        digitalWrite(15, HIGH);
        Serial2.write(state_req_1, 1);
        delay(2);
        digitalWrite(15, LOW);
        active_check = false;
      }

      start_time = millis() / 1000;

    } else {
      // Serial.println("i am working");
      active_check = true;
    }
  }
}


void read_fueled_data() {
  Serial.println();

  count = 0;

  digitalWrite(15, HIGH);
  if (id_1) Serial2.write(tran_data_1, 1);
  else if (id_2) Serial2.write(tran_data_2, 1);

  delay(2);
  digitalWrite(15, LOW);

  while (!Serial2.available()) {
    delay(2);
  }

  while (Serial2.available() > 0) {

    // Serial.print("i am B1 tran");
    Buffer[count] = Serial2.read();
    delay(5);

    Serial.print(count);
    Serial.print("/");
    Serial.print(Buffer[count], HEX);
    Serial.print(" ");

    count++;

    if (Buffer[count - 1] == 0xF0) count = 0;
  }
  Serial.println();

  for (int i = 0; i < 33; i++) {

    switch (Buffer[i]) {

      case 0xF7:
        // Serial.println("i am F7");
        i++;
        for (int j = 0; j < 4; j++) {
          pump_price_ary[j] = Buffer[i + j];

          Serial.print(pump_price_ary[j], HEX);
          Serial.print(" ");
        }
        Serial.println();

        break;

      case 0xF9:
        i++;
        for (int j = 0; j < 6; j++) {
          tran_price_ary[j] = Buffer[i + j];

          Serial.print(tran_price_ary[j], HEX);
          Serial.print(" ");
        }
        Serial.println();

        break;

      case 0xFA:
        i++;
        for (int j = 0; j < 6; j++) {
          tran_volume_ary[j] = Buffer[i + j];

          Serial.print(tran_volume_ary[j], HEX);
          Serial.print(" ");
        }
        Serial.println();
        break;
    }
  }

  price_state = bcd_converter(pump_price_ary, 4);
  price = bcd_converter(tran_price_ary, 6);
  liter = bcd_converter(tran_volume_ary, 6);

  Serial.println(price_state);
  Serial.println(price);
  Serial.println(liter);
}

void read_totalizer() {

  digitalWrite(15, HIGH);
  if (id_1) {
    Serial.println("Total 11111111111111111111111111");
    Serial2.write(final_data_1, 1);
    delay(2);
    // id_1 = false;
  } else if (id_2) {

    Serial.println("Total 2222222222222222222222222");

    Serial2.write(final_data_2, 1);
    delay(2);
    // id_2 = false;
  }

  digitalWrite(15, LOW);

  while (!Serial2.available()) {
    delay(2);
    Serial.println(".");
  }

  while (Serial2.available() > 0) {

    // if (Serial2.read() == 0xF5) break;

    Buffer[count] = Serial2.read();
    delay(5);

    Serial.print(count);
    Serial.print("/");
    Serial.print(Buffer[count], HEX);
    Serial.print(" ");

    count++;

    if (Buffer[count - 1] == 0xF0) count = 0;  /// to read grade one only
  }

  for (int i = 0; i < 32; i++) {
    switch (Buffer[i]) {
      case 0xF9:
        i++;
        for (int j = 0; j < 8; j++) {
          total_volume_ary[j] = Buffer[j + i];
          Serial.print(total_volume_ary[j], HEX);
          Serial.print(" ");
        }
        Serial.println();

        break;

      case 0xFA:
        i++;
        for (int j = 0; j < 8; j++) {
          total_amount_ary[j] = Buffer[j + i];
          Serial.print(total_amount_ary[j], HEX);
          Serial.print(" ");
        }

        Serial.println();
        break;
    }

    if (Buffer[i] == 0xF5) {
      Serial.println("bye .................");
    }
  }

  total_amount = bcd_converter(total_amount_ary, 8);
  total_state = bcd_converter(total_volume_ary, 8);

  // Serial.println(" /////////////////////////////////// ");
  Serial.println(total_amount);
  Serial.println(total_state);
}

void manage_msg() {

  if (Buffer[count - 1] == 0x71) preset_fun();
  else if (Buffer[count - 1] == 0x72) preset_fun_2();
}


void preset_fun() {
  digitalWrite(15, HIGH);
  Serial2.write(auth_1, 1);
  delay(2);
  digitalWrite(15, LOW);
}


void preset_fun_2() {
  digitalWrite(15, HIGH);
  Serial2.write(auth_2, 1);
  delay(2);
  digitalWrite(15, LOW);
}


void pump_state() {

  // Serial.println();
  // Serial.print("address count is => ");
  // Serial.println(add_count);

  delay(150);
  digitalWrite(15, HIGH);
  if (add_count == 1) Serial2.write(state_req_1, 1);
  else if (add_count == 2) Serial2.write(state_req_2, 1);
  delay(2);
  digitalWrite(15, LOW);

  Serial.println("send pump state");

  if (add_count == 2) add_count = 0;
}


String bcd_converter(char* data, size_t length) {
  char number[9] = { 0 };
  int digitIndex = 0;


  for (int i = length - 1; i >= 0; --i) {
    uint8_t byte = data[i];


    if ((byte & 0xF0) == 0xE0) {
      number[digitIndex++] = '0' + (byte & 0x0F);
    }
  }

  return String(number);
}


void mqttpplive() {

  // price = "1000";
  // liter = "1111";
  // Serial.println("mqtt livee");

  for (int i = 7; i < 50; i++) {
    mqttdatabuf[i] = 0;
  }

  mqtt_liter = "0";
  mqtt_price = "";

  mqtt_price = bcd_converter(pp_live, 6);

  Serial.print("live one price is => ");
  Serial.println(mqtt_price);

  Serial.print("buffer one data is => ");
  Serial.println(Buffer[0], HEX);


  pumpidchange();

  int arycount = 2;


  mqttdatabuf[2] = 'L';
  arycount++;

  char liter_ary[11];
  char price_ary[11];

  for (int y = 0; y < 11; y++) {
    liter_ary[y] = 0;
    price_ary[y] = 0;
  }

  mqtt_liter.toCharArray(liter_ary, mqtt_liter.length() + 1);
  mqtt_price.toCharArray(price_ary, price.length() + 1);
  // Serial.println(liter.length());

  int lengthcout = 0;
  if (mqtt_liter.length() < 4) {
    //
    mqttdatabuf[arycount++] = '.';
    for (int y = 0; y < mqtt_liter.length(); y++) {
      mqttdatabuf[arycount++] = liter_ary[y];
    }
  } else {
    for (int y = 0; y < mqtt_liter.length() - 3; y++) {
      mqttdatabuf[arycount++] = liter_ary[y];
      lengthcout++;
    }
    mqttdatabuf[arycount++] = '.';
    for (int y = 0; y < mqtt_liter.length() - lengthcout; y++) {
      mqttdatabuf[arycount++] = liter_ary[y + lengthcout];
    }
  }

  mqttdatabuf[arycount++] = 'P';
  for (int y = 0; y < mqtt_price.length(); y++) {
    mqttdatabuf[arycount++] = price_ary[y];
  }


  for (int y = 0; y < 50; y++) {
    if (mqttdatabuf[y] == 0x00) break;
    Serial.print(mqttdatabuf[y]);
    Serial.print(" ");
  }

  client.publish(pplive, mqttdatabuf);
}

void mqttpplive_2() {

  // price = "1000";
  // liter = "1111";
  // Serial.println("mqtt livee");

  for (int i = 7; i < 50; i++) {
    mqttdatabuf[i] = 0;
  }

  mqtt_liter = "0";
  mqtt_price = "";

  mqtt_price = bcd_converter(pp_live_2, 6);

  Serial.print("live two price is => ");
  Serial.println(mqtt_price);

  Serial.print("buffer two data is => ");
  Serial.println(Buffer[0], HEX);


  pumpidchange();

  int arycount = 2;


  mqttdatabuf[2] = 'L';
  arycount++;

  char liter_ary[11];
  char price_ary[11];

  for (int y = 0; y < 11; y++) {
    liter_ary[y] = 0;
    price_ary[y] = 0;
  }

  mqtt_liter.toCharArray(liter_ary, mqtt_liter.length() + 1);
  mqtt_price.toCharArray(price_ary, price.length() + 1);
  // Serial.println(liter.length());

  int lengthcout = 0;
  if (mqtt_liter.length() < 4) {
    //
    mqttdatabuf[arycount++] = '.';
    for (int y = 0; y < mqtt_liter.length(); y++) {
      mqttdatabuf[arycount++] = liter_ary[y];
    }
  } else {
    for (int y = 0; y < mqtt_liter.length() - 3; y++) {
      mqttdatabuf[arycount++] = liter_ary[y];
      lengthcout++;
    }
    mqttdatabuf[arycount++] = '.';
    for (int y = 0; y < mqtt_liter.length() - lengthcout; y++) {
      mqttdatabuf[arycount++] = liter_ary[y + lengthcout];
    }
  }

  mqttdatabuf[arycount++] = 'P';
  for (int y = 0; y < mqtt_price.length(); y++) {
    mqttdatabuf[arycount++] = price_ary[y];
  }


  for (int y = 0; y < 50; y++) {
    if (mqttdatabuf[y] == 0x00) break;
    Serial.print(mqttdatabuf[y]);
    Serial.print(" ");
  }

  client.publish(pplive, mqttdatabuf);
}

void mqttfinal() {


  for (int y = 0; y < 50; y++) {
    mqttdatabuf[y] = 0;
  }

  int arycount = 2;

  pumpidchange();

  mqttdatabuf[arycount++] = 'S';


  char liter_ary[11];
  char price_ary[11];
  char price_state_ary[11];
  char total_ary[11];
  char total_amount_ary[11];

  for (int y = 0; y < 11; y++) {
    liter_ary[y] = 0;
    price_ary[y] = 0;
    price_state_ary[y] = 0;
    total_amount_ary[y] = 0;
  }

  liter.toCharArray(liter_ary, liter.length() + 1);
  price.toCharArray(price_ary, price.length() + 1);
  price_state.toCharArray(price_state_ary, price_state.length() + 1);
  total_state.toCharArray(total_ary, total_state.length() + 1);
  total_amount.toCharArray(total_amount_ary, total_amount.length() + 1);

  for (int y = 0; y < price_state.length(); y++) {
    mqttdatabuf[arycount++] = price_state_ary[y];
  }

  mqttdatabuf[arycount++] = 'L';


  Serial.println(liter.length());

  int lengthcout = 0;
  if (liter.length() < 3) {
    //
    mqttdatabuf[arycount++] = '.';
    for (int y = 0; y < liter.length(); y++) {
      mqttdatabuf[arycount++] = liter_ary[y];
    }
  } else {
    for (int y = 0; y < liter.length() - 2; y++) {
      mqttdatabuf[arycount++] = liter_ary[y];
      lengthcout++;
    }
    mqttdatabuf[arycount++] = '.';
    for (int y = 0; y < liter.length() - lengthcout; y++) {
      mqttdatabuf[arycount++] = liter_ary[y + lengthcout];
    }
  }

  mqttdatabuf[arycount++] = 'P';
  for (int y = 0; y < price.length(); y++) {
    mqttdatabuf[arycount++] = price_ary[y];
  }

  mqttdatabuf[arycount++] = 'T';

  Serial.print("before arr count is => ");
  Serial.println(arycount);

  int y = total_state.length() - 2;

  for (int i = 0; i < total_state.length(); i++) {

    if (i == y) {
      mqttdatabuf[arycount++] = '.';
    }
    mqttdatabuf[arycount++] = total_ary[i];
  }

  Serial.print("after arr count is => ");
  Serial.println(arycount);

  mqttdatabuf[arycount] = 'A';
  arycount++;

  for (int y = total_amount.length() - 1; y >= 0; y--) {
    mqttdatabuf[arycount + y] = total_amount_ary[y];
  }

  for (int y = 0; y < 50; y++) {
    if (mqttdatabuf[y] == 0x00) break;
    Serial.print(mqttdatabuf[y]);
    Serial.print(" ");
  }

  client.publish(ppfinal, mqttdatabuf);
}


// void server_post_appro() {  /// for http post for appro route

//   String nozzleNo = "";
//   if (common_pump_Id_two < 10) nozzleNo = "0" + String(common_pump_Id_two);
//   else nozzleNo = String(common_pump_Id_two);

//   String Dev_ID = String(devicenum);


//   String serverAddress = "http://192.168.0.100:9000/api/detail-sale?depNo=" + Dev_ID + "&nozzleNo=" + nozzleNo;

//   // Serial.print("server Address two => ");
//   // Serial.println(serverAddress);

//   String carNo;
//   if (strlen(Car_no_two) == 0) carNo = "-";
//   else carNo = String(Car_no_two);

//   String member_id;
//   if (strlen(Member_id_two) == 0) member_id = "-";
//   else member_id = String(Member_id_two);

//   String member_name;
//   if (strlen(Member_name_two) == 0) member_name = "-";
//   else member_name = String(Member_name_two);

//   String vehicleType = vehicle_name;
//   String fuelType = Nozzle_name;
//   String cashType = cash_name_two;
//   String device = "HMI";

//   HTTPClient http;

//   http.begin(serverAddress);

//   http.addHeader("Authorization", "Bearer " + String(accessToken_two));

//   String formDataBoundary = "----WebKitFormBoundary";
//   String formData = "--" + formDataBoundary + "\r\n";
//   formData += "Content-Disposition: form-data; name=\"nozzleNo\"\r\n\r\n";
//   formData += nozzleNo + "\r\n";
//   formData += "--" + formDataBoundary + "\r\n";
//   formData += "Content-Disposition: form-data; name=\"vehicleType\"\r\n\r\n";
//   formData += vehicleType + "\r\n";
//   formData += "--" + formDataBoundary + "\r\n";
//   formData += "Content-Disposition: form-data; name=\"carNo\"\r\n\r\n";
//   formData += carNo + "\r\n";
//   formData += "--" + formDataBoundary + "\r\n";
//   formData += "Content-Disposition: form-data; name=\"cashType\"\r\n\r\n";
//   formData += cashType + "\r\n";
//   formData += "--" + formDataBoundary + "\r\n";
//   formData += "Content-Disposition: form-data; name=\"fuelType\"\r\n\r\n";
//   formData += fuelType + "\r\n";
//   formData += "--" + formDataBoundary + "\r\n";
//   formData += "Content-Disposition: form-data; name=\"device\"\r\n\r\n";
//   formData += device + "\r\n";
//   formData += "--" + formDataBoundary + "--\r\n";

//   http.addHeader("Content-Type", "multipart/form-data; boundary=" + formDataBoundary);

//   int httpResponseCode = http.POST(formData);

//   if (httpResponseCode > 0) {
//     Serial.print("HTTP Response code: ");
//     Serial.println(httpResponseCode);

//     String response = http.getString();
//     Serial.println(response);

//     DynamicJsonDocument doc(2048);

//     DeserializationError error = deserializeJson(doc, response);

//     if (error) {
//       Serial.print("Error parsing JSON: ");
//       Serial.println(error.c_str());
//       return;
//     }

//     rfid_condition = doc["con"];
//     String Server_reply = doc["msg"];

//     // Serial.print("server condi  is => ");
//     // Serial.println(rfid_condition);

//     Serial.print("server reply from start is => ");
//     Serial.println(Server_reply);

//     if (rfid_condition) {
//       char my_chr[30];
//       Server_reply.toCharArray(my_chr, Server_reply.length() + 1);

//       Serial2.write(Show_server_reply, 6);
//       Serial2.write(Show_server_data, 20);

//       Serial2.write(Show_infor, 6);
//       Serial2.write(my_chr, 30);

//     } else {

//       Serial2.write(Show_server_Error, 21);
//       Serial2.write(Show_server_Error, 21);
//     }

//   } else {
//     Serial.print("HTTP POST request failed, error: ");
//     Serial.println(httpResponseCode);
//   }

//   http.end();
//   Serial.println("http post is complete");
//   accessToken_two = "";
// }


// void server_post_preset(String check_data, int main_data) {

//   String nozzleNo = "";
//   if (common_pump_Id_two < 10) nozzleNo = "0" + String(common_pump_Id_two);
//   else nozzleNo = String(common_pump_Id_two);

//   String Dev_ID = String(devicenum);
//   String serverAddress = "http://192.168.0.100:9000/api/detail-sale/preset?depNo=" + Dev_ID + "&nozzleNo=" + nozzleNo;

//   String carNo;
//   if (strlen(Car_no_two) == 0) carNo = "-";
//   else carNo = String(Car_no_two);

//   String member_id;
//   if (strlen(Member_id_two) == 0) member_id = "-";
//   else member_id = String(Member_id_two);

//   String member_name;
//   if (strlen(Member_name_two) == 0) member_name = "-";
//   else member_name = String(Member_name_two);

//   String price_dd = "";
//   String liter_data = "";
//   if (check_data == "price") {
//     price_dd = String(main_data);

//   } else if (check_data == "liter") {
//     float liter_dd = main_data + 0.0;
//     liter_data = String(liter_dd);
//   }

//   String fuelType = Nozzle_name;
//   String salePrice = String(common_dispenser_price_two);
//   String vehicleType = vehicle_name;
//   String cashType = cash_name_two;
//   String device = "HMI";

//   Serial.print("Server address is => ");
//   Serial.println(serverAddress);

//   Serial.print("price or liter => ");
//   Serial.println(check_data);

//   Serial.print("float vlaue is => ");
//   Serial.println(liter_data);

//   Serial.print("Price vlaue is => ");
//   Serial.println(price_dd);

//   Serial.print("sale price => ");
//   Serial.println(salePrice);

//   HTTPClient http;

//   http.begin(serverAddress);

//   // Set authorization header with the token
//   http.addHeader("Authorization", "Bearer " + String(accessToken_two));

//   // Create the multipart/form-data payload
//   String formDataBoundary = "----WebKitFormBoundary";
//   String formData = "--" + formDataBoundary + "\r\n";
//   formData += "Content-Disposition: form-data; name=\"nozzleNo\"\r\n\r\n";
//   formData += nozzleNo + "\r\n";
//   formData += "--" + formDataBoundary + "\r\n";
//   formData += "Content-Disposition: form-data; name=\"vehicleType\"\r\n\r\n";
//   formData += vehicleType + "\r\n";
//   formData += "--" + formDataBoundary + "\r\n";
//   formData += "Content-Disposition: form-data; name=\"salePrice\"\r\n\r\n";
//   formData += salePrice + "\r\n";
//   formData += "--" + formDataBoundary + "\r\n";
//   formData += "Content-Disposition: form-data; name=\"carNo\"\r\n\r\n";
//   formData += carNo + "\r\n";
//   formData += "--" + formDataBoundary + "\r\n";
//   formData += "Content-Disposition: form-data; name=\"cashType\"\r\n\r\n";
//   formData += cashType + "\r\n";
//   formData += "--" + formDataBoundary + "\r\n";
//   formData += "Content-Disposition: form-data; name=\"fuelType\"\r\n\r\n";
//   formData += fuelType + "\r\n";
//   formData += "--" + formDataBoundary + "\r\n";
//   if (check_data == "price") {
//     formData += "Content-Disposition: form-data; name=\"kyat\"\r\n\r\n";
//     formData += price_dd + "\r\n";
//     formData += "--" + formDataBoundary + "\r\n";
//   } else if (check_data == "liter") {
//     formData += "Content-Disposition: form-data; name=\"liter\"\r\n\r\n";
//     formData += liter_data + "\r\n";
//     formData += "--" + formDataBoundary + "\r\n";
//   }
//   formData += "Content-Disposition: form-data; name=\"device\"\r\n\r\n";
//   formData += device + "\r\n";
//   formData += "--" + formDataBoundary + "--\r\n";

//   // Set content type for form data
//   http.addHeader("Content-Type", "multipart/form-data; boundary=" + formDataBoundary);

//   int httpResponseCode = http.POST(formData);

//   // Check for a successful response
//   if (httpResponseCode > 0) {
//     Serial.print("HTTP Response code: ");
//     Serial.println(httpResponseCode);

//     // Print the response payload
//     String response = http.getString();
//     Serial.println(response);

//     DynamicJsonDocument doc(2048);

//     DeserializationError error = deserializeJson(doc, response);

//     if (error) {
//       Serial.print("Error parsing JSON: ");
//       Serial.println(error.c_str());
//       return;
//     }

//     rfid_condition = doc["con"];
//     String Server_reply = doc["msg"];

//     Serial.print("server condi  is => ");
//     Serial.println(rfid_condition);

//     Serial.print("server reply is => ");
//     Serial.println(Server_reply);

//     if (rfid_condition) {
//       char my_chr[30];
//       Server_reply.toCharArray(my_chr, Server_reply.length() + 1);

//       Serial2.write(Show_server_reply, 6);
//       Serial2.write(Show_server_data, 20);

//       Serial2.write(Show_infor, 6);
//       Serial2.write(my_chr, 30);

//     } else {

//       Serial2.write(Show_server_Error, 21);
//       Serial2.write(Show_server_Error, 21);
//     }

//   } else {
//     Serial.print("HTTP POST request failed, error: ");
//     Serial.println(httpResponseCode);
//   }

//   // Close the connection
//   http.end();
//   Serial.println("http post is complete");
//   accessToken_two = "";
// }


void callback(char* topic, byte* payload, unsigned int length) {
  incommingMessage = "";

  for (int j = 0; j < length; j++) incommingMessage += (char)payload[j];

  // Serial.println();
  // Serial.println("Message arrived [" + String(topic) + "]" + incommingMessage);

  if (String(topic) == String(pumpapprobuf)) {  // pump req appro
    // rxledonoff();
    char pumpapproArray[13];
    incommingMessage.toCharArray(pumpapproArray, incommingMessage.length() + 1);
    Serial.print("Appro  is ");

    Serial.println(pumpapproArray);

    // Serial.println(pumpapproArray[1]);

    charArray[0] = pumpapproArray[0];
    charArray[1] = pumpapproArray[1];
  }

  if (String(topic) == String(pumppresetbuf)) {  // preset change
    // rxledonoff(); // red
    incommingMessage.toCharArray(presetmqttary, incommingMessage.length() + 1);
    Serial.print("preset is ");
    Serial.println(presetmqttary);

    // Serial.print(presetmqttary[0], HEX);
    // Serial.print(presetmqttary[1], HEX);

    charArray[0] = presetmqttary[0];
    charArray[1] = presetmqttary[1];

    pumpmqttnumchange();

    charArray[3] = presetmqttary[2];


    // if (presetcount) {

    //   preset_check = true;
    //   lastpresetsend = true;


    //   presetcount = false;
    // }
  }


  if (String(topic) == device_Id_topic) {
    DynamicJsonDocument doc(4096);  // Adjust the size based on your JSON data size
    DeserializationError error = deserializeJson(doc, incommingMessage);

    if (error) {
      Serial.print(F("JSON parsing failed: "));
      Serial.println(error.c_str());
      return;
    }

    int pumpid1 = 0;
    int pumpid2 = 0;
    int pumpid3 = 0;
    int pumpid4 = 0;
    int pumpid5 = 0;
    int pumpid6 = 0;
    int pumpid7 = 0;
    int pumpid8 = 0;

    devicenum = doc["devicenum"].as<const int>();
    nozzlenum = doc["nozzlenum"].as<const int>();
    pumpid1 = doc["pumpid1"].as<const int>();
    pumpid2 = doc["pumpid2"].as<const int>();
    pumpid3 = doc["pumpid3"].as<const int>();
    pumpid4 = doc["pumpid4"].as<const int>();
    pumpid5 = doc["pumpid5"].as<const int>();
    pumpid6 = doc["pumpid6"].as<const int>();
    pumpid7 = doc["pumpid7"].as<const int>();
    pumpid8 = doc["pumpid8"].as<const int>();

    Serial.println(devicenum);
    Serial.println(nozzlenum);
    Serial.println(pumpid1);
    Serial.println(pumpid2);
    Serial.println(pumpid3);
    Serial.println(pumpid4);
    Serial.println(pumpid5);
    Serial.println(pumpid6);
    Serial.println(pumpid7);
    Serial.println(pumpid8);

    EEPROM.write(101, pumpid1);
    EEPROM.commit();
    EEPROM.write(102, pumpid2);
    EEPROM.commit();
    EEPROM.write(103, pumpid3);
    EEPROM.commit();
    EEPROM.write(104, pumpid4);
    EEPROM.commit();
    EEPROM.write(105, pumpid5);
    EEPROM.commit();
    EEPROM.write(106, pumpid6);
    EEPROM.commit();
    EEPROM.write(107, pumpid7);
    EEPROM.commit();
    EEPROM.write(108, pumpid8);
    EEPROM.commit();
    EEPROM.write(109, devicenum);
    EEPROM.commit();
    EEPROM.write(110, nozzlenum);
    EEPROM.commit();
    Serial.println("yep all save");
  }

  // if (String(topic) == String(server_rpy)) {
  //   incommingMessage.toCharArray(server_rpy_ary, incommingMessage.length() + 1);
  //   // Serial.println("hey i am working");
  //   char temp_rp[4];

  //   for (int i = 0; i < 5; i++) {
  //     temp_rp[i] = server_rpy_ary[3 + i];
  //   }

  //   final_str = "";
  //   final_str = String(temp_rp);
  // }

  // if (String(topic) == String(Reset_topic)) {  // esp reset topic
  //   char resetid[3];
  //   incommingMessage.toCharArray(resetid, incommingMessage.length() + 1);
  //   Serial.println(resetid[1]);
  //   // delay(4000);
  //   if (atoi(resetid) == devicenum) {
  //     Serial.println("yep");
  //     // ESP.restart();
  //   }
  // }
}


void pumpmqttnumchange() {  //change incomming pumpid(0 - 32) from Mqtt to Device pumpid(0 - 2)

  if (charArray[0] == 0x30 && charArray[1] == 0x31) {
    pumpmqttnum = 1;
  } else if (charArray[0] == 0x30 && charArray[1] == 0x32) {
    pumpmqttnum = 2;
  } else if (charArray[0] == 0x30 && charArray[1] == 0x33) {
    pumpmqttnum = 3;
  } else if (charArray[0] == 0x30 && charArray[1] == 0x34) {
    pumpmqttnum = 4;
  } else if (charArray[0] == 0x30 && charArray[1] == 0x35) {
    pumpmqttnum = 5;
  } else if (charArray[0] == 0x30 && charArray[1] == 0x36) {
    pumpmqttnum = 6;
  } else if (charArray[0] == 0x30 && charArray[1] == 0x37) {
    pumpmqttnum = 7;
  } else if (charArray[0] == 0x30 && charArray[1] == 0x38) {
    pumpmqttnum = 8;
  } else if (charArray[0] == 0x30 && charArray[1] == 0x39) {
    pumpmqttnum = 9;
  } else if (charArray[0] == 0x31 && charArray[1] == 0x30) {
    pumpmqttnum = 10;
  } else if (charArray[0] == 0x31 && charArray[1] == 0x31) {
    pumpmqttnum = 11;
  } else if (charArray[0] == 0x31 && charArray[1] == 0x32) {
    pumpmqttnum = 12;
  } else if (charArray[0] == 0x31 && charArray[1] == 0x33) {
    pumpmqttnum = 13;
  } else if (charArray[0] == 0x31 && charArray[1] == 0x34) {
    pumpmqttnum = 14;
  } else if (charArray[0] == 0x31 && charArray[1] == 0x35) {
    pumpmqttnum = 15;
  } else if (charArray[0] == 0x31 && charArray[1] == 0x36) {
    pumpmqttnum = 16;
  } else if (charArray[0] == 0x31 && charArray[1] == 0x37) {
    pumpmqttnum = 17;
  } else if (charArray[0] == 0x31 && charArray[1] == 0x38) {
    pumpmqttnum = 18;
  } else if (charArray[0] == 0x31 && charArray[1] == 0x39) {
    pumpmqttnum = 19;
  } else if (charArray[0] == 0x32 && charArray[1] == 0x30) {
    pumpmqttnum = 20;
  } else if (charArray[0] == 0x32 && charArray[1] == 0x31) {
    pumpmqttnum = 21;
  } else if (charArray[0] == 0x32 && charArray[1] == 0x32) {
    pumpmqttnum = 22;
  } else if (charArray[0] == 0x32 && charArray[1] == 0x33) {
    pumpmqttnum = 23;
  } else if (charArray[0] == 0x32 && charArray[1] == 0x34) {
    pumpmqttnum = 24;
  } else if (charArray[0] == 0x32 && charArray[1] == 0x35) {
    pumpmqttnum = 25;
  } else if (charArray[0] == 0x32 && charArray[1] == 0x36) {
    pumpmqttnum = 26;
  } else if (charArray[0] == 0x32 && charArray[1] == 0x37) {
    pumpmqttnum = 27;
  } else if (charArray[0] == 0x32 && charArray[1] == 0x38) {
    pumpmqttnum = 28;
  } else if (charArray[0] == 0x32 && charArray[1] == 0x39) {
    pumpmqttnum = 29;
  } else if (charArray[0] == 0x33 && charArray[1] == 0x30) {
    pumpmqttnum = 30;
  } else if (charArray[0] == 0x33 && charArray[1] == 0x31) {
    pumpmqttnum = 31;
  } else if (charArray[0] == 0x33 && charArray[1] == 0x32) {
    pumpmqttnum = 32;
  }

  charArray[2] = 0x00;

  Serial.printf("pump mqtt number is %d\n", pumpmqttnum);

  if (pumpid1 == pumpmqttnum) {

    presetcount = true;

  } else if (pumpid2 == pumpmqttnum) {
    presetcount = true;
  }
  //    else if (pumpid3 == pumpmqttnum) {
  //     charArray[3] = 0x42;

  //   } else if (pumpid4 == pumpmqttnum) {
  //     charArray[3] = 0x30;

  //   } else if (pumpid5 == pumpmqttnum) {
  //     charArray[3] = 0x30;

  //   } else if (pumpid6 == pumpmqttnum) {
  //     charArray[3] = 0x30;

  //   } else if (pumpid7 == pumpmqttnum) {
  //     charArray[3] = 0x30;

  //   } else if (pumpid8 == pumpmqttnum) {
  //     charArray[3] = 0x30;
  //   }
}

void pumpidchange() {

  if (Buffer[0] == 0x91 || id_1) mqttpumpidchange(pumpid1);
  else if (Buffer[0] == 0x92 || id_2) mqttpumpidchange(pumpid2);
}

void mqttpumpidchange(int pumpid) {


  // Serial.print("PUmp id is => ");
  // Serial.println(pumpid);

  if (pumpid == 1) {
    mqttdatabuf[0] = 0x30;
    mqttdatabuf[1] = 0x31;
  } else if (pumpid == 2) {
    mqttdatabuf[0] = 0x30;
    mqttdatabuf[1] = 0x32;
  } else if (pumpid == 3) {
    mqttdatabuf[0] = 0x30;
    mqttdatabuf[1] = 0x33;
  } else if (pumpid == 4) {
    mqttdatabuf[0] = 0x30;
    mqttdatabuf[1] = 0x34;
  } else if (pumpid == 5) {
    mqttdatabuf[0] = 0x30;
    mqttdatabuf[1] = 0x35;
  } else if (pumpid == 6) {
    mqttdatabuf[0] = 0x30;
    mqttdatabuf[1] = 0x36;
  } else if (pumpid == 7) {
    mqttdatabuf[0] = 0x30;
    mqttdatabuf[1] = 0x37;
  } else if (pumpid == 8) {
    mqttdatabuf[0] = 0x30;
    mqttdatabuf[1] = 0x38;
  } else if (pumpid == 9) {
    mqttdatabuf[0] = 0x30;
    mqttdatabuf[1] = 0x39;
  } else if (pumpid == 10) {
    mqttdatabuf[0] = 0x31;
    mqttdatabuf[1] = 0x30;
  } else if (pumpid == 11) {
    mqttdatabuf[0] = 0x31;
    mqttdatabuf[1] = 0x31;
  } else if (pumpid == 12) {
    mqttdatabuf[0] = 0x31;
    mqttdatabuf[1] = 0x32;
  } else if (pumpid == 13) {
    mqttdatabuf[0] = 0x31;
    mqttdatabuf[1] = 0x33;
  } else if (pumpid == 14) {
    mqttdatabuf[0] = 0x31;
    mqttdatabuf[1] = 0x34;
  } else if (pumpid == 15) {
    mqttdatabuf[0] = 0x31;
    mqttdatabuf[1] = 0x35;
  } else if (pumpid == 16) {
    mqttdatabuf[0] = 0x31;
    mqttdatabuf[1] = 0x36;
  } else if (pumpid == 17) {
    mqttdatabuf[0] = 0x31;
    mqttdatabuf[1] = 0x37;
  } else if (pumpid == 18) {
    mqttdatabuf[0] = 0x31;
    mqttdatabuf[1] = 0x38;
  } else if (pumpid == 19) {
    mqttdatabuf[0] = 0x31;
    mqttdatabuf[1] = 0x39;
  } else if (pumpid == 20) {
    mqttdatabuf[0] = 0x32;
    mqttdatabuf[1] = 0x30;
  } else if (pumpid == 21) {
    mqttdatabuf[0] = 0x32;
    mqttdatabuf[1] = 0x31;
  } else if (pumpid == 22) {
    mqttdatabuf[0] = 0x32;
    mqttdatabuf[1] = 0x32;
  } else if (pumpid == 23) {
    mqttdatabuf[0] = 0x32;
    mqttdatabuf[1] = 0x33;
  } else if (pumpid == 24) {
    mqttdatabuf[0] = 0x32;
    mqttdatabuf[1] = 0x34;
  } else if (pumpid == 25) {
    mqttdatabuf[0] = 0x32;
    mqttdatabuf[1] = 0x35;
  } else if (pumpid == 26) {
    mqttdatabuf[0] = 0x32;
    mqttdatabuf[1] = 0x36;
  } else if (pumpid == 27) {
    mqttdatabuf[0] = 0x32;
    mqttdatabuf[1] = 0x37;
  } else if (pumpid == 28) {
    mqttdatabuf[0] = 0x32;
    mqttdatabuf[1] = 0x38;
  } else if (pumpid == 29) {
    mqttdatabuf[0] = 0x32;
    mqttdatabuf[1] = 0x39;
  } else if (pumpid == 33) {
    mqttdatabuf[0] = 0x33;
    mqttdatabuf[1] = 0x30;
  } else if (pumpid == 31) {
    mqttdatabuf[0] = 0x33;
    mqttdatabuf[1] = 0x31;
  } else if (pumpid = 32) {
    mqttdatabuf[0] = 0x33;
    mqttdatabuf[1] = 0x32;
  }
}


void initEEPROMdatashow() {
  // for (int j = 0; j < 50; j++) {
  //   ssidBuf[j] = EEPROM.read(j);
  // }
  // for (int j = 50; j < 100; j++) {
  //   passBuf[j - 50] = EEPROM.read(j);
  // }
  // Serial.print(ssidBuf);
  // Serial.print("  ");
  // Serial.println(passBuf);

  pumpid1 = EEPROM.read(101);
  pumpid2 = EEPROM.read(102);
  pumpid3 = EEPROM.read(103);
  pumpid4 = EEPROM.read(104);
  pumpid5 = EEPROM.read(105);
  pumpid6 = EEPROM.read(106);
  pumpid7 = EEPROM.read(107);
  pumpid8 = EEPROM.read(108);

  devicenum = EEPROM.read(109);
  nozzlenum = EEPROM.read(110);

  Serial.print(devicenum);
  Serial.print("  ");
  Serial.print(nozzlenum);
  Serial.print("  ");
  Serial.print(pumpid1);
  Serial.print("  ");
  Serial.print(pumpid2);
  Serial.print("  ");
  Serial.print(pumpid3);
  Serial.print("  ");
  Serial.print(pumpid4);
  Serial.print("  ");
  Serial.print(pumpid5);
  Serial.print("  ");
  Serial.print(pumpid6);
  Serial.print("  ");
  Serial.print(pumpid7);
  Serial.print("  ");
  Serial.print(pumpid8);
  Serial.println("  ");
}


void EEPROMinit() {

  if (devicenum == 1) {
    //pumpreqbuf

    pumpreqbuf[21] = '1';
    pumpapprobuf[20] = '1';
    ppfinal[20] = '1';
    pplive[23] = '1';
    devicebuf[0] = '1';
    activebuf[21] = '1';
    pricereqbuf[23] = '1';
    myreload[27] = '1';
    pumpfinalreloadbuf[21] = '1';
    // rdybuf[20] = '1';
    device_Id_topic[38] = '1';
  } else if (devicenum == 2) {
    pumpreqbuf[21] = '2';
    pumpapprobuf[20] = '2';
    ppfinal[20] = '2';
    pplive[23] = '2';
    devicebuf[0] = '2';
    activebuf[21] = '2';
    pricereqbuf[23] = '2';
    myreload[27] = '2';
    pumpfinalreloadbuf[21] = '2';
    // rdybuf[20] = '2';
    device_Id_topic[38] = '2';
  } else if (devicenum == 3) {
    pumpreqbuf[21] = '3';
    pumpapprobuf[20] = '3';
    ppfinal[20] = '3';
    pplive[23] = '3';
    devicebuf[0] = '3';
    activebuf[21] = '3';
    pricereqbuf[23] = '3';
    myreload[27] = '3';
    pumpfinalreloadbuf[21] = '3';
    // rdybuf[20] = '3';
    device_Id_topic[38] = '3';
  } else if (devicenum == 4) {
    pumpreqbuf[21] = '4';
    pumpapprobuf[20] = '4';
    ppfinal[20] = '4';
    pplive[23] = '4';
    devicebuf[0] = '4';
    activebuf[21] = '4';
    pricereqbuf[23] = '4';
    myreload[27] = '4';
    pumpfinalreloadbuf[21] = '4';
    // rdybuf[20] = '4';
    device_Id_topic[38] = '4';
  } else if (devicenum == 5) {
    pumpreqbuf[21] = '5';
    pumpapprobuf[20] = '5';
    ppfinal[20] = '5';
    pplive[23] = '5';
    devicebuf[0] = '5';
    activebuf[21] = '5';
    pricereqbuf[23] = '5';
    myreload[27] = '5';
    pumpfinalreloadbuf[21] = '5';
    // rdybuf[20] = '5';
    device_Id_topic[38] = '5';
  } else if (devicenum == 6) {
    pumpreqbuf[21] = '6';
    pumpapprobuf[20] = '6';
    ppfinal[20] = '6';
    pplive[23] = '6';
    devicebuf[0] = '6';
    activebuf[21] = '6';
    pricereqbuf[23] = '6';
    myreload[27] = '6';
    pumpfinalreloadbuf[21] = '6';
    // rdybuf[20] = '6';
    device_Id_topic[38] = '6';
  } else if (devicenum == 7) {
    pumpreqbuf[21] = '7';
    pumpapprobuf[20] = '7';
    ppfinal[20] = '7';
    pplive[23] = '7';
    devicebuf[0] = '7';
    activebuf[21] = '7';
    pricereqbuf[23] = '7';
    myreload[27] = '7';
    pumpfinalreloadbuf[21] = '7';
    // rdybuf[20] = '7';
    device_Id_topic[38] = '7';
  } else if (devicenum == 8) {
    pumpreqbuf[21] = '8';
    pumpapprobuf[20] = '8';
    ppfinal[20] = '8';
    pplive[23] = '8';
    devicebuf[0] = '8';
    activebuf[21] = '8';
    pricereqbuf[23] = '8';
    myreload[27] = '8';
    pumpfinalreloadbuf[21] = '8';
    // rdybuf[20] = '8';
    device_Id_topic[38] = '8';
  }
}


void initWiFi() {
  // for (int i = 0; i < 50; i++) {
  //   ssidtemp[i] = EEPROM.read(i);
  //   // Serial.print(getDataEEPROM(i));
  // }
  // for (int i = 50; i < 100; i++) {
  //   passtemp[i - 50] = EEPROM.read(i);
  //   // Serial.print(getDataEEPROM(i));
  // }

  WiFi.hostname("myesp");

  // WiFi.begin("POS_Server", "asdffdsa");

  WiFi.begin("akmh", "11112222");

  Serial.print("Connecting to WiFi ..");
  int wifitrytime = 0;
  while (WiFi.status() != WL_CONNECTED && wifitrytime != 20) {

    Serial.print('.');
    digitalWrite(wifiled, HIGH);
    delay(500);
    digitalWrite(wifiled, LOW);
    delay(500);
    wifitrytime++;
  }

  if (WiFi.status() == WL_CONNECTED) {
    Serial.print("You are connected in ");
    Serial.println(WiFi.localIP());
    // digitalWrite(wifiled, HIGH);
  }
}


void reconnect() {
  if (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    String clientId = "ESP8266Client-";  // Create a random client ID
    clientId += String(random(0xffff), HEX);
    // Attempt to connect
    if (client.connect(clientId.c_str(), mqtt_username, mqtt_password)) {
      Serial.println("connected");

      client.subscribe("Connect");                    // subscribe the topics here
      client.publish("connect", "Device1Connected");  // subscribe the topics here
      client.subscribe("detpos/#");
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");  // Wait 5 seconds before retrying
      delay(1000);
    }
  }
}
