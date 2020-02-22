/**
 * @file tl.h
 * @author Mirco De Marchi
 * @date 30-12-2019
 * @copyright University of Verona
 * @brief Top Level header file.
 * 
 * Project Name: First Assignment;
 * Design Name: Top Level;
 * Module Name: tl;
 * Target Devices: PYNQ;
 */

#ifndef TL_H
#define TL_H

#include <systemc.h>
#include <iostream>
#include "multiplierIEEE754_float.h"
#include "multiplierIEEE754_fsmd.h"
#include "constants.h"

class tl : public sc_core::sc_module
{
private:
    /// States.
    typedef enum {  
        ST_0,    
        ST_1,      
        ST_2,     
        ST_3,     
        ST_4
    } TYPE_STATE;

    /// Signals.
    sc_signal< sc_lv<SIZE> > res_m0, res_m1;
    sc_signal< sc_logic > res_rdy_m0, res_rdy_m1;

    sc_signal< sc_lv<SIZE> > tmp;

    sc_signal< TYPE_STATE > next_state;

    /// Modules.
    multiplierIEEE754_float *mul_float;
    multiplierIEEE754_fsmd  *mul_fsmd;

    /// @brief Check and serialize result of two multipliers module.
    /// FSM process.
    void serialize_res_fsm();

    /// Datapath process.
    void serialize_res_datapath();

public:
    /// Port IN.
    sc_in< sc_lv<SIZE> > op1;
    sc_in< sc_lv<SIZE> > op2;
    sc_in< sc_logic > in_rdy_m0, in_rdy_m1;
    sc_in< sc_logic > clk, rst;

    /// Port OUT.
    sc_out< sc_lv<SIZE> > res;
    sc_out< sc_lv<2> > res_rdy;

    /// The same of SC_CTOR(tl).
    SC_HAS_PROCESS(tl);
    tl(sc_core::sc_module_name module_name);

    ~tl();
};

#endif
