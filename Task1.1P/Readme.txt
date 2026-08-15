System Description:
My system is designed to help Linda safely enter her house when it is dark. When she presses the push button, both the porch light and hallway light turn on automatically. The porch light stays on for 30 seconds, while the hallway light stays on for 60 seconds. After the set time, each light turns off automatically.

Components Used:
1.Arduino Nano 33 IoT
2.Push button/switch
3.Porch LED
4.Hallway LED
5.Jumper wires
6.Breadboard
7.Arduino IoT Cloud

Code Description:
The code first sets the pins for the porch and hallway LEDs and makes sure both lights are off when the system starts. The code continuously checks if the push button has been pressed. When it detects a new press, it turns on both lights and starts separate timers for them. I used millis() to control the timers without stopping the rest of the program. After 30 seconds, the porch light turns off, and after 60 seconds, the hallway light turns off. The code also updates the Arduino IoT Cloud variables so the states of the switch and lights can be monitored through the cloud.

