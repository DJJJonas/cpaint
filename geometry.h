#include <stdio.h>
#include <stdlib.h>

// TODO: separate function definitions from header files to .c files
// TODO: and rewrite Makefile)

typedef struct {
  int x;
  int y;
} Point;

Point *newPoint(int x, int y) {
  Point *p = (Point *)malloc(sizeof(Point));
  p->x = x;
  p->y = y;
  return p;
}

typedef struct pln {
  Point *point;
  struct pln *next;
} PointListNode;
typedef struct {
  PointListNode *head;
  PointListNode *tail;
} PointList;

PointList *newPointList() {
  PointList *list = (PointList *)malloc(sizeof(PointList));
  list->head = NULL;
  list->tail = NULL;
  return list;
}

void insertPoint(PointList *list, Point *p) {
  PointListNode *node = (PointListNode *)malloc(sizeof(PointListNode));
  node->point = p;
  node->next = NULL;
  if (list->head == NULL) {
    list->head = node;
    list->tail = node;
  } else {
    list->tail->next = node;
    list->tail = node;
  }
}

// if size == 1, then it is a point, if size == 2, then it is a line and so on
typedef struct {
  int size;
  PointList *points;
} GeometryForm;

GeometryForm *newGeometryForm(int size) {
  GeometryForm *form = (GeometryForm *)malloc(sizeof(GeometryForm));
  form->size = size;
  form->points = newPointList();
  return form;
}

// GeometryForm will also have a list struct
typedef struct gfn {
  GeometryForm *form;
  struct gfn *next;
} GeometryFormNode;

typedef struct {
  GeometryFormNode *head;
  GeometryFormNode *tail;
} GeometryFormList;

GeometryFormList *newGeometryFormList() {
  GeometryFormList *list = (GeometryFormList *)malloc(sizeof(GeometryFormList));
  list->head = NULL;
  list->tail = NULL;
  return list;
}

void insertGeometryForm(GeometryFormList *list, GeometryForm *form) {
  GeometryFormNode *node = (GeometryFormNode *)malloc(sizeof(GeometryFormNode));
  node->form = form;
  node->next = NULL;
  if (list->head == NULL) {
    list->head = node;
    list->tail = node;
  } else {
    list->tail->next = node;
    list->tail = node;
  }
}
