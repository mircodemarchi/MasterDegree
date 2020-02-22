/**
 * @file multiplierIEEE754_fsmd.h
 * @author Mirco De Marchi
 * @date 30-12-2019
 * @copyright University of Verona
 * @brief Header file of floating point multiplier IEEE754 performing 
 * with a fsmd.
 * 
 * Project Name: First Assignment;
 * Design Name: Multiplier floating point IEEE754 fsmd perform;
 * Module Name: multiplierIEEE754_fsmd;
 * Target Devices: PYNQ;
 */

#ifndef MULTIPLIERIEEE754_FSMD_H
#define MULTIPLIERIEEE754_FSMD_H

#include <systemc.h>
#include <iostream>
#include "constants.h"

class multiplierIEEE754_fsmd : public sc_core::sc_module
{
private:
    /// States.
    typedef enum {
        ST_0,     
        ST_1,    
        ST_2,    
        ST_3,     
        ST_4,     
        ST_5,     
        ST_6,     
        ST_7,    
        ST_8,     
        ST_9,     
        ST_10,    
        ST_11,    
        ST_12,    
        ST_13,
        ST_ROUND,   // 14
        ST_NANv1,   // 15
        ST_NANv1_1, // 16
        ST_NANv1_2, // 17
        ST_NANv2,   // 18
        ST_INF,     // 19
        ST_ZERO,    // 20
        ST_END      // 21
    } TYPE_STATE;

    // Signals.
    sc_signal< sc_lv<SIZE_OF_EXPONENT> > exp, exp1, exp2;
    sc_signal< sc_lv<SIZE_OF_EXPONENT+1> > tmpexp;
    sc_signal< sc_lv<SIZE_OF_MANTISSA> > m;
    sc_signal< sc_lv<SIZE_OF_MANTISSA+1> > m1, m2;
    sc_signal< sc_lv<2*(SIZE_OF_MANTISSA+1)> > tmpm;
    sc_signal< sc_lv<1> > s, s1, s2;

    sc_signal< TYPE_STATE > next_state;

    /// FSM.
    void fsm();

    /// DATAPATH.
    void datapath();

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
    SC_HAS_PROCESS(multiplierIEEE754_fsmd);
    multiplierIEEE754_fsmd(sc_core::sc_module_name module_name);

    ~multiplierIEEE754_fsmd();
};

#endif

