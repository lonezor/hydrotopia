/*
 *  Hydrotopia
 *  Copyright (C) 2022 Johan Norberg <lonezor@gmail.com>
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program. If not, see <http://www.gnu.org/licenses/>
 */

#include <Adafruit_Sensor.h>
#include <DHT.h>
#include <DHT_U.h>


#define FALSE (0)
#define TRUE (!FALSE)

#define PIN_FAN_BUTTON_01 (28)
#define PIN_FAN_BUTTON_02 (29)
#define PIN_RELAY_FAN_01 (30)
#define PIN_RELAY_FAN_02 (31)
#define PIN_RELAY_ALARM (32)
#define PIN_LED_FAN_01 (33)
#define PIN_LED_FAN_02 (34)
#define PIN_LED_WARNING (35)
#define PIN_LED_ITERATION_IND (36)
#define PIN_LED_POWER (37)
#define PIN_DOOR_SENSOR (38)
#define PIN_DHT_01 (39)
#define PIN_DHT_02 (40)

#define TEMP_WARNING_THRESHOLD (30)
#define TEMP_ERROR_THRESHOLD (40)

#define DHT_TYPE (DHT22) // DHT 22 (AM2302)

//----------------------------------------------------------

DHT_Unified dht_01(PIN_DHT_01, DHT_TYPE);
DHT_Unified dht_02(PIN_DHT_02, DHT_TYPE);

int dht_01_temperature = 0;
int dht_02_temperature = 0;
int dht_01_humidity = 0;
int dht_02_humidity = 0;

int g_alarm = FALSE;
int g_door_alarm = FALSE;
int g_chassi_temp_alarm = FALSE;
int g_chassi_temp_warning = FALSE;

int g_iteration_ind_flag = 1;
unsigned long g_iteration_counter = 0;

//----------------------------------------------------------

void enable_relay(int pin)
{
    digitalWrite(pin, LOW);
}

void disable_relay(int pin)
{
    digitalWrite(pin, HIGH);
}

void dht_init()
{
    dht_01.begin();
    dht_02.begin();
}

void setup() {
  /********************************************************
   ******************** GPIO: PULLUP INPUT ****************
   ********************************************************/
  
  pinMode(PIN_FAN_BUTTON_01, INPUT_PULLUP);
  pinMode(PIN_FAN_BUTTON_02, INPUT_PULLUP);
  pinMode(PIN_DOOR_SENSOR, INPUT_PULLUP);

   /********************************************************
   ******************** GPIO: OUTPUTT **********************
   ********************************************************/
 
  pinMode(PIN_RELAY_FAN_01, OUTPUT);
  pinMode(PIN_RELAY_FAN_02, OUTPUT);
  pinMode(PIN_RELAY_ALARM, OUTPUT);
  pinMode(PIN_LED_FAN_01, OUTPUT);
  pinMode(PIN_LED_FAN_02, OUTPUT);
  pinMode(PIN_LED_WARNING, OUTPUT);
  pinMode(PIN_LED_ITERATION_IND, OUTPUT);
  pinMode(PIN_LED_POWER, OUTPUT);

  /********************************************************
   ******************** SERIAL COMMUNICATION **************
   ********************************************************/
  
  Serial.begin(9600);

  /********************************************************
   ******************** STARTUP STATE *********************
   ********************************************************/

  digitalWrite(PIN_LED_POWER, HIGH);

  dht_init();
}

void loop() {

  /********************************************************
   ******************** SENSOR INPUT **********************
   ********************************************************/
   
  int fan_button_01 = digitalRead(PIN_FAN_BUTTON_01);
  
  int fan_button_02 = digitalRead(PIN_FAN_BUTTON_02);
  
  int door_sensor = digitalRead(PIN_DOOR_SENSOR);

  // DHT Temperature #1
  sensors_event_t event;
  dht_01.temperature().getEvent(&event);
  if (isnan(event.temperature)) {
    dht_01_temperature = 0;
  } else {
    dht_01_temperature = (int)(event.temperature * 100);
  }

  // DHT Temperature #2
  dht_02.temperature().getEvent(&event);
  if (isnan(event.temperature)) {
    dht_02_temperature = 0;
  } else {
    dht_02_temperature = (int)(event.temperature * 100);
  }

  // DHT Humidity #1
  dht_01.humidity().getEvent(&event);
  if (isnan(event.relative_humidity)) {
    dht_01_humidity = 0;
  } else {
    dht_01_humidity = (int)(event.relative_humidity  * 100);
  }
  
  // DHT Humidity #2
  dht_02.humidity().getEvent(&event);
  if (isnan(event.relative_humidity)) {
    dht_02_humidity = 0;
  } else {
    dht_02_humidity = (int)(event.relative_humidity  * 100);
  }

  /********************************************************
   ******************** REACT TO INPUT ********************
   ********************************************************/
  
  if (fan_button_01 == LOW) {
      enable_relay(PIN_RELAY_FAN_01);
      digitalWrite(PIN_LED_FAN_01, HIGH);
  } else {
      disable_relay(PIN_RELAY_FAN_01);
      digitalWrite(PIN_LED_FAN_01, LOW);
  }

  if (fan_button_02 == LOW) {
      enable_relay(PIN_RELAY_FAN_02);
      digitalWrite(PIN_LED_FAN_02, HIGH);
  } else {
      disable_relay(PIN_RELAY_FAN_02);
      digitalWrite(PIN_LED_FAN_02, LOW);
  }

  // Door open: Open circuit --> pullup
  if (door_sensor == HIGH) {
    g_door_alarm = TRUE;
  } else {
    g_door_alarm = FALSE;
  }

  // Chassi temperature error condition
  if (dht_02_temperature / 100 >= TEMP_ERROR_THRESHOLD) {
    g_chassi_temp_alarm = TRUE;
  } else {
    g_chassi_temp_alarm = FALSE;
  }

  // Chassi temperature warning condition
  if (dht_02_temperature / 100 >= TEMP_WARNING_THRESHOLD) {
    g_chassi_temp_warning = TRUE;
  } else {
    g_chassi_temp_warning = FALSE;
  }
  
  if (g_door_alarm || g_chassi_temp_alarm) {
      enable_relay(PIN_RELAY_ALARM);
      g_alarm = TRUE;
  } else {
      disable_relay(PIN_RELAY_ALARM);
      g_alarm = FALSE;
  }

  if (g_chassi_temp_warning && !g_alarm) {
       digitalWrite(PIN_LED_WARNING, HIGH);
  } else {
      digitalWrite(PIN_LED_WARNING, LOW);
  }
 
   /********************************************************
   ************** COMMUNICATE STATUS ***********************
   ********************************************************/

  if (g_iteration_counter % 10 == 0) {
      g_iteration_ind_flag = !g_iteration_ind_flag;
      if (g_iteration_ind_flag) {
        digitalWrite(PIN_LED_ITERATION_IND, HIGH);   
      } else {
        digitalWrite(PIN_LED_ITERATION_IND, LOW);   
      }


    char status[1024];
    snprintf(status, sizeof(status), "temp=%d.%d, temp2=%d.%d, humidity=%d.%d, humidity2=%d.%d, door_alarm=%d, chassi_temp_warning=%d, chassi_temp_warning=%d",
      dht_01_temperature / 100, dht_01_temperature % 100,
      dht_02_temperature / 100, dht_02_temperature % 100,
      dht_01_humidity / 100, dht_01_humidity % 100,
      dht_02_humidity / 100, dht_02_humidity % 100,
      g_door_alarm,
      g_chassi_temp_warning,
      g_chassi_temp_alarm);
    
    Serial.println(status);
  }

   /********************************************************
   ******************** ITERATION CONTROL ********************
   ********************************************************/

   delay(100);  
   g_iteration_counter++;
}
