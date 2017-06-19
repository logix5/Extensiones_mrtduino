#include "Arduino.h"
#include "Display.h"


void enddisplaycommand ()
{
	Serial1.write(255);
	Serial1.write(255);
	Serial1.write(255);
}

Display::Display()
{
	
}

//Code	Decimal System	Indicator Color
//RED	63488	Red
//BLUE	31	Blue
//GRAY	33840	Gray
//BLACK	0	Black
//WHITE	65535	White
//GREEN	2016	Green
//BROWN	48192	Brown
//YELLOW	65504	Yellow

void Display::clear(unsigned int color)
{
	Serial1.print("cls ");
	Serial1.print(color);
	enddisplaycommand();
}

void Display::line(int x1, int y1, int x2, int y2, unsigned int color)
{
	
	Serial1.print("line ");
	Serial1.print(x1);
	Serial1.print(",");
	Serial1.print(y1);
	Serial1.print(",");
	Serial1.print(x2);
	Serial1.print(",");
	Serial1.print(y2);
	Serial1.print(",");
	Serial1.print(color);   
	enddisplaycommand();
}

void Display::circle(int x, int y, int r, unsigned int color)
{
	Serial1.print("cir ");
	Serial1.print(x);
	Serial1.print(",");
	Serial1.print(y);
	Serial1.print(",");
	Serial1.print(r);
	Serial1.print(",");
	Serial1.print(color);   
	enddisplaycommand();
}

void Display::circleFilled(int x, int y, int r, unsigned int color)
{
	Serial1.print("cirs ");
	Serial1.print(x);
	Serial1.print(",");
	Serial1.print(y);
	Serial1.print(",");
	Serial1.print(r);
	Serial1.print(",");
	Serial1.print(color);   
	enddisplaycommand();
}

void Display::rectangle(int x1, int y1, int x2, int y2, unsigned int color)
{
							
	Serial1.print("draw ");
	Serial1.print(x1);
	Serial1.print(",");
	Serial1.print(y1);
	Serial1.print(",");
	Serial1.print(x2);
	Serial1.print(",");
	Serial1.print(y2);
	Serial1.print(",");
	Serial1.print(color);   
	enddisplaycommand();
}

void Display::fill(int x1, int y1, int x2, int y2, unsigned int color)
{
							
	Serial1.print("fill ");
	Serial1.print(x1);
	Serial1.print(",");
	Serial1.print(y1);
	Serial1.print(",");
	Serial1.print(x2-x1);
	Serial1.print(",");
	Serial1.print(y2-y1);
	Serial1.print(",");
	Serial1.print(color);   
	enddisplaycommand();
}

void Display::EnviarComandoTexto (String Text,int valor)
{
	Serial1.print(Text);
	Serial.write(0x22);
	Serial1.print(valor);
	Serial.write(0x22);
	enddisplaycommand();
}

void Display::EnviarComandoValor (String Text,int valor)
{
	Serial1.print(Text);
	Serial1.print(valor);
	enddisplaycommand();
}


void Display::CambiarPagina(int idpagina)
{
	Serial1.print("page ");
	Serial1.print(idpagina);
	enddisplaycommand();
	
}

void Display::VisualizarImagen(int x, int y, int id_image)
{
	Serial1.print("pic ");
	Serial1.print(x1);
	Serial1.print(",");
	Serial1.print(y1);
	Serial1.print(",");
	Serial1.print(id_image);
  
	enddisplaycommand();
		
}


void Display::brightness(int brightness)
{
	Serial1.print("dim=");
	Serial1.print(brightness);

	enddisplaycommand();
}

void Display::enableDrawing()
{

	Serial1.print("thdra=1");
	enddisplaycommand();	

}


void Display::disableDrawing()
{
	Serial1.print("thdra=0");
	enddisplaycommand();	
}

void Display::drawingColor(int color)
{
							
	Serial1.print("thc=");
	Serial1.print(color);

	enddisplaycommand();
}
 

void Display::text(int x, int y, int font, unsigned int color, String text)
{

	Serial1.print("xstr ");
	Serial1.print(x);
	Serial1.print(",");
	Serial1.print(y);
	Serial1.print(",320,240,");
	Serial1.print(font);
	Serial1.print(",");
	Serial1.print(color);
	Serial1.print(",0,0,0,3,\"");
	Serial1.print(text);
	Serial1.print("\"");
	enddisplaycommand();

}

