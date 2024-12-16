#ifndef RUN_CONTROL_SIM_CPP
#define RUN_CONTROL_SIM_CPP

# include <iostream>

# include "rmpc/rmpc.h"
# include "rmpc/sim.h"

int main() {
    // initialize the environment
    std::array<double, NX> x_init = {0.0, 0, 0.1, 0};
    Env_Sim env(x_init);

    // initialize the controller
    RMPC rmpc(x_init);

    // simulate
    std::array<double, NX> x;
    std::array<double, NU> u;
    u[0] = 0.1;
    std::copy(x_init.begin(), x_init.end(), x.begin());

    std::array<std::array<double, NY>, N_STEPS> y_ref;
    for (auto& row : y_ref) {
        row.fill(0.0);
    }
    std::array<double, NX> y_ref_e;
    y_ref_e.fill(0.0);

    for (int i = 0; i < 100; i++) {
        for (int j = 0; j < NX; j++) {
            std::cout << x[j] << " ";
        }
        std::cout << std::endl;
        std::cout << "u: " << u[0] << std::endl;

        rmpc.set_reference(y_ref, y_ref_e);
        rmpc.control(x);
        u = rmpc.get_control();
        env.step(u, x);
    }

    return 0;
}


#endif