#ifndef RMPC_H
#define RMPC_H

#include <stdio.h>
#include <stdlib.h>

#include <numeric>
#include <vector>
#include <array>

// acados
#include "acados/utils/print.h"
#include "acados/utils/math.h"
#include "acados_c/ocp_nlp_interface.h"
#include "acados_c/external_function_interface.h"
#include "acados_solver_cartpole_model.h"

// blasfeo
#include "blasfeo/include/blasfeo_d_aux_ext_dep.h"

#define NX     CARTPOLE_MODEL_NX
#define NZ     CARTPOLE_MODEL_NZ
#define NU     CARTPOLE_MODEL_NU
#define NP     CARTPOLE_MODEL_NP
#define NY0    CARTPOLE_MODEL_NY0
#define NY     CARTPOLE_MODEL_NY
#define NYN    CARTPOLE_MODEL_NYN

#define NBX    CARTPOLE_MODEL_NBX
#define NBX0   CARTPOLE_MODEL_NBX0
#define NBU    CARTPOLE_MODEL_NBU
#define NG     CARTPOLE_MODEL_NG
#define NBXN   CARTPOLE_MODEL_NBXN
#define NGN    CARTPOLE_MODEL_NGN

#define NH     CARTPOLE_MODEL_NH
#define NHN    CARTPOLE_MODEL_NHN
#define NH0    CARTPOLE_MODEL_NH0
#define NPHI   CARTPOLE_MODEL_NPHI
#define NPHIN  CARTPOLE_MODEL_NPHIN
#define NPHI0  CARTPOLE_MODEL_NPHI0
#define NR     CARTPOLE_MODEL_NR

#define NS     CARTPOLE_MODEL_NS
#define NS0    CARTPOLE_MODEL_NS0
#define NSN    CARTPOLE_MODEL_NSN

#define NSBX   CARTPOLE_MODEL_NSBX
#define NSBU   CARTPOLE_MODEL_NSBU
#define NSH0   CARTPOLE_MODEL_NSH0
#define NSH    CARTPOLE_MODEL_NSH
#define NSHN   CARTPOLE_MODEL_NSHN
#define NSG    CARTPOLE_MODEL_NSG
#define NSPHI0 CARTPOLE_MODEL_NSPHI0
#define NSPHI  CARTPOLE_MODEL_NSPHI
#define NSPHIN CARTPOLE_MODEL_NSPHIN
#define NSGN   CARTPOLE_MODEL_NSGN
#define NSBXN  CARTPOLE_MODEL_NSBXN

#define N_STEPS CARTPOLE_MODEL_N

class RMPC {

    // acados solver
    cartpole_model_solver_capsule *acados_ocp_capsule;
    ocp_nlp_config *nlp_config;
    ocp_nlp_dims *nlp_dims;
    ocp_nlp_in *nlp_in;
    ocp_nlp_out *nlp_out;
    ocp_nlp_solver *nlp_solver;
    void *nlp_opts;

    // solver status
    std::array<int, NBX0> idxbx0_;
    int N = CARTPOLE_MODEL_N;
    double* new_time_steps = NULL;
    int status;

    // nlp solutions
    std::array<double, NX> x_current;
    std::array<double, NU> u_current;
    std::array<std::array<double, NX>, N_STEPS + 1> x_pred;
    std::array<std::array<double, NU>, N_STEPS> u_pred;
    std::array<std::array<double, NY>, N_STEPS> y_ref;

public:

    // constructor and destructor
    RMPC(std::array<double, NX> &x_init);
    ~RMPC();

    void control(std::array<double, NX> &x);
    std::array<double, NU> get_control();
    void set_reference(std::array<std::array<double, NY>, N_STEPS> &y_ref, std::array<double, NX> &y_ref_e);
};

#endif