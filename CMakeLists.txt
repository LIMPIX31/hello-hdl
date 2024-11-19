cmake_minimum_required(VERSION 3.29)
cmake_policy(SET CMP0074 NEW)
project(hello_hdl)

find_package(verilator HINTS $ENV{VERILATOR_ROOT} ${VERILATOR_ROOT})
if (NOT verilator_FOUND)
	message(
		FATAL_ERROR
		"Verilator was not found. Either install it, or set the VERILATOR_ROOT environment variable"
	)
endif ()

set(CMAKE_CXX_STANDARD 26)

add_executable(hello_hdl main.cpp)
target_compile_features(hello_hdl PUBLIC cxx_std_23)

verilate(hello_hdl
	INCLUDE_DIRS "."
	SOURCES top.v
)
