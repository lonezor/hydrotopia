cmake_minimum_required(VERSION 3.14)

project(hydroctrl)

include(GNUInstallDirs)

# This project needs C++20 std::chrono support
SET(CMAKE_CXX_STANDARD 20)

add_subdirectory(controller)
