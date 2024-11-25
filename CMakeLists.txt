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

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

add_executable(hello_hdl src/main.cpp)
target_compile_features(hello_hdl PUBLIC cxx_std_23)

verilate(hello_hdl COVERAGE TRACE
	INCLUDE_DIRS "src"
	VERILATOR_ARGS -f ./input.vc -x-assign fast
	SOURCES src/top.sv
)
