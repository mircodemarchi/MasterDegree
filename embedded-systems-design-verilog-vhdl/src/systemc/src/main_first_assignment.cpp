/**
 * @file main_first_assignment.cpp
 * @author Mirco De Marchi
 * @date 30-12-2019
 * @copyright University of Verona
 * @brief Main of First Assignment SystemC project.
 * 
 * Project Name: First Assignment;
 * Target Devices: PYNQ;
 */

#include "tl.h"
#include "tb.h"
#include "constants.h"

int sc_main(int argc, char * argv[]) 
{
#if DEBUG
    cout << __FILE__ << ": enter sc_main()" << endl;
#endif
    sc_signal< sc_lv<SIZE> > op1;
    sc_signal< sc_lv<SIZE> > op2;
    sc_signal< sc_logic > in_rdy_m0, in_rdy_m1;
    sc_signal< sc_logic > clk, rst;
    sc_signal< sc_lv<SIZE> > res;
    sc_signal< sc_lv<2> > res_rdy;

    tb testbench("testbench");
    tl dut("dut");

    dut.op1(op1);
    dut.op2(op2);
    dut.in_rdy_m0(in_rdy_m0);
    dut.in_rdy_m1(in_rdy_m1);
    dut.clk(clk);
    dut.rst(rst);
    dut.res(res);
    dut.res_rdy(res_rdy);

    testbench.op1(op1);
    testbench.op2(op2);
    testbench.in_rdy_m0(in_rdy_m0);
    testbench.in_rdy_m1(in_rdy_m1);
    testbench.clk(clk);
    testbench.rst(rst);
    testbench.res(res);
    testbench.res_rdy(res_rdy);

    sc_start();
    return 0;
}
