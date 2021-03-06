#ifndef WINDOW_H
#define WINDOW_H

#include <math.h>
#include <limits.h>
#include <cfloat>
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
    Q_OBJECT
public:
  Scene3D (QWidget* parent = 0);
  ~Scene3D ();
  //start prog
  int input_values (int argc, char *argv[]);
  //algorithm
  void recount_algorithm ();
  int update_arrays ();
  int alpha;

private:
  int func_number;
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
  void fill_vertex_array_half_points (std::vector<GLfloat> &VertexArray, int what_to_draw, int index1,
                                      int index2, int index3);
  void getVertexArray (std::vector<GLfloat> &VertexArray, double *func, double *x, int what_to_draw);
  void drawFigure ();
  void draw_points_in_nodes (std::vector<GLfloat> &VertexArray);

  void get_points ();
public slots:
  void change_numb_of_p(const QString &buf);
  void act_al1();
  void act_al2();
  void change_func ();
  void func1();
  void func2();
  void func3();
  void func4();
  void func5();
  void keyPressEvent(QKeyEvent* pe);
protected:
  void initializeGL();
  void resizeGL(int nWidth, int nHeight);
  void paintGL();
  void mousePressEvent(QMouseEvent* pe);
  void mouseMoveEvent(QMouseEvent* pe);
  void mouseReleaseEvent(QMouseEvent*);
  void wheelEvent(QWheelEvent* pe);
  //void keyPressEvent(QKeyEvent* pe);
  

private:
  double vertices[vertex_pos_COUNT]; //vertices
  std::vector<double> points;               //points
  std::vector<double> half_points;          //halfs
  std::vector<double> func;                 //function
  std::vector<double> x;                    //answer
  //int N;                        //how many triangulations
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
    int n_i1, n_i2, n_j;
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
 
 
class MainWindow : public QMainWindow // класс MainWindow как подкласс QMainWindow
{ 
   Q_OBJECT // макрос, который нужно использовать при работе с сигналами и слотами
              
   public:   
      MainWindow(); 
      Scene3D* draw_area;  // конструктор объекта главного окна
   protected:
      void keyPressEvent(QKeyEvent* pe);
}; 
 
 


double f1 (double x, double y);
double f2 (double x, double y);
double f3 (double x, double y);
double f4 (double x, double y);
double f5 (double x, double y);
#endif // WINDOW_H
