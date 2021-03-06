## Connector board ##

Simple connector board that has 6 RJ45 sockets and allows you to connect 6 boards together, with 2 of them having the ability to connect the serial channels together (ports 4 and 5 to port 1).

There is a reset button on the board which will reset all of the boards that are connected if they implement the reset pin connection.

![](https://raw.githubusercontent.com/SorX14/Tank/master/PCB/Connector%20board/numbered.png)

The design vaguely implies that the voltage board connects to port 4 and the controller board port 1. Any other modules can connect to the other ports. Another connector board could be used to daisy-chain for more connections.

### Parts list ###

- **RJ45 connector** - Molex 85504-5001 - Farnell no: 2313732
- **0805 10k resistor** - Should be easy enough to find
- **Tactile button** - Salvaged one from another project, again easy to find

### Resources ###

- **PCB top** [https://github.com/SorX14/Tank/blob/master/PCB/Connector%20board/top.png](https://github.com/SorX14/Tank/blob/master/PCB/Connector%20board/top.png)
- **PCB bottom** [https://github.com/SorX14/Tank/blob/master/PCB/Connector%20board/bottom.png](https://github.com/SorX14/Tank/blob/master/PCB/Connector%20board/bottom.png)