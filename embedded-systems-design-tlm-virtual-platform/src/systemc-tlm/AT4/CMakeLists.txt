cmake_minimum_required(VERSION 2.8.4)

# ------------------------------------------------------------------------------
# Set the project name.
project(SecondAssignment_TLM_AT4)
set(PROJECT_NAME SecondAssignment_TLM_AT4)

# ------------------------------------------------------------------------------
# Configure SystemC Library.

# Check for SYSTEM_HOME variable.
set(SYSTEM_HOME $ENV{SYSTEM_HOME} CACHE PATH "SystemC installation path")

if (NOT DEFINED ENV{SYSTEM_HOME})
    message(FATAL_ERROR 
        "SYSTEM_HOME is not defined. You must tell CMake where to find the SystemC installation path in SYSTEM_HOME variable.")
else()
    message(STATUS "SYSTEM_HOME=$ENV{SYSTEM_HOME}")
endif()

# Check for system platform and architeture.
set(SYSTEMC_LIBDIR $ENV{SYSTEM_HOME})

if (CMAKE_SYSTEM_NAME STREQUAL Darwin)
    set(SYSTEMC_LIBDIR ${SYSTEMC_LIBDIR}/lib-macosx)
elseif(CMAKE_SYSTEM_NAME STREQUAL Linux)
    set(SYSTEMC_LIBDIR ${SYSTEMC_LIBDIR}/lib-linux)
endif()

if (CMAKE_SIZEOF_VOID_P EQUAL 8)
    set(SYSTEMC_LIBDIR ${SYSTEMC_LIBDIR}64)
endif()

# Link SystemC Library.
link_directories(${SYSTEM_HOME})

# ------------------------------------------------------------------------------
# Set the compiler options.
# set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O1")
# set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O2")
# set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O3")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g3")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -ggdb")
# set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fomit-frame-pointer")
# set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fno-builtin")
# set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fno-stack-protector")
# set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall")
# set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Werror")
# set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -march=i686")
# set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -m32")
# set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=gnu99")
# set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wpedantic")
# set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pedantic-errors")
# set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wextra")
# set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wconversion")
# set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wcast-align")
# set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wcast-qual")
# set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wdisabled-optimization")
# set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wfloat-equal")
# set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wfloat-conversion")
# set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wformat=2")
# set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wformat-nonliteral")
# set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wformat-security")
# set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wformat-y2k")
# set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wimport")
# set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Winit-self")
# set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Winline")
# set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Winvalid-pch")
# set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-long-long")
# set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wmissing-field-initializers")
# set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wmissing-format-attribute")
# set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wmissing-include-dirs")
# set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wpacked")
# set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wpointer-arith")
# set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wredundant-decls")
# set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wshadow")
# set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wstack-protector")
# set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wstrict-aliasing=2")
# set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wswitch")
# set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wswitch-default")
# set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wswitch-enum")
# set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wunreachable-code")
# set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wunused")
# set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wunused-function")
# set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wunused-label")
# set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wunused-parameter")
# set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wunused-value")
# set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wunused-variable")
# set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wvariadic-macros")
# set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wwrite-strings")
# set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wsign-compare")
# set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wsign-conversion")
# set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wuninitialized")
# set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fmessage-length=0")

# ------------------------------------------------------------------------------
# Add the includes.
# MACRO(DIRTREELIST result curdir)
#     FILE(GLOB children RELATIVE ${curdir} ${curdir}/*)
#     SET(dirlist "${curdir}")
#     FOREACH(child ${children})
#         IF(IS_DIRECTORY ${curdir}/${child})
#             SET(dirlist "${dirlist}\n${curdir}/${child}")
#         ENDIF()
#     ENDFOREACH()
#     SET(${result} ${dirlist})
# ENDMACRO()
# DIRTREELIST(TREEDIRS $ENV{SYSTEM_HOME}/include)

include_directories(
    include
    $ENV{SYSTEM_HOME}/include
    #${TREEDIRS}
)

# ------------------------------------------------------------------------------
# Add the source files.
set(SOURCE_FILES
    src/main_multiplier_AT4.cpp
    src/multiplier_AT4_testbench.cpp
    src/multiplier_AT4.cpp
    )

# ------------------------------------------------------------------------------
# Set the linker options.
set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -lpthread")

# ------------------------------------------------------------------------------
# Add the executable.
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY
    ${CMAKE_SOURCE_DIR}/bin
    )

set(EXECUTABLE_NAME
    ${PROJECT_NAME}.x
    )

add_executable(${EXECUTABLE_NAME} ${SOURCE_FILES})
target_link_libraries(${EXECUTABLE_NAME} ${SYSTEMC_LIBDIR}/libsystemc.a)

# # ------------------------------------------------------------------------------
# # Add command for running
# add_custom_target(
#     run_AT4
#     COMMAND ${EXECUTABLE_NAME}
#     DEPENDS ${EXECUTABLE_NAME}
# )

# # ------------------------------------------------------------------------------
# # Add command for debug mode
# set(GDB_PROG cgdb)

# add_custom_target(
#     gdb_AT4
#     COMMAND ${GDB_PROG} ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/${EXECUTABLE_NAME}
#     DEPENDS ${EXECUTABLE_NAME}
# )

# # ------------------------------------------------------------------------------
# # Add command for testing time
# set(TIME_PROG time)

# add_custom_target(
#     time
#     COMMAND ${TIME_PROG} ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/${EXECUTABLE_NAME}
#     DEPENDS ${EXECUTABLE_NAME}
# )