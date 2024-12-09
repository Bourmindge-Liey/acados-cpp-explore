# include "rmpc/rmpc.h"
# include "rmpc/sim.h"

int main() {
    // initialize the environment
    double x_init[NX] = {1, 0, 0.2, 0};
    Env_Sim env(x_init);

    // initialize the controller
    RMPC rmpc(x_init);

    // simulate
    double x[NX];
    double u[NU];
    std::copy(std::begin(x_init), std::end(x_init), x);
    double y_ref[N_STEPS][NY];


    for (int i = 0; i < 100; i++) {
        rmpc.control(x, u);
        env.step(u, x);
    }
    

    return 0;
}