#include <stdio.h>
#include <stdlib.h>

// acados
#include "acados/utils/print.h"
#include "acados/utils/math.h"
#include "acados_c/ocp_nlp_interface.h"
#include "acados_c/external_function_interface.h"
#include "acados_solver_cartpole_model.h"

// blasfeo
#include "blasfeo/include/blasfeo_d_aux_ext_dep.h"

#define NX     CARTPOLE_MODEL_NX
#define NP     CARTPOLE_MODEL_NP
#define NU     CARTPOLE_MODEL_NU
#define NBX0   CARTPOLE_MODEL_NBX0


class RMPC {

    // cartpole_model_solver_capsule *acados_ocp_capsule = cartpole_model_acados_create_capsule();
    // int N = CARTPOLE_MODEL_N;
    // double* new_time_steps = NULL;
    // int status;
    
    // ocp_nlp_config *nlp_config = cartpole_model_acados_get_nlp_config(acados_ocp_capsule);
    // ocp_nlp_dims *nlp_dims = cartpole_model_acados_get_nlp_dims(acados_ocp_capsule);
    // ocp_nlp_in *nlp_in = cartpole_model_acados_get_nlp_in(acados_ocp_capsule);
    // ocp_nlp_out *nlp_out = cartpole_model_acados_get_nlp_out(acados_ocp_capsule);
    // ocp_nlp_solver *nlp_solver = cartpole_model_acados_get_nlp_solver(acados_ocp_capsule);
    // void *nlp_opts = cartpole_model_acados_get_nlp_opts(acados_ocp_capsule);


public:
    RMPC();

    ~RMPC() = default;

};