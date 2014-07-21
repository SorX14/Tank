## Voltage board ##

This board takes the high voltage from the battery, and converts it into 3.3v, 5v, and 12v. Each voltage rail is monitored with a potentiometer acting as a voltage divider. The accuracy and range isn't as good as if opamps were used but it gives a ballpark figure. Each analog input has a capacitor on the back side to smooth out the readings slightly.

The VIN rail also has a set of 4 vertically stacked LED's which visually indicate the state of the battery. 

The voltage regulators are TracoPower units which are more efficient than the pin-compatible linear regulators. They provide up to 1A for each voltage, and a maximum 32v input. The minimum voltage for the 12v rail is 18v which is a large difference.

The power output either goes directly out the top of the board, or the 3.3v and 5v rails are also passed down the RJ45 socket and cable.

The same ICP header pinout is used to program this board. 

There are spaces for capacitors for each voltage rail, but the TracoPower units have a limit to the maximum capacitance they can drive. 

There are a few pins that are broken out diagonally next to the crystal.

### Parts list ###

- **Passives** - Not going to list them, all 0805 sized
- **RJ45 connector** - Molex 85504-5001 - Farnell no: 2313732
- **ATMEGA328P-AU** - TQFP-32 package - Farnell no: 1715486
- **SMD crystal** - 16 Mhz TXC 7B-16.000MAAJ-T - Farnell no: 1841977
- **Voltage regulators** - TRACOPOWER TSR 1-24120, TSR 1-2433, TSR 1-2450 - Farnell no: 1672130, 1696319, 1696320
- **Potentiometer** - BOURNS 3266W-1-254LF - 250k 12turn, although the spec isn't important, only the footprint. 