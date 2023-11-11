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
vector<_vertex3f> colores_vertices;
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

/* asignación de colores */
void 	colors_random();
void 	colors_chess(float r1, float g1, float b1, float r2, float g2, float b2);

void asignar_gama_amarillos();
void asignar_gama_verdes();
void asignar_gama_rojos();
void asignar_randomColor();
void asigna_colores_default(int numCaras); 

//colores metálicos para el ala-x
enum MetallicType
{
    DEFAULT, // Por defecto, usa el color pasado como parámetro
    GOLD,
    SILVER,
    BRONZE
};

void colors_metallic(MetallicType metallicType);
void asignar_gama_metalicos();


vector<_vertex3i> caras;
vector<_vertex3f> colores_caras;
};

//*************************************************************************
// objetos o modelos
//*************************************************************************

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
       _tronco_piramide(float tam1=0.5, float tam2=0.2, float al=0.8);
	//_tronco_piramide(float tam1, float tam2, float al);
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

// tapa_in=0 sin tapa, tapa_in=1 con tapa
// tapa_su=0 sin tapa, tapa_su=1 con tapa
// tipo=0 revolucion normal
// tipo=1 esfera
// tipo=2 cono

//************************************************************************
// objeto por extrusión
//************************************************************************

class _extrusion: public _triangulos3D
{
public:
       _extrusion(vector<_vertex3f> poligono, float x, float y, float z);
};


//************************************************************************
// objeto por revolución especiales
//************************************************************************

 
//************************************************************************
// cilindro
//************************************************************************

class _cilindro: public _rotacion
{
public:
       _cilindro(float radio=1.0, float altura=2.0, int num=12);
};

//************************************************************************
// cono
//************************************************************************

class _cono: public _rotacion
{
public:
       _cono(float radio=1.0, float altura=2.0, int num=12);
};

//************************************************************************
// esfera
//************************************************************************

class _esfera: public _rotacion
{
public:
       _esfera(float radio=1.0, int latitud=6, int longitud=6);
};


//************************************************************************
// rotacion archivo PLY
//************************************************************************

class _rotacion_PLY: public _rotacion
{
public:
       _rotacion_PLY();
void  parametros_PLY(char *archivo, int num);
};


//************************************************************************
// objeto montaña fractal
//************************************************************************

class _montana: public _triangulos3D
{
public:
       _montana(int nivelmax, float sigma, float h);
};

// nivelmax, ha de ser par es el tamaño de la cuadrícula
// sigma, se relaciona con la altura máxima de la montaña
// h 0<=h<=1, dimensión de Hausdorff indica la forma del relieve, más suave 1



//************************************************************************
// práctica 3, objeto jerárquico articulado excavadora
//************************************************************************

//************************************************************************
// piezas
//************************************************************************

//************************************************************************
// pala
//************************************************************************

class _pala: public _triangulos3D
{
public:
      _pala(float radio=1.0, float ancho=2.0, int num=8);
};

//************************************************************************
// brazo
//************************************************************************

class _brazo: public _triangulos3D
{
public:
      _brazo();
void  draw(_modo modo, float r, float g, float b, float grosor);

float ancho;
float alto;
float fondo;

protected:
_cubo cubo;
};

//************************************************************************
// cabina
//************************************************************************

class _cabina: public _triangulos3D
{
public:
       _cabina();
void  draw(_modo modo, float r, float g, float b, float grosor);

float ancho;
float alto;
float fondo;

protected:
_cubo cubo;
};

//************************************************************************
// sustentación
//************************************************************************

class _sustentacion: public _triangulos3D
{
public:
      _sustentacion();
void  draw(_modo modo, float r, float g, float b, float grosor);
       
float ancho;
float alto;
float fondo;

float radio;

protected:
_cilindro rueda;
_cubo base;
};

//************************************************************************
// excavadora (montaje del objeto final)
//************************************************************************

class _excavadora: public _triangulos3D
{
public:
       _excavadora();
       
void  draw(_modo modo, float r, float g, float b, float grosor);

float giro_cabina;
float giro_primer_brazo;
float giro_segundo_brazo;
float giro_pala;

float giro_primer_brazo_max;
float giro_primer_brazo_min;
float giro_segundo_brazo_max;
float giro_segundo_brazo_min;
float giro_pala_max;
float giro_pala_min;

float tamanio_pala;

protected:
_pala pala;
_brazo brazo;
_cabina cabina;
_sustentacion sustentacion;
};



/***********************************************************************************************/
//Práctica 3 Versión del alumno: objeto jerárquico articulado ALA-X (Star Wars)
/***********************************************************************************************/





class _motor: public _triangulos3D
{
public:
       _motor();
       void draw(_modo modo, float r, float g, float b, float grosor);

       float radio, altura, num;

protected:
_cilindro cilindro;
};

class _canion: public _triangulos3D
{
public:
       _canion();
       void draw(_modo modo, float r, float g, float b, float grosor);

       float radio, altura, num;
       float radioMisil, alturaMisil, numMisil; 
                     /*disparo*/
       float disparo= 0.0f;
       float max = 10.0f;
       float min = 0.0f;

_cilindro cilindro;
_cilindro cilindroMisil;
};

class _ala: public _triangulos3D
{      
public:
       _ala();
       void  draw(_modo modo, float r, float g, float b, float grosor);

       float ancho, alto, fondo;


_cubo cubo;
_canion canion;
_motor motor;
};

class _puerta: public _triangulos3D
{
public:
       _puerta();
       void  draw(_modo modo, float r, float g, float b, float grosor);
       
       float tam1= 0.85;    float tam2= 0.65;       float alt= 0.15;

protected: 
_tronco_piramide tronco;

};
 class _cabinaX: public _triangulos3D
 {
 public:
        _cabinaX();
        void  draw(_modo modo, float r, float g, float b, float grosor);
        float anchoCubo = 0.85;
        float altoCubo = 1.0;
 protected:
 _cubo cubo;
 _tronco_piramide tronco; 

 };

class _punta: public _triangulos3D
 {
public: 
       _punta();
       void  draw(_modo modo, float r, float g, float b, float grosor);
       
protected: 
_piramide punta;

 };
 
 
class _alaX: public _triangulos3D
{
public:
       _alaX();
       void  draw(_modo modo, float r, float g, float b, float grosor);

       /*atributos de movimiento*/
       float giroPuerta = 12.0f;
       float giroPuertamin = 12.0f;
       float giroPuertamax = -20.0f;

       /*giros*/
       float giroAla= 0.0f;
       float giroAlamin = 0.0f;
       float giroAlamax = 20.0f;

       /*giro punta*/
       float giroPunta = 1.0f;
       float giroPuntaMax = 360.0f;


      

_punta punta;
_puerta puerta;
_cabinaX cabinaX;

/*
Ala 1 es el ala superior izquierda (mirando de frente)
Ala 2 es el ala superior derecha (mirando de frente)
Ala 3 es el ala inferior izquierda (mirando de frente)
Ala 4 es el ala inferior derecha (mirando de frente)
*/
_ala ala1;
_ala ala2;
_ala ala3;
_ala ala4;
};

