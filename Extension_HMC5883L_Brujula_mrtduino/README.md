# Extension Brújula HMC5883L
Extensión para usar la Brújula HMC5883L en el software MRTduino. Previamente debe instalarse la librería 
https://github.com/jrowberg/i2cdevlib/tree/master/Arduino/HMC5883L puesto que la I2cdev si viene en las opciones

Para calcular la desviciación con respecto al norte geográfico, usar esta web para sacar la declinación magnética http://www.magnetic-declination.com/
El valor que da la brújula hay que restarle este para obtenerlo con el geográfico.

![image](https://user-images.githubusercontent.com/28557392/27636404-b70ddb46-5c0b-11e7-84f4-06d9c2f2a1a4.png)

![image](https://user-images.githubusercontent.com/28557392/27636414-bf99985e-5c0b-11e7-86a9-d595cd68997a.png)
