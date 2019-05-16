#include "window.h"

int Scene3D::input_values (int argc,char *argv[])
{
  if (argc != 8)
    {
      printf ("Usage: %s p width height n i1 i2 j \n", argv[0]);
      return -1;
    }

  if (!(p = atoi (argv[1])) || 
      !(rect_params.a = atof (argv[2])) ||
      !(rect_params.b = atof (argv[3])) || 
      !(N_2 = atoi (argv[4])) ||
      !(rect_params.n_i1 = atoi (argv[5])) ||
      !(rect_params.n_i2 = atoi (argv[6])) ||
      !(rect_params.n_j = atoi (argv[7])))
    {
      printf ("Wrong parameters\n");
      return -2;
    }
   
   //параметры рисуемого прямоугольника 
  rect_params.x_center=0.;
  rect_params.y_center=0.;     

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

