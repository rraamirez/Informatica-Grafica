//**************************************************************************
// Práctica 3 
//**************************************************************************

#include <GL/glut.h>
#include <ctype.h>
#include <math.h>
#include <vector>
#include "objetos_B3.h"


using namespace std;

// tipos
typedef enum{CUBO, PIRAMIDE, TRONCO_PIRAMIDE, OBJETO_PLY, ROTACION, ROTACION_PLY, CILINDRO, CONO, ESFERA, EXTRUSION,
             MONTANA, EXCAVADORA, /*pruebas*/MOTOR, ALA, CANION, PUERTA, CABINA, PUNTA, /*MODELO FINAL*/ ALAX} _tipo_objeto;
_tipo_objeto t_objeto=CUBO;
_modo   modo=POINTS;

// variables que definen la posicion de la camara en coordenadas polares
GLfloat Observer_distance;
GLfloat Observer_angle_x;
GLfloat Observer_angle_y;

// variables que controlan la ventana y la transformacion de perspectiva
GLfloat Size_x,Size_y,Front_plane,Back_plane;

// variables que determninan la posicion y tamaño de la ventana X
int Window_x=50,Window_y=50,Window_width=650,Window_high=650;


// objetos
_cubo cubo;
_piramide piramide(0.85,1.3);
_objeto_ply  ply; 
_rotacion rotacion;
_rotacion_PLY rotacion_PLY;
_cilindro cilindro(1,2,6); 
_cono cono(1,2,6);
_esfera esfera(1,6,6);
_excavadora excavadora;
_extrusion *extrusion;
_montana montana(6,0.9, 0.8);

// _objeto_ply *ply;

float giro1=0.0, giro2=0.0, giro3=0.0;
int pulsar=0.0;
int paso=0;

/**Implementacion de pruebas*/
_motor motor;
_ala ala;
_canion canion;
_puerta puerta;
_cabinaX cabina;
_punta punta;
_alaX alaX;

//movimientos
float giro_puerta=0.0;
float giro_alas=0.0;
float giro_canion=0.0;
float giro_punta=0.0;
int paso_alax=0;
int pulsar_alax=0;


//**************************************************************************
//
//***************************************************************************

//prototipo de la función luces
void luces();
float alfa=0.0; //variable para la rotación de la luz



void clean_window()
{

glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
}


//**************************************************************************
// Funcion para definir la transformación de proyeccion
//***************************************************************************

void change_projection()
{

glMatrixMode(GL_PROJECTION);
glLoadIdentity();

// formato(x_minimo,x_maximo, y_minimo, y_maximo,plano_delantero, plano_traser)
//  plano_delantero>0  plano_trasero>PlanoDelantero)
glFrustum(-Size_x,Size_x,-Size_y,Size_y,Front_plane,Back_plane);
}

//**************************************************************************
// Funcion para definir la transformación*ply1 de vista (posicionar la camara)
//***************************************************************************

void change_observer()
{

// posicion del observador
glMatrixMode(GL_MODELVIEW);
glLoadIdentity();
glTranslatef(0,0,-Observer_distance);
glRotatef(Observer_angle_x,1,0,0);
glRotatef(Observer_angle_y,0,1,0);
}

//**************************************************************************
// Funcion que dibuja los ejes utilizando la primitiva grafica de lineas
//***************************************************************************

void draw_axis()
{
	
glDisable(GL_LIGHTING);
glLineWidth(2);
glBegin(GL_LINES);
// eje X, color rojo
glColor3f(1,0,0);
glVertex3f(-AXIS_SIZE,0,0);
glVertex3f(AXIS_SIZE,0,0);
// eje Y, color verde
glColor3f(0,1,0);
glVertex3f(0,-AXIS_SIZE,0);
glVertex3f(0,AXIS_SIZE,0);
// eje Z, color azul
glColor3f(0,0,1);
glVertex3f(0,0,-AXIS_SIZE);
glVertex3f(0,0,AXIS_SIZE);
glEnd();
}


//**************************************************************************
// Funcion que dibuja los objetos
//****************************2***********************************************

void draw_objects()
{

switch (t_objeto){
	case CUBO: cubo.draw(modo,1.0,0.0,0.0,5);break;
	case PIRAMIDE: piramide.draw(modo,1.0,0.0,0.0,5);break;
        case OBJETO_PLY: ply.draw(modo,1.0,0.6,0.0,3);break;
        case ROTACION: rotacion.draw(modo,1.0,0.0,0.0,5);break;
        case CILINDRO: cilindro.draw(modo,1.0,0.0,0.0,5);break;
        case CONO: cono.draw(modo,1.0,0.0,0.0,5);break;
        case ESFERA: esfera.draw(modo,1.0,0.0,0.0,5);break;
        case EXCAVADORA: excavadora.draw(modo,1.0,0.0,0.0,5);break;
        case EXTRUSION: extrusion->draw(modo,1.0,0.0,0.0,5);break;
        case MONTANA: montana.draw(modo,0.2,0.7,0.0,1);break;
        //IMPLEMENTACION DE PRUEBAS
        case MOTOR: motor.draw(modo,1.0,0.0,0.0,5);break;
        case ALA: ala.draw(modo,1.0,0.0,0.0,5);break;
        case CANION: canion.draw(modo,1.0,0.0,0.0,5);break;
        case PUERTA: puerta.draw(modo,1.0,0.0,0.0,5);break;
        case CABINA: cabina.draw(modo,1.0,0.0,0.0,5);break;
        case PUNTA: punta.draw(modo,1.0,0.0,0.0,5);break;
        //implementacion final ala-x
        case ALAX: alaX.draw(modo,1.0,0.0,0.0,5);break;
	}

}


//**************************************************************************
//
//***************************************************************************

void draw(void)
{
clean_window();
change_observer();
luces();
draw_axis();
draw_objects();
glutSwapBuffers();
}



//***************************************************************************
// Funcion llamada cuando se produce un cambio en el tamaño de la ventana
//
// el evento manda a la funcion:
// nuevo ancho
// nuevo alto
//***************************************************************************

void change_window_size(int Ancho1,int Alto1)
{
float Aspect_ratio;

Aspect_ratio=(float) Alto1/(float )Ancho1;
Size_y=Size_x*Aspect_ratio;
change_projection();
glViewport(0,0,Ancho1,Alto1);
glutPostRedisplay();
}

//***************************************************************************
// Funcion llamada cuando se aprieta una tecla normal
//
// el evento manda a la funcion:
// codigo de la tecla
// posicion x del raton
// posicion y del raton
//***************************************************************************

void normal_key(unsigned char Tecla1,int x,int y)
{
switch (toupper(Tecla1)){
	case 'Q':exit(0);
	case '1':modo=POINTS;break;
	case '2':modo=EDGES;break;
	case '3':modo=SOLID;break;
	case '4':modo=SOLID_COLORS;break;
    case '5':modo=ILUMINACION_PLANA;break;
    case '6':modo=ILUMINACION_GOURAUD;break;
    // 6 gouraud
    // 7 textura
    // I segunda luz
    // O y P rotacion positova y negativa de segunda luz
        case 'P':t_objeto=PIRAMIDE;break;
        case 'C':t_objeto=CUBO;break;
        case 'O':t_objeto=OBJETO_PLY;break;	
        case 'R':t_objeto=ROTACION;break;
        case 'L':t_objeto=CILINDRO;break;
        case 'N':t_objeto=CONO;break;
        case 'E':t_objeto=ESFERA;break;
        case 'X':t_objeto=EXTRUSION;break;
        case 'M':t_objeto=MONTANA;break;
        case 'A':t_objeto=ALAX;break;
        case 'S':if (pulsar_alax==0)
                    {giro_puerta=0.5;
                     giro_punta=1.0;
                     giro_alas=0.5;
                     giro_canion=0.01;
                     pulsar_alax=1;
                     }
                 else
                    {giro_puerta=0.0;
                     giro_punta=0.0;
                     giro_alas=0.0;
                     giro_canion=0.0;
                     pulsar_alax=0;
                     }    
                 break;
	}
glutPostRedisplay();
}

//***************************************************************************
// Funcion llamada cuando se aprieta una tecla especial
//
// el evento manda a la funcion:
// codigo de la tecla
// posicion x del raton
// posicion y del raton

//***************************************************************************

void special_key(int Tecla1,int x,int y)
{

switch (Tecla1){
   case GLUT_KEY_LEFT:Observer_angle_y--;break;
   case GLUT_KEY_RIGHT:Observer_angle_y++;break;
   case GLUT_KEY_UP:Observer_angle_x--;break;
   case GLUT_KEY_DOWN:Observer_angle_x++;break;
   case GLUT_KEY_PAGE_UP:Observer_distance*=1.2;break;
   case GLUT_KEY_PAGE_DOWN:Observer_distance/=1.2;break;
	
   case GLUT_KEY_F1:if(alaX.giroPuerta > alaX.giroPuertamax)alaX.giroPuerta-=5;break;
   case GLUT_KEY_F2:if(alaX.giroPuerta < alaX.giroPuertamin)alaX.giroPuerta+=5;
   case GLUT_KEY_F3:if(alaX.giroPunta < alaX.giroPuntaMax )alaX.giroPunta+=10; break;
   case GLUT_KEY_F4:
        if(alaX.giroAla < alaX.giroAlamax)alaX.giroAla+=5;
            break;
   case GLUT_KEY_F5:
    if(alaX.ala1.canion.disparo > 0.5){
            alaX.ala1.canion.disparo = 0.0;
            alaX.ala2.canion.disparo = 0.0;
            alaX.ala3.canion.disparo = 0.0;
            alaX.ala4.canion.disparo = 0.0;
            break;
        }

        alaX.ala1.canion.disparo+=0.1;
        alaX.ala2.canion.disparo+=0.1;
        alaX.ala3.canion.disparo+=0.1;
        alaX.ala4.canion.disparo+=0.1;
        break;

   case GLUT_KEY_F6:
        if(alaX.ala1.canion.disparo  < -0.5){
        alaX.ala1.canion.disparo = 0.0;
        alaX.ala2.canion.disparo = 0.0;
        alaX.ala3.canion.disparo = 0.0;
        alaX.ala4.canion.disparo = 0.0;
        break;
    }


        alaX.ala1.canion.disparo-=0.1;
        alaX.ala2.canion.disparo-=0.1;
        alaX.ala3.canion.disparo-=0.1;
        alaX.ala4.canion.disparo-=0.1;
        break;

    // así rotas la luz
    case GLUT_KEY_F9:
        alfa+=5;
        break;
	}

    

glutPostRedisplay();
}

//***************************************************************************
// Funcion llamada cuando se selecciona una opción del menú
//
// el evento manda a la funcion:
// entero que representa una entrada del menú 
//***************************************************************************

void menu(int key) {
    if (key == 0) {
        if (alaX.giroPuerta > alaX.giroPuertamax)
            alaX.giroPuerta -= 5;
    } else if (key == 1) {
        excavadora.giro_pala -= 4;
        if (excavadora.giro_pala < excavadora.giro_pala_min)
            excavadora.giro_pala = excavadora.giro_pala_min;
    } else if (key == 2) {
        if (pulsar_alax==0)
                    {giro_puerta=0.5;
                     giro_punta=1.0;
                     giro_alas=0.5;
                     giro_canion=0.01;
                     pulsar_alax=1;
                     }
                 else
                    {giro_puerta=0.0;
                     giro_punta=0.0;
                     giro_alas=0.0;
                     giro_canion=0.0;
                     pulsar_alax=0;
                     }    

    } else if (key == 3) {
        giro_puerta=0.0;
        giro_punta=0.0;
        giro_alas=0.0;
        giro_canion=0.0;
        pulsar_alax=0;
    }
}





//***************************************************************************
// Funcion de animación automática
//***************************************************************************

void animacion()
{
switch (paso){
  case 0:excavadora.giro_cabina-=giro1;
         if (excavadora.giro_cabina<-45) paso=1; break;
  case 1:excavadora.giro_primer_brazo-=giro3;
         if (excavadora.giro_primer_brazo<-80) paso=2; break;
  case 2: excavadora.giro_pala-=giro2;
        if (excavadora.giro_pala < excavadora.giro_pala_min)
           {excavadora.giro_pala = excavadora.giro_pala_min;
            paso=0;
            excavadora.giro_cabina=0.0;
            excavadora.giro_primer_brazo=0.0;
            excavadora.giro_pala=0.0;} 
        break;
   } 
  
glutPostRedisplay();
}


void animacion2()
{
int iters=0;
switch (paso_alax){
    case 0/*abrepuerta*/:alaX.giroPuerta-=giro_puerta;
            if (alaX.giroPuerta < alaX.giroPuertamax) paso_alax=1; break;
    case 1/*cierraPuerta*/:alaX.giroPuerta+=giro_puerta;
            if (alaX.giroPuerta > alaX.giroPuertamin) paso_alax=2; break;
    case 2/*giraPunta*/: 
            if(alaX.giroPunta < alaX.giroPuntaMax )alaX.giroPunta+=10;
            else paso_alax=3;
            break;
    case 3:
            if(alaX.giroAla < alaX.giroAlamax)alaX.giroAla+=giro_alas;
            else paso_alax=4;

            break;
    case 4:
        if(alaX.ala1.canion.disparo < 0.5){
            alaX.ala1.canion.disparo+=giro_canion;
            alaX.ala2.canion.disparo+=giro_canion;
            alaX.ala3.canion.disparo+=giro_canion;
            alaX.ala4.canion.disparo+=giro_canion;
        }
        else{
            alaX.ala1.canion.disparo = 0.0;
            alaX.ala2.canion.disparo = 0.0;
            alaX.ala3.canion.disparo = 0.0;
            alaX.ala4.canion.disparo = 0.0;
            paso_alax=5;
        }
        break;
    case 5:
       if(alaX.ala1.canion.disparo >  -0.5){
            alaX.ala1.canion.disparo-=giro_canion;
            alaX.ala2.canion.disparo-=giro_canion;
            alaX.ala3.canion.disparo-=giro_canion;
            alaX.ala4.canion.disparo-=giro_canion;
        }
        else{
            alaX.giroPunta = 0;
            alaX.giroAla = 0;
            alaX.ala1.canion.disparo=0;
            alaX.ala2.canion.disparo=0;
            alaX.ala3.canion.disparo=0;
            alaX.ala4.canion.disparo=0;
            paso_alax=0;
        }
        break;

}
glutPostRedisplay();
}


//***************************************************************************
// Funcion de incializacion
//***************************************************************************

void initialize(void)
{
// se inicalizan la ventana y los planos de corte
Size_x=0.5;
Size_y=0.5;
Front_plane=1;
Back_plane=1000;

// se incia la posicion del observador, en el eje z
Observer_distance=4*Front_plane;
Observer_angle_x=0;
Observer_angle_y=0;

// se indica el color para limpiar la ventana	(r,v,a,al)
// blanco=(1,1,1,1) rojo=(1,0,0,1), ...
glClearColor(1,1,1,1);

// se habilita el z-bufer
glEnable(GL_DEPTH_TEST);

change_projection();
glViewport(0,0,Window_width,Window_high);
}


void luces(){
    GLfloat luz_ambiental[]={0.05,0.05,0.05,1.0},
            luz_difusa[]={1.0,1.0,1.0,1.0},
            luz_especular[]={0.9,0.9,0.9,1.0},
            luz_posicion[]={0.0,0.0,20.0,1.0};

    GLfloat luz_ambiental2[]={0.05,0.05,0.05,1.0},
            luz_difusa2[]={1.0,1.0,1.0,1.0},
            luz_especular2[]={0.9,0.9,0.9,1.0},
            luz_posicion2[]={0.0,10.0,20.0,1.0};

    glLightfv(GL_LIGHT1,GL_AMBIENT,luz_ambiental);
    glLightfv(GL_LIGHT1,GL_DIFFUSE,luz_difusa);
    glLightfv(GL_LIGHT1,GL_SPECULAR,luz_especular);
    glLightfv(GL_LIGHT1,GL_POSITION,luz_posicion);

    glLightfv(GL_LIGHT2,GL_AMBIENT,luz_ambiental2);
    glLightfv(GL_LIGHT2,GL_DIFFUSE,luz_difusa2);
    glLightfv(GL_LIGHT2,GL_SPECULAR,luz_especular2);
    glLightfv(GL_LIGHT2,GL_POSITION,luz_posicion2);

    glPushMatrix();
    glRotatef(alfa, 0, 0, 1);
    glLightfv(GL_LIGHT2,GL_POSITION,luz_posicion2);
    glPopMatrix();

    glDisable(GL_LIGHT0);
    glEnable(GL_LIGHT1);
    glEnable(GL_LIGHT2);
    glEnable(GL_LIGHTING);
}

//***************************************************************************
// Programa principal
//
// Se encarga de iniciar la ventana, asignar las funciones e comenzar el
// bucle de eventos
//***************************************************************************


int main(int argc, char *argv[] )
{
 
// perfil 

vector<_vertex3f> perfil, poligono;
_vertex3f aux;

aux.x=1.0; aux.y=-1.0; aux.z=0.0;
perfil.push_back(aux);
aux.x=1.5; aux.y=0.0; aux.z=0.0;
perfil.push_back(aux);
aux.x=1.0; aux.y=1.0; aux.z=0.0;
perfil.push_back(aux);
aux.x=1.5; aux.y=1.2; aux.z=0.0;
perfil.push_back(aux);


rotacion.parametros(perfil,6,1,1,0);
rotacion_PLY.parametros_PLY("seta",10);


aux.x=1.0; aux.y=0.0; aux.z=1.0;
poligono.push_back(aux);
aux.x=1.0; aux.y=0.0; aux.z=-1.0;
poligono.push_back(aux);
aux.x=-1.2; aux.y=0.0; aux.z=-1.0;
poligono.push_back(aux);
aux.x=-0.8; aux.y=0.0; aux.z=0.0;
poligono.push_back(aux);
aux.x=-1.2; aux.y=0.0; aux.z=1.0;
poligono.push_back(aux);

extrusion= new _extrusion(poligono, 0.25, 1.0, 0.25);

// se llama a la inicialización de glut
glutInit(&argc, argv);

// se indica las caracteristicas que se desean para la visualización con OpenGL
// Las posibilidades son:
// GLUT_SIMPLE -> memoria de imagen simple
// GLUT_DOUBLE -> memoria de imagen doble
// GLUT_INDEX -> memoria de imagen con color indizado
// GLUT_RGB -> memoria de imagen con componentes rojo, verde y azul para cada pixel
// GLUT_RGBA -> memoria de imagen con componentes rojo, verde, azul y alfa para cada pixel
// GLUT_DEPTH -> memoria de profundidad o z-bufer
// GLUT_STENCIL -> memoria de estarcido_rotation Rotation;
glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);

// posicion de la esquina inferior izquierdad de la ventana
glutInitWindowPosition(Window_x,Window_y);

// tamaño de la ventana (ancho y alto)
glutInitWindowSize(Window_width,Window_high);

// llamada para crear la ventana, indicando el titulo (no se visualiza hasta que se llama
// al bucle de eventos)
glutCreateWindow("PRACTICA - 4 - Raul Ramirez");

// asignación de la funcion llamada "dibujar" al evento de dibujo
glutDisplayFunc(draw);
// asignación de la funcion llamada "change_window_size" al evento correspondiente
glutReshapeFunc(change_window_size);
// asignación de la funcion llamada "normal_key" al evento correspondiente
glutKeyboardFunc(normal_key);
// asignación de la funcion llamada "tecla_Especial" al evento correspondiente
glutSpecialFunc(special_key);

 // Creamos menú
glutCreateMenu(menu);
glutAddMenuEntry("Activar animación", 2);
glutAddMenuEntry("Desactivar animación", 3);
glutAttachMenu(GLUT_LEFT_BUTTON);


glutIdleFunc(animacion);
glutIdleFunc(animacion2);


// funcion de inicialización
initialize();

// creación del objeto ply
ply.parametros(argv[1]);

//ply = new _objeto_ply(argv[1]);

// inicio del bucle de eventos
glutMainLoop();
return 0;
}
