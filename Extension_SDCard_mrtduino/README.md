# Extension_SDCARD_mrtduino
Extensión para usar el SDcard en MRTduino software y placa. 
Las librerías usadas son la SPI para usar este protocolo de comunicaciones y la SD propia de Arduino 

Los pines, salvo el CS se conectan a los pines del pueto SPI

Dejo en el src las librería SPI necesaria y todas las de SD modificando los includes. Tenemos que ver como meterlas y las coja del directorio de Arduino, para que no haya que meterlas en el directorio src

Por otro lado cuando se queda pillado con busqueda de librerias, los proyectos cuando compilan estan en esta ruta:

***C:\Users\Tu_usuario\AppData\Roaming\kr.co.MyRobotTime.MRTduino.v0.9.1.4\Local Store\scratchTemp\***  aqui se pueden borrar si la info que hera tiene librerias de otros proyectos!!

![image](https://user-images.githubusercontent.com/28557392/27502363-c419048c-5873-11e7-9e93-c19e19ba2132.png)
![image](https://user-images.githubusercontent.com/28557392/27502374-d6573e34-5873-11e7-8413-560606f7d33a.png)









