
// Opciones de configuración:
    char ssid[11]        = "Nombre_BT";    // Nombre para el modulo Bluetooth.
    char baudios         = '4';           // 1=>1200 baudios, 2=>2400, 3=>4800, 4=>9600 (por defecto), 5=>19200, 6=>38400, 7=>57600, 8=>115200
    char password[10]    = "1234";        // Contraseña para el emparejamiento del modulo.


void setup()
{
    Serial1.begin(9600);
    Serial.begin(9600);
   
       
    
    // Ahora se procede a la configuración del modulo:
    
        // Se inicia la configuración:
            Serial1.print("AT"); delay(1000);

        // Se ajusta el nombre del Bluetooth:
            Serial1.print("AT+NAME"); Serial1.print(ssid); delay(1000);

        // Se ajustan los baudios:
            Serial1.print("AT+BAUD"); Serial1.print(baudios); delay(1000);

        // Se ajusta la contraseña:
            Serial1.print("AT+PIN"); Serial1.print(password); delay(1000);    
            Serial.print("Configurada");
}

void loop()
{
    
    
}
