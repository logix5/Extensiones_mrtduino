# Extension_NRF24L01

Extensión para realizar transmisiones y recepciones por radio 2.4 Ghz. Podemos usar los chips con Antena si queremos que la distancia de alcance
sea mayor.Usa el protocolo de comunicación SPI, y por el momento hecho sólo para transmitir el contenido de una variable en Scratch o
para recibir el contenido también en otra variable de Scratch.

Por el momento las variables definidas son doubles en Scratch, y hay que pasarle un puntero a ellas.

Para el pincho USB CH340:
Para verlo ponerAT? saldrán símbolos raros pero se intuye.

AT Commands
Baudrate : AT+BAUD=n where n =  1-6 (1:4800,2:9600,3:14400,4:19200,5:38400,6:115200) (default 9600Kbps)
NRF Rate : AT+RATE=n where n =  1-3 (1:250K, 2:1M, 3:2M ) (default 2Mbps)
Local Address : AT+RXA=0Xnn,0Xnn,0Xnn,0Xnn,0Xnn where nn are the local receiving address (default 0xff,0xff,0xff,0xff,0xff)
Target Address  : AT+TXA=0Xnn,0Xnn,0Xnn,0Xnn,0Xnn where nn are the target address
Operating Freq. : AT+FREQ=2.nnnG where nnn = 400 / 525 (default 2.400G)
Checksum mode : AT+CRC=n where n = 8 /16 (default : 16 bit)
System info : AT?

Mucho cuidado con el canal, los bit de CRC, el pipe, y la Frecuencia de transmisión
