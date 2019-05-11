#ifndef WINDOW_H
#define WINDOW_H

#include <math.h>
#include <QWidget>
#include <QPushButton>
#include <QPainter>
#include <QLabel>
#include <QRadioButton>
#include <string.h>
#include <iostream>
#include <QGLWidget>
#include <QtOpenGL>

#include <QMainWindow>
#include <vector>

struct args;

enum vertex_pos
{
  bottom_left_x = 0,
  bottom_left_y,
  top_left_x,
  top_left_y,
  top_right_x,
  top_right_y,
  bottom_right_x,
  bottom_right_y,

  vertex_pos_COUNT
};

class Scene3D : public QGLWidget
{
public:
  Scene3D (QWidget* parent = 0);
  ~Scene3D ();
  //start prog
  int input_values (int argc, char *argv[]);
  //algorithm
  void recount_algorithm ();
  int update_arrays ();

private:
  void scale_plus();
  void scale_minus();
  void rotate_up();
  void rotate_down();
  void rotate_left();
  void rotate_right();
  void translate_down();
  void translate_up();
  void defaultScene();
  void drawAxis();

  void fill_vertex_array (std::vector<GLfloat> &VertexArray, double *x, double *func, int index1, int index2, int index3);
  void fill_vertex_array_half_points (std::vector<GLfloat> &VertexArray, int status, int index1,
                                      int index2, int index3);
  void getVertexArray (std::vector<GLfloat> &VertexArray, double *func, double *x, int status);
  void drawFigure ();
  void draw_points_in_nodes (std::vector<GLfloat> &VertexArray);

  void get_points ();

protected:
  void initializeGL();
  void resizeGL(int nWidth, int nHeight);
  void paintGL();
  void mousePressEvent(QMouseEvent* pe);
  void mouseMoveEvent(QMouseEvent* pe);
  void mouseReleaseEvent(QMouseEvent*);
  void wheelEvent(QWheelEvent* pe);
  void keyPressEvent(QKeyEvent* pe);

private:
  double vertices[vertex_pos_COUNT]; //vertices
  std::vector<double> points;               //points
  std::vector<double> half_points;          //halfs
  std::vector<double> func;                 //function
  std::vector<double> x;                    //answer
  int N;                        //how many triangulations
  int N_2;                       //2^N
  int p;                        //threads
  int P;                        //points
  int all_triangles;            //triangles
  double (*f) (double, double); //function
  int what_to_draw;             //what_to_draw
  double max_function;
  double max_residual;
  struct rect_paramteres
  {
    double x_center;
    double y_center;
    double a;
    double b;
    int n_i1, n_j1, n_i2, n_j2, n_i, n_j;
  } rect_params;

  std::vector<GLfloat> VertexArray_real;
  std::vector<GLfloat> VertexArray_residual;
  std::vector<GLfloat> VertexArray_appr;
  GLfloat xRot;
  GLfloat yRot;
  GLfloat zRot;
  GLfloat zTra;
  GLfloat nSca;

  QPoint ptrMousePosition;
};
 

#endif // WINDOW_H
