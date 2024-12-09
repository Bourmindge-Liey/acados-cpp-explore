#ifndef RMPC_SIM_H
#define RMPC_SIM_H

// standard
#include <stdio.h>
#include <stdlib.h>
#include <array>
#include <vector> 

// acados
#include "acados/utils/print.h"
#include "acados/utils/math.h"
#include "acados_c/sim_interface.h"
#include "acados_sim_solver_cartpole_model.h"

#define NX     CARTPOLE_MODEL_NX
#define NZ     CARTPOLE_MODEL_NZ
#define NU     CARTPOLE_MODEL_NU
#define NP     CARTPOLE_MODEL_NP

class Env_Sim {
public:

    Env_Sim(double init_state[NX]);
    ~Env_Sim();

    void set_state(double x[NX]);
    void step(double u0[NU], double x_next[NX]);

private:
    // solver components
    cartpole_model_sim_solver_capsule *capsule;
    sim_config *acados_sim_config;
    sim_in *acados_sim_in;
    sim_out *acados_sim_out;
    void *acados_sim_dims;
    
    // sim data
    double x_current[NX];
    std::vector<std::array<double, NX>> x_record;
    std::vector<std::array<double, NU>> u_record;
    
    // sim status
    int status;
};

#endif // RMPC_SIM_H