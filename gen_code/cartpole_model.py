import os
import yaml

from casadi import MX, vertcat, sin, cos, fmod, pi
from acados_template import AcadosModel

def create_model(model_options: dict) -> AcadosModel:

    ###################### model parameters ###############
    m_c = model_options['m_cart']
    m_p = model_options['m_pole']
    l_p = model_options['l_pole']
    g = model_options['g']

    ###################### states ####################### 
    pos = MX.sym("pos")         # position of the cart [m]
    phi = MX.sym("phi")         # angle of the pole [rad]
    d_pos = MX.sym("d_pos")     # velocity of the cart [m/s]
    d_phi = MX.sym("d_phi")     # angular velocity of the pole [rad/s]
    x = vertcat(pos, phi, d_pos, d_phi)   

    ##################### input ###################
    u_f = MX.sym("u_f")     # horizontal force applied on cart [N]
    u = vertcat(u_f)

    ###################### derivatives ###############
    pos_dot = MX.sym("pos_dot")
    phi_dot = MX.sym("phi_dot")
    d_pos_dot = MX.sym("d_pos_dot")
    d_phi_dot = MX.sym("d_phi_dot")
    xdot = vertcat(pos_dot, phi_dot, d_pos_dot, d_phi_dot)

    ####################### parameters ######################
    p = vertcat([])

    ########################## algebraic eqns. #######################
    z = vertcat([])

    ######################### dynamics #######################
    d_pos_expl = d_pos
    d_phi_expl = d_phi

    denom = m_c + m_p * (1 - cos(phi)**2)
    d_d_pos_expl = (-m_p*l_p*sin(phi)*d_phi**2 + m_p*g*cos(phi)*sin(phi) + u_f) / denom
    d_d_phi_expl = (-m_p*l_p*cos(phi)*sin(phi)*d_phi**2 + (m_c+m_p)*g*sin(phi) + u_f*cos(phi)) / denom / l_p

    f_expl = vertcat(d_pos_expl, d_phi_expl, d_d_pos_expl, d_d_phi_expl)
    f_impl = xdot - f_expl
    
    ############################# build model ############################ 
    model = AcadosModel()
    model.name = model_options['name']

    model.x = x
    model.u = u
    model.xdot = xdot
    model.p = p
    model.z = z 
    model.f_expl_expr = f_expl
    model.f_impl_expr = f_impl

    return model
