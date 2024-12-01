import os, sys, yaml

project_root = os.path.abspath(os.path.dirname(__file__))
sys.path.append(project_root)

from acados_template import AcadosOcp, AcadosOcpSolver, AcadosSimSolver
from acados_template.builders import ocp_get_default_cmake_builder, sim_get_default_cmake_builder
from cartpole_model import create_model
from cartpole_ocp import create_ocp



def gen_ocp(nmpc_options: dict) -> tuple[AcadosOcp, AcadosOcpSolver, AcadosSimSolver]:

    #### first is to create a model
    model_options_path = os.path.join(os.getcwd(), nmpc_options['config_path'], nmpc_options['model_param_file']) 
    with open(model_options_path) as f:
        model_options = yaml.load(f, yaml.FullLoader)
    model = create_model(model_options)

    '''
    In a ocp, model, constraint, cost, solver, discretization, prediction steps
    '''
    ocp_options_path = os.path.join(os.getcwd(), nmpc_options['config_path'], nmpc_options['ocp_param_file']) 
    with open(ocp_options_path) as f:
        ocp_options = yaml.load(f, yaml.FullLoader)
    ocp = create_ocp(model, ocp_options)

    ocp_solver = AcadosOcpSolver(ocp, cmake_builder=ocp_get_default_cmake_builder())
    sim_solver = AcadosSimSolver(ocp, cmake_builder=sim_get_default_cmake_builder())
    return ocp, ocp_solver, sim_solver



if __name__ == "__main__":
    
    nmpc_options_path = os.path.join(os.getcwd(), "config", "base_options.yaml") 
    with open(nmpc_options_path) as f:
        nmpc_options = yaml.load(f, yaml.FullLoader)

    ocp, ocp_solver, sim_solver = gen_ocp(nmpc_options)
