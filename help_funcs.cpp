#include "window.h"
#include "algorithm.h"

void Scene3D::fill_vertex_array (std::vector<GLfloat> &VertexArray, double *x, double *func, int index1, int index2, int index3)
{
  VertexArray.push_back (points[2 * index1]);
  VertexArray.push_back (points[2 * index1 + 1]);
  if (x){
    VertexArray.push_back (func[index1] - x[index1]);
    //printf("%d %e\n",index1,func[index1] - x[index1]);
    }
  else
    VertexArray.push_back (func[index1]);

  VertexArray.push_back (points[2 * index2]);
  VertexArray.push_back (points[2 * index2 + 1]);
  if (x){
    VertexArray.push_back (func[index2] - x[index2]);
    //printf("%d %e\n",index2,func[index2] - x[index2]);
}
  else
    VertexArray.push_back (func[index2]);

  VertexArray.push_back (points[2 * index3]);
  VertexArray.push_back (points[2 * index3 + 1]);
  if (x){
    VertexArray.push_back (func[index3] - x[index3]);
    //printf("%d %e\n",index3,func[index3] - x[index3]);
}
  else
    VertexArray.push_back (func[index3]);  
}

void Scene3D::fill_vertex_array_half_points (std::vector<GLfloat> &VertexArray, int what_to_draw, int index1, int index2, int index3)
{
  double x1, x2;

  x1 = (points[2 * index1] + points[2 * index2]) / 2;
  x2 = (points[2 * index1 + 1] + points[2 * index2 + 1]) / 2;
  VertexArray.push_back (x1);
  VertexArray.push_back (x2);
  if (what_to_draw == 0)
    VertexArray.push_back (f (x1, x2));
  else if (what_to_draw == 1)
    VertexArray.push_back ((x[index1] + x[index2]) / 2);
  else{
    VertexArray.push_back (f (x1, x2) - (x[index1] + x[index2]) / 2);
    //printf("%d %d %e\n",index1, index2, f (x1, x2) - (x[index1] + x[index2]) / 2);
  }  

  x1 = (points[2 * index1] + points[2 * index3]) / 2;
  x2 = (points[2 * index1 + 1] + points[2 * index3 + 1]) / 2;
  VertexArray.push_back (x1);
  VertexArray.push_back (x2);

  if (what_to_draw == 0)
    VertexArray.push_back (f (x1, x2));
  else if (what_to_draw == 1)
    VertexArray.push_back ((x[index1] + x[index3]) / 2);
  else{
    VertexArray.push_back (f (x1, x2) - (x[index1] + x[index3]) / 2);
    //printf("%d %d %e\n",index1, index3,f (x1, x2) - (x[index1] + x[index3]) / 2);
  }  

  x1 = (points[2 * index2] + points[2 * index3]) / 2;
  x2 = (points[2 * index2 + 1] + points[2 * index3 + 1]) / 2;
  VertexArray.push_back (x1);
  VertexArray.push_back (x2);

  if (what_to_draw == 0)
    VertexArray.push_back (f (x1, x2));
  else if (what_to_draw == 1)
    VertexArray.push_back ((x[index2] + x[index3]) / 2);
  else{
    VertexArray.push_back (f (x1, x2) - (x[index2] + x[index3]) / 2);
    //printf("%d %d %e\n",index2, index3, f (x1, x2) - (x[index2] + x[index3]) / 2);
  }  
}


void Scene3D::getVertexArray (std::vector<GLfloat> &VertexArray, double *func, double *x, int what_to_draw)
{
  VertexArray.clear ();
  int row, col, index1, index2, index3;
  for (row = 0; row < N_2; row+=((N_2>128)?N_2/128:1))
    {
        int lim;
        if(row<rect_params.n_i1 || row>=rect_params.n_i2) lim=N_2;
        else lim=rect_params.n_j;
      for (col = 0; col < lim; col+=((N_2>128)?N_2/128:1))
        {
          index1 = get_index (row, col, N_2, rect_params.n_i1, rect_params.n_i2, rect_params.n_j);
          index2 = get_index (row + ((N_2>128)?N_2/128:1), col, N_2, rect_params.n_i1, rect_params.n_i2, rect_params.n_j);
          index3 = get_index (row + ((N_2>128)?N_2/128:1), col +((N_2>128)?N_2/128:1), N_2, rect_params.n_i1, rect_params.n_i2, rect_params.n_j);
          fill_vertex_array (VertexArray, x, func, index1, index2, index3);
          fill_vertex_array_half_points (VertexArray, what_to_draw, index1, index2, index3);

          index1 = get_index (row, col, N_2, rect_params.n_i1, rect_params.n_i2, rect_params.n_j);
          index2 = get_index (row, col + ((N_2>128)?N_2/128:1), N_2, rect_params.n_i1, rect_params.n_i2, rect_params.n_j);
          index3 = get_index ( row + ((N_2>128)?N_2/128:1), col + ((N_2>128)?N_2/128:1), N_2, rect_params.n_i1, rect_params.n_i2, rect_params.n_j);
          fill_vertex_array (VertexArray, x, func, index1, index2, index3);
          fill_vertex_array_half_points (VertexArray, what_to_draw, index1, index2, index3);
        }
    }  
    
  if(what_to_draw==2){  
	  
	  double max = 0;
	  
	  for (row = 0; row < N_2; row++)
	    {
            int lim;
            if(row<rect_params.n_i1 || row>=rect_params.n_i2) lim=N_2;
            else lim=rect_params.n_j;
	      for (col = 0; col < lim; col++)
	        {
				std::vector<GLfloat> BufArray;
	          index1 = get_index (row, col, N_2, rect_params.n_i1, rect_params.n_i2, rect_params.n_j);
	          index2 = get_index (row + 1, col, N_2, rect_params.n_i1, rect_params.n_i2, rect_params.n_j);
	          index3 = get_index (row + 1, col +1, N_2, rect_params.n_i1, rect_params.n_i2, rect_params.n_j);
	          fill_vertex_array (BufArray, x, func, index1, index2, index3);
	          fill_vertex_array_half_points (BufArray, what_to_draw, index1, index2, index3);
	
	          index1 = get_index (row, col, N_2, rect_params.n_i1, rect_params.n_i2, rect_params.n_j);
	          index2 = get_index (row, col + 1, N_2, rect_params.n_i1, rect_params.n_i2, rect_params.n_j);
	          index3 = get_index ( row + 1, col + 1, N_2, rect_params.n_i1, rect_params.n_i2, rect_params.n_j);
	          fill_vertex_array (BufArray, x, func, index1, index2, index3);
	          fill_vertex_array_half_points (BufArray, what_to_draw, index1, index2, index3);
	          
	          for(int k=2;k<(int)BufArray.size();k+=12)  
				if (fabs ((BufArray[k]+BufArray[k+3]+BufArray[k+9])/3.) > max)
	            max = fabs ((BufArray[k]+BufArray[k+3]+BufArray[k+9])/3.) ;
	        }
	    }
	    
		max_residual = max;
		printf ("Max residual = %e\n", max_residual);
	}      
}

int Scene3D::update_arrays ()
{
    //общее количество узлов
  P = (N_2 + 1) * (rect_params.n_i1 + 1) 
      + (rect_params.n_j + 1) * (rect_params.n_i2-rect_params.n_i1-1) 
      + (N_2 + 1) * (N_2 - rect_params.n_i2+1) ;

  all_triangles = 2 * N_2 * N_2;

  points.clear ();
  func.clear ();
  x.clear ();

  points.resize (2 * P);
  func.resize (P);
  x.resize (P);
  return 0;
}

void Scene3D::draw_points_in_nodes (std::vector<GLfloat> &VertexArray)
{
  int i;
  GLfloat x1, y1, z1, x2, y2, z2, x3, y3, z3;
  double masch=1.;
  
  //if(what_to_draw==2 && fabs(max_residual)>1e-16)
      //masch=1./fabs(max_residual);
  
  for (i = 0; i < (int)VertexArray.size () / 9; /*(N_2>4)?i+=N_2/4:*/i++)
    {
      x1 = VertexArray[9 * i + 0] + rect_params.x_center;
      x2 = VertexArray[9 * i + 3] + rect_params.x_center;
      x3 = VertexArray[9 * i + 6] + rect_params.x_center;

      y1 = VertexArray[9 * i + 1] + rect_params.y_center;
      y2 = VertexArray[9 * i + 4] + rect_params.y_center;
      y3 = VertexArray[9 * i + 7] + rect_params.y_center;

      z1 = VertexArray[9 * i + 2]*masch;
      z2 = VertexArray[9 * i + 5]*masch;
      z3 = VertexArray[9 * i + 8]*masch;   
            
      
      //printf("x1=%f y1=%f i=%d\nx2=%f y2=%f\nx3=%f y3=%f\n\n",x1,y1,i,x2,y2,x3,y3);

      glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
      glBegin(GL_TRIANGLES);
      glVertex3f (x1, y1, z1);
      glVertex3f (x2, y2, z2);
      glVertex3f (x3, y3, z3);
      glEnd();
    }
}




void Scene3D::change_numb_of_p(const QString &buf){
	int c;
	c=buf.toInt();
	if(c>1){
		N_2=c;
	/*	
		if(f){
			delete[] f;
			delete[] x;
			delete[] d;
			delete [] alpha;
		}	
		
		f=new double[4*numb_of_p];
	    x=new double[numb_of_p];
	    d=new double[numb_of_p];
	    alpha = new double [numb_of_p];
	  
	    double buff=(b-a)/(double)(numb_of_p-1);
	    for(int i=0;i<numb_of_p-1;i++) x[i]=a+i*buff;
			  x[numb_of_p-1]=b;	
			  
		build_func();	  
		
		update();*/
	
	
		update_arrays ();
		recount_algorithm ();	
	}	
}
