cmake_minimum_required(VERSION 2.8.3)

project(signal_scope)

list(APPEND CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake")
#list(APPEND CMAKE_PREFIX_PATH "${CMAKE_INSTALL_PREFIX}")


find_package(catkin REQUIRED COMPONENTS
  roscpp
  python_qt
  ctk_python_console
  topic_tools
)

catkin_package(
  INCLUDE_DIRS
#    include
  CATKIN_DEPENDS roscpp
)

include_directories(
  include
  ${catkin_INCLUDE_DIRS}
)

find_package(PkgConfig)


option(USE_PYTHON "Enable Python support." ON)

add_subdirectory(src/signal_scope)
