import numpy as np
from utils.path_utils import get_config_from_file
from gen_ocp import gen_ocp
from utils.plot_utils import plot_pendulum

acados_options = get_config_from_file("base_options.yaml")
ocp, ocp_solver, acados_sim = gen_ocp(acados_options)

Nsim = 100
N = ocp_solver.N
nx = ocp.model.x.rows()
nu = ocp.model.u.rows()
Tf = ocp.solver_options.tf
Fmax = ocp.constraints.ubu

simX = np.zeros((Nsim + 1, nx))
simU = np.zeros((Nsim, nu))

xcurrent = np.array([0.1, 0., 0.1, 0])
simX[0, :] = xcurrent

yref = np.array([0, 0, 0, 0, 0])
yref_N = np.array([0, 0, 0, 0])

# initialize solver
for stage in range(N + 1):
    ocp_solver.set(stage, "x", xcurrent)
for stage in range(N):
    ocp_solver.set(stage, "u", np.zeros((nu,)))

# closed loop
for i in range(Nsim):
    # setup stage
    ocp_solver.set(0, "lbx", xcurrent)
    ocp_solver.set(0, "ubx", xcurrent)

    for j in range(N):
        ocp_solver.set(j, "yref", yref)
    ocp_solver.set(N, "yref", yref_N)

    # solve
    status = ocp_solver.solve()
    if status != 0:
        print(f"wrong at {i} step")
    simU[i,:] = ocp_solver.get(0, "u")

    # simulate
    xcurrent = acados_sim.simulate(xcurrent, simU[i, :])
    simX[i + 1, :] = xcurrent

plot_pendulum(np.linspace(0, Nsim * 0.05, Nsim+1), Fmax, simU, simX, latexify=True)