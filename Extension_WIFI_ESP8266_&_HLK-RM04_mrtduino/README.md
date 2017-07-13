# Extension_WIFI_ESP8266_HLK-RM04_mrtduino
Extensión para realizar transmisiones y recepciones por wifi. Para hacer programas más complejos lo mejor es usar la 
librería https://github.com/itead/ITEADLIB_Arduino_WeeESP8266

Para esta extensión hemso usado el puerto Serie HW de la placa Arduino Leonardo. Aqui LA TX va con la RX del módulo Wifi y viceversa. 
Sino queremos eso tendremos que usar la librería SoftwareSerial.

El módulo cuando se enciende se pone un led a rojo, y cuando transmite se enciende otro led de color azul.

Se ha dejado un ejemplo para configurarlo como cliente con comandos AT directamente ( Muy instructivo)

El otro módulo requiere de una configuración especial que se hace conectandose a él desde aplicación web.

PTE usar la librería y usarla para recibir de un equipo remoto

PTE montar IOT desde cualquier parte

**Configurar nuestro HLK-RM04** 

-Conectarle alimentación de 5v. Patilla 1 al positivo "+" de cuaqluier puerto y patilla 2 al negativo "-" de cualquier puerto de la placa mrtduino. 
-Con un portátil/móvil/tablet buscarlo en redes wifi disponibles. Aparece como Hi-Link-xxxx. Conectar con esa red y de clave 12345678.
-Abrir su página de configuración en la dirección 192.168.16.254/Serial2Net.asp  y hacer log-in: admin/admin

En la configuración para nuestras prácticas:

-Hay que poner WIFI(CLIENT)-SERIAL, el nombre SSID de vuestra WIFI, vuestra clave y el tipo de encriptación que usa.
-Mejor elegir IP STATICA. Poner una libre y gateway de vuestro router.Esto para que siempre nos acordemos como conectarnos a él
-Aplicar cambios y ya se puede (dejar pasar 1 minuto) acceder a esta misma página desde vuestra RED poniendo la nueva IP configurada.

**IMPORTANTE:** _Si algo falla y no podemos contactar desde nuestra RED local ni por wifi con el portátil, podemos reinicializar el módulo poniendo a masa el pin 10 durante mas de 6 segundos._

Hay otra página de configuración mucho más completa en 192.168.16.254/home.asp, pero no se necesita para nuestro tema.

Si todo esta bien y "vemos" el módulo desde nuestra RED local, ya podemos conectar el 
Pin 20 (Rx) al pin S del puerto 16 (Tx) de la placa mrtduino
Pin 21 (Tx) al Pin S del puerto 15 (Rx) de la placa mrtduino

Y así tendremos disponible la comunicación serie HLK-RM04-Arduino y viceversa.










