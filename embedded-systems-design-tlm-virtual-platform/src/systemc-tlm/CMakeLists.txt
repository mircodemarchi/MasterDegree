cmake_minimum_required(VERSION 2.8.4)

# ------------------------------------------------------------------------------
# Set the project name.
project(SecondAssignment)
set(PROJECT_NAME SecondAssignment)

set(TLM_UT SecondAssignment_TLM_UT.x)
set(TLM_LT SecondAssignment_TLM_LT.x)
set(TLM_RTL SecondAssignment_TLM_RTL.x)
set(TLM_AT4 SecondAssignment_TLM_AT4.x)

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


add_subdirectory(UT)
add_subdirectory(RTL)
add_subdirectory(AT4)
add_subdirectory(LT)

# ------------------------------------------------------------------------------
# Add command for testing time
set(TIME_PROG time)

add_custom_target(
    time

    # Print TML UT report time
    COMMAND echo ""
    COMMAND echo ""
    COMMAND echo "-- TLM UT: report time"
    COMMAND ${TIME_PROG} ${CMAKE_SOURCE_DIR}/bin/${TLM_UT}

    # Print TML LT report time
    COMMAND echo ""
    COMMAND echo ""
    COMMAND echo "-- TLM LT: report time"
    COMMAND ${TIME_PROG} ${CMAKE_SOURCE_DIR}/bin/${TLM_LT}

    # Print TML RTL report time
    COMMAND echo ""
    COMMAND echo ""
    COMMAND echo "-- TLM RTL: report time"
    COMMAND ${TIME_PROG} ${CMAKE_SOURCE_DIR}/bin/${TLM_RTL}

    # Print TML AT4 report time
    COMMAND echo ""
    COMMAND echo ""
    COMMAND echo "-- TLM AT4: report time"
    COMMAND ${TIME_PROG} ${CMAKE_SOURCE_DIR}/bin/${TLM_AT4}
    
    DEPENDS ${TLM_UT}
    DEPENDS ${TLM_LT}
    DEPENDS ${TLM_RTL}
    DEPENDS ${TLM_AT4}
)