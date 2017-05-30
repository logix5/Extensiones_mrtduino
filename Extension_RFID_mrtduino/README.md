# Extension_RFID_mrtduino
Extensión para usar el RFID y sus dos tarjetas en MRTduino software y placa. Se ha programado la lectura de tarjetas y enviar su código al puerto serie, y por otro lado la posibilidad de almacenar tarjetas con la opción de validarlas si es la que se pasa una tarjeta por el lector.
Las librerías usadas son la SPI para usar este protocolo de comunicaciones y la MFRC255 de aquí: https://github.com/miguelbalboa/rfid

![nfc-font-b-rfid-b-font-rc522-rf-ic-card-font-b-rfid-b-font-reader](https://cloud.githubusercontent.com/assets/28557392/26601850/c6c65a86-4581-11e7-8250-9ccbd20d4358.jpg)

Hubo que soldar los pines, y es importante no olvidar que se tiene que conectar al pin de *alimentación de 3.3V* de la placa MRTduino. Luego otros pines se conectan a los pines del pueto SPI, y a los que configuremos nosotros cuando inicializamos el módulo.

Dejo en el src las librerías necesarias, que por el momento hay que copiarlas junto a las de MRT para que funcione. 









