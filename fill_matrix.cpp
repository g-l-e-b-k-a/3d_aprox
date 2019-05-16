#include "algorithm.h"

int get_index (int row, int col, int N_cols, int n_i1, int n_i2, int n_j)
{
	if(row<=n_i1) return row * (N_cols + 1) + col; // +1 because of borders of rectangle
	else if(row<n_i2) return (n_i1+1)*(N_cols + 1)+(row-n_i1-1)*(n_j+1)+col;
         else return (n_i1+1)*(N_cols + 1)+(n_i2-n_i1-1)*(n_j+1)+(row-n_i2)*(N_cols + 1)+col;
}

void get_pos(int pos, int N_cols, int n_i1, int n_i2, int n_j, int &res_i, int &res_j){  //(get_index)^(-1)
	if(pos<=(N_cols+1)*(n_i1+1)){
		res_i=pos/(N_cols+1);
		res_j=pos-res_i*(N_cols+1);
	}else if(pos<(N_cols+1)*(n_i1+1)+(n_i2-n_i1-1)*(n_j+1)){
		pos-=(N_cols+1)*(n_i1+1);
		res_i=n_i1+1+pos/(n_j+1);
		res_j=pos%(n_j+1);
	}else{
        pos-=(N_cols+1)*(n_i1+1)+(n_i2-n_i1-1)*(n_j+1);
		res_i=n_i2+pos/(N_cols+1);
		res_j=pos%(N_cols+1);
    }
} 

int not_in_point(int *x, int *y, int P_i, int P_j, int num, int n){
	for(int i=0;i<num;i++){
		switch(n){
			case 0:{ if(x[i]==P_i+1 && y[i]==P_j  ) return 0; }
			case 1:{ if(x[i]==P_i+1 && y[i]==P_j+1) return 0; }
			case 2:{ if(x[i]==P_i   && y[i]==P_j+1) return 0; }
			case 3:{ if(x[i]==P_i-1 && y[i]==P_j  ) return 0; }
			case 4:{ if(x[i]==P_i-1 && y[i]==P_j-1) return 0; }
			case 5:{ if(x[i]==P_i   && y[i]==P_j-1) return 0; }
		}
	}
	
	return 1;
}			

void init_matrix_b (std::vector<double> &points, double *b, int P, double (*f)(double, double), int N_2, int n_i1, int n_i2, int n_j, double s)
{
	N_2=N_2;
	n_i1=n_i1;
    n_i2=n_i2;
	n_j=n_j;
	s=s;
  for (int i = 0; i < P; i++)
    {
		b[i]=f(points[2 * i], points[2 * i + 1]);
		/*int P_i, P_j, num;
		int x[6], y[6];
		double s_N=0., s_Ai=0., s_NN=0.;
		
		get_pos(i,N_2,n_i,n_j,P_i,P_j);
		
		num=get_links(P_i,P_j,N_2,N_2,x,y,n_i,n_j);
		
		double arg_i_pred=0., arg_j_pred=0.;
		
		for(int j=0;j<num;j++){
			int pos=get_index(x[j],y[j],N_2,n_i,n_j);
			double arg_i=points[2 * i], arg_j=points[2 * i + 1];
			
			s_N+=f (points[2 * pos], points[2 * pos + 1]);
			if (x[j]!=P_i) arg_i=(points[2 * i]+points[2 * pos])/2.;
			if (y[j]!=P_j) arg_j=(points[2 * i + 1]+points[2 * pos + 1])/2.; 
			s_Ai+=f (arg_i, arg_j); 
			if(j>0) s_NN+= f((points[2 * pos]+arg_i_pred)/2., (points[2 * pos + 1]+arg_j_pred)/2.);
			arg_i_pred=points[2 * pos];
			arg_j_pred=points[2 * pos+1];
		}
		
		s_Ai+=(6-num)*f(points[2 * i], points[2 * i + 1]);
		
		//printf("s = %f\ns_Ai = %f\ns_NN = %f\ns_N = %f\n",s,s_Ai,s_NN,s_N);
		
		b[i]=1./4.*(3./2.*f(points[2 * i], points[2 * i + 1])+
				   5./6.*s_Ai+1./6.*s_NN+1./12.*s_N);
		//printf("%f\n",b[i]);*/		   
	}  
}

int get_values (int i, int j, int n, int m, double s, double *a, int n_i1, int n_i2, int n_j)
{
  if ((i > 0 && i < n_i1 && j > 0 && j < m) || (i>0 && i<n && j > 0 && j < n_j) || (i > n_i2 && i<n && j>0 && j<m))
    {
      a[0] = s / 2;
      a[1] = a[2] = a[3] = a[4] = a[5] = a[6] = s / 12;
      return 6;
    }
  if (i==n_i2 && j==n_j)
    {
      a[0] = 5.*s / 12;
      a[1] = a[2] = a[5] = a[6] = s / 12;
      a[3] = a[4] = s / 24;
      return 6;
    }
  if (i == 0 && j == 0)
    {
      a[0] = s / 6;
      a[1] = s / 24;
      a[2] = s / 12;
      a[3] = s / 24;
      return 3;
    }
  if ((i == n && j == m) || (i == n_i1 && j == m))
    {
      a[0] = s / 6;
      a[1] = s / 24;
      a[2] = s / 12;
      a[3] = s / 24;
      return 3;
    }
  if (i == n && j == 0)
    {
      a[0] = s / 12;
      a[1] = a[2] = s / 24;
      return 2;
    }
  if ((i == 0 && j == m) || (i == n_i2 && j == m))
    {
      a[0] = s / 12;
      a[1] = a[2] = s / 24;
      return 2;
    }
  
  if (i == n_i1 && j == n_j)
    {
      a[0] = s / 3.;
      a[2] = a[3] = a[4] = s/12.;
      a[1] = a[5] = s / 24;
      return 5;
    }  
    
  if ((i == 0) || (i==n_i2))
    {
      a[0] = s / 4;
      a[4] = s / 24;
      a[3] = a[2] = s / 12;
      a[1] = s / 24;
      return 4;
    }
    
  if (j == 0)
    {
      a[0] = s / 4;
      a[3] = a[2] = s / 12;
      a[1] = a[4] = s / 24;
      return 4;
    }  
    
  if (i == n || i==n_i1)
    {
      a[0] = s / 4;
      a[3] = a[2] = s / 12;
      a[1] = a[4] = s / 24;
      return 4;
    }
  if (j == m || j==n_j)
    {
      a[0] = s / 4;
      a[1] = s / 24;
      a[2] = a[3] = s / 12;
      a[4] = s / 24;
      return 4;
    }
  return -1;
}

int fill_matrix (int n, int m, int N, int NZ, double s, int *I, double *A, int k, int p, int n_i1, int n_i2, int n_j)
{
  int w, l, i, j, nz;
  double a[6/*max_nz*/ + 1];
  int err = 0;

  int i1 = k * N / p;
  int i2 = (k + 1) * N / p;

  for (l = i1; l < i2; l++)
    {
		if(l<(n_i1+1)*(m+1)){
		      i = l / (m + 1);
		      j = l - i * (m + 1);
		}else if(l<(n_i1+1)*(m+1)+(n_i2-n_i1-1)*(n_j+1)){
			 i = n_i1+1+(l-(n_i1+1)*(m+1)) / (n_j + 1);
		     j = (l-(n_i1+1)*(m+1)) % (n_j + 1); 
		}else{
             i = n_i2+(l-(n_i1+1)*(m+1)-(n_i2-n_i1-1)*(n_j+1)) / (m + 1);
             j = (l-(n_i1+1)*(m+1)-(n_i2-n_i1-1)*(n_j+1)) % (m + 1);
        }         
      nz = get_values (i, j, n, m, s, a, n_i1, n_i2, n_j);
      A[l] = a[0];
      if (I[l + 1] - I[l] != nz)
        {
          err = 1;
          break;
        }
      for (w = 0; w < nz; w++)
        A[I[l] + w] = a[w + 1];
    }
  if (I[N] != N + 1 + NZ)
    err++;
  reduce_sum<int> (p, &err, 1);
  return err;
}

int fill_matrix_structure (int n, int m, int N, int NZ, int *I, int n_i1, int n_i2, int n_j)
{
  int len = N + 1 + NZ;
  int i, j, k, l, rows = 0, pos = N + 1;
  int x[6/*max_nz*/], y[6/*max_nz*/];

  for (i = 0; i <= n_i1; i++)
    {
      for (j = 0; j <= m; j++)
        {
          I[rows] = pos;
          k = get_links (i, j, n, m, x, y, n_i1, n_i2, n_j);
          for (l = 0; l < k; l++)
            {
              I[pos + l] = get_index(x[l],y[l],n,n_i1,n_i2,n_j); 
            }
          pos += k;
          rows++;
        }
    }
    
  for (i = n_i1+1; i < n_i2; i++)
    {
      for (j = 0; j <= n_j; j++)
        {
          I[rows] = pos;
          k = get_links (i, j, n, m, x, y, n_i1, n_i2, n_j);
          for (l = 0; l < k; l++)
            {
              I[pos + l] = get_index(x[l],y[l],n,n_i1,n_i2,n_j); 
            }
          pos += k;
          rows++;
        }
    }
    
    for (i = n_i2; i <= n; i++)
    {
      for (j = 0; j <= m; j++)
        {
          I[rows] = pos;
          k = get_links (i, j, n, m, x, y, n_i1, n_i2, n_j);
          for (l = 0; l < k; l++)
            {
              I[pos + l] = get_index(x[l],y[l],n,n_i1,n_i2,n_j);
            }
          pos += k;
          rows++;
        }
    }  
  I[rows] = pos;
  if (pos != len){
    return -1;
  }
  return 0;
}

int get_num_links (int i, int j, int n, int m, int n_i, int n_j)
{
  if ((i > 0 && i < n && j > 0 && j < m) || (i>0 && i<n && j>0 && j<n_j))
    return 6;
  if(i==n_i && j==n_j)
	return 5;	
  if ((i == 0 && j == 0) || (i == n && j == n_j) || (i == n_i && j == m))
    return 3;
  if ((i == 0 && j == m) || (i == n && j == 0))
    return 2;
  return 4;
}

int get_links (int i, int j, int n, int m, int *x/*link_i*/, int *y/*link_j*/, int n_i1, int n_i2, int n_j)
{
  if ((i > 0 && i < n_i1 && j > 0 && j < m) || (i>0 && i<n && j > 0 && j < n_j) || (i > n_i2 && i<n && j>0 && j<m) || (i==n_i2 && j==n_j))
    {
      x[0] = i + 1;
      x[1] = i + 1;
      x[2] = i;
      x[3] = i - 1;
      x[4] = i - 1;
      x[5] = i;

      y[0] = j;
      y[1] = j + 1;
      y[2] = j + 1;
      y[3] = j;
      y[4] = j - 1;
      y[5] = j - 1;

      return 6;
    }

  if (i == 0 && j == 0)
    {
      x[0] = i + 1;
      y[0] = j;

      x[1] = i + 1;
      y[1] = j + 1;

      x[2] = i;
      y[2] = j + 1;

      return 3;
    }

  if ((i == n && j == m) || (i == n_i1 && j == m))
    {
      x[0] = i - 1;
      y[0] = j;

      x[1] = i - 1;
      y[1] = j - 1;

      x[2] = i;
      y[2] = j - 1;

      return 3;
    }
    
  if (i==n_i1 && j==n_j){
	  x[0] = i;
      y[0] = j + 1;

      x[1] = i - 1;
      y[1] = j;

      x[2] = i - 1;
      y[2] = j - 1;
      
      x[3] = i;
      y[3] = j - 1;
      
      x[4] = i + 1;
      y[4] = j;

      return 5; 
  }      

  if (i == n && j == 0)
    {
      x[0] = i;
      y[0] = j + 1;

      x[1] = i - 1;
      y[1] = j;

      return 2;
    }

  if ((i == 0 && j == m) || (i == n_i2 && j == m))
    {
      x[0] = i + 1;
      y[0] = j;

      x[1] = i;
      y[1] = j - 1;

      return 2;
    }

  if ((i == 0) || (i==n_i2))
    {
      x[0] = i;
      y[0] = j - 1;

      x[1] = i + 1;
      y[1] = j;

      x[2] = i + 1;
      y[2] = j + 1;

      x[3] = i;
      y[3] = j + 1;

      return 4;
    }

  if (j == 0)
    {
	  //Hello, bro
      x[0] = i+1;
      y[0] = j;

      x[1] = i + 1;
      y[1] = j + 1;

      x[2] = i;
      y[2] = j + 1;

      x[3] = i - 1;
      y[3] = j;

      return 4;
    }

  if (i == n || i==n_i1)
    {
      x[0] = i;
      y[0] = j + 1;

      x[1] = i - 1;
      y[1] = j;

      x[2] = i - 1;
      y[2] = j - 1;

      x[3] = i;
      y[3] = j - 1;

      return 4;
    }

  if (j == m || j == n_j)
    {
      x[0] = i;
      y[0] = j - 1;

      x[1] = i - 1;
      y[1] = j - 1;

      x[2] = i - 1;
      y[2] = j;

      x[3] = i + 1;
      y[3] = j;

      return 4;
    }

  return -1;
}

int assemble_matrix (int n, int m, int *N, int **I, double **A, int k, int p, double w, double h, int n_i1, int n_i2, int n_j)
{
  int NZ = get_nz_matrix (n, m, n_i1, n_i2, n_j);
  *N = (m + 1) * (n_i1 + 1) 
      + (n_j + 1) * (n_i2-n_i1-1) 
      + (m + 1) * (n - n_i2+1) ;
  if (NZ < 0)
    return -1;

  int err = 0;

  reduce_sum<int> (p);

  if (k == 0)
    err = fill_matrix_structure (n, m, *N, NZ, *I, n_i1, n_i2, n_j);

  reduce_sum<int> (p, &err, 1);
  if (err)
    return -3;

  //Hello, Bro
  /*double b = 1.,
         a = -1.,
         d = 1.,
         c = -1.;*/

  double hx = w / n,
         hy = h / m;

  double s = hx * hy;

  err = fill_matrix (n, m, *N, NZ, s, *I, *A, k, p, n_i1, n_i2, n_j);
  if (err)
    return -4;

  // check symmetry
  return 0;
}
