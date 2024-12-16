import sys
import os
sys.path.append(os.path.abspath(os.path.join(os.path.dirname(__file__), '../..')))

from build.pybind_dummy_examples import pybind_dummy_examples


print(pybind_dummy_examples.add(1,2))