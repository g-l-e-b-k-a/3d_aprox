#include "window.h"

int Scene3D::input_values (int argc,char *argv[])
{
  if (argc != 2)
    {
      printf ("Usage: %s p\n", argv[0]);
      return -1;
    }

  if (!(p = atoi (argv[1])))
    {
      printf ("Wrong p\n");
      return -2;
    }

  if (p <= 0)
    {
      printf ("p needs to be >= 1\n");
      return -3;
    }
   
   //параметры рисуемого прямоугольника 
  rect_params.x_center=0.;
  rect_params.y_center=0.;   

    //длина и ширина прямоугольника
  printf ("Input width:");
  if (!scanf ("%lf", &rect_params.a))
    {
      printf ("wrong input\n");
      return -1;
    }

  printf ("Input height:");
  if (!scanf ("%lf", &rect_params.b))
    {
      printf ("wrong input\n");
      return -1;
    }
    
    //количество точек разбиения
 printf ("Input n:");
  if (!scanf ("%d", &N_2))
    {
      printf ("wrong input\n");
      return -1;
    }    
    
    //координаты выреза (целые числа - номера узлов)
  printf ("Input i1,i2,j of segment:");
  if (scanf ("%d%d%d", &rect_params.n_i1,&rect_params.n_i2,&rect_params.n_j)!=3)
    {
      printf ("wrong input\n");
      return -1;
    } 
  

  //init vertices

  vertices[bottom_left_x] = -rect_params.a/2.;
  vertices[bottom_left_y] = -rect_params.b/2.;

  vertices[bottom_right_x] = rect_params.a/2.;
  vertices[bottom_right_y] = -rect_params.b/2.;

  vertices[top_left_x] = -rect_params.a/2.;
  vertices[top_left_y] = rect_params.b/2.;

  vertices[top_right_x] = rect_params.a/2.;
  vertices[top_right_y] = rect_params.b/2.;

  //N_2 = 2;

  if (update_arrays () < 0)
    return -5;
  recount_algorithm ();
  return 1;
}

