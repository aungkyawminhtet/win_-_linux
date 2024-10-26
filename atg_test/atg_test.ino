
char tank_1[8] = { 0x01, 0x69, 0x32, 0x30, 0x31, 0x30, 0x31, 0x03 };
char tank_2[8] = { 0x01, 0x69, 0x32, 0x30, 0x31, 0x30, 0x32, 0x03 };
char tank_3[8] = { 0x01, 0x69, 0x32, 0x30, 0x31, 0x30, 0x33, 0x03 };
char tank_4[8] = { 0x01, 0x69, 0x32, 0x30, 0x31, 0x30, 0x34, 0x03 };

char buffer[100];

char tank_product[8];
char tank_volume[8];
char temperature[8];
char water_level[8];


int state_count;
int count;

void setup() {
  Serial.begin(115200);
  Serial2.begin(9600, SERIAL_8N1, 16, 17);
  Serial2.write(tank_1, 8);
  // Serial2.write(tank_4, 8);
  // Serial.println("Done");
}

void loop() {

  if (Serial2.available()) {

    buffer[count] = Serial2.read();
    delay(5);

    Serial.print(count);
    Serial.print("/");
    Serial.print(buffer[count], HEX);
    Serial.print(" ");
    count++;

    if (buffer[count - 7] == 0x26 && buffer[count - 6] == 0x26 && buffer[count - 1] == 0x03) {

      for (int i = 0; i < 8; i++) {

        tank_volume[i] = buffer[26 + i];
        tank_product[i] = buffer[34 + i];
      
        // water_level[i] = buffer[58 + i];
        // temperature[i] = buffer[66 + i];
      }

      Serial.println();

      // float tank_volume_data = IEEE_converter(tank_volume); 
      float tank_product_data = IEEE_converter(tank_product);
      // float water_height = IEEE_converter(water_level);
      // float atg_temp = IEEE_converter(temperature);



      // float tank_volume_data = IEEE_converter(tank_volume);
      // float tank_product_data = IEEE_converter(tank_product);
      // float water_height = IEEE_converter(water_level);
      // float atg_temp = IEEE_converter(temperature);

      // IEEE_converter(water_level);
      // IEEE_converter(temperature);


      // Serial.print("ATG tank volume is => ");
      // Serial.println(tank_volume_data, 4);
      Serial.print("ATG tank product is => ");
      Serial.println(tank_product_data, 4);
      // Serial.print("ATG water height is => ");
      // Serial.println(water_height, 4);
      // Serial.print("ATG Temperatur is => ");
      // Serial.println(atg_temp, 4);

      if (buffer[17] == 0x30 && buffer[18] == 0x34) {
        Serial.println("this is tank 4");
        count = 0;
        // state_count++;
        // state_fun();
      }
    }

    // if (buffer[17] == 0x30 && buffer[18] == 0x34) {
    //   Serial.println("this is tank 4");
    // }

    // for (int i ; i < 8 ; i++{
    //   temperature[i] =
    // });
  }
}


void state_fun() {
  if (state_count == 1) Serial2.write(tank_1, 8);
  else if (state_count == 2) Serial2.write(tank_2, 8);
  else if (state_count == 3) Serial2.write(tank_2, 8);
  else if (state_count == 4) Serial2.write(tank_2, 8);

  if (state_count > 4) state_count = 0;
}

float IEEE_converter(char *data_arr) {

  // uint32_t hexData_ascii = ((uint32_t)data_arr[0] << 24) |
  //                          ((uint32_t)data_arr[1] << 16) |
  //                          ((uint32_t)data_arr[2] << 8) |
  //                          (uint32_t)data_arr[3];
  
  // // Typecast to floa
  // float *floatPtr = (float *)&hexData_ascii;
  // float floatValue = *floatPtr;


  // return floatValue;

  uint32_t hexData_ascii = (uint32_t)strtol(data_arr, NULL, 16);

  float *floatPtr = (float *)&hexData_ascii;
  float floatValue = *floatPtr;

  Serial.print("Converted float value: ");
  Serial.println(floatValue, 4);

  return floatValue;
}



// float f_function(uint32_t hexData) {

//   float *floatPtr = (float *)&hexData;
//   float floatValue = *floatPtr;

//   // Serial.print("Converted float value: ");
//   // Serial.println(floatValue, 4);
//   return floatValue;
// }


/* 
// for(int i = 0 ; i < 50 ; i++) {
  //   buffer[i] = Serial2.read();
  //   delay(5);
  //   Serial.print(i);
  //   Serial.print("/");
  //   Serial.print(buffer[i], HEX);
  //   Serial.print(" ");
  // }
  // Serial.println();


/////////////////

  // uint32_t hexData = 0x42B7B333;   // temp data
  // uint32_t hexData = (uint32_t)strtol(dd, NULL, 16);
  // float dd = f_function(hexData);
  // Serial.print("Converted float dd value: ");
  // Serial.println(dd, 6);

  // delay(5);
*/
