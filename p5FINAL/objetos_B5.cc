//**************************************************************************
// Práctica 5
//**************************************************************************
 
#include "objetos_B5.h"
#include "file_ply_stl.hpp"
#include <time.h> 


//*************************************************************************
// _puntos3D
//*************************************************************************

_puntos3D::_puntos3D()
{
}

//*************************************************************************
// dibujar puntos
//*************************************************************************

void _puntos3D::draw_puntos(float r, float g, float b, int grosor)
{
//**** usando vertex_array ****
glPointSize(grosor);
glColor3f(r,g,b);

glEnableClientState(GL_VERTEX_ARRAY);
glVertexPointer(3,GL_FLOAT,0,&vertices[0]);
glDrawArrays(GL_POINTS,0,vertices.size()); 

/*int i;
glPointSize(grosor);
glColor3f(r,g,b);
glBegin(GL_POINTS);
for (i=0;i<vertices.size();i++){
	glVertex3fv((GLfloat *) &vertices[i]);
	}
glEnd();*/
}


//*************************************************************************
// _triangulos3D
//*************************************************************************

_triangulos3D::_triangulos3D()
{
  b_normales_caras=false; b_normales_vertices=false;

  ambiente=_vertex4f(0.8,0.4,0.2,1.0);    //coeficientes ambiente
  difuso=_vertex4f(0.8,0.4,0.2,1.0);      //coeficientes difuso
  especular=_vertex4f(0.5,0.5,0.5,1.0);   //coeficientes especular
  brillo=120;                             //exponente del brillo
}


//*************************************************************************
// dibujar en modo arista
//*************************************************************************

void _triangulos3D::draw_aristas(float r, float g, float b, int grosor)
{
//**** usando vertex_array ****
glPolygonMode(GL_FRONT_AND_BACK,GL_LINE);
glLineWidth(grosor);
glColor3f(r,g,b);

glEnableClientState(GL_VERTEX_ARRAY);
glVertexPointer(3,GL_FLOAT,0,&vertices[0]);
glDrawElements(GL_TRIANGLES,caras.size()*3,GL_UNSIGNED_INT,&caras[0]);

/*int i;
glPolygonMode(GL_FRONT_AND_BACK,GL_LINE);
glLineWidth(grosor);
glColor3f(r,g,b);
glBegin(GL_TRIANGLES);
for (i=0;i<caras.size();i++){
	glVertex3fv((GLfloat *) &vertices[caras[i]._0]);
	glVertex3fv((GLfloat *) &vertices[caras[i]._1]);
	glVertex3fv((GLfloat *) &vertices[caras[i]._2]);
	}
glEnd();*/
}

//*************************************************************************
// dibujar en modo sólido con un único color
//*************************************************************************

void _triangulos3D::draw_solido(float r, float g, float b)
{
int i;
glPolygonMode(GL_FRONT_AND_BACK,GL_FILL);
glColor3f(r,g,b);
glBegin(GL_TRIANGLES);
for (i=0;i<caras.size();i++){
	glVertex3fv((GLfloat *) &vertices[caras[i]._0]);
	glVertex3fv((GLfloat *) &vertices[caras[i]._1]);
	glVertex3fv((GLfloat *) &vertices[caras[i]._2]);
	}
glEnd();

}

//*************************************************************************
// dibujar en modo sólido con colores diferentes para cada cara
//*************************************************************************

void _triangulos3D::draw_solido_colores( )
{
int i;
glPolygonMode(GL_FRONT_AND_BACK,GL_FILL);
glBegin(GL_TRIANGLES);
for (i=0;i<caras.size();i++){
	glColor3f(colores_caras[i].r,colores_caras[i].g,colores_caras[i].b);
	glVertex3fv((GLfloat *) &vertices[caras[i]._0]);
	glVertex3fv((GLfloat *) &vertices[caras[i]._1]);
	glVertex3fv((GLfloat *) &vertices[caras[i]._2]);
	}
glEnd();
}

void _triangulos3D::draw_iluminacion_plana()
{
  int i;
  if (b_normales_caras == false)
    calcular_normales_caras();
  glEnable(GL_LIGHTING);
  glShadeModel(GL_FLAT); 
  glEnable(GL_NORMALIZE); 
  
  glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, (GLfloat *)&ambiente);
  glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, (GLfloat *)&difuso);
  glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, (GLfloat *)&especular);
  glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, brillo);
  glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
  glBegin(GL_TRIANGLES);

  for (i = 0; i < caras.size(); i++)
  {
    glNormal3fv((GLfloat *)&normales_caras[i]);
    glVertex3fv((GLfloat *)&vertices[caras[i]._0]);
    glVertex3fv((GLfloat *)&vertices[caras[i]._1]);
    glVertex3fv((GLfloat *)&vertices[caras[i]._2]);
  }
  glEnd();
  glDisable(GL_LIGHTING);
}

void _triangulos3D::draw_iluminacion_suave()
{
  if (!b_normales_vertices)
    calcular_normales_vertices();
  glEnable(GL_LIGHTING);
  glShadeModel(GL_SMOOTH);
  glEnable(GL_NORMALIZE);
  glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, (GLfloat *)&ambiente);
  glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, (GLfloat *)&difuso);
  glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, (GLfloat *)&especular);
  glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, brillo);
  glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

  glBegin(GL_TRIANGLES);
  for (int i = 0; i < caras.size(); i++)
  {
    glNormal3fv((GLfloat *)&normales_vertices[caras[i]._0]);
    glVertex3fv((GLfloat *)&vertices[caras[i]._0]);
    glNormal3fv((GLfloat *)&normales_vertices[caras[i]._1]);
    glVertex3fv((GLfloat *)&vertices[caras[i]._1]);
    glNormal3fv((GLfloat *)&normales_vertices[caras[i]._2]);
    glVertex3fv((GLfloat *)&vertices[caras[i]._2]);
  }
  glEnd();
  glDisable(GL_LIGHTING);
}

//*************************************************************************
// dibujar en modo seleccion
//*************************************************************************

void _triangulos3D::draw_seleccion(int r, int g, int b)
{
int i;

glPolygonMode(GL_FRONT_AND_BACK,GL_FILL);
glColor3ub(r,g,b);
glBegin(GL_TRIANGLES);
for (i=0;i<caras.size();i++){
	glVertex3fv((GLfloat *) &vertices[caras[i]._0]);
	glVertex3fv((GLfloat *) &vertices[caras[i]._1]);
	glVertex3fv((GLfloat *) &vertices[caras[i]._2]);
	}
glEnd();
}


//*************************************************************************
// dibujar con distintos modos
//*************************************************************************

void _triangulos3D::draw(_modo modo, float r, float g, float b, float grosor)
{
switch (modo){
	case POINTS:draw_puntos(r, g, b, grosor);break;
	case EDGES:draw_aristas(r, g, b, grosor);break;
	case SOLID:draw_solido(r, g, b);break;
	case SOLID_COLORS:draw_solido_colores();break;
  case DIFFUSE_FLAT:draw_iluminacion_plana();break;
  case SOLID_GOURAUD:draw_iluminacion_suave();break;
	case SELECT:draw_seleccion(r, g, b);break;
	}
}

//*************************************************************************
// normales 
//*************************************************************************

void _triangulos3D::calcular_normales_caras()
{
int i, n_c;
_vertex3f va, vb;
float modulo;

n_c=caras.size();
normales_caras.resize(n_c);
for (i=0;i<n_c;i++)
  {va=vertices[caras[i]._1]-vertices[caras[i]._0];
   vb=vertices[caras[i]._2]-vertices[caras[i]._1];
   normales_caras[i].x=va.y*vb.z-va.z*vb.y;
   normales_caras[i].y=va.z*vb.x-va.x*vb.z;   
   normales_caras[i].z=va.x*vb.y-va.y*vb.x; 
   modulo=sqrt(normales_caras[i].x*normales_caras[i].x+
              normales_caras[i].y*normales_caras[i].y+
              normales_caras[i].z*normales_caras[i].z);
   normales_caras[i].x/=modulo;
   normales_caras[i].y/=modulo;   
   normales_caras[i].z/=modulo;     
  }
}

//*************************************************************************

void _triangulos3D::calcular_normales_vertices()
{
int i, n_c, n_v;
float modulo;

n_v=vertices.size();
normales_vertices.resize(n_v);

n_c=caras.size();

for (i=0;i<n_v;i++)
 normales_vertices[i]=_vertex3f(0.0,0.0,0.0);
 
for (i=0;i<n_c;i++)
  {
  normales_vertices[caras[i]._0]+=normales_caras[i];
  normales_vertices[caras[i]._1]+=normales_caras[i];
  normales_vertices[caras[i]._2]+=normales_caras[i];
  } 
  
for (i=0;i<n_v;i++)
      {modulo=sqrt(normales_vertices[i].x*normales_vertices[i].x+
                   normales_vertices[i].y*normales_vertices[i].y+
                   normales_vertices[i].z*normales_vertices[i].z);
       normales_vertices[i].x/=modulo;
       normales_vertices[i].y/=modulo;
       normales_vertices[i].z/=modulo;            
      }
}

//*************************************************************************

/*******************************************/
//normales de vértices de la esfera
void _esfera::calcular_normales_vertices()
{
  normales_vertices.resize(vertices.size());
  for (int i = 0; i < vertices.size(); i++)
  {
    normales_vertices[i] = _vertex3f(0.0, 0.0, 0.0);
    normales_vertices[i]._0 = vertices[i]._0;
    normales_vertices[i]._1 =
        vertices[i]._1;
    normales_vertices[i]._2 = vertices[i]._2;
    normales_vertices[i].normalize();
  }
  b_normales_vertices = true;
}

//*************************************************************************
// asignación colores
//*************************************************************************

void _triangulos3D::colors_random()
{
int i, n_c;
n_c=caras.size();
colores_caras.resize(n_c);
srand (time(NULL));
for (i=0;i<n_c;i++)  
  {colores_caras[i].r=rand()%1000/1000.0;
   colores_caras[i].g=rand()%1000/1000.0;
   colores_caras[i].b=rand()%1000/1000.0;
  }
}

//*************************************************************************

void _triangulos3D::colors_chess(float r1, float g1, float b1, float r2, float g2, float b2)
{
int i, n_c;
n_c=caras.size();
colores_caras.resize(n_c);
for (i=0;i<n_c;i++)  
  {if (i%2==0) 
     {colores_caras[i].r=r1;
      colores_caras[i].g=g1;
      colores_caras[i].b=b1;
     }
   else 
     {colores_caras[i].r=r2;
      colores_caras[i].g=g2;
      colores_caras[i].b=b2;
     } 
  }
}

//*************************************************************************

void _triangulos3D::colors_diffuse_flat (float kr, float kg, float kb,
                             float lpx, float lpy, float lpz)
{
int i, n_c;
float modulo, escalar;
_vertex3f l;

n_c=caras.size();
colores_caras.resize(n_c);
for (i=0;i<n_c;i++)
  {
   l.x=lpx-vertices[caras[i]._0].x;
   l.y=lpy-vertices[caras[i]._0].y;
   l.z=lpz-vertices[caras[i]._0].z;
   modulo=sqrt(l.x*l.x+l.y*l.y+l.z*l.z);
   l.x/=modulo;
   l.y/=modulo;
   l.z/=modulo;
   
   escalar=l.x*normales_caras[i].x+l.y*normales_caras[i].y+l.z*normales_caras[i].z;
   if (escalar>0.0)
     {colores_caras[i].r=kr*escalar;
      colores_caras[i].g=kg*escalar;
      colores_caras[i].b=kb*escalar;
     }
   else {colores_caras[i].r=0.0;
        colores_caras[i].g=0.0;
        colores_caras[i].b=0.0;
        }
  }
}



void _triangulos3D::asignar_gama_rojos(){
	float start = 1.0;
	for(int i = 0; i < colores_caras.size(); i++){
		colores_caras[i].r = start; 
		colores_caras[i].g = 0.0;
		colores_caras[i].b = 0.0;
		start-=1.0/colores_caras.size();
	}
}

void _triangulos3D::asignar_gama_verdes(){
    float start = 1.0;
    for(int i = 0; i < colores_caras.size(); i++){
        colores_caras[i].r = 0.0; 
        colores_caras[i].g = start; 
        colores_caras[i].b = 0.0; 
        start -= 1.0 / colores_caras.size();
    }
}


void _triangulos3D::asignar_gama_amarillos(){
    float start = 1.0;
	for(int i = 0; i < colores_caras.size(); i++){
		colores_caras[i].r = start; 
		colores_caras[i].g = start; 
		colores_caras[i].b = 0.0; 
		start -= 1.0 / colores_caras.size();
	}
}

void _triangulos3D::asigna_colores_default(int numCaras){
	vector<_vertex3f> colores_predeterminados;
	colores_predeterminados.resize(12);
	colores_predeterminados[0].x = 1.0; colores_predeterminados[0].y = 0.0; colores_predeterminados[0].z = 0.0; // Rojo
    colores_predeterminados[1].x = 0.0; colores_predeterminados[1].y = 1.0; colores_predeterminados[1].z = 0.0; // Verde
    colores_predeterminados[2].x = 0.0; colores_predeterminados[2].y = 0.0; colores_predeterminados[2].z = 1.0; // Azul
    colores_predeterminados[3].x = 1.0; colores_predeterminados[3].y = 1.0; colores_predeterminados[3].z = 0.0; // Amarillo
    colores_predeterminados[4].x = 1.0; colores_predeterminados[4].y = 0.0; colores_predeterminados[4].z = 1.0; // Magenta
    colores_predeterminados[5].x = 0.0; colores_predeterminados[5].y = 1.0; colores_predeterminados[5].z = 1.0; // Cian
    colores_predeterminados[6].x = 0.5; colores_predeterminados[6].y = 0.5; colores_predeterminados[6].z = 0.0; // Amarillo oscuro
    colores_predeterminados[7].x = 0.0; colores_predeterminados[7].y = 0.5; colores_predeterminados[7].z = 0.5; // Verde azulado
    colores_predeterminados[8].x = 0.5; colores_predeterminados[8].y = 0.0; colores_predeterminados[8].z = 0.5; // Morado
    colores_predeterminados[9].x = 0.5; colores_predeterminados[9].y = 0.5; colores_predeterminados[9].z = 0.5; // Gris
    colores_predeterminados[10].x = 0.0; colores_predeterminados[10].y = 0.0; colores_predeterminados[10].z = 0.5; // Azul oscuro
    colores_predeterminados[11].x = 0.0; colores_predeterminados[11].y = 0.0; colores_predeterminados[11].z = 0.3; // Azul oscuro (variación)

	auto it = colores_predeterminados.begin();
	int numAsignaciones = 0;
	while(it != colores_predeterminados.end() && numAsignaciones < numCaras){
		colores_caras[numAsignaciones].r = (*it).x;
		colores_caras[numAsignaciones].g = (*it).y;
		colores_caras[numAsignaciones].b = (*it).z;
		++numAsignaciones;
		++it;
	}
}


//*************************************************************************
// objetos o modelos
//*************************************************************************

//*************************************************************************
// clase cubo
//*************************************************************************

_cubo::_cubo(float tam)
{
//vertices
vertices.resize(8);
vertices[0].x=-tam;vertices[0].y=-tam;vertices[0].z=tam;
vertices[1].x=tam;vertices[1].y=-tam;vertices[1].z=tam;
vertices[2].x=tam;vertices[2].y=tam;vertices[2].z=tam;
vertices[3].x=-tam;vertices[3].y=tam;vertices[3].z=tam;
vertices[4].x=-tam;vertices[4].y=-tam;vertices[4].z=-tam;
vertices[5].x=tam;vertices[5].y=-tam;vertices[5].z=-tam;
vertices[6].x=tam;vertices[6].y=tam;vertices[6].z=-tam;
vertices[7].x=-tam;vertices[7].y=tam;vertices[7].z=-tam;

// triangulos
caras.resize(12);
caras[0]._0=0;caras[0]._1=1;caras[0]._2=3;
caras[1]._0=3;caras[1]._1=1;caras[1]._2=2;
caras[2]._0=1;caras[2]._1=5;caras[2]._2=2;
caras[3]._0=5;caras[3]._1=6;caras[3]._2=2;
caras[4]._0=5;caras[4]._1=4;caras[4]._2=6;
caras[5]._0=4;caras[5]._1=7;caras[5]._2=6;
caras[6]._0=0;caras[6]._1=7;caras[6]._2=4;
caras[7]._0=0;caras[7]._1=3;caras[7]._2=7;
caras[8]._0=3;caras[8]._1=2;caras[8]._2=7;
caras[9]._0=2;caras[9]._1=6;caras[9]._2=7;
caras[10]._0=0;caras[10]._1=1;caras[10]._2=4;
caras[11]._0=1;caras[11]._1=5;caras[11]._2=4; 

// normales
calcular_normales_caras();

//colores de las caras
colors_random();
}


//*************************************************************************
// clase piramide
//*************************************************************************

_piramide::_piramide(float tam, float al)
{
int i;
//vertices 
vertices.resize(5); 
vertices[0].x=-tam;vertices[0].y=0;vertices[0].z=tam;
vertices[1].x=tam;vertices[1].y=0;vertices[1].z=tam;
vertices[2].x=tam;vertices[2].y=0;vertices[2].z=-tam;
vertices[3].x=-tam;vertices[3].y=0;vertices[3].z=-tam;
vertices[4].x=0;vertices[4].y=al;vertices[4].z=0;

caras.resize(6);
caras[0]._0=0;caras[0]._1=1;caras[0]._2=4;
caras[1]._0=1;caras[1]._1=2;caras[1]._2=4;
caras[2]._0=2;caras[2]._1=3;caras[2]._2=4;
caras[3]._0=3;caras[3]._1=0;caras[3]._2=4;
caras[4]._0=3;caras[4]._1=1;caras[4]._2=0;
caras[5]._0=3;caras[5]._1=2;caras[5]._2=1;

// normales
calcular_normales_caras();

//colores de las caras
colors_random();
}


_tronco_piramide::_tronco_piramide(float tam1, float tam2, float alt){
	
  //vertices 
	vertices.resize(8); 
	vertices[0].x=-tam1;vertices[0].y=0;vertices[0].z=tam1;
	vertices[1].x=tam1;vertices[1].y=0;vertices[1].z=tam1;
	vertices[2].x=tam1;vertices[2].y=0;vertices[2].z=-tam1;
	vertices[3].x=-tam1;vertices[3].y=0;vertices[3].z=-tam1;
	vertices[4].x=-tam2;vertices[4].y=alt;vertices[4].z=tam2;
	vertices[5].x=tam2;vertices[5].y=alt;vertices[5].z=tam2;
	vertices[6].x=tam2;vertices[6].y=alt;vertices[6].z=-tam2;
	vertices[7].x=-tam2;vertices[7].y=alt;vertices[7].z=-tam2;

	// triangulos
	caras.resize(12);
	//cara baja
	caras[0]._0=3;caras[0]._1=1;caras[0]._2=0;
	caras[1]._0=3;caras[1]._1=2;caras[1]._2=1;

	//cara arriba
	caras[2]._0=4;caras[2]._1=5;caras[2]._2=6;
	caras[3]._0=4;caras[3]._1=6;caras[3]._2=7;

	//cara izquierda
	caras[4]._0=3;caras[4]._1=0;caras[4]._2=7;
	caras[5]._0=0;caras[5]._1=4;caras[5]._2=7;

	//cara delante
	caras[6]._0=0;caras[6]._1=1;caras[6]._2=4;
	caras[7]._0=1;caras[7]._1=5;caras[7]._2=4;

	//cara derecha
	caras[8]._0=1;caras[8]._1=2;caras[8]._2=5;
	caras[9]._0=2;caras[9]._1=6;caras[9]._2=5;

	//cara trasera
	caras[10]._0=2;caras[10]._1=3;caras[10]._2=6;
	caras[11]._0=3;caras[11]._1=6;caras[11]._2=7;

	colores_caras.resize(12);
  asignar_gama_metalicos();
  calcular_normales_caras();
	
	// int numColores = caras.size();
	// asigna_colores_default(numColores);
	//asignar_gama_amarillos();
	//asignar_gama_verdes();
	//asignar_randomColor();
	//asignar_gama_rojos();
}

//*************************************************************************
// clase objeto ply
//*************************************************************************


_objeto_ply::_objeto_ply() 
{
   // leer lista de coordenadas de vértices y lista de indices de vértices
 
}



void _objeto_ply::parametros(char *archivo)
{
int i, n_ver,n_car;

vector<float> ver_ply ;
vector<int>   car_ply ;
 
_file_ply::read(archivo, ver_ply, car_ply );

n_ver=ver_ply.size()/3;
n_car=car_ply.size()/3;

printf("Number of vertices=%d\nNumber of faces=%d\n", n_ver, n_car);

vertices.resize(n_ver);
caras.resize(n_car);
// vértices

for (i=0;i<n_ver;i++)
  {vertices[i].x=ver_ply[3*i];
   vertices[i].y=ver_ply[3*i+1];
   vertices[i].z=ver_ply[3*i+2];
  }

// vértices
for (i=0;i<n_car;i++)
  {caras[i].x=car_ply[3*i];
   caras[i].y=car_ply[3*i+1];
   caras[i].z=car_ply[3*i+2];
  }
 
// normales
calcular_normales_caras();

// colores
//colors_random();
colors_diffuse_flat(0.8,0.9,0.2,   20,20,20);
}


//************************************************************************
// objeto por revolucion
//************************************************************************

_rotacion::_rotacion()
{

}


void _rotacion::parametros(vector<_vertex3f> perfil, int num, 
                           int tapa_in, int tapa_su, int tipo)
{
int i,j;
_vertex3f vertice_aux;
_vertex3i cara_aux;
int num_aux;
float radio;

// tratamiento de los vértice
radio=sqrt(perfil[0].x*perfil[0].x+perfil[0].y*perfil[0].y);

num_aux=perfil.size();
if (tipo==2) num_aux=1;
vertices.resize(num_aux*num);

for (j=0;j<num;j++)
  {for (i=0;i<num_aux;i++)
     {
      vertice_aux.x=perfil[i].x*cos(2.0*M_PI*j/(1.0*num))+
                    perfil[i].z*sin(2.0*M_PI*j/(1.0*num));
      vertice_aux.z=-perfil[i].x*sin(2.0*M_PI*j/(1.0*num))+
                    perfil[i].z*cos(2.0*M_PI*j/(1.0*num));
      vertice_aux.y=perfil[i].y;
      vertices[i+j*num_aux]=vertice_aux;
     }
  }

// tratamiento de las caras 
if (tipo<2)
{
for (j=0;j<num;j++)
  {for (i=0;i<num_aux-1;i++)
     {cara_aux._0=i+j*num_aux;
      cara_aux._1=i+((j+1)%num)*num_aux;
      cara_aux._2=i+1+j*num_aux;
      caras.push_back(cara_aux);

      cara_aux._0=i+((j+1)%num)*num_aux;
      cara_aux._1=i+1+((j+1)%num)*num_aux;
      cara_aux._2=i+1+j*num_aux;
      caras.push_back(cara_aux);
     }
   }
}
 
// tapa inferior
if (tapa_in==1)
{
  // punto central de la tapa
  vertice_aux.x=0.0;
  if (tipo==1) vertice_aux.y=-radio;
  else vertice_aux.y=perfil[0].y;
  vertice_aux.z=0.0;
  vertices.push_back(vertice_aux);
  //caras tapa inferior

  cara_aux._0=num_aux*num;
  for (j=0;j<num;j++)
    {
     cara_aux._1=((j+1)%num)*num_aux;
     cara_aux._2=j*num_aux;
     caras.push_back(cara_aux);
    }

}
 
// tapa superior

if (tapa_su==1)
{
  // punto central de la tapa
  vertice_aux.x=0.0;
  vertice_aux.y=perfil[num_aux-1].y;
  if (tipo==1) vertice_aux.y=radio;
  if (tipo==2) vertice_aux.y=perfil[1].y;
  vertice_aux.z=0.0;
  vertices.push_back(vertice_aux);

  //caras tapa superior
  if (tapa_in==1) cara_aux._0=num_aux*num+1;
  else cara_aux._0=num_aux*num;

  for (j=0;j<num;j++)
    {cara_aux._1=j*num_aux+num_aux-1;
     cara_aux._2=((j+1)%num)*num_aux+num_aux-1;
     caras.push_back(cara_aux);
    }
}

// normales
calcular_normales_caras();
calcular_normales_vertices();

//colores de las caras
colors_random();
}



//************************************************************************
// objeto cilindro (caso especial de rotacion)
//************************************************************************

_cilindro::_cilindro(float radio, float altura, int num)
{
vector<_vertex3f> perfil;
_vertex3f aux;

aux.x=radio; aux.y=-altura/2.0; aux.z=0.0;
perfil.push_back(aux);
aux.x=radio; aux.y=altura/2.0; aux.z=0.0;
perfil.push_back(aux);
parametros(perfil,num,0,1,1);
}


//************************************************************************
// objeto cono (caso especial de rotacion)
//************************************************************************

_cono::_cono(float radio, float altura, int num)
{
vector<_vertex3f> perfil;
_vertex3f aux;

aux.x=radio; aux.y=0; aux.z=0.0;
perfil.push_back(aux);
aux.x=0.0; aux.y=altura; aux.z=0.0;
perfil.push_back(aux);
parametros(perfil,num,1,1,1);
}

//************************************************************************
// objeto esfera (caso especial de rotacion)
//************************************************************************

_esfera::_esfera(float radio, int latitud, int longitud)
{
vector<_vertex3f> perfil;
_vertex3f aux;
int i;
for (i=1;i<latitud;i++)
  {aux.x=radio*cos(M_PI*i/(latitud*1.0)-M_PI/2.0);
   aux.y=radio*sin(M_PI*i/(latitud*1.0)-M_PI/2.0);
   aux.z=0.0;
   perfil.push_back(aux);
  }
parametros(perfil,longitud,1,1,1);
}


//************************************************************************
// rotacion archivo PLY (caso especial de rotacion)
//************************************************************************

_rotacion_PLY::_rotacion_PLY()
{

}

void _rotacion_PLY::parametros_PLY(char *archivo, int num)
{

}


//************************************************************************

//************************************************************************
// objeto por extrusión
//************************************************************************


_extrusion::_extrusion(vector<_vertex3f> poligono, float x, float y, float z)
{
int i;
_vertex3f vertice_aux;
_vertex3i cara_aux;
int num_aux;

// tratamiento de los vértice

num_aux=poligono.size();
vertices.resize(num_aux*2);
for (i=0;i<num_aux;i++)
    {
      vertices[2*i]=poligono[i];
      vertices[2*i+1].x=poligono[i].x+x;
      vertices[2*i+1].y=poligono[i].y+y;
      vertices[2*i+1].z=poligono[i].z+z;
    }
    
// tratamiento de las caras 

caras.resize(num_aux*2);
int c=0;
for (i=0;i<num_aux;i++)         
  {
   caras[c]._0=i*2;
   caras[c]._1=(i*2+2)%(num_aux*2);
   caras[c]._2=i*2+1;    
   c=c+1;
   caras[c]._0=(i*2+2)%(num_aux*2);
   caras[c]._1=(i*2+2)%(num_aux*2)+1;
   caras[c]._2=i*2+1;    
   c=c+1;    
   }  
   
// normales
calcular_normales_caras();

//colores de las caras
colors_chess(0.2,1.0,0.2,0.3,0.8,0.1);
}

//************************************************************************
// objeto montaña fractal
//************************************************************************

float gauss(float ga, float gf )
{
float sum;
int i;
sum=0.0;
for (i=0;i<4;i++) sum=sum+rand()%32767;
return gf*sum/4.0-ga;
}


_montana::_montana(int nivelmax, float sigma, float h)

{

if (nivelmax>8) nivelmax=8;
int i,j,etapa;
float ga=sqrt(12.0);
float gf=2*ga/(32767*1.0);
int num=pow(2,nivelmax)+1;
srand (time(NULL));

vertices.resize(num*num);

for (j=0;j<num;j++)
  for (i=0;i<num;i++)
    {vertices[i+j*num].x=-0.1*(num-1)/2.0+i*0.1;
     vertices[i+j*num].z=-0.1*(num-1)/2.0+j*0.1;
     vertices[i+j*num].y=0.0;
    }

vertices[0].y=sigma*gauss(ga,gf);
vertices[num-1].y=sigma*gauss(ga,gf);
vertices[num*(num-1)].y=sigma*gauss(ga,gf);
vertices[num*num-1].y=sigma*gauss(ga,gf);

int d1=num-1;
int d2=(num-1)/2;

for (etapa=0;etapa<nivelmax;etapa++)
   {
    sigma=sigma*pow(0.5,0.5*h);
    for (j=d2;j<num-d2;j=j+d1)
       for (i=d2;i<num-d2;i=i+d1)
         {vertices[i+j*num].y=sigma*gauss(ga,gf)+
             (vertices[i+d2+(j+d2)*num].y+vertices[i+d2+(j-d2)*num].y+
              vertices[i-d2+(j+d2)*num].y+vertices[i-d2+(j-d2)*num].y)/4.0;
         }
     sigma=sigma*pow(0.5,0.5*h);
     for (i=d2;i<num-d2;i=i+d1)
	{vertices[i].y=sigma*gauss(ga,gf)+(vertices[i+d2].y+
                     vertices[i-d2].y+vertices[i+d2*num].y)/3.0;
         vertices[i+num*(num-1)].y=sigma*gauss(ga,gf)+
                    (vertices[i+d2+num*(num-1)].y+
                     vertices[i-d2+num*(num-1)].y+
                     vertices[i+(num-1-d2)*num].y)/3.0;
	 vertices[i*num].y=sigma*gauss(ga,gf)+(vertices[(i+d2)*num].y+
                     vertices[(i-d2)*num].y+vertices[d2+i*num].y)/3.0;
         vertices[num-1+i*num].y=sigma*gauss(ga,gf)+
                    (vertices[num-1+(i+d2)*num].y+
                     vertices[num-1+(i-d2)*num].y+
                     vertices[num-1-d2+i*num].y)/3;
	}
      	
     for (j=d2;j<num-d2;j=j+d1)
	for (i=d1;i<num-d2;i=i+d1)
          vertices[i+j*num].y=sigma*gauss(ga,gf)+
                   (vertices[i+(j+d2)*num].y+vertices[i+(j-d2)*num].y+
                    vertices[i+d2+j*num].y+vertices[i-d2+j*num].y)/4.0;
     for (j=d1;j<num-d2;j=j+d1)
	for (i=d2;i<num-d2;i=i+d1)
	  vertices[i+j*num].y=sigma*gauss(ga,gf)+
                   (vertices[i+(j+d2)*num].y+vertices[i+(j-d2)*num].y+
                    vertices[i+d2+j*num].y+vertices[i-d2+j*num].y)/4.0;

     d1=(int)d1/2;
     d2=(int)d2/2;
    }


// caras 
caras.resize((num-1)*(num-1)*2);
int c=0; 
for (j=0;j<num-1;j++)
  for (i=0;i<num-1;i++)
    {caras[c]._0=i+j*num;
     caras[c]._1=i+1+j*num; 
     caras[c]._2=i+1+(j+1)*num;
     c=c+1; 
     caras[c]._0=i+j*num;
     caras[c]._1=i+1+(j+1)*num; 
     caras[c]._2=i+(j+1)*num;
     c=c+1;
    }

// normales
calcular_normales_caras();

//colores de las caras
colors_chess(0.2,1.0,0.2,0.3,0.8,0.1);

}

//************************************************************************
// práctica 3, objeto jerárquico articulado
//************************************************************************

//************************************************************************
// piezas
//************************************************************************

//************************************************************************
// pala
//************************************************************************

_pala::_pala(float radio, float ancho, int num)
{
vector<_vertex3f> perfil;
_vertex3f vertice_aux;
_vertex3i cara_aux;
int i, j;

vertice_aux.x=radio; vertice_aux.y=0; vertice_aux.z=-ancho/2.0;
perfil.push_back(vertice_aux);
vertice_aux.z=ancho/2.0;
perfil.push_back(vertice_aux);

// tratamiento de los vértices

for (j=0;j<=num;j++)
  {for (i=0;i<2;i++)	
     {
      vertice_aux.x=perfil[i].x*cos(M_PI*j/(1.0*num))-
                    perfil[i].y*sin(M_PI*j/(1.0*num));
      vertice_aux.y=perfil[i].x*sin(M_PI*j/(1.0*num))+
                    perfil[i].y*cos(M_PI*j/(1.0*num));
      vertice_aux.z=perfil[i].z;
      vertices.push_back(vertice_aux);
     }
  }
  
// tratamiento de las caras 

for (j=0;j<num;j++)
   {cara_aux._0=j*2;
    cara_aux._1=(j+1)*2;
    cara_aux._2=(j+1)*2+1;
    caras.push_back(cara_aux);
    
    cara_aux._0=j*2;
    cara_aux._1=(j+1)*2+1;
    cara_aux._2=j*2+1;
    caras.push_back(cara_aux);
   }
   
// tapa inferior
vertice_aux.x=0;
vertice_aux.y=0;
vertice_aux.z=-ancho/2.0;
vertices.push_back(vertice_aux); 

for (j=0;j<num;j++)
   {cara_aux._0=j*2;
    cara_aux._1=(j+1)*2;
    cara_aux._2=vertices.size()-1;
    caras.push_back(cara_aux);
   }
  
// tapa superior
vertice_aux.x=0;
vertice_aux.y=0;
vertice_aux.z=ancho/2.0;
vertices.push_back(vertice_aux); 

for (j=0;j<num;j++)
   {cara_aux._0=j*2+1;
    cara_aux._1=(j+1)*2+1;
    cara_aux._2=vertices.size()-1;
    caras.push_back(cara_aux);
   }
  
colors_chess(1.0,1.0,0.0,0.0,0.0,1.0);
}

//************************************************************************
// brazo
//************************************************************************

_brazo::_brazo()
{
ancho=0.6;
alto=0.1;
fondo=0.1;
colors_chess(1.0,1.0,0.0,0.0,0.0,1.0);
};

void _brazo::draw(_modo modo, float r, float g, float b, float grosor)
{
glPushMatrix();
glScalef(ancho, alto, fondo);
glTranslatef(0.5,0,0);
cubo.draw(modo, r, g, b, grosor);
glPopMatrix();
};

//************************************************************************
// cabina
//************************************************************************

_cabina::_cabina()
{
ancho=0.4;
alto=0.6;
fondo=0.4;
cubo.colors_chess(1.0,1.0,0.0,0.0,0.0,1.0);
};

void _cabina::draw(_modo modo, float r, float g, float b, float grosor)
{
glPushMatrix();
glScalef(ancho, alto, fondo);
cubo.draw(modo, r, g, b, grosor);
glPopMatrix();
};

//************************************************************************
// sustentación
//************************************************************************

_sustentacion::_sustentacion()
{
ancho=1.2;
alto=0.3;
fondo=0.8;
radio=0.15;
base.colors_chess(1.0,1.0,0.0,0.0,0.0,1.0);
};

void _sustentacion::draw(_modo modo, float r, float g, float b, float grosor)
{
glPushMatrix();
glTranslatef(2*ancho/6,-alto/2.0,0);
glRotatef(90,1,0,0);
glScalef(radio, fondo/2.2, radio);
rueda.draw(modo, r, g, b, grosor);
glPopMatrix();

glPushMatrix();
glTranslatef(-2*ancho/6,-alto/2.0,0);
glRotatef(90,1,0,0);
glScalef(radio, fondo/2.2, radio);
rueda.draw(modo, r, g, b, grosor);
glPopMatrix();

glPushMatrix();
glScalef(ancho, alto, fondo);
base.draw(modo, r, g, b, grosor);
glPopMatrix();
};

//************************************************************************
// excavadora (montaje del objeto final)
//************************************************************************

_excavadora::_excavadora()
{
giro_cabina = 0.0;
giro_primer_brazo = 0.0;
giro_primer_brazo_max = 0;
giro_primer_brazo_min = -90;
giro_segundo_brazo = 0.0;
giro_segundo_brazo_max = 30;
giro_segundo_brazo_min = 0;
giro_pala = 0.0;
giro_pala_max = 50.0;
giro_pala_min = -90.0;

tamanio_pala=0.15;


int color=80;
piezas=5;
grosor_select=2;
color_pick=_vertex3f(1.0,0.0,0.0); 
color_select.resize(piezas);
activo.resize(piezas);

for (int i=0;i<piezas;i++)
  {activo[i]=0;
   color_select[i].r=color_select[i].g=color_select[i].b=color;
   color+=20;
  }
};

//************************************************************************

void _excavadora::draw(_modo modo, float r, float g, float b, float grosor)
{
float r_p,g_p,b_p;
int tam=2;

r_p=color_pick.r;
g_p=color_pick.g;
b_p=color_pick.b;

glPushMatrix();

if (activo[0]==1) sustentacion.draw(EDGES, r_p, g_p, b_p, grosor_select);
else sustentacion.draw(modo, r, g, b, grosor);

glTranslatef(0,(cabina.alto+sustentacion.alto)/2.0,0);
glRotatef(giro_cabina,0,1,0);
if (activo[1]==1) cabina.draw(EDGES, r_p, g_p, b_p, grosor_select);
else cabina.draw(modo, r, g, b, grosor);

glTranslatef(cabina.ancho/2.0,0,0);
glRotatef(giro_segundo_brazo,0,0,1);
if (activo[2]==1) brazo.draw(EDGES, r_p, g_p, b_p, grosor_select);
else brazo.draw(modo, r, g, b, grosor);

glTranslatef(brazo.ancho,0,0);
glRotatef(giro_primer_brazo,0,0,1);
if (activo[3]==1) brazo.draw(EDGES, r_p, g_p, b_p, grosor_select);
else brazo.draw(modo, r, g, b, grosor);

glTranslatef(brazo.ancho,0,0);
glRotatef(giro_pala,0,0,1);
glTranslatef(tamanio_pala,0,0);
glScalef(tamanio_pala, tamanio_pala, tamanio_pala);
if (activo[4]==1) pala.draw(EDGES, r_p, g_p, b_p, grosor_select);
else pala.draw(modo, r, g, b, grosor);

glPopMatrix();
};

//************************************************************************

void _excavadora::seleccion()
{
_vertex3i color;

glPushMatrix();
color=color_select[0];
sustentacion.draw(SELECT, color.r, color.g, color.b, 1);

glTranslatef(0,(cabina.alto+sustentacion.alto)/2.0,0);
glRotatef(giro_cabina,0,1,0);
color=color_select[1];
cabina.draw(SELECT, color.r, color.g, color.b, 1);

glTranslatef(cabina.ancho/2.0,0,0);
glRotatef(giro_segundo_brazo,0,0,1);
color=color_select[2];
brazo.draw(SELECT, color.r, color.g, color.b, 1);

glTranslatef(brazo.ancho,0,0);
glRotatef(giro_primer_brazo,0,0,1);
color=color_select[3];
brazo.draw(SELECT, color.r, color.g, color.b, 1);

glTranslatef(brazo.ancho,0,0);
glRotatef(giro_pala,0,0,1);
glTranslatef(tamanio_pala,0,0);
glScalef(tamanio_pala, tamanio_pala, tamanio_pala);
color=color_select[4];
pala.draw(SELECT, color.r, color.g, color.b, 1);

glPopMatrix();
};

//Práctica 3 Versión del alumno: objeto jerárquico articulado ALA-X (Star Wars)
/***********************************************************************************************/

void _triangulos3D::asignar_gama_metalicos(){
    float start = 1.0;
    float paso = 1.0 / colores_caras.size();

    for (int i = 0; i < colores_caras.size(); i++) {
        // Asignar el mismo valor a las componentes roja, verde y azul para obtener un tono de gris
        colores_caras[i].r = start;
        colores_caras[i].g = start;
        colores_caras[i].b = start;

        // Asegurar que el valor nunca sea menor que 0.0
        start = std::max(start - paso, 0.0f);
    }
}

//clase motor
_motor::_motor()
{
  radio = 0.24;
  altura = 0.55;
  num = 0.2;
  cilindro.asignar_gama_metalicos();
};

void _motor::draw(_modo modo, float r, float g, float b, float grosor)
{
    glPushMatrix();
    glRotatef(90.0f, 1.0f, 0.0f, 0.0f);
    //glTranslatef(0, 0, 0);
    glScalef(radio, altura, num);
    cilindro.draw(modo, r, g, b, grosor);
    glPopMatrix();
};

//clase cañon
_canion::_canion()
{

  radio = 0.24;
  altura = 0.55;
  num = 0.2;
  cilindro.asignar_gama_metalicos();

  radioMisil = 0.1;
  alturaMisil = 1.3;
  numMisil = 0.1;
  cilindroMisil.asignar_gama_metalicos();

};

void _canion::draw(_modo modo, float r, float g, float b, float grosor)
{
    glPushMatrix();
      glRotatef(90.0f, 1.0f, 0.0f, 0.0f);
      // glTranslatef(0, 0, altura/2);
      glScalef(radio, altura, num);
      cilindro.draw(modo, r, g, b, grosor);
    glPopMatrix();

    glPushMatrix();
      glTranslatef(0, 0, alturaMisil-disparo);
      glRotatef(90.0f, 1.0f, 0.0f, 0.0f);
      glScalef(radioMisil, alturaMisil, numMisil);
      cilindroMisil.draw(modo, r, g, b, grosor);
    glPopMatrix();
};

//clase ala
_ala::_ala()
{
  ancho=2.0;
  alto=0.1;
  fondo=1.05;
  
  cubo.asignar_gama_metalicos();
};

void _ala::draw(_modo modo, float r, float g, float b, float grosor)
{
    glPushMatrix();
      glTranslatef(-fondo, 0, fondo/2);
      glRotatef(180.0f, 0.0f, 1.0f, 0.0f);
      glScalef(ancho, alto, fondo);
      //glTranslatef(0.5,0,0);
      cubo.draw(modo, r, g, b, grosor);
    glPopMatrix();

    //ponemos motor
    glPushMatrix();
      glTranslatef(0, 0, fondo/2);
      motor.draw(modo, r, g, b, grosor);
    glPopMatrix();

    //ponemos cañon
    glPushMatrix();
      glTranslatef(-2*fondo, 0, fondo/2);
      canion.draw(modo, r, g, b, grosor);
    glPopMatrix();

};

//clase puerta
_puerta::_puerta()
{
  tronco.asignar_gama_metalicos();
};


void _puerta::draw(_modo modo, float r, float g, float b, float grosor)
{
    glPushMatrix();
    glTranslatef(0.0f, 0.0f, 0.5f);
      glScalef(1.0, 0.15, 1.0);
      tronco.draw(modo, r, g, b, grosor);
    glPopMatrix();
};

//clase para la cabina
 _cabinaX::_cabinaX()
 {
   cubo.asignar_gama_metalicos();
   tronco.asignar_gama_metalicos();
 };
 
 
void _cabinaX::draw(_modo modo, float r, float g, float b, float grosor)
{
   glPushMatrix();
     glScalef(1.0, 0.7, 1.0);
     cubo.draw(modo, r, g, b, grosor);
   glPopMatrix();

  glPushMatrix();
    glScalef(1.0, 0.7, 2.5);
    glTranslatef(0,0,0.2);
    glRotatef(90.0f, 1.0f, 0.0f, 0.0f);
    tronco.draw(modo, r, g, b, grosor);
  glPopMatrix();

};

//CLASE PARA LA PUNTA  

_punta::_punta()
{
  punta.asignar_gama_metalicos();
};

void _punta::draw(_modo modo, float r, float g, float b, float grosor)
{
  glPushMatrix();
    glRotatef(90.0f, 1.0f, 0.0f, 0.0f);
    glScalef(0.5, 0.5, 0.5);
    punta.draw(modo, r, g, b, grosor);
  glPopMatrix();
};


//**********************
//Montaje de modelo final
//***********************

_alaX::_alaX()
{
  int color=80;
piezas=7;
grosor_select=2;
color_pick=_vertex3f(1.0,0.0,0.0); 
color_select.resize(piezas);
activo.resize(piezas);

for (int i=0;i<piezas;i++)
  {activo[i]=0;
   color_select[i].r=color_select[i].g=color_select[i].b=color;
   color+=20;
  }

};

void _alaX::draw(_modo modo, float r, float g, float b, float grosor)
{
  float r_p,g_p,b_p;
	r_p=color_pick.r;
	g_p=color_pick.g;
	b_p=color_pick.b;


//montaje de la cabina
glPushMatrix();
  if(activo[0]==1)cabinaX.draw(modo, r_p, g_p, b_p, grosor);
  else cabinaX.draw(modo, r, g, b, grosor);
  //cabinaX.draw(modo, r, g, b, grosor);
glPopMatrix();

//montaje de la puerta
glPushMatrix();
  //glRotatef(3.0f, 1.0f, 0.0f, 0.0f);
  glTranslatef(0.0, 0.32, 0.9);
  glScalef(0.6, 0.6, 0.7);
  glRotatef(giroPuerta, 1.0f, 0.0f, 0.0f);
  if(activo[1]==1)puerta.draw(modo, r_p, g_p, b_p, grosor);
  else{
    puerta.draw(modo, r, g, b, grosor);
  }
glPopMatrix();

//montaje de la punta

glPushMatrix();
  glRotatef(giroPunta, 0.0f, 0.0f, 1.0f);
  glTranslatef(0, 0, 2.5);
  if(activo[2]==1)punta.draw(modo, r_p, g_p, b_p, grosor);
  else punta.draw(modo, r, g, b, grosor);
glPopMatrix();

//montaje de las 4 alas

//alaIzdaSup
glPushMatrix();
  glRotatef(-giroAla, 0.0f, 0.0f, 1.0f);
  glTranslatef(-0.7, 0.2, -0.5);
  //glRotatef(-15.0f, 0.0f, 0.0f, 1.0f); /*combate*/
  if(activo[3]==1)ala1.draw(modo, r_p, g_p, b_p, grosor);
  else ala1.draw(modo, r, g, b, grosor);
glPopMatrix();

//alaDchaSup
glPushMatrix();
  glRotatef(giroAla, 0.0f, 0.0f, 1.0f);
  glTranslatef(0.7, 0.2, -0.5);
  glRotatef(180.0f, 0.0f, 0.0f, 1.0f);
  //glRotatef(195.0f, 0.0f, 0.0f, 1.0f); /*combate*/
  if(activo[4]==1)ala2.draw(modo, r_p, g_p, b_p, grosor);
  else ala2.draw(modo, r, g, b, grosor);
glPopMatrix();


//alaIzdaInf
glPushMatrix();
glRotatef(giroAla, 0.0f, 0.0f, 1.0f);
  glTranslatef(-0.7, -0.2, -0.5);
  //glRotatef(15.0f, 0.0f, 0.0f, 1.0f); /*combate*/
  if(activo[5]==1)ala3.draw(modo, r_p, g_p, b_p, grosor);
  else ala3.draw(modo, r, g, b, grosor);
glPopMatrix();

//alaDchaInf
glPushMatrix();
  glRotatef(-giroAla, 0.0f, 0.0f, 1.0f);
  glTranslatef(0.7, -0.2, -0.5);
  glRotatef(-180.0f, 0.0f, 0.0f, 1.0f);
  //glRotatef(-195.0f, 0.0f, 0.0f, 1.0f); /*combate*/
  if(activo[6]==1)ala4.draw(modo, r_p, g_p, b_p, grosor);
  else ala4.draw(modo, r, g, b, grosor);
glPopMatrix();


};

//seleccion p5

void _alaX::seleccion()
{
_vertex3i color;

//////////////////////////////////////////////


glPushMatrix();
//montaje de la cabina
color=color_select[0];
cabinaX.draw(SELECT, color.r, color.g, color.b, 1);
glPopMatrix();

//montaje de la puerta
  //glRotatef(3.0f, 1.0f, 0.0f, 0.0f);
  glPushMatrix();
  glTranslatef(0.0, 0.32, 0.9);
  glScalef(0.6, 0.6, 0.7);
  glRotatef(giroPuerta, 1.0f, 0.0f, 0.0f);
  color=color_select[1];

  puerta.draw(SELECT, color.r, color.g, color.b, 1);
  glPopMatrix();

//montaje de la punta
glPushMatrix();
  glRotatef(giroPunta, 0.0f, 0.0f, 1.0f);
  glTranslatef(0, 0, 2.5);
  color=color_select[2];

  punta.draw(SELECT, color.r, color.g, color.b, 1);
  glPopMatrix();

//montaje de las 4 alas

//alaIzdaSup
glPushMatrix();
  glRotatef(-giroAla, 0.0f, 0.0f, 1.0f);
  glTranslatef(-0.7, 0.2, -0.5);
  //glRotatef(-15.0f, 0.0f, 0.0f, 1.0f); /*combate*/
  color=color_select[3];

  ala1.draw(SELECT, color.r, color.g, color.b, 1);
  glPopMatrix();

//alaDchaSup
glPushMatrix();
  glRotatef(giroAla, 0.0f, 0.0f, 1.0f);
  glTranslatef(0.7, 0.2, -0.5);
  glRotatef(180.0f, 0.0f, 0.0f, 1.0f);
  //glRotatef(195.0f, 0.0f, 0.0f, 1.0f); /*combate*/
  color=color_select[4];

  ala2.draw(SELECT, color.r, color.g, color.b, 1);
glPopMatrix();

//alaIzdaInf
glPushMatrix();
glRotatef(giroAla, 0.0f, 0.0f, 1.0f);
  glTranslatef(-0.7, -0.2, -0.5);
  //glRotatef(15.0f, 0.0f, 0.0f, 1.0f); /*combate*/
  color=color_select[5];

  ala3.draw(SELECT, color.r, color.g, color.b, 1);
  glPopMatrix();

//alaDchaInf
glPushMatrix();
  glRotatef(-giroAla, 0.0f, 0.0f, 1.0f);
  glTranslatef(0.7, -0.2, -0.5);
  glRotatef(-180.0f, 0.0f, 0.0f, 1.0f);
  //glRotatef(-195.0f, 0.0f, 0.0f, 1.0f); /*combate*/
  color=color_select[6];

  ala4.draw(SELECT, color.r, color.g, color.b, 1);
glPopMatrix();
};