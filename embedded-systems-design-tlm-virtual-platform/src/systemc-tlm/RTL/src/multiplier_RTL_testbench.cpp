#include "multiplier_RTL_testbench.h"

multiplier_RTL_testbench::multiplier_RTL_testbench(sc_module_name name_)
    : sc_module(name_) {
    SC_THREAD(run);
    sensitive << clk.pos();

    SC_THREAD(clk_gen);
}

void multiplier_RTL_testbench::clk_gen() {
    while (true) {
        clk.write(sc_dt::SC_LOGIC_1);
        wait(PERIOD / 2, sc_core::SC_NS);
        clk.write(sc_dt::SC_LOGIC_0);
        wait(PERIOD / 2, sc_core::SC_NS);
    }
}

void multiplier_RTL_testbench::run() {
    sc_lv<32> temp_data_in_op2, temp_data_in_op1, result = 0;

    for (int i = 1; i <= 10000000; i++) {
        wait();
        reset_to_RTL.write(SC_LOGIC_1);
        wait();
        reset_to_RTL.write(SC_LOGIC_0);
        wait();
        temp_data_in_op1 = rand();
        temp_data_in_op2 = rand();

        p_out_data_op1.write(temp_data_in_op1);
        p_out_data_op2.write(temp_data_in_op2);

        p_out_enable.write(SC_LOGIC_1);
        wait();

        while (p_in_enable.read() != 1)
            wait();
        
        result = p_in_data.read();
    }

    reset_to_RTL.write(SC_LOGIC_0);
    p_out_enable.write(SC_LOGIC_0);
    p_out_data_op1.write(0);
    p_out_data_op2.write(0);

    sc_stop();
}
