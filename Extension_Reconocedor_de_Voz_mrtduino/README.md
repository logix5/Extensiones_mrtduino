# Extension Reconocedor de Voz

SOFTWARE SERIAL
No todos los pines de las placas Leonardo y Micro soportan interrupciones de cambio de nivel, así que solamente los siguientes se pueden utilizar para RX: 8, 9, 10, 11, 14 (MISO), 15 (SCK), 16 (MOSI).

Cada vez que queramos grabar, cargar, modificar o borrar los comandos que tengamos en el módulo, tendremos que arrancar  vr_sample_train. Después de cargar el programa, abrimos el monitor serie y lo configuramos para trabajar a 115.500 baudios. 

lista ordenes
Ahí nos aparecen los comandos que tenemos disponibles y su forma de utilizarlos. Basta con introducirlos en la línea de comandos. Los que nosotros utilizaremos serán: sigtrain y load; 

El comando sigtrain  sirve para grabar los comandos de voz. Junto al comando le pasaremos la posición en la que queremos que se guarde y el nombre que queremos asignarle.

