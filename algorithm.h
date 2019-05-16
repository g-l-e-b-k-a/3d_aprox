#ifndef ALGORITHM_H
#define ALGORITHM_H

#include <vector>
#include <pthread.h>
#include <stdio.h>

struct args
{
  double *a, *b, *x;
  std::vector<double> points;
  double vertices[8/*vertex_pos_COUNT*/];
  int *jnz;
  int N2, P, p, k;
  double (*f)(double, double);
  double width,height;
  int n_i1, n_i2, n_j;
};

int not_in_point(int *x, int *y, int P_i, int P_j, int num, int n);
void get_pos(int pos, int N_cols, int n_i1, int n_i2, int n_j, int &res_i, int &res_j);
void init_matrix_b (std::vector<double> &points, double *b, int P, double (*f)(double, double), int N_2, int n_i1, int n_i2, int n_j, double s);
void matr_mult (double *a, int *jnz, int n, double *x, double *b, int k, int p);
void sub_vect (int n, double *r, double tau, double *v, int k, int p);
void preconditioner (double *a /*int *jnz*/, int n, double *y, double *x, int k, int p);
double scalp (double *x, double *y, int n, int k, int p);
int solve (double *a, int *jnz, int n, double *b, double *x, double eps, int maxit, double *r, double *u, double *v, int k, int p);
void *use_algorithm (void *arg);

int init_matrix (double *&a, int *&jnz, int P, int N2, int k, int p, double s);

int get_index (int row, int col, int N_cols, int n_i1, int n_i2, int n_j);
int get_values (int i, int j, int n, int m, double s, double *a, int n_i1, int n_i2, int n_j);
int fill_matrix (int n, int m, int N, int NZ, double s, int *I, double *A, int k, int p, int n_i1, int n_i2, int n_j);
int fill_matrix_structure (int n, int m, int N, int NZ, int *I, int n_i1, int n_i2, int n_j);
int get_num_links (int i, int j, int n, int m, int n_i1, int n_i2, int n_j);
int get_links (int i, int j, int n, int m, int *x/*link_i*/, int *y/*link_j*/, int n_i1, int n_i2, int n_j);
int assemble_matrix (int n, int m, int *N, int **I, double **A, int k, int p, double w, double h, int n_i1, int n_i2, int n_j);

int get_nz_matrix (int n, int m, int n_i1, int n_i2, int n_j);

template <typename T>
void reduce_sum (int p, T *a = 0, int n = 0)
{
  static pthread_mutex_t m = PTHREAD_MUTEX_INITIALIZER;
  static pthread_cond_t c_in = PTHREAD_COND_INITIALIZER;
  static pthread_cond_t c_out = PTHREAD_COND_INITIALIZER;

  static int t_in = 0;
  static int t_out = 0;
  static T *p_a = 0;

  int i;

  if (p <= 1)
    return;

  pthread_mutex_lock (&m);
  if (!p_a)
    p_a = a;
  else if (a)
    for (i = 0; i < n; i++)
      p_a[i] += a[i];

  t_in++;
  if (t_in < p)
    while (t_in < p)
      pthread_cond_wait (&c_in, &m);
  else
    {
      t_out = 0;
      pthread_cond_broadcast (&c_in);
    }

  if (p_a != a)
    for (i = 0; i < n; i++)
      a[i] = p_a[i];

  t_out++;
  if (t_out < p)
    while (t_out < p)
      pthread_cond_wait (&c_out, &m);
  else
    {
      p_a = 0, t_in = 0;
      pthread_cond_broadcast (&c_out);
    }
  pthread_mutex_unlock (&m);
}

#endif // ALGORITHM_H

