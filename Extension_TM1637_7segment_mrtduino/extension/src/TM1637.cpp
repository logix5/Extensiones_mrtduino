//TM1637.cpp

#include "TM1637.h"

TM1637::TM1637(byte nuevo_pin_reloj,byte nuevo_pin_datos,byte nuevo_cantidad_digitos,byte nuevo_brillo)
{
  // Representación de los decimales en un display de 7 segmentos
  segmentos_numero[0]=0x3F;
  segmentos_numero[1]=0x06;
  segmentos_numero[2]=0x5B;
  segmentos_numero[3]=0x4F;
  segmentos_numero[4]=0x66;
  segmentos_numero[5]=0x6D;
  segmentos_numero[6]=0x7D; // 0x7C; El 6 feo ahorra energía
  segmentos_numero[7]=0x27; // 0x07; El 7 clásico ahorra energía
  segmentos_numero[8]=0x7F;
  segmentos_numero[9]=0x6F; // 0x67; El 9 de palo ahorra energía
  segmentos_numero[10]=0x40; // Dibujo usado para simbolizar un de error (será una tira de guiones creada con el segmento g)
  // Valores máximos (más uno, para ahorrar en el bucle) El TM1637 puede representar 6 dígitos como máximo. Más rápido almacenar los valores que calcularlos
  valores_maximos[0]=0; // Añadido para poder comparar directamente con el número de dígitos
  valores_maximos[1]=10;
  valores_maximos[2]=100;
  valores_maximos[3]=1000;
  valores_maximos[4]=10000;
  valores_maximos[5]=100000;
  valores_maximos[6]=1000000;
  // Valores mínimos. El TM1637 puede representar 6 dígitos como máximo. Más rápido almacenar los valores que calcularlos
  valores_minimos[0]=0; // Añadido para poder comparar directamente con el número de dígitos
  valores_minimos[1]=0;
  valores_minimos[2]=-9;
  valores_minimos[3]=-99;
  valores_minimos[4]=-999;
  valores_minimos[5]=-9999;
  valores_minimos[6]=-99999;
  pin_reloj=nuevo_pin_reloj;
  pin_datos=nuevo_pin_datos;
  cantidad_digitos=nuevo_cantidad_digitos;
  brillo=nuevo_brillo;
}

TM1637::~TM1637()
{
  desactivar();
}

boolean TM1637::activar()
{
  pinMode(pin_reloj,INPUT);
  pinMode(pin_datos,INPUT);
  delayMicroseconds(TM1637_PAUSA);
  delayMicroseconds(TM1637_PAUSA);
  return cambiar_brillo(brillo);
}

boolean TM1637::desactivar()
{
  boolean estado;
  inicio_envio();
  estado=enviar(TM1637_DESACTIVAR);
  fin_envio();
  return estado;
}

boolean TM1637::cambiar_brillo(byte nuevo_brillo) // Como no se borra el contenido de los registros también puede usarse para cambiar el brillo
{
  boolean estado;
  inicio_envio();
  estado=enviar(TM1637_ACTIVAR|(nuevo_brillo&0B00000111)); // Ignorar todos los bits menos los tres últimos para no alterar el código de la orden por error
  fin_envio();
  return estado;
}

boolean TM1637::borrar()
{
  byte todo_ceros[cantidad_digitos];
  for(byte numero_digito=0;numero_digito<cantidad_digitos;numero_digito++)
  {
    todo_ceros[numero_digito]=0;
  }
  return escribir_segmentos(todo_ceros);
}

boolean TM1637::escribir_valor(long valor,byte coma)
{
  byte segmento[cantidad_digitos];
  byte numero_digito=0;
  long resto=abs(valor);
  byte digito;
  if(valor>valores_maximos[cantidad_digitos]-1||valor<valores_minimos[cantidad_digitos])
  {
    for(numero_digito=0;numero_digito<cantidad_digitos;numero_digito++)
    {
      segmento[numero_digito]=SEGMENTO_G;
    }
  }
  else
  {
    if(valor==0)
    {
      numero_digito++;
      segmento[cantidad_digitos-numero_digito]=segmentos_numero[0];
    }
    else
    {
      while(numero_digito<cantidad_digitos&&resto>0)
      {
        numero_digito++;
        digito=resto%10;
        segmento[cantidad_digitos-numero_digito]=segmentos_numero[digito];
        resto/=10;
      }
      if(valor<0)
      {
        numero_digito++;
        segmento[cantidad_digitos-numero_digito]=SEGMENTO_G;
      }
    }
    while(numero_digito<cantidad_digitos)
    {
      numero_digito++;
      segmento[cantidad_digitos-numero_digito]=0; // Apagar los que no se usen
    }
  }
  return escribir_segmentos(segmento);
}

boolean TM1637::escribir_horas_minutos(byte horas,byte minutos,boolean puntos,boolean cero_inicial)
{
  // No se verifican los valores para poder usarla de forma flexible
  byte segmento[4]; // hora y minutos siempre son 4 dígitos
  segmento[0]=horas/10;
  if(segmento[0]>0||cero_inicial)
  {
    segmento[0]=segmentos_numero[segmento[0]];
  }
  else
  {
    segmento[0]=0;
  }
  segmento[1]=segmentos_numero[horas%10];
  if(puntos)
  {
    segmento[1]|=CON_PUNTOS;
  }
  segmento[2]=segmentos_numero[minutos/10];
  segmento[3]=segmentos_numero[minutos%10];
  return escribir_segmentos(segmento);
}

boolean TM1637::escribir_minutos_segundos(byte horas,byte minutos,boolean puntos,boolean cero_inicial)
{
  return escribir_horas_minutos(horas,minutos,puntos,cero_inicial);
}

boolean TM1637::escribir_horas_minutos_segundos(byte horas,byte minutos,byte segundos,boolean cero_inicial)
{
  byte segmento[6]; // hora, minutos y segundos siempre son 6 dígitos
  segmento[0]=horas/10;
  if(segmento[0]>0||cero_inicial)
  {
    segmento[0]=segmentos_numero[segmento[0]];
  }
  else
  {
    segmento[0]=0;
  }
  segmento[1]=segmentos_numero[horas%10];
  segmento[2]=segmentos_numero[minutos/10]|CON_PUNTOS; // Siempre con puntos
  segmento[3]=segmentos_numero[minutos%10];
  segmento[4]=segmentos_numero[segundos/10]|CON_PUNTOS; // Siempre con puntos
  segmento[5]=segmentos_numero[segundos%10];
  return escribir_segmentos(segmento);
}

boolean TM1637::escribir_digito(byte posicion,byte digito,boolean coma)
{
  return escribir_segmento(posicion,numero_a_segmento(digito));
}

boolean TM1637::escribir_digitos(byte *digito,byte coma)
{
  byte segmento[cantidad_digitos];
  for(byte numero_digito=0;numero_digito<cantidad_digitos;numero_digito++)
  {
    segmento[numero_digito]=numero_a_segmento(digito[numero_digito]);
    if(numero_digito==coma-1)
    {
      segmento[numero_digito]|=CON_PUNTOS;
    }
  }
  return escribir_segmentos(segmento);
}

boolean TM1637::escribir_segmento(byte posicion,byte segmento)
{
  boolean estado;
  inicio_envio();
  estado=enviar(TM1637_ESCRIBIR_EN_POSICION);
  fin_envio();
  inicio_envio();
  estado|=enviar(TM1637_INICIO_DATOS|(posicion&0B00000011)); // Ignorar todos los bits menos los dos últimos para no alterar el código de la orden por error
  estado|=enviar(segmento);
  fin_envio();
  return estado;
}

boolean TM1637::escribir_segmentos(byte *segmento) // Escribe toda la pantalla, el vector debe contener todos los dígitos
{
  boolean estado;
  byte numero_digito;
  inicio_envio();
  estado=enviar(TM1637_ESCRIBIR_INCREMENTAR);
  fin_envio();
  inicio_envio();
  estado|=enviar(TM1637_INICIO_DATOS);
  for(numero_digito=0;numero_digito<cantidad_digitos;numero_digito++)
  {
    estado|=enviar(segmento[numero_digito]);
  }
  fin_envio();
  return estado;
}

// No se utiliza digitalWrite para enviar un nivel alto. Establecer el modo como entrada hace que el pin pase a estado de alta impedancia, la resistencia de pullup hace que el nivel sea alto

boolean TM1637::enviar(byte dato)
{
  boolean respuesta;
  for(byte numero_bit=0;numero_bit<8;numero_bit++)
  {
    pinMode(pin_reloj,OUTPUT);
    digitalWrite(pin_reloj,LOW);
    if(dato&(1<<numero_bit))
    {
      pinMode(pin_datos,INPUT);
    }
    else
    {
      pinMode(pin_datos,OUTPUT);
      digitalWrite(pin_datos,LOW);
    }
    delayMicroseconds(TM1637_PAUSA);
    pinMode(pin_reloj,INPUT);
    delayMicroseconds(TM1637_PAUSA);
  }
  // Leer respuesta TM1637 (estado)
  pinMode(pin_reloj,OUTPUT);
  digitalWrite(pin_reloj,LOW);
  delayMicroseconds(TM1637_PAUSA);
  pinMode(pin_datos,INPUT);
  pinMode(pin_reloj,INPUT);
  delayMicroseconds(TM1637_PAUSA);
  respuesta=digitalRead(pin_datos);
  return respuesta;
}

void TM1637::inicio_envio()
{
  pinMode(pin_reloj,INPUT);
  pinMode(pin_datos,OUTPUT);
  digitalWrite(pin_datos,LOW);
  delayMicroseconds(TM1637_PAUSA);
  delayMicroseconds(TM1637_PAUSA);
}

void TM1637::fin_envio()
{
  pinMode(pin_reloj,OUTPUT);
  digitalWrite(pin_reloj,LOW);
  pinMode(pin_datos,OUTPUT);
  digitalWrite(pin_datos,LOW);
  delayMicroseconds(TM1637_PAUSA);
  pinMode(pin_reloj,INPUT);
  delayMicroseconds(TM1637_PAUSA);
  pinMode(pin_datos,INPUT); // Al terminar, reloj y datos quedan en alta impedancia hasta la siguiente operación
}

byte TM1637::numero_a_segmento(byte numero)
{
  if(numero>9)
  {
    return(segmentos_numero[10]);
  }
  else
  {
    return(segmentos_numero[numero]);
  }
}