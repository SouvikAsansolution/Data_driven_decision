# AAU Learning Factory - Data Driven Decision - Sensor page

**[Go back to Front Page](README.md)**

## Flexible sensors to be placed via the ESP32/ESP8266

### Temperature & Humidity sensor
![Temperature and Humidity](https://github.com/glinvad/AAU_Learning_Factory_-_Data_driven_decision/blob/main/Pictures/DHT11%20temperature%20and%20humidit.png)

|             | Range       | Accuracy     |
|-------------|-------------|--------------|
| Humidity    | 20-90% RH   | +/-5%        |
| Temperature | 0-50 &deg;C | +/- 2 &deg;C |

[Datasheet](https://www.mouser.com/datasheet/2/758/DHT11-Technical-Data-Sheet-Translated-Version-1143054.pdf)

### IMU - acceleration X, Y, Z + Gyroscope rX, rY, rZ
![IMU](https://github.com/glinvad/AAU_Learning_Factory_-_Data_driven_decision/blob/main/Pictures/MPU6050%20IMU.png)

|              | Range           | Accuracy |
|--------------|-----------------|----------|
| Gyroscope    | +/- 500 &deg;/s | +/- 0,2% |
| Acceleration | +/- 8 G         | +/- 3%   |

[Datasheet](https://cdn-learn.adafruit.com/downloads/pdf/mpu6050-6-axis-accelerometer-and-gyro.pdf?timestamp=1573074835)

### Sound level sensor
![Sound level](https://github.com/glinvad/AAU_Learning_Factory_-_Data_driven_decision/blob/main/Pictures/Keyes%20KY003738%20sound%20detector.png)

- Digital Out: You can use a potentiometer to configure an extreme value for the sonic. IF the value exceeds the extreme value it will send a signal via digital out.
- Analog Out: Direct microphone signal as voltage value 
- LED1: Shows that the sensor is supplied with voltage 
- LED2: Shows that a magnetic field was detected

Changing the sensitivity of the sensor can be done on the potmeter (blue box with a copper screw). ClockWise will **increase** sensitivity and CounterClockWise will **reduce** sensitivity.

[Datasheet](https://datasheetspdf.com/datasheet/KY-038.html)

### Detection sensor
![Line sensor](https://github.com/glinvad/AAU_Learning_Factory_-_Data_driven_decision/blob/main/Pictures/Keyes%20KY0033%20line%20tracker.png)
A more correct name would be a IR Line tracking sensor. 
The sensor has IR light emitter and detector. The sensor returns the status of the IR light reflected from a surface as ON or OFF. The on board LED shows the status of the sensor. You need to calibrate the sensor first, basically block the sensor and adjust the pot until the led goes on. Then remove the object and the on board LED should go off.

[Datasheet](http://arduinolearning.com/code/ir-line-tracking-sensor-example.php)

### Light level sensor
![Light sensor](https://github.com/glinvad/AAU_Learning_Factory_-_Data_driven_decision/blob/main/Pictures/Pmod%20ALS%20light%20detector.png)

- Simple ambient light sensor
- Convert light to digital date with 8-bit resolution

[Datasheet](https://dk.rs-online.com/web/p/sensor-udvikling/1346463/?cm_mmc=DK-PLA-DS3A-_-google-_-CSS_DK_DK_Raspberry_Pi_%26_Arduino_og_udviklingsv%C3%A6rkt%C3%B8j_Whoop-_-(DK:Whoop!)+Sensor-udvikling-_-1346463&matchtype=&aud-826607888587:pla-333448502146&gclid=Cj0KCQjw5JSLBhCxARIsAHgO2ScXUaACLlXdenm5INfFL5qBJpMpJbLapJkwcfxWVFqWVqXZkAjLlOQaAko4EALw_wcB&gclsrc=aw.ds)

w
## Data labels for the Festo AAU Smart production lab 
Data labels and sensors with on the FESTO Smart production lab. The data is avalible via OPC UA and the "PLC_Data" block in the client demo (*Purple box*)

Each PLC is named **STPLC**## (read the name ot the PLC on the front of the FESTO station):

data avalible from the each PLC is:

- RFID sensor data
- Poximity Sensor data
- State of Pneumaic stopper (O/1)

![Festo Data](https://github.com/glinvad/AAU_Learning_Factory_-_Data_driven_decision/blob/main/Pictures/Labels_festo.png)



![AAU logo](https://github.com/glinvad/AAU_Learning_Factory_-_Data_driven_decision/blob/main/Pictures/AAUlogo.png)

## Wiring Diagrams for ESPs
This section contains the wiring diagrams for each of the ESPs that have been handed out to the groups. The Odd numbered EPSs three sensors attached namely: Light level sensor, Detection sensor and IMU. The even numbered ESPs are equipped with two sensors these are: Temperature & Humidity sensor and Sound level sensor.

### Odd ESP Wiring Diagram
This is the diagram for wiring the odd numbered ESP's sensors to their respective pins.

![Odd ESP Wiring](https://github.com/glinvad/AAU_Learning_Factory_-_Data_driven_decision/blob/main/Pictures/OddESPDiagram.png)

A table with short description of each sensor pin is shown below. The power and ground pins/wires(red & black ) have been omitted from this table.

|Pin number   | Color       | Description     |
|-------------|-------------|-----------------|
| A0          | Purple      | DHT11 sensor pin|
| D0          | Orange      | Detection pin   |
| D1          | Grey        | SCL pin for IMU |
| D2          | Yellow      | SDA pin for IMU |
| D5          | Brown       | SCK pin for light sensor|
| D6          | Blue        | SDO pin for light sensor|
| D8          | Green       | CS trigger pin light sensor|


### Even ESP Wiring Diagram
This is the diagram for wiring the even numbered ESP's sensors to their respective pins.

![Even ESP Wiring](https://github.com/glinvad/AAU_Learning_Factory_-_Data_driven_decision/blob/main/Pictures/EvenESPDiagram.png)

A table with short description of each sensor pin is shown below. The power and ground pins/wires(red & black) have been omitted from this table.

|Pin number   | Color       | Description     |
|-------------|-------------|-----------------|
| A0          | Purple      | DHT11 sensor pin|
| D5          | White       | Analog sound pin|