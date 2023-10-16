//**************************************************************************
// Práctica 2 
//**************************************************************************

#include <vector>
#include <GL/gl.h>
#include "vertex.h"
#include <stdlib.h>


const float AXIS_SIZE=5000;
typedef enum{POINTS,EDGES,SOLID,SOLID_COLORS} _modo;

//*************************************************************************
// clase punto
//*************************************************************************

class _puntos3D
{
public:

  
	_puntos3D();
void 	draw_puntos(float r, float g, float b, int grosor);

vector<_vertex3f> vertices;
};

//*************************************************************************
// clase triángulo
//*************************************************************************

class _triangulos3D: public _puntos3D
{
public:

	_triangulos3D();
void 	draw_aristas(float r, float g, float b, int grosor);
void    draw_solido(float r, float g, float b);
void 	draw_solido_colores();
void 	draw(_modo modo, float r, float g, float b, float grosor);

void asignar_gama_amarillos();
void asignar_gama_verdes();
void asignar_gama_rojos();
void asignar_randomColor();
void asigna_colores_default(int numCaras); //Implementación limitada a un máximo de 12 colores

vector<_vertex3i> caras;
vector<_vertex3f> colores;

};


//*************************************************************************
// clase cubo
//*************************************************************************

class _cubo: public _triangulos3D
{
public:

	_cubo(float tam=0.5);
};


//*************************************************************************
// clase piramide
//*************************************************************************

class _piramide: public _triangulos3D
{
public:

	_piramide(float tam=0.5, float al=1.0);
};


class _tronco_piramide: public _triangulos3D
{
public:

	_tronco_piramide(float tam1=0.5, float tam2=0.2, float al=1.0);
};

//*************************************************************************
// clase objeto ply
//*************************************************************************

class _objeto_ply: public _triangulos3D
{
public:
   _objeto_ply();

void  parametros(char *archivo);
};

//************************************************************************
// objeto por revolución
//************************************************************************

class _rotacion: public _triangulos3D
{
public:
       _rotacion();
       
void  parametros(vector<_vertex3f> perfil, int num,
                 int tapa_in, int tapa_su, int tipo);
};
//0 revolucion 
//1 esfera
//2 cono

//************************************************************************
// objeto por extrusión
//************************************************************************

class _extrusion: public _triangulos3D
{
public:
       _extrusion(vector<_vertex3f> poligono, float x, float y, float z);
};

//objetos por revolucion especiales

//CLASE CILINDRO
class _cilindro: public _rotacion
{
public:
	   _cilindro(float radio = 0.5, float altura = 1.0, int lados = 12);
};

//clase cono
class _cono: public _rotacion
{
public:
	   _cono(float radio = 0.5, float altura = 2.0, int lados = 12);
};

//esfera por rotación
class _esfera: public _rotacion
{
public:
	   _esfera(float radio, int n1, int n2);
};

//clase ply para leer objetos por rotación
class _rotacion_PLY: public _rotacion{
public:
	_rotacion_PLY();
	void parametros_PLY(char *archivo, int num);
};

