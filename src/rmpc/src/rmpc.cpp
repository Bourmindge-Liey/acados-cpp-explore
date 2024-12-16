#include "rmpc/rmpc.h"


RMPC::RMPC(std::array<double, NX> &x_init) {

    printf("Creating RMPC object\n");

    // initialize data
    acados_ocp_capsule = cartpole_model_acados_create_capsule();
    status = cartpole_model_acados_create_with_discretization(acados_ocp_capsule, N, new_time_steps);
    if (status) {
        printf("cartpole_model_acados_create() returned status %d. Exiting.\n", status);
        exit(1);
    }

    nlp_config = cartpole_model_acados_get_nlp_config(acados_ocp_capsule);
    nlp_dims = cartpole_model_acados_get_nlp_dims(acados_ocp_capsule);
    nlp_in = cartpole_model_acados_get_nlp_in(acados_ocp_capsule);
    nlp_out = cartpole_model_acados_get_nlp_out(acados_ocp_capsule);
    nlp_solver = cartpole_model_acados_get_nlp_solver(acados_ocp_capsule);
    nlp_opts = cartpole_model_acados_get_nlp_opts(acados_ocp_capsule);

    // initial condition
    std::iota(idxbx0_.begin(), idxbx0_.end(), 0);
    double lbx0[NBX0];
    double ubx0[NBX0];
    std::copy(x_init.begin(), x_init.begin(), lbx0);
    std::copy(x_init.end(), x_init.end(), ubx0);    
    ocp_nlp_constraints_model_set(nlp_config, nlp_dims, nlp_in, 0, "lbx", lbx0);
    ocp_nlp_constraints_model_set(nlp_config, nlp_dims, nlp_in, 0, "ubx", ubx0);

    //initialise the reference, states and inputs
    for (int i = 0; i < N_STEPS; i++) {
        ocp_nlp_cost_model_set(nlp_config, nlp_dims, nlp_in, i, "yref", y_ref.begin());
        ocp_nlp_out_set(nlp_config, nlp_dims, nlp_out, i, "x", x_init.begin());
        ocp_nlp_out_set(nlp_config, nlp_dims, nlp_out, i, "u", u_pred.begin());
    }
    ocp_nlp_out_set(nlp_config, nlp_dims, nlp_out, N_STEPS, "x", &x_init);
}


void RMPC::control(std::array<double, NX> &x) {

    printf("Computing control\n");

    // set initial condition
    ocp_nlp_constraints_model_set(nlp_config, nlp_dims, nlp_in, 0, "idxbx", idxbx0_.begin());
    ocp_nlp_constraints_model_set(nlp_config, nlp_dims, nlp_in, 0, "lbx", x.begin());
    ocp_nlp_constraints_model_set(nlp_config, nlp_dims, nlp_in, 0, "ubx", x.begin());


    for (int ii = 0; ii < 1; ii++) {
        int rti_phase = 0;
        for (int i = 0; i < N_STEPS; i++)
            {
                ocp_nlp_out_set(nlp_config, nlp_dims, nlp_out, i, "x", x.begin());
                // ocp_nlp_out_set(nlp_config, nlp_dims, nlp_out, i, "u", u_current.begin());
            }
            ocp_nlp_out_set(nlp_config, nlp_dims, nlp_out, N, "x", x.begin());
            ocp_nlp_solver_opts_set(nlp_config, nlp_opts, "rti_phase", &rti_phase);

        // solve
        status = cartpole_model_acados_solve(acados_ocp_capsule);
        if (status) {
            printf("acados returned status %d. Exiting.\n", status);
            exit(1);
        }
    }

    // // get solution
    ocp_nlp_out_get(nlp_config, nlp_dims, nlp_out, 0, "u", u_current.begin());
    ocp_nlp_out_get(nlp_config, nlp_dims, nlp_out, 1, "x", x.begin());
    for (int i = 0; i < N_STEPS; i++) {
        ocp_nlp_out_get(nlp_config, nlp_dims, nlp_out, i, "x", x_pred.begin());
        ocp_nlp_out_get(nlp_config, nlp_dims, nlp_out, i, "u", u_pred.begin());
    }
    ocp_nlp_out_get(nlp_config, nlp_dims, nlp_out, N_STEPS, "x", x_pred.begin());
    
}


std::array<double, NU> RMPC::get_control() {
    return u_current;
}


void RMPC::set_reference(std::array<std::array<double, NY>, N_STEPS> &y_ref, std::array<double, NX> &y_ref_e) {

    printf("Setting reference\n");

    for (int i = 0; i < N_STEPS; i++) {
        ocp_nlp_cost_model_set(nlp_config, nlp_dims, nlp_in, i, "yref", y_ref[i].begin());
    }
    ocp_nlp_cost_model_set(nlp_config, nlp_dims, nlp_in, N_STEPS, "yref", y_ref_e.begin());
}





RMPC::~RMPC() {
    cartpole_model_acados_free(acados_ocp_capsule);
    cartpole_model_acados_free_capsule(acados_ocp_capsule);
}


