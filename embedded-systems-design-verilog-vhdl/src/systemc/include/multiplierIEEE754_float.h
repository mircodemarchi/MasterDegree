/**
 * @file multiplierIEEE754_float.h
 * @author Mirco De Marchi
 * @date 30-12-2019
 * @copyright University of Verona
 * @brief Header file of floating point multiplier IEEE754 using float params.
 * 
 * Project Name: First Assignment;
 * Design Name: Multiplier floating point IEEE754 float params;
 * Module Name: multiplierIEEE754_float;
 * Target Devices: PYNQ;
 */

#ifndef MULTIPLIERIEEE754_FLOAT_H
#define MULTIPLIERIEEE754_FLOAT_H

#include <systemc.h>
#include <iostream>
#include "constants.h"

class multiplierIEEE754_float : public sc_core::sc_module
{
private:
    /// States.
    typedef enum {
        ST_0,     
        ST_1
    } TYPE_STATE;

    // Signals
    sc_signal< TYPE_STATE > next_state;

    /// @brief Structure to convert uint in float bit to bit.
    typedef union {
        unsigned int uint;
        float        floating_point;
    } ieee754_single_precision;

    /// @brief Do the multiplication between op1 and op2.
    /// FSM process.
    void doit_fsm();

    /// @brief Do the multiplication between op1 and op2.
    /// Datapath process.
    void doit_datapath();

public:
    /// Port IN.
    sc_in< sc_lv<SIZE> > op1;
    sc_in< sc_lv<SIZE> > op2;
    sc_in< sc_logic > in_rdy;
    sc_in< sc_logic > clk, rst;

    /// Port OUT.
    sc_out< sc_lv<SIZE> > res;
    sc_out< sc_logic > res_rdy;

    /// The same of SC_CTOR(multiplierIEEE754_float).
    SC_HAS_PROCESS(multiplierIEEE754_float);
    multiplierIEEE754_float(sc_core::sc_module_name module_name);

    ~multiplierIEEE754_float();
};

#endif

