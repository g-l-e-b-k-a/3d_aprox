#include <QtGui>
#include "window.h"
#include "algorithm.h"
#include <math.h>
#include <string.h>
#include <QDebug>

double f1 (double x, double y)
{
  (void)x;
  (void)y;
  return exp(x*x-y*y);//exp(x*x-y*y);//x+y;//x*0.+y*0.+1.;//x+y;//exp(x*x-y*y);//x+y;//x*0.+y*0.+1.;//exp(x*x-y*y);//x+y;//exp(x*x-y*y);//exp(x-y);//x*0.+y*0.+1.;//x+y;
}

double f2 (double x, double y)
{
  (void)x;
  (void)y;
  return 1.;//exp(x*x-y*y);//x+y;//x*0.+y*0.+1.;//x+y;//exp(x*x-y*y);//x+y;//x*0.+y*0.+1.;//exp(x*x-y*y);//x+y;//exp(x*x-y*y);//exp(x-y);//x*0.+y*0.+1.;//x+y;
}
double f3 (double x, double y)
{
  (void)x;
  (void)y;
  return x+y;//exp(x*x-y*y);//x+y;//x*0.+y*0.+1.;//x+y;//exp(x*x-y*y);//x+y;//x*0.+y*0.+1.;//exp(x*x-y*y);//x+y;//exp(x*x-y*y);//exp(x-y);//x*0.+y*0.+1.;//x+y;
}
double f4 (double x, double y)
{
  (void)x;
  (void)y;
  return cos(x)*sin(y);//exp(x*x-y*y);//x+y;//x*0.+y*0.+1.;//x+y;//exp(x*x-y*y);//x+y;//x*0.+y*0.+1.;//exp(x*x-y*y);//x+y;//exp(x*x-y*y);//exp(x-y);//x*0.+y*0.+1.;//x+y;
}
double f5 (double x, double y)
{
  (void)x;
  (void)y;
  return x*y;//exp(x*x-y*y);//x+y;//x*0.+y*0.+1.;//x+y;//exp(x*x-y*y);//x+y;//x*0.+y*0.+1.;//exp(x*x-y*y);//x+y;//exp(x*x-y*y);//exp(x-y);//x*0.+y*0.+1.;//x+y;
}
Scene3D::Scene3D (QWidget* parent) : QGLWidget(parent)
{
  xRot = -90; yRot = 0; zRot = 0; zTra = 0; nSca = 1;
  //N = 1;
  N_2 = 2;
  f = f1;
  what_to_draw = 0;
  all_triangles = 1;
  max_function = 1.;
  max_residual = 1.;
}

Scene3D::~Scene3D ()
{
}

void Scene3D::recount_algorithm ()
{
  int i;
  pthread_t tid;

  //fill arrays of triangulation

  //P = 2 * (N_2 + 1) * (N_2 + 1);
  //Выделить нужное количество памяти (сделать вектора нужного размера)
  if (update_arrays () < 0)
    return;

  //Вычислить координаты узлов и значений функции в них
  get_points ();
  
  double *a, *b, *ans;
  int *jnz;

  int len = P + 1 + get_nz_matrix (N_2, N_2,rect_params.n_i1,rect_params.n_i2,rect_params.n_j);

  if (   !(a   = new double [len])
      || !(jnz = new int [len])
      || !(b   = new double [P])
      || !(ans = new double [P]))
    return; /// \todo delete all initialized memory

  memset (a, 0, len * sizeof (double));
  memset (jnz, 0, len * sizeof (int));
  memset (b, 0, P * sizeof (double));
  memset (ans, 0, P * sizeof (double));

  init_matrix_b (points, b, P, f, N_2,rect_params.n_i1,rect_params.n_i2,rect_params.n_j, ((double)rect_params.a/(double)N_2)*((double)rect_params.b/(double)N_2)/2.);

  args *arg = new args [p];
  for (i = 0; i < p; i++)
    {
      arg[i].a = a;
      arg[i].p = p;
      arg[i].b = b;
      arg[i].k = i;
      arg[i].x = ans;
      arg[i].P = P;
      arg[i].N2 = N_2;
      arg[i].jnz = jnz;
      arg[i].points = points;
      arg[i].f = f;
      arg[i].width = rect_params.a;
      arg[i].height = rect_params.b;
      arg[i].n_i1 = rect_params.n_i1;
      arg[i].n_i2 = rect_params.n_i2;
      arg[i].n_j = rect_params.n_j;
      memcpy (arg[i].vertices, vertices, vertex_pos_COUNT * sizeof (double));
    }
  for (i = 1; i < p; i++)
    pthread_create (&tid, 0, use_algorithm, arg + i);
  use_algorithm (arg + 0);
  //x = x_new; // copy vector

  for (int idx = 0; idx < P; idx++){
    x[idx] = ans[idx];
    //printf("%f %f\n",x[idx],f(points[2*idx],points[2*idx+1]));
  };  

  updateGL();
  delete [] arg;
  delete [] a;
  delete [] jnz;
  delete [] b;
  delete [] ans;
 
  printf("n = %d\n",N_2); 
  
  getVertexArray (VertexArray_real, func.data (), 0, 0);
  getVertexArray (VertexArray_appr, x.data (), 0, 1);
  getVertexArray (VertexArray_residual, func.data (), x.data (), 2);
}

void Scene3D::drawFigure ()
{
  glLineWidth (1.0f);

  if (what_to_draw % 2 == 0)
    {
      glColor3f (0, 0, 255);
      draw_points_in_nodes (VertexArray_real);
      glColor3f (255, 0, 0);
      draw_points_in_nodes (VertexArray_appr);
    }
  if (what_to_draw % 2 == 1)
    {
      glColor3f (0, 1, 0);
      draw_points_in_nodes (VertexArray_residual);
    }
}

void Scene3D::paintGL ()
{
  max_function = func[0];
  for (int i = 1; i < P; i++)
    {
      if (func[i] > max_function)
        max_function = func[i];
    }
  glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  drawAxis ();
  drawFigure ();

  glMatrixMode (GL_MODELVIEW);
  glLoadIdentity();

  glScalef (nSca, nSca, nSca);
  glTranslatef (0.0f, zTra, 0.0f);
  glRotatef (xRot, 1.0f, 0.0f, 0.0f);
  glRotatef (yRot, 0.0f, 1.0f, 0.0f);
  glRotatef (zRot, 0.0f, 0.0f, 1.0f);
}

void Scene3D::get_points ()
{
  double x1 = vertices[bottom_left_x],
         y1 = vertices[bottom_left_y],
         x2 = vertices[top_right_x],
         y2 = vertices[top_right_y];

  double len_x = x2 - x1,
         len_y = y2 - y1,
         step_x = len_x / N_2,
         step_y = len_y / N_2;

  double curr_x = 0.,
         curr_y = 0.;

  /// Centers are always 0, and then in drawing parallel offset is added
  /*double x_center = 0.,
         y_center = 0.;*/

  for (int row = 0; row <= rect_params.n_i1; row++)
    {
      curr_y = y1 + row * step_y;
      for (int col = 0; col <= N_2; col++)
        {
          curr_x = x1 + col * step_x;
          
		  int ind = get_index (row, col, N_2, rect_params.n_i1, rect_params.n_i2, rect_params.n_j);
		  points[2 * ind] = curr_x;
		  points[2 * ind + 1] = curr_y;
            
        }
    }
    
    for (int row = rect_params.n_i1+1; row < rect_params.n_i2; row++)
    {
      curr_y = y1 + row * step_y;
      for (int col = 0; col <= rect_params.n_j; col++)
        {
          curr_x = x1 + col * step_x;
          
		  int ind = get_index (row, col, N_2, rect_params.n_i1, rect_params.n_i2, rect_params.n_j);
		  points[2 * ind] = curr_x;
		  points[2 * ind + 1] = curr_y;
            
        }
    }
    
    for (int row = rect_params.n_i2; row <= N_2; row++)
    {
      curr_y = y1 + row * step_y;
      for (int col = 0; col <= N_2; col++)
        {
          curr_x = x1 + col * step_x;
          
		  int ind = get_index (row, col, N_2, rect_params.n_i1, rect_params.n_i2, rect_params.n_j);
		  points[2 * ind] = curr_x;
		  points[2 * ind + 1] = curr_y;
            
        }
    }
    

  for (int i = 0; i < P; i++)
    func[i] = f (points[2 * i], points[2 * i + 1]);
}
