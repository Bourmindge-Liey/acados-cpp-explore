cmake_minimum_required(VERSION 3.12)
project(rmpc)

if(CMAKE_COMPILER_IS_GNUCXX OR CMAKE_CXX_COMPILER_ID MATCHES "Clang")
  add_compile_options(-Wall -Wextra -Wpedantic)
endif()


####################### find dependencies #####################
set(PKG_DEPENDENCIES
  ament_cmake
  yaml-cpp
  pybind11
)

foreach(pkg_name ${PKG_DEPENDENCIES})
  find_package(${pkg_name} REQUIRED)
endforeach()


######################### acados lib #######################

########## object target names
set(MODEL_OBJ model_cartpole_model)
set(OCP_OBJ ocp_cartpole_model)
set(GEN_CODE_DIR c_generated_code_cartpole_model)

########### model
set(MODEL_SRC
  include/${GEN_CODE_DIR}/cartpole_model_model/cartpole_model_impl_dae_fun.c
  include/${GEN_CODE_DIR}/cartpole_model_model/cartpole_model_impl_dae_fun_jac_x_xdot_z.c
  include/${GEN_CODE_DIR}/cartpole_model_model/cartpole_model_impl_dae_jac_x_xdot_u_z.c
)
add_library(${MODEL_OBJ} OBJECT ${MODEL_SRC} )
target_compile_options(${MODEL_OBJ} PRIVATE -Ofast)

########## optimal control problem - mostly CasADi exports
set(OCP_SRC
  include/${GEN_CODE_DIR}/cartpole_model_cost/cartpole_model_cost_y_0_fun.c
  include/${GEN_CODE_DIR}/cartpole_model_cost/cartpole_model_cost_y_0_fun_jac_ut_xt.c
  include/${GEN_CODE_DIR}/cartpole_model_cost/cartpole_model_cost_y_0_hess.c
  include/${GEN_CODE_DIR}/cartpole_model_cost/cartpole_model_cost_y_fun.c
  include/${GEN_CODE_DIR}/cartpole_model_cost/cartpole_model_cost_y_fun_jac_ut_xt.c
  include/${GEN_CODE_DIR}/cartpole_model_cost/cartpole_model_cost_y_hess.c
  include/${GEN_CODE_DIR}/cartpole_model_cost/cartpole_model_cost_y_e_fun.c
  include/${GEN_CODE_DIR}/cartpole_model_cost/cartpole_model_cost_y_e_fun_jac_ut_xt.c
  include/${GEN_CODE_DIR}/cartpole_model_cost/cartpole_model_cost_y_e_hess.c
  include/${GEN_CODE_DIR}/acados_solver_cartpole_model.c
)
add_library(${OCP_OBJ} OBJECT ${OCP_SRC})
target_compile_options(${OCP_OBJ} PRIVATE -Ofast)

############ set some search paths for preprocessor and linker
set(ACADOS_INCLUDE_PATH /home/a/acados/include CACHE PATH "Define the path which contains the include directory for acados.")
set(ACADOS_LIB_PATH /home/a/acados/lib CACHE PATH "Define the path which contains the lib directory for acados.")


# c-compiler flags for debugging
# set(CMAKE_C_FLAGS_DEBUG "-O0 -ggdb")
set(CMAKE_C_FLAGS "-fPIC -std=c99  -DACADOS_WITH_QPOASES -Ofast")
set(CMAKE_CXX_FLAGS "-fPIC -DACADOS_WITH_QPOASES -Ofast")
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


add_library(${PROJECT_NAME} SHARED
  src/rmpc.cpp
  $<TARGET_OBJECTS:${MODEL_OBJ}> 
  $<TARGET_OBJECTS:${OCP_OBJ}>
)
 
target_include_directories(${PROJECT_NAME}
    PUBLIC 
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:include>
)
target_compile_features(${PROJECT_NAME} PUBLIC c_std_99 cxx_std_17)

target_compile_options(${PROJECT_NAME} PRIVATE -Ofast)

target_link_libraries(${PROJECT_NAME} yaml-cpp)
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

ament_export_targets(export_${PROJECT_NAME} HAS_LIBRARY_TARGET)

ament_export_dependencies(${PKG_DEPENDENCIES})

ament_package()
