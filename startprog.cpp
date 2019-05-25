#include "window.h"

int Scene3D::input_values (int argc,char *argv[])
{
  if (argc != 9)
    {
      printf ("Usage: %s p width height n i1 i2 j alpha\n", argv[0]);
      return -1;
    }

  if (!(p = atoi (argv[1])) || 
      ((rect_params.a = atof (argv[2])) <= 0) ||
      ((rect_params.b = atof (argv[3])) <= 0) ||
      !(N_2 = atoi (argv[4])) ||
      !(rect_params.n_i1 = atoi (argv[5])) ||
      !(rect_params.n_i2 = atoi (argv[6])) ||
      !(rect_params.n_j = atoi (argv[7])) ||
      !(alpha = atoi (argv[8])))
    {
      printf ("Wrong parameters\n");
      return -2;
    }
    
  if(! ((0<rect_params.n_i1) && (rect_params.n_i1<rect_params.n_i2) && (rect_params.n_i2<N_2))
     || ((rect_params.n_j <= 0) || (rect_params.n_j >= N_2)) || (alpha <= 1)) {
	  printf("Must be: 0 < i1 < n_i2 < n;    0 < n_j < n;   alpha > 1\n");
	  return -3;
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

