# Extension MPU6050
Extensión para usar el MPU6050 inclinómetro y acelerómetro en el software MRTduino. Previamente debe instalarse la librería 
 https://github.com/jrowberg/i2cdevlib/tree/master/Arduino/MPU6050 puesto que la I2cdev si viene en las opciones. En este caso esta metida en MRt porque lo usa
 otro producto COCONUT, con lo cual hemos renombrado las variables. Hay que verificar que esta hecho porque se puede rehusar seguro alguna función

Queda pendiente usar el DMP creando una librería para obtener un IMU más completo de cara a tener mejor orientación del Robot mezclado datos del inclinómetro 
y del acelerómetro. Para ello crearíamos una librería interna y luego además usando el programa Processing2 podemos representar modelados  viendo comp varía. 
Esto ideal por ejemplo para hace run cuadricoptero

![image](https://user-images.githubusercontent.com/28557392/27809040-94e8e76e-604c-11e7-8a09-a5aa742862bd.png)
