import numpy as np
import scipy.linalg
from casadi import vertcat
from acados_template import AcadosModel, AcadosOcp

def create_ocp(model: AcadosModel, ocp_options: dict) -> AcadosOcp:
    
    ocp = AcadosOcp()
    ocp.model = model

    N = ocp_options["n_pred"]       # prediction steps [-]
    Ts = ocp_options["Ts"]          # step time [s]
    Tf = N * Ts                     # prediction length [s]

    ####################### constraints ###########################
    ocp.constraints.lbu = np.array([-10])
    ocp.constraints.ubu = np.array([10])
    ocp.constraints.idxbu = np.array([0])

    ocp.constraints.x0 = np.zeros(model.x.size()[0])

    # lg < Cx + Du < ug
    ocp.constraints.C = np.array([[1, 0, 0, 0], 
                                  [0, 0, 1, 0]])
    ocp.constraints.D = np.zeros([2,1])
    ocp.constraints.lg = np.ones(2) * -10
    ocp.constraints.ug = np.ones(2) * 10

    # lb_e < x_e < ub_e
    ocp.constraints.lbx_e = np.array([-5])
    ocp.constraints.ubx_e = np.array([5])
    ocp.constraints.idxbx_e = np.array([0])

    ############################# costs ################################
    x = ocp.model.x
    u = ocp.model.u

    n_x = x.rows()
    n_u = u.size()[0]
    n_y = n_x + n_u
    n_y_e = n_x
    
    cost_options = ocp_options["cost"]
    ocp.cost.cost_type = cost_options["cost_type"]
    ocp.cost.cost_type_e = cost_options["cost_type_e"]

    Q_mat = np.diag(cost_options['Q_mat'])
    R_mat = np.diag(cost_options["R_mat"])
    ocp.cost.W = scipy.linalg.block_diag(Q_mat, R_mat)
    ocp.cost.W_e = Q_mat

    ####### I want to overcome the 2*pi discontinuity ?????
    ocp.model.cost_y_expr = vertcat(x, u)
    ocp.model.cost_y_expr_e = vertcat(x)

    ocp.cost.yref = np.ones(n_x + n_u)
    ocp.cost.yref_e = np.zeros(n_x)

    ################################# solver options ####################################
    ocp.solver_options.N_horizon = N
    ocp.solver_options.tf = Tf
    ocp.solver_options.nlp_solver_type = ocp_options["solver_type"]
    ocp.solver_options.integrator_type = ocp_options["integrator_type"]
    ocp.solver_options.qp_solver = 'PARTIAL_CONDENSING_HPIPM'
    ocp.solver_options.hessian_approx = 'GAUSS_NEWTON'

    ocp.code_export_directory = ocp_options['export_dir'] + ocp.code_export_directory + "_" + ocp.model.name
    return ocp