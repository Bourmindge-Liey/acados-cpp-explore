from acados_template import AcadosSim


def create_sim(model, sim_options):
    Ts = sim_options["Ts"]
    integrator_type = sim_options["integrator_type"]

    sim = AcadosSim()
    sim.model = model
    sim.solver_options.integrator_type = integrator_type  # Explicit Runge-Kutta integrator
    sim.solver_options.T = Ts  # Simulation step size
    return sim