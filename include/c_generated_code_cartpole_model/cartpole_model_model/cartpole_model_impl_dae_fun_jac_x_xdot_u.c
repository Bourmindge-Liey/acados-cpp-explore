/* This file was automatically generated by CasADi 3.6.6.
 *  It consists of: 
 *   1) content generated by CasADi runtime: not copyrighted
 *   2) template code copied from CasADi source: permissively licensed (MIT-0)
 *   3) user code: owned by the user
 *
 */
#ifdef __cplusplus
extern "C" {
#endif

/* How to prefix internal symbols */
#ifdef CASADI_CODEGEN_PREFIX
  #define CASADI_NAMESPACE_CONCAT(NS, ID) _CASADI_NAMESPACE_CONCAT(NS, ID)
  #define _CASADI_NAMESPACE_CONCAT(NS, ID) NS ## ID
  #define CASADI_PREFIX(ID) CASADI_NAMESPACE_CONCAT(CODEGEN_PREFIX, ID)
#else
  #define CASADI_PREFIX(ID) cartpole_model_impl_dae_fun_jac_x_xdot_u_ ## ID
#endif

#include <math.h>

#ifndef casadi_real
#define casadi_real double
#endif

#ifndef casadi_int
#define casadi_int int
#endif

/* Add prefix to internal symbols */
#define casadi_f0 CASADI_PREFIX(f0)
#define casadi_s0 CASADI_PREFIX(s0)
#define casadi_s1 CASADI_PREFIX(s1)
#define casadi_s2 CASADI_PREFIX(s2)
#define casadi_s3 CASADI_PREFIX(s3)
#define casadi_s4 CASADI_PREFIX(s4)
#define casadi_s5 CASADI_PREFIX(s5)
#define casadi_sq CASADI_PREFIX(sq)

/* Symbol visibility in DLLs */
#ifndef CASADI_SYMBOL_EXPORT
  #if defined(_WIN32) || defined(__WIN32__) || defined(__CYGWIN__)
    #if defined(STATIC_LINKED)
      #define CASADI_SYMBOL_EXPORT
    #else
      #define CASADI_SYMBOL_EXPORT __declspec(dllexport)
    #endif
  #elif defined(__GNUC__) && defined(GCC_HASCLASSVISIBILITY)
    #define CASADI_SYMBOL_EXPORT __attribute__ ((visibility ("default")))
  #else
    #define CASADI_SYMBOL_EXPORT
  #endif
#endif

casadi_real casadi_sq(casadi_real x) { return x*x;}

static const casadi_int casadi_s0[8] = {4, 1, 0, 4, 0, 1, 2, 3};
static const casadi_int casadi_s1[5] = {1, 1, 0, 1, 0};
static const casadi_int casadi_s2[3] = {0, 0, 0};
static const casadi_int casadi_s3[14] = {4, 4, 0, 0, 3, 3, 7, 0, 2, 3, 0, 1, 2, 3};
static const casadi_int casadi_s4[11] = {4, 4, 0, 1, 2, 3, 4, 0, 1, 2, 3};
static const casadi_int casadi_s5[7] = {4, 1, 0, 3, 0, 2, 3};

/* cartpole_model_impl_dae_fun_jac_x_xdot_u:(i0[4],i1[4],i2,i3[],i4[],i5[])->(o0[4],o1[4x4,7nz],o2[4x4,4nz],o3[4x1,3nz]) */
static int casadi_f0(const casadi_real** arg, casadi_real** res, casadi_int* iw, casadi_real* w, int mem) {
  casadi_real a0, a1, a10, a11, a12, a13, a14, a15, a16, a17, a2, a3, a4, a5, a6, a7, a8, a9;
  a0=arg[1]? arg[1][0] : 0;
  a1=-5.0000000000000003e-02;
  a2=arg[0]? arg[0][1] : 0;
  a3=sin(a2);
  a4=(a1*a3);
  a5=arg[0]? arg[0][3] : 0;
  a6=casadi_sq(a5);
  a7=(a4*a6);
  a8=9.8100000000000009e-01;
  a2=cos(a2);
  a9=(a8*a2);
  a10=(a9*a3);
  a7=(a7+a10);
  a10=arg[2]? arg[2][0] : 0;
  a7=(a7+a10);
  a11=1.;
  a12=1.0000000000000001e-01;
  a13=casadi_sq(a2);
  a13=(a11-a13);
  a13=(a12*a13);
  a13=(a11+a13);
  a7=(a7/a13);
  a0=(a0-a7);
  if (res[0]!=0) res[0][0]=a0;
  a0=arg[1]? arg[1][1] : 0;
  a0=(a0-a5);
  if (res[0]!=0) res[0][1]=a0;
  a0=arg[1]? arg[1][2] : 0;
  a0=(a0-a7);
  if (res[0]!=0) res[0][2]=a0;
  a0=arg[1]? arg[1][3] : 0;
  a1=(a1*a2);
  a14=(a1*a3);
  a15=(a14*a6);
  a16=1.0791000000000002e+01;
  a17=(a16*a3);
  a15=(a15+a17);
  a17=(a10*a2);
  a15=(a15+a17);
  a15=(a15/a13);
  a17=5.0000000000000000e-01;
  a17=(a15/a17);
  a0=(a0-a17);
  if (res[0]!=0) res[0][3]=a0;
  a0=(a6*a1);
  a9=(a9*a2);
  a8=(a8*a3);
  a8=(a3*a8);
  a9=(a9-a8);
  a0=(a0+a9);
  a0=(a0/a13);
  a7=(a7/a13);
  a9=(a2+a2);
  a9=(a9*a3);
  a12=(a12*a9);
  a7=(a7*a12);
  a0=(a0-a7);
  a0=(-a0);
  if (res[1]!=0) res[1][0]=a0;
  if (res[1]!=0) res[1][1]=a0;
  a0=2.;
  a1=(a1*a2);
  a7=(a3*a4);
  a1=(a1-a7);
  a6=(a6*a1);
  a16=(a16*a2);
  a6=(a6+a16);
  a10=(a10*a3);
  a6=(a6-a10);
  a6=(a6/a13);
  a15=(a15/a13);
  a15=(a15*a12);
  a6=(a6-a15);
  a6=(a0*a6);
  a6=(-a6);
  if (res[1]!=0) res[1][2]=a6;
  a5=(a5+a5);
  a4=(a4*a5);
  a4=(a4/a13);
  a4=(-a4);
  if (res[1]!=0) res[1][3]=a4;
  a6=-1.;
  if (res[1]!=0) res[1][4]=a6;
  if (res[1]!=0) res[1][5]=a4;
  a14=(a14*a5);
  a14=(a14/a13);
  a14=(a0*a14);
  a14=(-a14);
  if (res[1]!=0) res[1][6]=a14;
  if (res[2]!=0) res[2][0]=a11;
  if (res[2]!=0) res[2][1]=a11;
  if (res[2]!=0) res[2][2]=a11;
  if (res[2]!=0) res[2][3]=a11;
  a11=(1./a13);
  a11=(-a11);
  if (res[3]!=0) res[3][0]=a11;
  if (res[3]!=0) res[3][1]=a11;
  a2=(a2/a13);
  a0=(a0*a2);
  a0=(-a0);
  if (res[3]!=0) res[3][2]=a0;
  return 0;
}

CASADI_SYMBOL_EXPORT int cartpole_model_impl_dae_fun_jac_x_xdot_u(const casadi_real** arg, casadi_real** res, casadi_int* iw, casadi_real* w, int mem){
  return casadi_f0(arg, res, iw, w, mem);
}

CASADI_SYMBOL_EXPORT int cartpole_model_impl_dae_fun_jac_x_xdot_u_alloc_mem(void) {
  return 0;
}

CASADI_SYMBOL_EXPORT int cartpole_model_impl_dae_fun_jac_x_xdot_u_init_mem(int mem) {
  return 0;
}

CASADI_SYMBOL_EXPORT void cartpole_model_impl_dae_fun_jac_x_xdot_u_free_mem(int mem) {
}

CASADI_SYMBOL_EXPORT int cartpole_model_impl_dae_fun_jac_x_xdot_u_checkout(void) {
  return 0;
}

CASADI_SYMBOL_EXPORT void cartpole_model_impl_dae_fun_jac_x_xdot_u_release(int mem) {
}

CASADI_SYMBOL_EXPORT void cartpole_model_impl_dae_fun_jac_x_xdot_u_incref(void) {
}

CASADI_SYMBOL_EXPORT void cartpole_model_impl_dae_fun_jac_x_xdot_u_decref(void) {
}

CASADI_SYMBOL_EXPORT casadi_int cartpole_model_impl_dae_fun_jac_x_xdot_u_n_in(void) { return 6;}

CASADI_SYMBOL_EXPORT casadi_int cartpole_model_impl_dae_fun_jac_x_xdot_u_n_out(void) { return 4;}

CASADI_SYMBOL_EXPORT casadi_real cartpole_model_impl_dae_fun_jac_x_xdot_u_default_in(casadi_int i) {
  switch (i) {
    default: return 0;
  }
}

CASADI_SYMBOL_EXPORT const char* cartpole_model_impl_dae_fun_jac_x_xdot_u_name_in(casadi_int i) {
  switch (i) {
    case 0: return "i0";
    case 1: return "i1";
    case 2: return "i2";
    case 3: return "i3";
    case 4: return "i4";
    case 5: return "i5";
    default: return 0;
  }
}

CASADI_SYMBOL_EXPORT const char* cartpole_model_impl_dae_fun_jac_x_xdot_u_name_out(casadi_int i) {
  switch (i) {
    case 0: return "o0";
    case 1: return "o1";
    case 2: return "o2";
    case 3: return "o3";
    default: return 0;
  }
}

CASADI_SYMBOL_EXPORT const casadi_int* cartpole_model_impl_dae_fun_jac_x_xdot_u_sparsity_in(casadi_int i) {
  switch (i) {
    case 0: return casadi_s0;
    case 1: return casadi_s0;
    case 2: return casadi_s1;
    case 3: return casadi_s2;
    case 4: return casadi_s2;
    case 5: return casadi_s2;
    default: return 0;
  }
}

CASADI_SYMBOL_EXPORT const casadi_int* cartpole_model_impl_dae_fun_jac_x_xdot_u_sparsity_out(casadi_int i) {
  switch (i) {
    case 0: return casadi_s0;
    case 1: return casadi_s3;
    case 2: return casadi_s4;
    case 3: return casadi_s5;
    default: return 0;
  }
}

CASADI_SYMBOL_EXPORT int cartpole_model_impl_dae_fun_jac_x_xdot_u_work(casadi_int *sz_arg, casadi_int* sz_res, casadi_int *sz_iw, casadi_int *sz_w) {
  if (sz_arg) *sz_arg = 6;
  if (sz_res) *sz_res = 4;
  if (sz_iw) *sz_iw = 0;
  if (sz_w) *sz_w = 0;
  return 0;
}

CASADI_SYMBOL_EXPORT int cartpole_model_impl_dae_fun_jac_x_xdot_u_work_bytes(casadi_int *sz_arg, casadi_int* sz_res, casadi_int *sz_iw, casadi_int *sz_w) {
  if (sz_arg) *sz_arg = 6*sizeof(const casadi_real*);
  if (sz_res) *sz_res = 4*sizeof(casadi_real*);
  if (sz_iw) *sz_iw = 0*sizeof(casadi_int);
  if (sz_w) *sz_w = 0*sizeof(casadi_real);
  return 0;
}


#ifdef __cplusplus
} /* extern "C" */
#endif
