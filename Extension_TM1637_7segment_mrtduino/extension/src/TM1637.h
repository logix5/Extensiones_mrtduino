//TM1637.h

#if defined(ARDUINO) && ARDUINO>=100
  #include "Arduino.h"
#else
  #include "WProgram.h"
#endif

#define TM1637_PAUSA 3 // Un poco más de dos, la mitad de cuatro microsegundos, para una frecuencia máxima de 250 KHz (con dos no funciona)

#define TM1637_ACTIVAR 0x88 // Inicializar OR Activar [OR brillo]
#define TM1637_DESACTIVAR 0x80 // Inicializar (sin activar ni brillo)
#define TM1637_ESCRIBIR_EN_POSICION 0x44
#define TM1637_ESCRIBIR_INCREMENTAR 0x40
#define TM1637_INICIO_DATOS 0xC0

#define SEGMENTO_A 0B00000001
#define SEGMENTO_B 0B00000010
#define SEGMENTO_C 0B00000100
#define SEGMENTO_D 0B00001000
#define SEGMENTO_E 0B00010000
#define SEGMENTO_F 0B00100000
#define SEGMENTO_G 0B01000000
#define CON_PUNTOS 0B10000000  // Para mostrar el punto decimal (o dos puntos en un display de reloj) haciendo una operación OR con el número
#define TODO_OFF   0B00000000

class TM1637
{
  private:
    byte pin_reloj;
    byte pin_datos;
    byte cantidad_digitos;
    byte brillo;
    byte segmentos_numero[11]; // Representación del 0 al 9 en un display de 7 segmentos (el último es el que representa el error)
    long valores_maximos[7]; // El TM1637 puede representar 6 dígitos como máximo
    long valores_minimos[7];
    void inicio_envio();
    void fin_envio();
    boolean enviar(byte dato);
  protected:
  public:
    TM1637(byte nuevo_pin_reloj,byte nuevo_pin_datos,byte nuevo_numero_digitos=4,byte nuevo_brillo=0);
    ~TM1637();
    boolean activar();
    boolean desactivar();
    boolean cambiar_brillo(byte nuevo_brillo);
    boolean borrar();
    boolean escribir_valor(long valor,byte coma=0);
    boolean escribir_horas_minutos(byte horas,byte minutos,boolean puntos=true,boolean cero_inicial=false);
    boolean escribir_minutos_segundos(byte horas,byte minutos,boolean puntos=true,boolean cero_inicial=true);
    boolean escribir_horas_minutos_segundos(byte horas,byte minutos,byte segundos,boolean cero_inicial=false);
    boolean escribir_digito(byte posicion,byte digito,boolean coma=false);
    boolean escribir_digitos(byte *digito,byte coma=0);
    boolean escribir_segmento(byte posicion,byte segmento);
    boolean escribir_segmentos(byte *segmento);
    byte numero_a_segmento(byte numero);
};