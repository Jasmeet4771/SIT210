System Description:
My system is designed to collect temperature and light data from sensors and send it to the web. The DHT22 sensor measures the temperature, while the BH1750 sensor measures the light level in lux. The Arduino Nano 33 IoT connects to Wi-Fi and sends the sensor readings to ThingSpeak every 30 seconds. The data can then be viewed online using the ThingSpeak channel.
Components Used:
1. Arduino Nano 33 IoT
2. DHT22 temperature and humidity sensor
3. BH1750 light sensor
4. Jumper wires
5. Breadboard
6. Wi-Fi connection
7. ThingSpeak
Code Description:
The code first sets up the DHT22 and BH1750 sensors and connects the Arduino to Wi-Fi. The readTemperature() function gets the temperature from the DHT22, while the readLight() function gets the light level from the BH1750. The sendData() function sends both readings to ThingSpeak using the channel ID and API key. The system checks the sensor readings and sends the data every 30 seconds. If there is a problem reading the temperature, the code displays an error message in the Serial Monitor.

