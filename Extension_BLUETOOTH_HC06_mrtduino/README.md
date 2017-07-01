# Extension BLUETOOTH_HC06
Extensión Bluetooth para el chip HC-06. Sólo se ha probado el envío de caracteres sueltos, pero también esta creado para manejar la aplicación móvil de la placa 
trackduino (placa rusa). El problema es que para probar esta aplicación se necesita configurar el módulo de bluetooth HC-06 a velocidad 115200. Esta configuración 
no funciona y no hay manera de modificar los valores por defecto que tenía. En nuestro caso 38400.

Dejamos en util, un programa para configurarlo desde Arduino directamente. Hay dos opciones con nuestra placa mrtduino (Leonardo), usar el puerto Serie1 o 
usando la librería SoftwareSerial usar otros dos pines. En el caso de la utilidad es usando el puerto serie1

Los comandos AT que soporta son:
Prueba de funcionamiento:
Envíar: AT
Recibe: OK

Configurar el Baudrate:
Envíar: AT+BAUD<Numero>
El parámetro número es un caracter hexadecimal de ‘1’ a ‘c’ que corresponden a los siguientes Baud Rates: 1=1200, 2=2400, 3=4800, 4=9600, 5=19200, 6=38400, 7=57600, 8=115200, 9=230400, A=460800, B=921600, C=1382400
Recibe: OK<baudrate>

Configurar el Nombre de dispositivo Bluetooth:
Envíar: AT+NAME<Nombre>
Recibe: OKsetname

Configurar el código PIN de emparejamiento:
Envíar: AT+PIN<pin de 4 digitos>
Recibe: OK<pin de 4 digitos>

Obtener la version del firmware:
Enviar: AT+VERSION
Recibe: Linvor1.8




