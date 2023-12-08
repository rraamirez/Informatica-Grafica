#include <vector>
#include <GL/glut.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <string>

const float AxisSize = 10000;

// NECESARIO PARA CARGAR LAS IMÁGENES DE DISCO
#include "CImg.h"
using namespace cimg_library;

using namespace std;

// Functions declarations
void Init();
void OnDraw();
void SpecialKeys(int key, int x, int y);

// FUNCTION TO PREPARE THE TEXTURE
void prepara_textura(char *file, GLuint *tex_id);

// FUNCTION TO DRAW
void dibuja_cubo();

// FUNCTION TO RELEASE THE GPU TEXTURE
void libera_textura(GLuint *tex_id);

// TEXTURE ID
GLuint textura_id;

// CUBE VERTICES AND TEXTURE COORDINATES
GLfloat cubeVertices[][3] = {
    {-0.7, -0.7, -0.7},
    {0.7, -0.7, -0.7},
    {0.7, 0.7, -0.7},
    {-0.7, 0.7, -0.7},
    {-0.7, -0.7, 0.7},
    {0.7, -0.7, 0.7},
    {0.7, 0.7, 0.7},
    {-0.7, 0.7, 0.7}};


GLfloat texCoords[][2] = {
    {0.0, 0.25},
    {1.0, 0.25},
    {1.0, 1.0},
    {0.0, 1.0}};

float latitud = 0.0, longitud = 0.0, radio = 5.0;

void Init(int argc, char **argv) {
    // Initialize GLUT window manager
    // Create the window
    glutInit(&argc, argv);
    glutInitWindowSize(800, 600);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutCreateWindow("Textured Cube");

    glEnable(GL_DEPTH_TEST);
    glMatrixMode(GL_PROJECTION);
    gluPerspective(40.0, 800 / 600.0f, 0.1, 150);

    prepara_textura("./skybox.jpg", &textura_id);
}

void draw_axis() {
    glLineWidth(2);
    glBegin(GL_LINES);
    // X-axis, red color
    glColor3f(1, 0, 0);
    glVertex3f(-AxisSize, 0, 0);
    glVertex3f(AxisSize, 0, 0);
    // Y-axis, green color
    glColor3f(0, 1, 0);
    glVertex3f(0, -AxisSize, 0);
    glVertex3f(0, AxisSize, 0);
    // Z-axis, blue color
    glColor3f(0, 0, 1);
    glVertex3f(0, 0, -AxisSize);
    glVertex3f(0, 0, AxisSize);
    glEnd();
    glLineWidth(1);
    glColor3f(1, 1, 1);
}

void OnDraw(void) {
    // Orbital camera
    float observador[3];
    float phir, thetar;
    phir = latitud * M_PI / 180.0;
    thetar = longitud * M_PI / 180.0;
    observador[0] = radio * sin(thetar) * cos(phir);
    observador[1] = radio * sin(phir);
    observador[2] = radio * cos(phir) * cos(thetar);

    // Cleaning the screen
    glClearColor(0.5, 0.5, 0.5, 1);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // Define view
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    // Eye position -> observador
    // Target -> (0,0,0)
    // Define positive Y axis -> (0.0, 1.0, 0.0)
    gluLookAt(observador[0], observador[1], observador[2], 0.0, 0.0, 0.0, 0.0, 1.0 * cos(phir), 0.0);

    draw_axis();
    // Put your code here to draw objects
    dibuja_cubo();

    // Do not delete this line
    glutSwapBuffers();
}

void SpecialKeys(int key, int x, int y) {
    switch (key) {
    case GLUT_KEY_LEFT:
        longitud += 2.0;
        break;
    case GLUT_KEY_RIGHT:
        longitud -= 2.0;
        break;
    case GLUT_KEY_UP:
        latitud += 2.0;
        break;
    case GLUT_KEY_DOWN:
        latitud -= 2.0;
        break;
    case GLUT_KEY_PAGE_UP:
        radio += 1.0;
        break;
    case GLUT_KEY_PAGE_DOWN:
        radio -= 1.0;
        if (radio < 1)
            radio = 1;
        break;
    }
    glutPostRedisplay();
}

int main(int argc, char **argv) {
    Init(argc, argv);
    // Enter the callbacks
    glutDisplayFunc(OnDraw);
    glutSpecialFunc(SpecialKeys);

    glutMainLoop(); // Begin the loop

    // Release the texture
    libera_textura(&textura_id);
    return 0;
}

// Function to prepare the texture from a file
void prepara_textura(char *file, GLuint *tex_id) {
    vector<unsigned char> data;
    CImg<unsigned char> image;
    image.load(file);

    // Pack the data properly
    for (long y = 0; y < image.height(); y++)
        for (long x = 0; x < image.width(); x++) {
            unsigned char *r = image.data(x, y, 0, 0);
            unsigned char *g = image.data(x, y, 0, 1);
            unsigned char *b = image.data(x, y, 0, 2);
            data.push_back(*r);
            data.push_back(*g);
            data.push_back(*b);
        }

    glGenTextures(1, tex_id);
    glBindTexture(GL_TEXTURE_2D, *tex_id);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    // Transfer the data to GPU
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, image.width(), image.height(), 0, GL_RGB, GL_UNSIGNED_BYTE, &data[0]);
}

void dibuja_cubo() {
    glEnable(GL_TEXTURE_2D);
    glEnableClientState(GL_VERTEX_ARRAY);
    glEnableClientState(GL_TEXTURE_COORD_ARRAY);

    glBindTexture(GL_TEXTURE_2D, textura_id);

    // Front face
    glBegin(GL_QUADS);
    glTexCoord2fv(texCoords[0]);
    glVertex3fv(cubeVertices[0]);
    glTexCoord2fv(texCoords[1]);
    glVertex3fv(cubeVertices[1]);
    glTexCoord2fv(texCoords[2]);
    glVertex3fv(cubeVertices[2]);
    glTexCoord2fv(texCoords[3]);
    glVertex3fv(cubeVertices[3]);
    glEnd();

    // Back face
    glBegin(GL_QUADS);
    glTexCoord2fv(texCoords[0]);
    glVertex3fv(cubeVertices[4]);
    glTexCoord2fv(texCoords[1]);
    glVertex3fv(cubeVertices[5]);
    glTexCoord2fv(texCoords[2]);
    glVertex3fv(cubeVertices[6]);
    glTexCoord2fv(texCoords[3]);
    glVertex3fv(cubeVertices[7]);
    glEnd();

    // Top face
    glBegin(GL_QUADS);
    glTexCoord2fv(texCoords[0]);
    glVertex3fv(cubeVertices[3]);
    glTexCoord2fv(texCoords[1]);
    glVertex3fv(cubeVertices[2]);
    glTexCoord2fv(texCoords[2]);
    glVertex3fv(cubeVertices[6]);
    glTexCoord2fv(texCoords[3]);
    glVertex3fv(cubeVertices[7]);
    glEnd();

    // Bottom face
    glBegin(GL_QUADS);
    glTexCoord2fv(texCoords[0]);
    glVertex3fv(cubeVertices[0]);
    glTexCoord2fv(texCoords[1]);
    glVertex3fv(cubeVertices[1]);
    glTexCoord2fv(texCoords[2]);
    glVertex3fv(cubeVertices[5]);
    glTexCoord2fv(texCoords[3]);
    glVertex3fv(cubeVertices[4]);
    glEnd();

    // Right face
    glBegin(GL_QUADS);
    glTexCoord2fv(texCoords[0]);
    glVertex3fv(cubeVertices[1]);
    glTexCoord2fv(texCoords[1]);
    glVertex3fv(cubeVertices[5]);
    glTexCoord2fv(texCoords[2]);
    glVertex3fv(cubeVertices[6]);
    glTexCoord2fv(texCoords[3]);
    glVertex3fv(cubeVertices[2]);
    glEnd();

    // Left face
    glBegin(GL_QUADS);
    glTexCoord2fv(texCoords[0]);
    glVertex3fv(cubeVertices[0]);
    glTexCoord2fv(texCoords[1]);
    glVertex3fv(cubeVertices[4]);
    glTexCoord2fv(texCoords[2]);
    glVertex3fv(cubeVertices[7]);
    glTexCoord2fv(texCoords[3]);
    glVertex3fv(cubeVertices[3]);
    glEnd();

    glDisableClientState(GL_VERTEX_ARRAY);
    glBindTexture(GL_TEXTURE_2D, 0);
    glDisable(GL_TEXTURE_2D);
}


// Function to release the GPU texture
void libera_textura(GLuint *tex_id) {
    glDeleteTextures(1, tex_id);
}
