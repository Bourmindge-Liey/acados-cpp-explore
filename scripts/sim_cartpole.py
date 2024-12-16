import sys
import os
sys.path.append(os.path.abspath(os.path.join(os.path.dirname(__file__), '..')))

from build.rmpc import rmpc_bind

import numpy as np
from gen_code.utils.path_utils import get_config_from_file
from gen_code.gen_ocp import gen_ocp
from gen_code.utils.plot_utils import plot_pendulum


acados_options = get_config_from_file("base_options.yaml")
ocp, ocp_solver, acados_sim = gen_ocp(acados_options)

Nsim = 100
N = ocp_solver.N
nx = ocp.model.x.rows()
nu = ocp.model.u.rows()
Tf = ocp.solver_options.tf
Fmax = ocp.constraints.ubu

simX_py = np.zeros((Nsim + 1, nx))
simU_py = np.zeros((Nsim, nu))
xcurrent_py = np.array([0.1, 0, 0.1, 0])
simX_py[0, :] = xcurrent_py

simX_cpp = np.zeros((Nsim + 1, nx))
simU_cpp = np.zeros((Nsim, nu))
xcurrent_cpp = np.array([0.1, 0, 0.1, 0])
simX_cpp[0, :] = xcurrent_cpp


yref = np.zeros((N, (nx+nu)))
yref_N = np.array([0, 0, 0, 0])


# initialize solver
for stage in range(N + 1):
    ocp_solver.set(stage, "x", xcurrent_py)
for stage in range(N):
    ocp_solver.set(stage, "u", np.zeros((nu,)))

cpp_solver = rmpc_bind.RMPC(xcurrent_cpp)
cpp_solver.set_reference(yref, yref_N)

# closed loop
for i in range(Nsim):
    # setup stage
    ocp_solver.set(0, "lbx", xcurrent_py)
    ocp_solver.set(0, "ubx", xcurrent_py)

    for j in range(N):
        ocp_solver.set(j, "yref", yref[j])
    ocp_solver.set(N, "yref", yref_N)

    # solve
    status = ocp_solver.solve()
    if status != 0:
        print(f"wrong at {i} step")
    simU_py[i,:] = ocp_solver.get(0, "u")


    cpp_solver.control(xcurrent_cpp)
    simU_cpp[i, :] = cpp_solver.get_control()

    # simulate
    xcurrent_py = acados_sim.simulate(xcurrent_py, simU_py[i, :])
    simX_py[i + 1, :] = xcurrent_py

    xcurrent_cpp = acados_sim.simulate(xcurrent_cpp, simU_cpp[i, :])
    simX_cpp[i + 1, :] = xcurrent_cpp


import matplotlib.pyplot as plt

time = np.linspace(0, Nsim * 0.05, Nsim + 1)

plt.figure()
plt.subplot(3, 1, 1)
plt.plot(time, simX_py[:, 0], label='Python')
plt.plot(time, simX_cpp[:, 0], label='C++')
plt.ylabel('Position')
plt.legend()

plt.subplot(3, 1, 2)
plt.plot(time, simX_py[:, 2], label='Python')
plt.plot(time, simX_cpp[:, 2], label='C++')
plt.ylabel('Angle')
plt.legend()

plt.subplot(3, 1, 3)
plt.plot(time[:-1], simU_py[:, 0], label='Python')
plt.plot(time[:-1], simU_cpp[:, 0], label='C++')
plt.ylabel('Input')
plt.xlabel('Time [s]')
plt.legend()

plt.show()

# plot_pendulum(np.linspace(0, Nsim * 0.05, Nsim+1), Fmax, simU_py, simX_py, latexify=True)
# plot_pendulum(np.linspace(0, Nsim * 0.05, Nsim+1), Fmax, simU_cpp, simX_cpp, latexify=True)