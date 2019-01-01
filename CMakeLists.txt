cmake_minimum_required(VERSION 2.8.3)

project(signal_scope)
add_compile_options(-std=c++11)

list(APPEND CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake")
#list(APPEND CMAKE_PREFIX_PATH "${CMAKE_INSTALL_PREFIX}")


find_package(catkin REQUIRED COMPONENTS
  roscpp bot_core_lcmtypes drc_lcmtypes
  python_qt
  topic_tools
)

catkin_package(
  INCLUDE_DIRS
#    include
  CATKIN_DEPENDS roscpp bot_core_lcmtypes drc_lcmtypes
)

include_directories(
  include
  ${catkin_INCLUDE_DIRS}
)

find_package(PkgConfig)
pkg_check_modules(DRCLCMTYPES drc-lcmtypes)
set(use_builtin_lcmtypes_default OFF)
if(DRCLCMTYPES_FOUND)
  set(use_builtin_lcmtypes_default ON)
endif()


option(USE_BUILTIN_LCMTYPES "Compile builtin LCM types" ${use_builtin_lcmtypes_default})
option(USE_PYTHON "Enable Python support." ON)

add_subdirectory(src/signal_scope)
