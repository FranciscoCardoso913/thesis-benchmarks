/**
 * This version is stamped on May 10, 2016
 *
 * Contact:
 *   Louis-Noel Pouchet <pouchet.ohio-state.edu>
 *   Tomofumi Yuki <tomofumi.yuki.fr>
 *
 * Web address: http://polybench.sourceforge.net
 */
/* atax.c: this file is part of PolyBench/C */

#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <math.h>

/* Include polybench common header. */
/**
 * This version is stamped on May 10, 2016
 *
 * Contact:
 *   Louis-Noel Pouchet <pouchet.ohio-state.edu>
 *   Tomofumi Yuki <tomofumi.yuki.fr>
 *
 * Web address: http://polybench.sourceforge.net
 */
/*
 * polybench.h: this file is part of PolyBench/C
 *
 * Polybench header for instrumentation.
 *
 * Programs must be compiled with `-I utilities utilities/polybench.c'
 *
 * Optionally, one can define:
 *
 * -DPOLYBENCH_TIME, to report the execution time,
 *   OR (exclusive):
 * -DPOLYBENCH_PAPI, to use PAPI H/W counters (defined in polybench.c)
 *
 *
 * See README or utilities/polybench.c for additional options.
 *
 */
#ifndef POLYBENCH_H
# define POLYBENCH_H

# include <stdlib.h>

/* Array padding. By default, none is used. */
# ifndef POLYBENCH_PADDING_FACTOR
/* default: */
#  define POLYBENCH_PADDING_FACTOR 0
# endif

/* Inter-array padding, for use with . By default, none is used. */
# ifndef POLYBENCH_INTER_ARRAY_PADDING_FACTOR
/* default: */
#  define POLYBENCH_INTER_ARRAY_PADDING_FACTOR 0
#  undef POLYBENCH_ENABLE_INTARRAY_PAD
# else
#  define POLYBENCH_ENABLE_INTARRAY_PAD
# endif


/* C99 arrays in function prototype. By default, do not use. */
# ifdef POLYBENCH_USE_C99_PROTO
#  define POLYBENCH_C99_SELECT(x,y) y
# else
/* default: */
#  define POLYBENCH_C99_SELECT(x,y) x
# endif


/* Scalar loop bounds in SCoPs. By default, use parametric loop bounds. */
# ifdef POLYBENCH_USE_SCALAR_LB
#  define POLYBENCH_LOOP_BOUND(x,y) x
# else
/* default: */
#  define POLYBENCH_LOOP_BOUND(x,y) y
# endif

/* Use the 'restrict' keyword to declare that the different arrays do not
 * alias. By default, we do not use it as it is only supported in C99 and
 * even here several compilers do not properly get it.
 */
# ifdef POLYBENCH_USE_RESTRICT
#  define POLYBENCH_RESTRICT restrict
# else
/* default: */
#  define POLYBENCH_RESTRICT
# endif

/* Macros to reference an array. Generic for heap and stack arrays
   (C99).  Each array dimensionality has his own macro, to be used at
   declaration or as a function argument.
   Example:
   int b[x] => POLYBENCH_1D_ARRAY(b, x)
   int A[N][N] => POLYBENCH_2D_ARRAY(A, N, N)
*/
# ifndef POLYBENCH_STACK_ARRAYS
#  define POLYBENCH_ARRAY(x) *x
#  ifdef POLYBENCH_ENABLE_INTARRAY_PAD
#   define POLYBENCH_FREE_ARRAY(x) polybench_free_data((void*)x);
#  else
#   define POLYBENCH_FREE_ARRAY(x) free((void*)x);
#  endif
#  define POLYBENCH_DECL_VAR(x) (*x)
# else
#  define POLYBENCH_ARRAY(x) x
#  define POLYBENCH_FREE_ARRAY(x)
#  define POLYBENCH_DECL_VAR(x) x
# endif
/* Macros for using arrays in the function prototypes. */
# define POLYBENCH_1D(var, dim1,ddim1) var[POLYBENCH_RESTRICT POLYBENCH_C99_SELECT(dim1,ddim1) + POLYBENCH_PADDING_FACTOR]
# define POLYBENCH_2D(var, dim1, dim2, ddim1, ddim2) var[POLYBENCH_RESTRICT POLYBENCH_C99_SELECT(dim1,ddim1) + POLYBENCH_PADDING_FACTOR][POLYBENCH_C99_SELECT(dim2,ddim2) + POLYBENCH_PADDING_FACTOR]
# define POLYBENCH_3D(var, dim1, dim2, dim3, ddim1, ddim2, ddim3) var[POLYBENCH_RESTRICT POLYBENCH_C99_SELECT(dim1,ddim1) + POLYBENCH_PADDING_FACTOR][POLYBENCH_C99_SELECT(dim2,ddim2) + POLYBENCH_PADDING_FACTOR][POLYBENCH_C99_SELECT(dim3,ddim3) + POLYBENCH_PADDING_FACTOR]
# define POLYBENCH_4D(var, dim1, dim2, dim3, dim4, ddim1, ddim2, ddim3, ddim4) var[POLYBENCH_RESTRICT POLYBENCH_C99_SELECT(dim1,ddim1) + POLYBENCH_PADDING_FACTOR][POLYBENCH_C99_SELECT(dim2,ddim2) + POLYBENCH_PADDING_FACTOR][POLYBENCH_C99_SELECT(dim3,ddim3) + POLYBENCH_PADDING_FACTOR][POLYBENCH_C99_SELECT(dim4,ddim4) + POLYBENCH_PADDING_FACTOR]
# define POLYBENCH_5D(var, dim1, dim2, dim3, dim4, dim5, ddim1, ddim2, ddim3, ddim4, ddim5) var[POLYBENCH_RESTRICT POLYBENCH_C99_SELECT(dim1,ddim1) + POLYBENCH_PADDING_FACTOR][POLYBENCH_C99_SELECT(dim2,ddim2) + POLYBENCH_PADDING_FACTOR][POLYBENCH_C99_SELECT(dim3,ddim3) + POLYBENCH_PADDING_FACTOR][POLYBENCH_C99_SELECT(dim4,ddim4) + POLYBENCH_PADDING_FACTOR][POLYBENCH_C99_SELECT(dim5,ddim5) + POLYBENCH_PADDING_FACTOR]
/* Macros for using arrays within the functions. */
# define POLYBENCH_1D_F(var, dim1,ddim1) var[POLYBENCH_C99_SELECT(dim1,ddim1) + POLYBENCH_PADDING_FACTOR]
# define POLYBENCH_2D_F(var, dim1, dim2, ddim1, ddim2) var[POLYBENCH_C99_SELECT(dim1,ddim1) + POLYBENCH_PADDING_FACTOR][POLYBENCH_C99_SELECT(dim2,ddim2) + POLYBENCH_PADDING_FACTOR]
# define POLYBENCH_3D_F(var, dim1, dim2, dim3, ddim1, ddim2, ddim3) var[POLYBENCH_C99_SELECT(dim1,ddim1) + POLYBENCH_PADDING_FACTOR][POLYBENCH_C99_SELECT(dim2,ddim2) + POLYBENCH_PADDING_FACTOR][POLYBENCH_C99_SELECT(dim3,ddim3) + POLYBENCH_PADDING_FACTOR]
# define POLYBENCH_4D_F(var, dim1, dim2, dim3, dim4, ddim1, ddim2, ddim3, ddim4) var[POLYBENCH_C99_SELECT(dim1,ddim1) + POLYBENCH_PADDING_FACTOR][POLYBENCH_C99_SELECT(dim2,ddim2) + POLYBENCH_PADDING_FACTOR][POLYBENCH_C99_SELECT(dim3,ddim3) + POLYBENCH_PADDING_FACTOR][POLYBENCH_C99_SELECT(dim4,ddim4) + POLYBENCH_PADDING_FACTOR]
# define POLYBENCH_5D_F(var, dim1, dim2, dim3, dim4, dim5, ddim1, ddim2, ddim3, ddim4, ddim5) var[POLYBENCH_C99_SELECT(dim1,ddim1) + POLYBENCH_PADDING_FACTOR][POLYBENCH_C99_SELECT(dim2,ddim2) + POLYBENCH_PADDING_FACTOR][POLYBENCH_C99_SELECT(dim3,ddim3) + POLYBENCH_PADDING_FACTOR][POLYBENCH_C99_SELECT(dim4,ddim4) + POLYBENCH_PADDING_FACTOR][POLYBENCH_C99_SELECT(dim5,ddim5) + POLYBENCH_PADDING_FACTOR]


/* Macros to allocate heap arrays.
   Example:
   polybench_alloc_2d_array(N, M, double) => allocates N x M x sizeof(double)
					  and returns a pointer to the 2d array
 */
# define POLYBENCH_ALLOC_1D_ARRAY(n1, type)	\
  (type(*)[n1 + POLYBENCH_PADDING_FACTOR])polybench_alloc_data (n1 + POLYBENCH_PADDING_FACTOR, sizeof(type))
# define POLYBENCH_ALLOC_2D_ARRAY(n1, n2, type)		\
  (type(*)[n1 + POLYBENCH_PADDING_FACTOR][n2 + POLYBENCH_PADDING_FACTOR])polybench_alloc_data ((n1 + POLYBENCH_PADDING_FACTOR) * (n2 + POLYBENCH_PADDING_FACTOR), sizeof(type))
# define POLYBENCH_ALLOC_3D_ARRAY(n1, n2, n3, type)		\
  (type(*)[n1 + POLYBENCH_PADDING_FACTOR][n2 + POLYBENCH_PADDING_FACTOR][n3 + POLYBENCH_PADDING_FACTOR])polybench_alloc_data ((n1 + POLYBENCH_PADDING_FACTOR) * (n2 + POLYBENCH_PADDING_FACTOR) * (n3 + POLYBENCH_PADDING_FACTOR), sizeof(type))
# define POLYBENCH_ALLOC_4D_ARRAY(n1, n2, n3, n4, type)			\
  (type(*)[n1 + POLYBENCH_PADDING_FACTOR][n2 + POLYBENCH_PADDING_FACTOR][n3 + POLYBENCH_PADDING_FACTOR][n4 + POLYBENCH_PADDING_FACTOR])polybench_alloc_data ((n1 + POLYBENCH_PADDING_FACTOR) * (n2 + POLYBENCH_PADDING_FACTOR) * (n3 + POLYBENCH_PADDING_FACTOR) * (n4 + POLYBENCH_PADDING_FACTOR), sizeof(type))
# define POLYBENCH_ALLOC_5D_ARRAY(n1, n2, n3, n4, n5, type)		\
  (type(*)[n1 + POLYBENCH_PADDING_FACTOR][n2 + POLYBENCH_PADDING_FACTOR][n3 + POLYBENCH_PADDING_FACTOR][n4 + POLYBENCH_PADDING_FACTOR][n5 + POLYBENCH_PADDING_FACTOR])polybench_alloc_data ((n1 + POLYBENCH_PADDING_FACTOR) * (n2 + POLYBENCH_PADDING_FACTOR) * (n3 + POLYBENCH_PADDING_FACTOR) * (n4 + POLYBENCH_PADDING_FACTOR) * (n5 + POLYBENCH_PADDING_FACTOR), sizeof(type))

/* Macros for array declaration. */
# ifndef POLYBENCH_STACK_ARRAYS
#  define POLYBENCH_1D_ARRAY_DECL(var, type, dim1, ddim1)		\
  type POLYBENCH_1D_F(POLYBENCH_DECL_VAR(var), dim1, ddim1); \
  var = POLYBENCH_ALLOC_1D_ARRAY(POLYBENCH_C99_SELECT(dim1, ddim1), type);
#  define POLYBENCH_2D_ARRAY_DECL(var, type, dim1, dim2, ddim1, ddim2)	\
  type POLYBENCH_2D_F(POLYBENCH_DECL_VAR(var), dim1, dim2, ddim1, ddim2); \
  var = POLYBENCH_ALLOC_2D_ARRAY(POLYBENCH_C99_SELECT(dim1, ddim1), POLYBENCH_C99_SELECT(dim2, ddim2), type);
#  define POLYBENCH_3D_ARRAY_DECL(var, type, dim1, dim2, dim3, ddim1, ddim2, ddim3) \
  type POLYBENCH_3D_F(POLYBENCH_DECL_VAR(var), dim1, dim2, dim3, ddim1, ddim2, ddim3); \
  var = POLYBENCH_ALLOC_3D_ARRAY(POLYBENCH_C99_SELECT(dim1, ddim1), POLYBENCH_C99_SELECT(dim2, ddim2), POLYBENCH_C99_SELECT(dim3, ddim3), type);
#  define POLYBENCH_4D_ARRAY_DECL(var, type, dim1, dim2, dim3, dim4, ddim1, ddim2, ddim3, ddim4) \
  type POLYBENCH_4D_F(POLYBENCH_DECL_VAR(var), dim1, dim2, dim3, dim4, ddim1, ddim2, ddim3, ddim4); \
  var = POLYBENCH_ALLOC_4D_ARRAY(POLYBENCH_C99_SELECT(dim1, ddim1), POLYBENCH_C99_SELECT(dim2, ddim2), POLYBENCH_C99_SELECT(dim3, ddim3), POLYBENCH_C99_SELECT(dim4, ddim4), type);
#  define POLYBENCH_5D_ARRAY_DECL(var, type, dim1, dim2, dim3, dim4, dim5, ddim1, ddim2, ddim3, ddim4, ddim5) \
  type POLYBENCH_5D_F(POLYBENCH_DECL_VAR(var), dim1, dim2, dim3, dim4, dim5, ddim1, ddim2, ddim3, ddim4, ddim5); \
  var = POLYBENCH_ALLOC_5D_ARRAY(POLYBENCH_C99_SELECT(dim1, ddim1), POLYBENCH_C99_SELECT(dim2, ddim2), POLYBENCH_C99_SELECT(dim3, ddim3), POLYBENCH_C99_SELECT(dim4, ddim4), POLYBENCH_C99_SELECT(dim5, ddim5), type);
# else
#  define POLYBENCH_1D_ARRAY_DECL(var, type, dim1, ddim1)		\
  type POLYBENCH_1D_F(POLYBENCH_DECL_VAR(var), dim1, ddim1);
#  define POLYBENCH_2D_ARRAY_DECL(var, type, dim1, dim2, ddim1, ddim2)	\
  type POLYBENCH_2D_F(POLYBENCH_DECL_VAR(var), dim1, dim2, ddim1, ddim2);
#  define POLYBENCH_3D_ARRAY_DECL(var, type, dim1, dim2, dim3, ddim1, ddim2, ddim3) \
  type POLYBENCH_3D_F(POLYBENCH_DECL_VAR(var), dim1, dim2, dim3, ddim1, ddim2, ddim3);
#  define POLYBENCH_4D_ARRAY_DECL(var, type, dim1, dim2, dim3, dim4, ddim1, ddim2, ddim3, ddim4) \
  type POLYBENCH_4D_F(POLYBENCH_DECL_VAR(var), dim1, dim2, dim3, dim4, ddim1, ddim2, ddim3, ddim4);
#  define POLYBENCH_5D_ARRAY_DECL(var, type, dim1, dim2, dim3, dim4, dim5, ddim1, ddim2, ddim3, ddim4, ddim5) \
  type POLYBENCH_5D_F(POLYBENCH_DECL_VAR(var), dim1, dim2, dim3, dim4, dim5, ddim1, ddim2, ddim3, ddim4, ddim5);
# endif


/* Dead-code elimination macros. Use argc/argv for the run-time check. */
# ifndef POLYBENCH_DUMP_ARRAYS
#  define POLYBENCH_DCE_ONLY_CODE    if (argc > 42 && ! strcmp(argv[0], ""))
# else
#  define POLYBENCH_DCE_ONLY_CODE
# endif

#define POLYBENCH_DUMP_TARGET stderr
#define POLYBENCH_DUMP_START    fprintf(POLYBENCH_DUMP_TARGET, "==BEGIN DUMP_ARRAYS==\n")
#define POLYBENCH_DUMP_FINISH   fprintf(POLYBENCH_DUMP_TARGET, "==END   DUMP_ARRAYS==\n")
#define POLYBENCH_DUMP_BEGIN(s) fprintf(POLYBENCH_DUMP_TARGET, "begin dump: %s", s)
#define POLYBENCH_DUMP_END(s)   fprintf(POLYBENCH_DUMP_TARGET, "\nend   dump: %s\n", s)

# define polybench_prevent_dce(func)		\
  POLYBENCH_DCE_ONLY_CODE			\
  func


/* Performance-related instrumentation. See polybench.c */
# define polybench_start_instruments
# define polybench_stop_instruments
# define polybench_print_instruments


/* PAPI support. */
# ifdef POLYBENCH_PAPI
extern const unsigned int polybench_papi_eventlist[];
#  undef polybench_start_instruments
#  undef polybench_stop_instruments
#  undef polybench_print_instruments
#  define polybench_set_papi_thread_report(x)	\
   polybench_papi_counters_threadid = x;
#  define polybench_start_instruments				\
  polybench_prepare_instruments();				\
  polybench_papi_init();					\
  int evid;							\
  for (evid = 0; polybench_papi_eventlist[evid] != 0; evid++)	\
    {								\
      if (polybench_papi_start_counter(evid))			\
	continue;						\

#  define polybench_stop_instruments		\
      polybench_papi_stop_counter(evid);	\
    }						\
  polybench_papi_close();			\

#  define polybench_print_instruments polybench_papi_print();
# endif


/* Timing support. */
# if defined(POLYBENCH_TIME) || defined(POLYBENCH_GFLOPS)
#  undef polybench_start_instruments
#  undef polybench_stop_instruments
#  undef polybench_print_instruments
#  define polybench_start_instruments polybench_timer_start();
#  define polybench_stop_instruments polybench_timer_stop();
#  define polybench_print_instruments polybench_timer_print();
extern double polybench_program_total_flops;
extern void polybench_timer_start();
extern void polybench_timer_stop();
extern void polybench_timer_print();
# endif

/* PAPI support. */
# ifdef POLYBENCH_PAPI
extern int polybench_papi_start_counter(int evid);
extern void polybench_papi_stop_counter(int evid);
extern void polybench_papi_init();
extern void polybench_papi_close();
extern void polybench_papi_print();
# endif

/* Function prototypes. */
extern void* polybench_alloc_data(unsigned long long int n, int elt_size);
extern void polybench_free_data(void* ptr);

/* PolyBench internal functions that should not be directly called by */
/* the user, unless when designing customized execution profiling */
/* approaches. */
extern void polybench_flush_cache();
extern void polybench_prepare_instruments();


#endif /* !POLYBENCH_H */

/* Include benchmark-specific header. */
/**
 * This version is stamped on May 10, 2016
 *
 * Contact:
 *   Louis-Noel Pouchet <pouchet.ohio-state.edu>
 *   Tomofumi Yuki <tomofumi.yuki.fr>
 *
 * Web address: http://polybench.sourceforge.net
 */
#ifndef _ATAX_H
# define _ATAX_H

/* Default to LARGE_DATASET. */
# if !defined(MINI_DATASET) && !defined(SMALL_DATASET) && !defined(MEDIUM_DATASET) && !defined(LARGE_DATASET) && !defined(EXTRALARGE_DATASET)
#  define LARGE_DATASET
# endif

# if !defined(M) && !defined(N)
/* Define sample dataset sizes. */
#  ifdef MINI_DATASET
#   define M 38
#   define N 42
#  endif

#  ifdef SMALL_DATASET
#   define M 116
#   define N 124
#  endif

#  ifdef MEDIUM_DATASET
#   define M 390
#   define N 410
#  endif

#  ifdef LARGE_DATASET
#   define M 1900
#   define N 2100
#  endif

#  ifdef EXTRALARGE_DATASET
#   define M 1800
#   define N 2200
#  endif


#endif /* !(M N) */

# define _PB_M POLYBENCH_LOOP_BOUND(M,m)
# define _PB_N POLYBENCH_LOOP_BOUND(N,n)


/* Default data type */
# if !defined(DATA_TYPE_IS_INT) && !defined(DATA_TYPE_IS_FLOAT) && !defined(DATA_TYPE_IS_DOUBLE)
#  define DATA_TYPE_IS_DOUBLE
# endif

#ifdef DATA_TYPE_IS_INT
#  define DATA_TYPE int
#  define DATA_PRINTF_MODIFIER "%d "
#endif

#ifdef DATA_TYPE_IS_FLOAT
#  define DATA_TYPE float
#  define DATA_PRINTF_MODIFIER "%0.2f "
#  define SCALAR_VAL(x) x##f
#  define SQRT_FUN(x) sqrtf(x)
#  define EXP_FUN(x) expf(x)
#  define POW_FUN(x,y) powf(x,y)
# endif

#ifdef DATA_TYPE_IS_DOUBLE
#  define DATA_TYPE double
#  define DATA_PRINTF_MODIFIER "%0.2lf "
#  define SCALAR_VAL(x) x
#  define SQRT_FUN(x) sqrt(x)
#  define EXP_FUN(x) exp(x)
#  define POW_FUN(x,y) pow(x,y)
# endif

#endif /* !_ATAX_H */


/* Array initialization. */
/* Array initialization. */
#pragma coral null A : not-null -> not-null
#pragma coral null x : not-null -> not-null
static
void init_array (int m, int n,
                 DATA_TYPE *A,
                 DATA_TYPE *x)
{
    int i, j;
    DATA_TYPE fn;
    fn = (DATA_TYPE)n;

    for (i = 0; i < n; i++)
        x[i] = 1 + (i / fn);
    for (i = 0; i < m; i++)
        for (j = 0; j < n; j++)
            A[i * n + j] = (DATA_TYPE) ((i + j) % n) / (5 * m);
}


/* DCE code. Must scan the entire live-out data.
   Can be used also to check the correctness of the output. */
#pragma coral null y : not-null -> not-null
static
void print_array(int n, DATA_TYPE *y)

{
    int i;

    POLYBENCH_DUMP_START;
    POLYBENCH_DUMP_BEGIN("y");
    for (i = 0; i < n; i++)
    {
        if (i % 20 == 0) fprintf (POLYBENCH_DUMP_TARGET, "\n");
        fprintf (POLYBENCH_DUMP_TARGET, DATA_PRINTF_MODIFIER, y[i]);
    }
    POLYBENCH_DUMP_END("y");
    POLYBENCH_DUMP_FINISH;
}


/* Main computational kernel. The whole function will be timed,
   including the call and return. */
/* Main computational kernel. */
#pragma coral null A : not-null -> not-null
#pragma coral null x : not-null -> not-null
#pragma coral null y : not-null -> not-null
#pragma coral null tmp : not-null -> not-null
static
void kernel_atax(int m, int n,
                 DATA_TYPE *A,
                 DATA_TYPE *x,
                 DATA_TYPE *y,
                 DATA_TYPE *tmp)
{
    int i, j;

    for (i = 0; i < _PB_N; i++)
        y[i] = 0;

    for (i = 0; i < _PB_M; i++)
    {
        tmp[i] = SCALAR_VAL(0.0);
        for (j = 0; j < _PB_N; j++)
            tmp[i] = tmp[i] + A[i * n + j] * x[j];

        for (j = 0; j < _PB_N; j++)
            y[j] = y[j] + A[i * n + j] * tmp[i];
    }
}


int main(int argc, char** argv)
{
    /* Retrieve problem size. */
    int m = M;
    int n = N;

    int * z;
    int a = *z;


    /* Variable declaration/allocation using standard malloc */
    DATA_TYPE *A = (DATA_TYPE*) malloc(m * n * sizeof(DATA_TYPE));
    DATA_TYPE *x = (DATA_TYPE*) malloc(n * sizeof(DATA_TYPE));
    DATA_TYPE *y = (DATA_TYPE*) malloc(n * sizeof(DATA_TYPE));
    DATA_TYPE *tmp = (DATA_TYPE*) malloc(m * sizeof(DATA_TYPE));
    
    if (A == NULL || x == NULL || y == NULL || tmp == NULL) {
       fprintf(stderr, "Memory allocation failed!\n");
       return 1; 
    }
    /* Initialize array(s). */
    init_array (m, n, A, x);

    /* Start timer. */
    polybench_start_instruments;

    /* Run kernel. */
    kernel_atax (m, n, A, x, y, tmp);

    /* Stop and print timer. */
    polybench_stop_instruments;
    polybench_print_instruments;

    /* Prevent dead-code elimination. */
    polybench_prevent_dce(print_array(n, y));

    /* Be clean. */
    free(A);
    free(x);
    free(y);
    free(tmp);

    return 0;
}
