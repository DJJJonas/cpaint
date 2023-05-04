#include "geometry.h"
#include <GL/glut.h>
#include <stdio.h>
#include <stdlib.h>

#define MAX_PONTOS 100
#define MAX_PONTOS_POLIGONO 100
#define MAX_POLIGONOS 100

#define WIDTH 600
#define HEIGHT 500

#define MODE_SELECT 0
#define MODE_POINT 1
#define MODE_LINE 2
#define MODE_POLYGON 3

typedef struct {
  GeometryForm *form;
  int count;
} PendingGeometryForm;

PendingGeometryForm *newPendingGeometryForm(int count) {
  PendingGeometryForm *p = malloc(sizeof(PendingGeometryForm));
  p->count = count;
  p->form = newGeometryForm(count);
  return p;
}

int mode = 1;
GeometryFormList *geometryList = NULL;
PendingGeometryForm *pendingGeometryForm = NULL;

void freePendingGeometryForm() {
  free(pendingGeometryForm);
  pendingGeometryForm = NULL;
}

void drawGeometryForms() {
  GeometryFormNode *node = geometryList->head;
  while (node != NULL) {
    if (node->form->size == 1) { // GeometryForm is a point
      glPointSize(5.0);
      glColor3f(1.0, 1.0, 1.0);
      glBegin(GL_POINTS);
      Point *point = node->form->points->head->point;
      glVertex2i(point->x, point->y);
      glEnd();
    } else if (node->form->size == 2) { // GeometryForm is a line
      glLineWidth(3.0);
      glColor3f(1.0, 1.0, 1.0);
      glBegin(GL_LINES);
      Point *p1 = node->form->points->head->point;
      Point *p2 = node->form->points->tail->point;
      glVertex2i(p1->x, p1->y);
      glVertex2i(p2->x, p2->y);
      glEnd();
    } else if (node->form->size > 2) { // GeometryForm is a polygon
      glLineWidth(3.0);
      glColor3f(1.0, 1.0, 1.0);
      glBegin(GL_LINE_LOOP);
      PointListNode *pointNode = node->form->points->head;
      while (pointNode != NULL) {
        Point *point = pointNode->point;
        glVertex2i(point->x, point->y);
        pointNode = pointNode->next;
      }
      glEnd();
    }
    node = node->next;
  }
}

void init() {
  glClearColor(0.0, 0.0, 0.0, 0.0);
  glMatrixMode(GL_PROJECTION);
  gluOrtho2D(0.0, WIDTH, 0.0, HEIGHT);
}

//------MOUSE------
void mouse(int button, int state, int x, int y) {
  // Left mouse button pressed
  if (button == GLUT_LEFT_BUTTON && state == GLUT_UP) {
    if (mode == MODE_POINT) {
      GeometryForm *g = newGeometryForm(1);
      Point *p = newPoint(x, HEIGHT - y);
      insertPoint(g->points, p);
      insertGeometryForm(geometryList, g);
      drawGeometryForms();
    } else if (mode == MODE_LINE) {
      if (pendingGeometryForm == NULL) {
        pendingGeometryForm = newPendingGeometryForm(2);
        Point *p = newPoint(x, HEIGHT - y);
        insertPoint(pendingGeometryForm->form->points, p);
        pendingGeometryForm->count--;
      } else {
        Point *p = newPoint(x, HEIGHT - y);
        GeometryForm *g = pendingGeometryForm->form;
        insertPoint(g->points, p);
        insertGeometryForm(geometryList, g);
        freePendingGeometryForm();
        drawGeometryForms();
      }
    }
    // TODO: add polygon mode and maybe rework pendingGeometryForm
  }
  glutPostRedisplay();
}

//------MENU------
void optionMenu(int opcao) {
  pendingGeometryForm = NULL;
  switch (opcao) {
  case MODE_SELECT:
    mode = MODE_SELECT;
    break;
  case MODE_POINT:
    mode = MODE_POINT;
    break;
  case MODE_LINE:
    mode = MODE_LINE;
    break;
  case MODE_POLYGON:
    mode = MODE_POLYGON;
    break;
  default:
    printf("Invalid option\n");
    break;
  }
  glutPostRedisplay();
}

void display() {
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();
  glFlush();
}

//------MAIN-------

int main(int argc, char **argv) {
  geometryList = newGeometryFormList();

  glutInit(&argc, argv);
  glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
  glutInitWindowPosition(10, 10);
  glutInitWindowSize(WIDTH, HEIGHT);
  glutCreateWindow("PAINT");
  init();
  glClear(GL_COLOR_BUFFER_BIT);
  glutDisplayFunc(display);
  glutMouseFunc(mouse);

  glutCreateMenu(optionMenu);
  glutAddMenuEntry("SELECIONAR OBJETO", MODE_SELECT);
  glutAddMenuEntry("PONTO", MODE_POINT);
  glutAddMenuEntry("RETA", MODE_LINE);
  glutAddMenuEntry("POLIGONO", MODE_POLYGON);

  glutAttachMenu(GLUT_RIGHT_BUTTON);

  glutMainLoop();
  return 0;
}
