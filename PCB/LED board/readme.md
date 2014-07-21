## LED board ##

This board connects using the same RJ45 connector and pinout, and has the ability to drive 12V LED strips. 

There is no other sensors or pin breakouts on the board which is probably a mistake.

Runs @ 3.3v, and communicates via I2C or serial to a 'master' device. Has a footprint for a cap but in the end wasn't required, turns into a useful testing point for voltages.

The board is programmed via a custom shaped ISP header next to the RJ45 socket. There is a I2C active LED, along with a debug LED to help with testing.

The actual LED strip control side had a slice out of the ground plane to prevent ground loops. The MOSFETS can handle up to 20v, and a max dissipation of 1.3A.

There is a solder bridge up the top of the board that allows 2 boards to be programmed with the same code, and then one of them having the bridge closed; changing its I2C address.

### Parts list ###

- **Passives** - Not going to list them, all 0805 sized
- **RJ45 connector** - Molex 85504-5001 - Farnell no: 2313732
- **ATMEGA328P-AU** - TQFP-32 package - Farnell no: 1715486
- **SMD crystal** - 16 Mhz TXC 7B-16.000MAAJ-T - Farnell no: 1841977
- **N-channel MOSFETS** - NDS331N - Farnell no: 1839006
- **LEDs** - 0805 sized, various colours. Try and get the same brightness for each as otherwise it'll look strange.

### Mistakes ###

Rear silkscreen is mirrored.