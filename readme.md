## Tank ##

A project to learn about I2C comms and create a tank capable of travelling over tough terrain operating in manual PC, manual RC or full auto waypointing mode.

The design is centred around separate modules offloading the processing for each part to a separate processor. Most of this is wasteful as the chips are capable of much more, but I wanted to learn about I2C.

The tank has 2 standard RC ESCs with reverse, so require servo signalling which Ardiuno supports easily.

The controller board uses the much more powerful Teensy 3.1 which does all of the heavy lifting, and also powers a small OLED screen showing the current status.

It can be controlled with a normal RC transmitter, or using a radio linked to a PC. GPS coordinates can also be loaded into the tank to allow autonomous operation.

### PCB ###
Each PCB has its own readme in the seperate directories. All PCBs were ordered through [OSHPark](https://oshpark.com/) and they took 1 week to arrive once they're made by the fab house.




