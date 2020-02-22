/**
 * @file tb.h
 * @author Mirco De Marchi
 * @date 30-12-2019
 * @copyright University of Verona
 * @brief Test Bench header file.
 * 
 * Project Name: First Assignment;
 * Design Name: Test Bench;
 * Module Name: tb;
 * Target Devices: PYNQ;
 */

#ifndef TB_H
#define TB_H

#include <systemc.h>
#include <iostream>
#include "constants.h"

class tb : public sc_core::sc_module
{
private:
    
    // Rinning stimuli for Top Level.
    void run();

    // Update clock value.
    void clk_gen();


public:
    /// Port IN.
    sc_in< sc_lv<SIZE> > res;
    sc_in< sc_lv<2> > res_rdy;

    /// Port OUT.
    sc_out< sc_lv<SIZE> > op1;
    sc_out< sc_lv<SIZE> > op2;
    sc_out< sc_logic > in_rdy_m0, in_rdy_m1;
    sc_out< sc_logic > clk, rst;

    /// The same of SC_CTOR(tb).
    SC_HAS_PROCESS(tb);
    tb(sc_core::sc_module_name module_name);

    ~tb();
};

#endif
