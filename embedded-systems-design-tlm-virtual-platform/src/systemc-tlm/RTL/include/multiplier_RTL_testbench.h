#ifndef multiplier_RTL_TESTBENCH_H
#define multiplier_RTL_TESTBENCH_H

#define PERIOD 10

#include <systemc.h>

class multiplier_RTL_testbench : public sc_core::sc_module {

private:
    void run();
    void clk_gen();

public:
    sc_in< sc_logic > p_in_enable;
    sc_in< sc_lv<32> > p_in_data;
    sc_out< sc_logic > p_out_enable;
    sc_out< sc_lv<32> > p_out_data_op1;
    sc_out< sc_lv<32> > p_out_data_op2;
    sc_out< sc_logic > reset_to_RTL;
    sc_out< sc_logic > clk;

    SC_HAS_PROCESS(multiplier_RTL_testbench);
    multiplier_RTL_testbench(sc_module_name name);
};

#endif
