/**
 * @file tl.cpp
 * @author Mirco De Marchi
 * @date 30-12-2019
 * @copyright University of Verona
 * @brief Top Level.
 * 
 * Project Name: First Assignment;
 * Design Name: Top Level;
 * Module Name: tl;
 * Target Devices: PYNQ;
 */

#include "tl.h"

tl::tl(sc_core::sc_module_name module_name)
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

    mul_float = new multiplierIEEE754_float("mul_float");
    mul_fsmd  = new multiplierIEEE754_fsmd("mul_fsmd");

    // Connect float multiplier.
    mul_float->op1(op1);
    mul_float->op2(op2);
    mul_float->in_rdy(in_rdy_m0);
    mul_float->clk(clk);
    mul_float->rst(rst);
    mul_float->res(res_m0);
    mul_float->res_rdy(res_rdy_m0);

    // Connect fsmd multiplier.
    mul_fsmd->op1(op1);
    mul_fsmd->op2(op2);
    mul_fsmd->in_rdy(in_rdy_m1);
    mul_fsmd->clk(clk);
    mul_fsmd->rst(rst);
    mul_fsmd->res(res_m1);
    mul_fsmd->res_rdy(res_rdy_m1);

    SC_METHOD(serialize_res_fsm);
    sensitive << rst.pos();
    sensitive << clk.pos();

    SC_METHOD(serialize_res_datapath);
    sensitive << next_state;
}

tl::~tl()
{
}

void tl::serialize_res_fsm() {
    if (rst.read() == sc_dt::SC_LOGIC_1) 
    {
#if DEBUG
        cout << __FILE__ << ": reset flag on" << endl;
#endif
        next_state = ST_0;
    }
    else 
    {
#if DEBUG
        cout << __FILE__ << ": state " << next_state << endl;
#endif
        switch (next_state)
        {
            case ST_0: 
            {
#if DEBUG
                cout << __FILE__ 
                    << ": res_rdy_m0=" << res_rdy_m0 
                    << " res_rdy_m1=" << res_rdy_m1 
                    << endl;
#endif
                // Change state.
                if (res_rdy_m0 == sc_dt::SC_LOGIC_1 && res_rdy_m1 == sc_dt::SC_LOGIC_0)
                {
                    next_state = ST_1;
                }
                else if (res_rdy_m0 == sc_dt::SC_LOGIC_0 && res_rdy_m1 == sc_dt::SC_LOGIC_1)
                {
                    next_state = ST_2;
                }
                else if (res_rdy_m0 == sc_dt::SC_LOGIC_1 && res_rdy_m1 == sc_dt::SC_LOGIC_1)
                {
                    next_state = ST_3;
                }
                else // (res_rdy_m0 == 0 && res_rdy_m1 == 0)
                {
                    next_state = ST_0;
                }
                
                break;
            }
            case ST_1: 
            {
                next_state = ST_0;
                break;
            }
            case ST_2: 
            {
                next_state = ST_0;
                break;
            }
            case ST_3: 
            {
                next_state = ST_4;
                break;
            }
            case ST_4: 
            {
                next_state = ST_0;
                break;
            }
            default: 
            {
                next_state = ST_0;
                break;
            }
        }
    }
}

void tl::serialize_res_datapath() 
{
    switch (next_state)
    {
        case ST_0: 
        {
            res.write(0);
            res_rdy.write("00");
            tmp = 0;
            
            break;
        }
        case ST_1: 
        {
            res_rdy.write("01");
            res.write(res_m0.read());

            break;
        }
        case ST_2: 
        {
            res_rdy.write("10");
            res.write(res_m1.read());
            
            break;
        }
        case ST_3: 
        {
            res_rdy.write("01");
            res.write(res_m0.read());
            tmp = res_m1.read();
            
            break;
        }
        case ST_4: 
        {
            res_rdy.write("10");
            res.write(tmp.read());

            break;
        }
        default: 
        {
            // no operations.
            break;
        }
    }
}
