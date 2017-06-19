
#ifndef Display_h
#define Display_h

#include "Arduino.h"

class Display
{
  public:
    Display();
    void clear(unsigned int color);
	void line(int x1, int y1, int x2, int y2, unsigned int color);
    void circle(int x, int y, int r, unsigned int color);
    void circleFilled(int x, int y, int r, unsigned int color);
	void rectangle(int x1, int y1, int x2, int y2,unsigned int color);
	void fill(int x1, int y1, int x2, int y2, unsigned int color);
	void text(int x, int y, int font, unsigned int color, String text);
	
	void EnviarComandoTexto (String Text,int valor);
	void EnviarComandoValor (String Text,int valor);
	
	void CambiarPagina(int pagina);
	
	void VisualizarImagen(int x, int y, int id_image);
	void brightness(int brightness);
	void enableDrawing();
	void disableDrawing();
	void drawingColor(int color);
	
};

void enddisplaycommand ();

#endif