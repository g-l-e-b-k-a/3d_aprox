#include "window.h"
#include "algorithm.h"

#define EPS 1e-15
#define MAX_IT 5000

void get_my_rows (int n, int k, int p, int *i1, int *i2)
{
  *i1 = n * k;
  *i1 /= p;
  *i2 = n * (k + 1);
  *i2 = *i2 / p - 1;
}

int get_nz_matrix (int n, int m, int n_i1, int n_i2, int n_j)
{
  return   (n_i1 - 1) * (m - 1) * 6 // middle points
		 + (n_i2 - n_i1 + 1) * (n_j - 1) * 6
         + (n - n_i2 - 1) * (m - 1) * 6
         + 6
		 + 5
         + (n - 1) * 4 // left
         + (n - 1 - 2) * 4 // right 
         + 2 * (m - 1) *4 // up and down
         + 2 * (m - n_j - 1) * 4 // cutout up and down
         + 3 * 3 + 3 * 2; // vertex points
}

void matr_mult (double *a, int *jnz, int n, double *x, double *b, int k, int p)
{
  int i1, i2, i, start, len, j, jj;
  double s = 0;

	
  get_my_rows (n, k, p, &i1, &i2);

  for (i = i1; i <= i2; i++)
    {
      s = a[i] * x[i];
      start = jnz[i];
      len = jnz[i + 1] - jnz[i];
      for (j = 0; j < len; j++)
        {
          jj = start + j;
          s += a[jj] * x[jnz[jj]];
        }    
      b[i] = s;
    }

  reduce_sum <int>(p);
}

// u = u - tau * v
void sub_vect (int n, double *u, double tau, double *v, int k, int p)
{
  int i1, i2, i;

  get_my_rows (n, k, p, &i1, &i2);
  for (i = i1; i <= i2; i++)
    u[i] -= tau * v[i];

  reduce_sum <int> (p);
}

double scalp (double *x, double *y, int n, int k, int p)
{
  int i1, i2, i;
  double s = 0;
  get_my_rows (n, k, p, &i1, &i2);
  for (i = i1; i <= i2; i++)
    s += x[i] * y[i];
  reduce_sum <double> (p, &s, 1);
  return s;
}

// D * x = y => x = D^(-1) * y
void preconditioner (double *d, int n, double *y, double *x, int k, int p)
{
  int i1, i2, i;
  get_my_rows (n, k, p, &i1, &i2);
  for (i = i1; i <= i2; i++)
    x[i] = y[i] / d[i];
  reduce_sum <int> (p);
}


int solve (double *a, int *jnz, int n, double *b, double *x, double eps,
           int maxit, double *r, double *u, double *v, int k, int p)
{
  double c1, c2, tau;
  int it;

  // r = Ax - b => r = Ax, r -= 1 * b
  matr_mult  (a, jnz, n, x, r, k, p);
  sub_vect   (n, r, 1, b, k, p);
  c1 = scalp (r, r, n, k, p);

  if (fabs (c1) < eps * eps)
    {
      return 0;
    }

  for (it = 1; it < maxit; it++)
    {
      // u = D^(-1) * r
      preconditioner (a, n, r, u, k, p);

      // v = A * u
      matr_mult (a, jnz, n, u, v, k, p);

      c1 = scalp (v, r, n, k, p);
      c2 = scalp (v, v, n, k, p);

      if (fabs (c1) < eps * eps || fabs (c2) < eps * eps)
        {
          if (k == 0)
            printf ("iterations = %d\n", it/*, sqrt (c2)*/);
          return it;
        }
      tau = c1 / c2;
      sub_vect (n, r, tau, v, k, p);
      sub_vect (n, x, tau, u, k, p);
    }

  printf ("iterations = %d, GOT MAX ITERATIONS\n", maxit);
  return maxit;
}

void *use_algorithm (void *arg)
{
  args *ar = (args *) arg;
  
  static double *ce;

  int err = 0;
  static double *r, *u, *v;
  if (ar->k == 0)
    {
      r = new double  [ar->P];
      u = new double  [ar->P];
      v = new double  [ar->P];
      ce = new double [ar->P];
      memset (r, 0, ar->P * sizeof(double));
      memset (u, 0, ar->P * sizeof(double));
      memset (v, 0, ar->P * sizeof(double));
      memset (ce, 0, ar->P * sizeof(double));
    }
    
  int N = 0;
  
  err = assemble_matrix (ar->N2, ar->N2, &N, &ar->jnz, &ar->a, ar->k, ar->p, ar->width, ar->height, ar->n_i1, ar->n_i2, ar->n_j);
  
  reduce_sum<int> (ar->p, &err, 1);
  if (err)
    {
      if (ar->k == 0)
        printf ("Cannot assemble matrix: error %d\n", err);
      return 0;
    }
    
  matr_mult (ar->a, ar->jnz, N, ar->b, ce, ar->k, ar->p);
  
  reduce_sum <int> (ar->p);

  solve (ar->a, ar->jnz, N, ce, ar->x, EPS, MAX_IT, r, u, v, ar->k, ar->p);
  
  reduce_sum<int> (ar->p, &err, 1);

  if (ar->k == ar->p - 1)
    {
      delete [] r;
      delete [] u;
      delete [] v;
      delete [] ce;
    }
  return 0;
}
