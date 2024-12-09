import os, sys, yaml

project_root = os.path.abspath(os.path.dirname(os.path.dirname(__file__)))
sys.path.append(project_root)

from build.rmpc.rmpc_bind import RMPC

controller = RMPC()