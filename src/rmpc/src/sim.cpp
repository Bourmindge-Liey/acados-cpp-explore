# include "rmpc/sim.h"


Env_Sim::Env_Sim(double init_state[NX]) {
    int status = 0;
    capsule = cartpole_model_acados_sim_solver_create_capsule();
    status = cartpole_model_acados_sim_create(capsule);

    if (status) {
        printf("acados_create() returned status %d. Exiting.\n", status);
        exit(1);
    }

    acados_sim_config = cartpole_model_acados_get_sim_config(capsule);
    acados_sim_in = cartpole_model_acados_get_sim_in(capsule);
    acados_sim_out = cartpole_model_acados_get_sim_out(capsule);
    acados_sim_dims = cartpole_model_acados_get_sim_dims(capsule);

    // initial condition
    std::copy(init_state, init_state + NX, x_current);
}


Env_Sim::~Env_Sim() {
    int status = cartpole_model_acados_sim_free(capsule);
    if (status) {
        printf("cartpole_model_acados_sim_free() returned status %d. \n", status);
    }
    cartpole_model_acados_sim_solver_free_capsule(capsule);
}


void Env_Sim::set_state(double x[NX]) {
    std::copy(x, x + NX, x_current);
}


void Env_Sim::step(double u0[NU], double x_next[NX]) {
    // set inputs
    sim_in_set(acados_sim_config, acados_sim_dims,
        acados_sim_in, "x", x_current);
    sim_in_set(acados_sim_config, acados_sim_dims,
        acados_sim_in, "u", u0);

    // solve
    int status = cartpole_model_acados_sim_solve(capsule);
    if (status != ACADOS_SUCCESS) {
        printf("acados_solve() failed with status %d.\n", status);
    }

    // get outputs
    sim_out_get(acados_sim_config, acados_sim_dims,
            acados_sim_out, "x", x_current);

    // copy the next state to x_next
    std::copy(x_current, x_current + NX, x_next);

    // record state and control
    x_record.push_back({x_current[0], x_current[1], x_current[2], x_current[3]});
}