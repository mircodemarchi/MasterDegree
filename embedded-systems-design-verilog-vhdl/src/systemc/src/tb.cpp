/**
 * @file tb.cpp
 * @author Mirco De Marchi
 * @date 30-12-2019
 * @copyright University of Verona
 * @brief Test Bench.
 * 
 * Project Name: First Assignment;
 * Design Name: Test Bench;
 * Module Name: tb;
 * Target Devices: PYNQ;
 */

#include <fstream>
#include <stdlib.h>
#include <time.h>
#include "tb.h"

tb::tb(sc_core::sc_module_name module_name)
    : sc_module(module_name)
    , op1("op1")
    , op2("op2")
    , in_rdy_m0("in_rdy_m0")
    , in_rdy_m1("in_rdy_m1")
    , clk("clk")
    , rst("rst")
    , res("res")
    , res_rdy("res_rdy")
{
#if DEBUG
    cout << __FILE__ << ": constructor" << endl;
#endif

    SC_THREAD(run);
    sensitive << clk.pos();

    SC_THREAD(clk_gen);
}

tb::~tb()
{
}

void tb::run() 
{
    cout << "Start tb::run()." << endl;
    int i = 1;
    unsigned int res1, res2;

    // Reset the machine.
    wait(); wait();
    rst.write(sc_dt::SC_LOGIC_1);
    wait();
    rst.write(sc_dt::SC_LOGIC_0);

    // Init values.
    in_rdy_m0.write(sc_dt::SC_LOGIC_0);
    in_rdy_m1.write(sc_dt::SC_LOGIC_0);
    wait();

    // Open files
    ifstream op_fd;
    std::string str, comment;

    op_fd.open("../test_list.txt");

    if (!op_fd.is_open())
    {
        cout << "Error opening files." << endl;
        sc_stop();
    }

    // Start reading.
    while (getline(op_fd, str))
    {
        op1.write(std::stol(str.substr(2, 8), nullptr, 16));
        op2.write(std::stol(str.substr(13, 8), nullptr, 16));
        comment = str.substr(22, std::string::npos);

        in_rdy_m0.write(sc_dt::SC_LOGIC_1);
        in_rdy_m1.write(sc_dt::SC_LOGIC_1);
        wait();

        cout << "[" << i << "] " << "USER TESTS: " << comment << endl;
        cout << "\t" << "op1=" << op1.read() << " op2=" << op2.read() << endl;

        in_rdy_m0.write(sc_dt::SC_LOGIC_0);
        in_rdy_m1.write(sc_dt::SC_LOGIC_0);

        // Wait response.
        while(res_rdy.read() == "00") wait();

        if (res_rdy.read() == "01")
        {
            res1 = res.read().to_uint();
            cout << "\t" << "Display Float Multiplier result: \t" 
                << res.read() << endl;

            while(res_rdy.read() == "01" || res_rdy.read() == "00") wait();

            res2 = res.read().to_uint();
            cout << "\t" << "Display FSMD Multiplier result: \t" 
                << res.read() << endl;
        }
        else if (res_rdy.read() == "10")
        {
            res1 = res.read().to_uint();
            cout << "\t" << "Display FSMD Multiplier result: \t" 
                << res.read() << endl;

            while(res_rdy.read() == "10"|| res_rdy.read() == "00") wait();

            res2 = res.read().to_uint();
            cout << "\t" << "Display Float Multiplier result: \t" 
                << res.read() << endl;
        }

        if (res1 == res2)
        { 
            cout << "\t" << "SUCCESS!" << endl;
        }
        else 
        {
            cout << "\t" << "FAIL!" << endl;
        }

        i++;
        wait();
    }

#if RANDOM_TEST
    // Random test.
    srand(time(NULL));
    cout << endl;
    while (i < 101) 
    {
        op1.write(((unsigned int) rand()) * 2);
        op2.write(((unsigned int) rand()) * 2);
        in_rdy_m0.write(sc_dt::SC_LOGIC_1);
        in_rdy_m1.write(sc_dt::SC_LOGIC_1);
        wait();

        cout << "[" << i << "] RANDOM TEST " << "op1=" << op1.read() << " op2=" << op2.read() << endl;

        in_rdy_m0.write(sc_dt::SC_LOGIC_0);
        in_rdy_m1.write(sc_dt::SC_LOGIC_0);

        // Wait response.
        while(res_rdy.read() == "00") wait();

        if (res_rdy.read() == "01")
        {
            res1 = res.read().to_uint();
            cout << "\t" << "Display Float Multiplier result: \t" 
                << res.read() << endl;

            while(res_rdy.read() == "01" || res_rdy.read() == "00") wait();

            res2 = res.read().to_uint();
            cout << "\t" << "Display FSMD Multiplier result: \t" 
                << res.read() << endl;
        }
        else if (res_rdy.read() == "10")
        {
            res1 = res.read().to_uint();
            cout << "\t" << "Display FSMD Multiplier result: \t" 
                << res.read() << endl;

            while(res_rdy.read() == "10"|| res_rdy.read() == "00") wait();

            res2 = res.read().to_uint();
            cout << "\t" << "Display Float Multiplier result: \t" 
                << res.read() << endl;
        }

        if (res1 == res2)
        {
            cout << "\t" << "SUCCESS!" << endl;
        }
        else 
        {
            cout << "\t" << "FAIL!" << endl;
        }

        i++;
        wait();
    } 
#endif

    cout << "End tb::run()." << endl;

    op_fd.close();
    
    sc_stop();
}

void tb::clk_gen() 
{
    while(true)
    {
        clk.write(sc_dt::SC_LOGIC_1);
        wait(CLK_PERIOD / 2, sc_core::SC_NS);
        clk.write(sc_dt::SC_LOGIC_0);
        wait(CLK_PERIOD / 2, sc_core::SC_NS);
    }
}
