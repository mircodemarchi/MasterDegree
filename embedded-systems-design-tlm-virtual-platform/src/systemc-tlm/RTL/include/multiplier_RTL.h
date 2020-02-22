#ifndef multiplier_RTL_H
#define multiplier_RTL_H

#include <systemc.h>

#define SIZE 32
#define SIZE_OF_MANTISSA 23
#define SIZE_OF_EXPONENT 8

SC_MODULE(multiplier_RTL) {

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

    /// Port IN.
    sc_in< sc_lv<SIZE> > op1;
    sc_in< sc_lv<SIZE> > op2;
    sc_in< sc_logic > in_rdy;
    sc_in< sc_logic > rst, clk; 

    /// Port OUT.
    sc_out< sc_lv<SIZE> > res;
    sc_out< sc_logic > res_rdy;

    // Signals.
    sc_signal< sc_lv<SIZE_OF_EXPONENT> > exp, exp1, exp2;
    sc_signal< sc_lv<SIZE_OF_EXPONENT+1> > tmpexp;
    sc_signal< sc_lv<SIZE_OF_MANTISSA> > m;
    sc_signal< sc_lv<SIZE_OF_MANTISSA+1> > m1, m2;
    sc_signal< sc_lv<2*(SIZE_OF_MANTISSA+1)> > tmpm;
    sc_signal< sc_lv<1> > s, s1, s2;

    // TYPE_STATE state, next_state;
    sc_signal< TYPE_STATE > state, next_state;

    /// FSM.
    void fsm();

    /// DATAPATH.
    void datapath();

    // SC_HAS_PROCESS(multiplierIEEE754_fsmd);
    // multiplierIEEE754_fsmd(sc_core::sc_module_name module_name);
    SC_CTOR(multiplier_RTL)
    {
        SC_METHOD(fsm);
        sensitive << clk.pos();
        sensitive << rst;

        SC_METHOD(datapath);
        sensitive << next_state;
    }
};

#endif
