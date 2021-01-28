//


#include <Arduino.h>
#include "eh900_class.h"
#include "eh900_adjust.h"

eh900 level_meter;
Measurement meas_unit(&level_meter);

String command = "";

uint32_t offset_01 = 0;
uint32_t offset_23 = 0;
float_t gain_comp_01 = 0.0;
float_t gain_comp_23 = 0.0;
uint16_t current_set = 750;

//  ダミーセンサの抵抗値を基準にして電圧測定をキャリブレーションする
constexpr float RL = 113.74;  


void setup() {
  // start serial port at 9600 bps and wait for port to open:
  Serial.begin(9600);
  while (!Serial) {
    delay(100); // wait for serial port to connect. Needed for native USB port only
  }
  Serial.println("INIT:--");
  Serial.print("Memory : "); 
  if (level_meter.init()){
    Serial.print(" -- OK ");
  } else {
    Serial.print(" -- Fail..");
  };

  Serial.print("Meas. Unit : "); 
  if (meas_unit.init()){
      Serial.println(" -- OK ");
  } else {
      Serial.println(" -- Fail..");
  };

  level_meter.setAdcErrComp01(1.0);
  level_meter.setAdcErrComp23(1.0);
  level_meter.setAdcOfsComp01(0);
  level_meter.setAdcOfsComp23(0);

}

void loop() {
  // if we get a valid byte, read analog ins:
  if (Serial.available()) {
    // get incoming byte:
    command = Serial.readStringUntil('\n');
    Serial.println(command);
    int inByte = (int)command.c_str()[0];
    // read first analog input:
    switch (inByte){
      case (int)'o':
        Serial.println("Offset measure..");
        if( incomming_command() == 'g'){
          Serial.println("  ....measureing");
          offset_01 = meas_unit.read_voltage(0,true);
          offset_23 = meas_unit.read_voltage(1,true);
          Serial.print("  offset_01: ");Serial.println(offset_01);
          Serial.print("  offset_23: ");Serial.println(offset_23); 
          level_meter.setAdcOfsComp01((uint16_t)offset_01);
          level_meter.setAdcOfsComp23((uint16_t)offset_23);
          Serial.println("  ....fine. ");
        }
      break;
      
      case (int)'g':
        Serial.println("Gain measure..");
        if( incomming_command() == 'g'){
          Serial.println("  ....measureing");
          meas_unit.currentOn();
          delay(1000);
          uint32_t gain_01 = meas_unit.read_voltage(0,false);
          Serial.print("  gain_01[uV]: ");Serial.println(gain_01);
          
          uint32_t gain_23 = meas_unit.read_current((uint16_t)offset_23);
          Serial.print("  gain_23[uA]: ");Serial.println(gain_23);

          uint32_t v_sensor = (uint32_t)round((float_t)gain_23 * RL);
          Serial.print("Vsensor[uV] should be :"); Serial.println(v_sensor);
          
          float_t gain_01_comp = ((float_t)v_sensor/(float_t)gain_01);
          Serial.print("ADC 0-1 comp :"); Serial.println(gain_01_comp, 8);

          level_meter.setAdcErrComp01(gain_01_comp);

          Serial.println("  ....fine. ");
          meas_unit.currentOff();
        }
      break;
      
      case (int)'c':
        Serial.println("Current adjust..");
        adjust_current();
      break;

      case (int)'w':
        Serial.println("Save to Memory");
        level_meter.setTimerPeriod(600);
        level_meter.setSensorLength(20);
        level_meter.setLiquidLevel(0);
        level_meter.setMode(Timer);
     
        Serial.print("AD Error Comp 01: "); Serial.println(level_meter.getAdcErrComp01(),8);
        Serial.print("AD Error Comp 23: "); Serial.println(level_meter.getAdcErrComp23(),8);
        Serial.print("AD OFFSET Comp 01: "); Serial.println(level_meter.getAdcOfsComp01());
        Serial.print("AD OFFSET Comp 23: "); Serial.println(level_meter.getAdcOfsComp23());
        Serial.print("Current Sorce setting: "); Serial.println(level_meter.getCurrentSetting());
        
        Serial.println("");
        Serial.println(".... Storing the parameters.");
        level_meter.storeParameter();
        level_meter.recallParameter();
        Serial.println(".... done.");

        Serial.print("AD Error Comp 01: "); Serial.println(level_meter.getAdcErrComp01(),8);
        Serial.print("AD Error Comp 23: "); Serial.println(level_meter.getAdcErrComp23(),8);
        Serial.print("AD OFFSET Comp 01: "); Serial.println(level_meter.getAdcOfsComp01());
        Serial.print("AD OFFSET Comp 23: "); Serial.println(level_meter.getAdcOfsComp23());
        Serial.print("Current Sorce setting: "); Serial.println(level_meter.getCurrentSetting());
      break;
      
      default:
        Serial.println("no-command");
      break;
      
    }
  }
  delay(100);
}

char incomming_command(void){
  while (!Serial.available()) {
   delay(100);
  }
  // get incoming byte:
  String command = Serial.readStringUntil('\n');
  Serial.println(command);  

  return command.c_str()[0];
}

void adjust_current(void){
  uint16_t current = 750;
  meas_unit.setCurrent(current);
  meas_unit.currentOn();
  meas_unit.read_current((uint16_t)offset_23);

  char command = incomming_command();
  while( command != 'q'){
    if (command == 'U'){
      current = current + 10;
    }
    if (command == 'D'){
      current = current - 10;
    }
    if (command == 'u'){
      current++;
    }
    if (command == 'd'){
      current--;
    }
    meas_unit.setCurrent(current);
    meas_unit.read_current((uint16_t)offset_23);
    command = incomming_command();
  }
  meas_unit.currentOff();
  Serial.print("Current setting :");Serial.println(current);
  current_set = current;
  level_meter.setCurrentSetting(current_set);
  return;
}