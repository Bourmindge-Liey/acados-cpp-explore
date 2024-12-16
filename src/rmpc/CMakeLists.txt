cmake_minimum_required(VERSION 3.12)
project(rmpc)

if(CMAKE_COMPILER_IS_GNUCXX OR CMAKE_CXX_COMPILER_ID MATCHES "Clang")
  add_compile_options(-Wall -Wextra -Wpedantic)
endif()

set(CMAKE_BUILD_TYPE Debug)
####################### find dependencies #####################
set(PKG_DEPENDENCIES
  ament_cmake
  yaml-cpp
)
foreach(pkg_name ${PKG_DEPENDENCIES})
  find_package(${pkg_name} REQUIRED)
endforeach()

find_package(pybind11 CONFIG REQUIRED)
find_package(Python3 REQUIRED COMPONENTS Interpreter Development)
######################### acados lib #######################

########## object target names
set(MODEL_OBJ model_cartpole_model)
set(OCP_OBJ ocp_cartpole_model)
set(SIM_OBJ sim_cartpole_model)
set(GEN_CODE_DIR include/c_generated_code_cartpole_model)

########### model
set(MODEL_SRC
  ${GEN_CODE_DIR}/cartpole_model_model/cartpole_model_impl_dae_fun.c
  ${GEN_CODE_DIR}/cartpole_model_model/cartpole_model_impl_dae_fun_jac_x_xdot_z.c
  ${GEN_CODE_DIR}/cartpole_model_model/cartpole_model_impl_dae_jac_x_xdot_u_z.c
)
add_library(${MODEL_OBJ} OBJECT ${MODEL_SRC} )
target_compile_options(${MODEL_OBJ} PRIVATE -Ofast)

########## optimal control problem - mostly CasADi exports
set(OCP_SRC
  ${GEN_CODE_DIR}/cartpole_model_cost/cartpole_model_cost_y_0_fun.c
  ${GEN_CODE_DIR}/cartpole_model_cost/cartpole_model_cost_y_0_fun_jac_ut_xt.c
  ${GEN_CODE_DIR}/cartpole_model_cost/cartpole_model_cost_y_0_hess.c
  ${GEN_CODE_DIR}/cartpole_model_cost/cartpole_model_cost_y_fun.c
  ${GEN_CODE_DIR}/cartpole_model_cost/cartpole_model_cost_y_fun_jac_ut_xt.c
  ${GEN_CODE_DIR}/cartpole_model_cost/cartpole_model_cost_y_hess.c
  ${GEN_CODE_DIR}/cartpole_model_cost/cartpole_model_cost_y_e_fun.c
  ${GEN_CODE_DIR}/cartpole_model_cost/cartpole_model_cost_y_e_fun_jac_ut_xt.c
  ${GEN_CODE_DIR}/cartpole_model_cost/cartpole_model_cost_y_e_hess.c
  ${GEN_CODE_DIR}/acados_solver_cartpole_model.c
)
add_library(${OCP_OBJ} OBJECT ${OCP_SRC})
target_compile_options(${OCP_OBJ} PRIVATE -Ofast)

########## simulation ###############
set(SIM_SRC ${GEN_CODE_DIR}/acados_sim_solver_cartpole_model.c)
add_library(${SIM_OBJ} OBJECT ${SIM_SRC})
target_compile_options(${SIM_OBJ} PRIVATE -Ofast)

############ set some search paths for preprocessor and linker
set(ACADOS_INCLUDE_PATH /home/a/acados/include CACHE PATH "Define the path which contains the include directory for acados.")
set(ACADOS_LIB_PATH /home/a/acados/lib CACHE PATH "Define the path which contains the lib directory for acados.")


# c-compiler flags for debugging
# set(CMAKE_C_FLAGS_DEBUG "-O0 -ggdb")
set(CMAKE_C_FLAGS "-fPIC -std=c99  -DACADOS_WITH_QPOASES -Ofast")
set(CMAKE_CXX_FLAGS "-fPIC -DACADOS_WITH_QPOASES -Ofast")
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
#-fno-diagnostics-show-line-numbers -g

include_directories(
   ${ACADOS_INCLUDE_PATH}
   ${ACADOS_INCLUDE_PATH}/acados
   ${ACADOS_INCLUDE_PATH}/blasfeo/include
   ${ACADOS_INCLUDE_PATH}/hpipm/include
)

# linker flags
link_directories(${ACADOS_LIB_PATH})

# link to libraries
if(UNIX)
    link_libraries(acados hpipm blasfeo m )
else()
    link_libraries(acados hpipm blasfeo )
endif()

####################### acados lib finish #######################

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include
                    ${CMAKE_CURRENT_SOURCE_DIR}/include/c_generated_code_cartpole_model
)


add_library(${PROJECT_NAME} 
  src/rmpc.cpp
  $<TARGET_OBJECTS:${MODEL_OBJ}> 
  $<TARGET_OBJECTS:${OCP_OBJ}>
)
target_compile_features(${PROJECT_NAME} PUBLIC c_std_99 cxx_std_17)
target_compile_options(${PROJECT_NAME} PRIVATE -Ofast)
ament_target_dependencies(${PROJECT_NAME} ${PKG_DEPENDENCIES})

install(
  DIRECTORY include/
  DESTINATION include
)
install(
  TARGETS ${PROJECT_NAME}
  EXPORT export_${PROJECT_NAME}
  DESTINATION lib/${PROJECT_NAME}
  LIBRARY DESTINATION lib
  ARCHIVE DESTINATION lib
  RUNTIME DESTINATION bin
  INCLUDES DESTINATION include
)


set(SIM_NAME sim_env_cartpole)
add_library(${SIM_NAME} 
  src/sim.cpp
  $<TARGET_OBJECTS:${MODEL_OBJ}> 
  $<TARGET_OBJECTS:${SIM_OBJ}>
)
install(
  TARGETS ${SIM_NAME}
  EXPORT export_${SIM_NAME}
  DESTINATION lib/${SIM_NAME}
  LIBRARY DESTINATION lib
  ARCHIVE DESTINATION lib
  RUNTIME DESTINATION bin
  INCLUDES DESTINATION include
)


set(CONTROL_SIM_NAME control_cartpole_model)
add_executable(${CONTROL_SIM_NAME} src/run_control_sim.cpp)
target_link_libraries(${CONTROL_SIM_NAME} ${PROJECT_NAME} ${SIM_NAME})
install(
  TARGETS ${CONTROL_SIM_NAME}
  EXPORT export_${CONTROL_SIM_NAME}
  DESTINATION lib/${CONTROL_SIM_NAME}
  LIBRARY DESTINATION lib
  ARCHIVE DESTINATION lib
  RUNTIME DESTINATION bin
  INCLUDES DESTINATION include
)


set(MODULE_NAME rmpc_bind)
pybind11_add_module(
  ${MODULE_NAME} src/bind_cpp.cpp   
  src/rmpc.cpp
  $<TARGET_OBJECTS:${MODEL_OBJ}> 
  $<TARGET_OBJECTS:${OCP_OBJ}>)


ament_export_targets(export_${PROJECT_NAME} HAS_LIBRARY_TARGET)
ament_export_dependencies(${PKG_DEPENDENCIES})
ament_package()
