import os, yaml


def get_config_from_file(file_name: str) -> dict:
    with open(os.path.join(os.getcwd(), "config", file_name)) as f:
        acados_options = yaml.load(f, yaml.FullLoader)
    return acados_options