### 1 Overview

The control chassi uses an Ardiono Mega 2560 in order to handle the following:
 - Input: Power button to control chassi fan #1
 - Input: Power button to control chassi fan #2
 - Input: Ambient temperature and humidity sensor (measuring outside the box)
 - Input: Ambient temperature and humdity sensor (measuring inside the box)
 - Input: Magnetic door sensor
 - Output: Fan #1 relay activation/deactivation (12V)
 - Output: Fan #2 relay activation/deactivation (12V)
 - Output: Blinking alarm relay activation/deactivation (12V)
 - Output: 5 LED indicators (power, loop iteration, warning, fan #1 activated, fan #2 activated)
 - Output: Serial console towards raspberry pi with status information

The Raspberry controller activates system wide alarm based on door sensor alarm. The
fan temperature warning and alarm is handled with the local indicators only.

Sensor readings are used by the Raspberry controller to make decisions or to add
details to graphs.

### 2 Dependencies

After installing Arduino IDE the following libraries needs to be installed:
 - Temperature and Humidity Sensor: DHT sensor library (v1.4.3 or later)
 - DHT sensor library dependency: Adafruit Unified Sensor (v1.1.5 or later)
