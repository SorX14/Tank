# Communication #
##Overview##
The tank is capable of being controlled via radio (XRF) or RC PWM. Both of these control methods should work seamlessly if one is out of range etc.

The **XRF** method is a 2-way protocol and provides the base station with diagnostic and other information. The XRF control method also allows for GPS waypointing.

##XRF##
This method uses an ARF module which is a standard XRF module (XBEE footprint) but with an amplifier giving it a larger effective range.

The protocol that the base station and tank use allows for the following to be transmitted/received:

### FROM TANK ###
- Tank location, heading, pitch, roll
- Voltage of all rails, percentage of remaining power
- Received signal strength

### TO TANK ###
- Speed and direction
- LED light patterns
- Turret actions

The custom protocol uses TCP/IP ideas such as counting packets, checksums and retries.

It should be possible to send a GPS coordinate, and the tank to automatically figure out the direction to turn, and then drive until the waypoint has been reached.

###RC###

The RC mode is a fall-back control scheme that doesn't need a base station, just a bog-standard RC set.

As a result, there is no ability to control the tanks turret add on. 