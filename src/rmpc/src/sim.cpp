# include "rmpc/sim.h"

Env_Sim::Env_Sim(std::array<double, NX> &init_state) {
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
    std::copy(init_state.begin(), init_state.end(), x_current.begin());
}


Env_Sim::~Env_Sim() {
    int status = cartpole_model_acados_sim_free(capsule);
    if (status) {
        printf("cartpole_model_acados_sim_free() returned status %d. \n", status);
    }
    cartpole_model_acados_sim_solver_free_capsule(capsule);
}


void Env_Sim::set_state(std::array<double, NX> &x) {
    std::copy(x.begin(), x.end(), x_current.begin());
}


void Env_Sim::step(std::array<double, NU> &u0, std::array<double, NX> &x_next) {
    // set inputs
    sim_in_set(acados_sim_config, acados_sim_dims,
        acados_sim_in, "x", x_current.begin());
    sim_in_set(acados_sim_config, acados_sim_dims,
        acados_sim_in, "u", u0.begin());

    // solve
    int status = cartpole_model_acados_sim_solve(capsule);
    if (status != ACADOS_SUCCESS) {
        printf("acados_solve() failed with status %d.\n", status);
    }

    // get outputs
    sim_out_get(acados_sim_config, acados_sim_dims,
            acados_sim_out, "x", x_current.begin());

    // copy the next state to x_next
    std::copy(x_current.begin(), x_current.end(), x_next.begin());

    // record state and control
    x_record.push_back({x_current[0], x_current[1], x_current[2], x_current[3]});
}