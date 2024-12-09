# include <iostream>

# include "rmpc/rmpc.h"
# include "rmpc/sim.h"

int main() {
    // initialize the environment
    double x_init[NX] = {0.0, 0, 0.1, 0};
    Env_Sim env(x_init);

    // initialize the controller
    RMPC rmpc(x_init);

    // simulate
    double x[NX];
    double u[NU];
    std::copy(std::begin(x_init), std::end(x_init), x);

    double y_ref[N_STEPS][NY];
    std::fill(&y_ref[0][0], &y_ref[0][0] + sizeof(y_ref) / sizeof(y_ref[0][0]), 0.0);
    double y_ref_e[NX];
    std::fill(&y_ref_e[0], &y_ref_e[0] + sizeof(y_ref_e) / sizeof(y_ref_e[0]), 0.0);

    for (int i = 0; i < 100; i++) {
        for (int j = 0; j < NX; j++) {
            std::cout << x[j] << " ";
        }
        std::cout << std::endl;
        std::cout << "u: " << u[0] << std::endl;

        rmpc.set_reference(y_ref, y_ref_e);
        rmpc.control(x, u);
        env.step(u, x);
    }


    return 0;
}