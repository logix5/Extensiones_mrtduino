# Extension Brújula HMC5883L
Extensión para usar la Brújula HMC5883L en el software MRTduino. Previamente debe instalarse las librerías 
https://github.com/jrowberg/i2cdevlib/tree/master/Arduino/HMC5883L y https://github.com/jrowberg/i2cdevlib/tree/master/Arduino/I2Cdev

Para calcular la desviciación con respecto al norte geográfico, usar esta web para sacar la declinación magnética http://www.magnetic-declination.com/
El valor que da la brújula hay que restarle este para obtenerlo con el geográfico