/*

 CloudBot.h es una librería que permite conectar tus robot de Makeblock o Placas de Arduino
 con el Cloud de http://cloud.makeblock.es


 Nota: Cloud es una iniciativa particular y gratuita de https://www.makeblock.es dirigida a usuarios de Makeblock 
 y otros robots compatibles.
 
 Desarrollado: Jose Carlos ( jose@makeblock.es )
 v1.6

*/

// #include <MeMCore.h>   // Si tienes las librerías de Makeblock para Arduino

#include "cloudbot.h"


void setup() {
  
  Serial.begin(19200);
  while (!Serial) {
  
  }

  /*
    1- Conectar el WIFI al mBot 
    2- Configurar el wifi accediendo a la red Wifi creada CloudBotWifi
  
  */
  
  Serial.println("Hola Cloud");

// cloud::begin(PORT_1);  //<- Si tengo la libreria de makeblock puedo acceder con el puerto
   cloud::begin(9,11);   //<- O también con 2 pines ( útil si uso otra placa de Arduino )

  int esp8266_version = cloud::wifiVersion();

  if( esp8266_version ){
    Serial.print("ESP8266 WIFI - Firmware Version: "); 
    Serial.println( esp8266_version );
  }else{
    Serial.print("Error: Revisa las conexión con tu módulo wifi y que hayas subido el firmware correctamente "); 
  }
  

 //- Obtén tu cloud-key de  http://cloud.makeblock.es  para logarte
 if(cloud::wifiConnected()){ 
    Serial.println("Accediendo al Cloud: ");
    Serial.println( cloud::login("P3CH8T4VVSP3Z78TEVVS"));
  
    Serial.println("Puedes ver el contenido de tu robot en:");
    Serial.print("http://cloud.makeblock.es/robot/" );
    Serial.println(cloud::myID());  //<- ID público de mi robot ( en realidad es el ID del módulo wifi que use)
 }else{ 
    Serial.print("Error: cloud.makeblock.es no responde. Comprueba tu conexión a internet " );
 } 
   
}





void loop() { 
  
  //- Probando la librería con el terminal de Arduino
  //-  Abre la ventana del Terminal y ponlo a una velocidad de 19200
  //-  Puedes pulsar las teclas que se indican a continuación para probar cada caso 

  
  if(Serial.available()) {
    char t = Serial.read();

    //   1 -  Guardar un dato en el Cloud 
    if( t=='1'){ 
      //- Almaceno un dato con acceso público
      Serial.println(cloud::setData( "estado", "Hola Mundo"));
      
      //- Puedo almacenar un dato privado ( empieza por "_" )
      Serial.println(cloud::setData( "_midato", "Hola Mundo"));

      //- Puedo almacenar datos en una lista pública ( termina por ":" )
      Serial.println(cloud::setData( "lista:", "uno"));
      Serial.println(cloud::setData( "lista:", "dos"));

      //- Puedo almacenar datos en una lista privada ( empieza por "_" y termina por ":"  )
      Serial.println(cloud::setData( "_coordenadas:", "123"));
      Serial.println(cloud::setData( "_coordenadas:", "456"));

      //- Puedo Almacenar un Dato en otro robot ( el robot y la variable debe existir )
      Serial.println(cloud::setData( "ID_ROBOT", "dato", "valor"));
      
    }

    //   2 -   Leer Datos del Cloud
    if( t=='2'){
      
       //- Leo cualquier dato existente 
       Serial.println(cloud::getData( "estado"));

       //- Leo el último dato registrado en una lista (puedo omitir ":" al final porque la variable ya existe)
       Serial.println(cloud::getData( "lista"));

       //- Puedo Leer un dato de otro robot 
       Serial.println(cloud::getData( "ID_ROBOT", "dato"));
    }

    //   3 -   Contar los valores almacenados en una lista
    if( t=='3'){
      Serial.println(cloud::count( "lista"));
    }

    //   4 -   Acceder a un dato de una lista dada su posición ( primer elemento es el "1" no el "0")
    if( t=='4'){
      Serial.println(cloud::getDataAt( "lista", 1));
    }

    //   5 -   Eliminar un dato o lista
    if( t=='5'){
      Serial.println(cloud::delData( "lista:"));
    }

     //   c -   Ver si estamos conectados al WIFI
     if( t=='c'){
      Serial.println(cloud::wifiConnected());  
    }

    //   p -   Puedo hacer un Ping al cloud para ver si está accesible
     if( t=='p'){
      Serial.println(cloud::cloudPing());  
    }

    //   e -   Puedo ver si el cloud me ha devuelto algún error en la última acción 
     if( t=='e'){
      Serial.println(cloud::lastError());  //Autoconnect Wifi
    }

    //- Por si quiero desconectar el WIFI y conectarme a otra red distinta 
    //  Volver a acceder con el movil a la red wifi CloudBotWifi(
    if( t=='d'){
      cloud::wifiConnect();
    }

    //while (Serial.available()) { char t = Serial.read();  }
  }

 

  delay(100);
  

  
}

