/**
 * @file multiplierIEEE754_float.cpp
 * @author Mirco De Marchi
 * @date 30-12-2019
 * @copyright University of Verona
 * @brief Floating point multiplier IEEE754 using float params.
 * 
 * Project Name: First Assignment;
 * Design Name: Multiplier floating point IEEE754 float params;
 * Module Name: multiplierIEEE754_float;
 * Target Devices: PYNQ;
 */

#include "multiplierIEEE754_float.h"

multiplierIEEE754_float::multiplierIEEE754_float(sc_core::sc_module_name module_name)
    : sc_module(module_name)
    , op1("op1_mfloat")
    , op2("op2_mfloat")
    , in_rdy("in_rdy_mfloat")
    , clk("clk_mfloat")
    , rst("rst_mfloat")
    , res("res_mfloat")
    , res_rdy("res_rdy_mfloat")
{
#if DEBUG
    cout << __FILE__ << ": enter constructor" << endl;
#endif

    SC_METHOD(doit_fsm);
    sensitive << clk.pos();
    sensitive << rst.pos();

    SC_METHOD(doit_datapath);
    sensitive << next_state;
}

multiplierIEEE754_float::~multiplierIEEE754_float()
{
}

void multiplierIEEE754_float::doit_fsm() 
{
    if (rst.read() == sc_dt::SC_LOGIC_1) 
    {
#if DEBUG
        cout << __FILE__ << ": reset flag on" << endl;
#endif
        next_state = ST_0;
    }
    else if (clk.read() == sc_dt::SC_LOGIC_1) 
    {
#if DEBUG
        cout << __FILE__ << ": state " << next_state << endl;
#endif
        switch (next_state)
        {
            case ST_0:
            {
                if (in_rdy.read() == sc_dt::SC_LOGIC_1)
                {
#if DEBUG
                    cout << __FILE__ << ": in_rdy on" << endl;
#endif
                    next_state = ST_1;
                }
                else 
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
            
            default:
            {
                next_state = ST_0;
                break;   
            }
        }
    }
}

void multiplierIEEE754_float::doit_datapath() 
{
    switch (next_state)
    {
        case ST_0:
        {
            res_rdy.write(sc_dt::SC_LOGIC_0);
            res.write(0);
            break;   
        }

        case ST_1:
        {
            ieee754_single_precision tmp_op1, tmp_op2, mul;

            tmp_op1.uint = op1.read().to_uint();
            tmp_op2.uint = op2.read().to_uint();
            mul.floating_point = tmp_op1.floating_point * tmp_op2.floating_point;

            res.write(mul.uint);
            res_rdy.write(sc_dt::SC_LOGIC_1);
#if DEBUG
            cout << __FILE__ << ": op1=" << op1.read().to_string()
                << " op2=" << op2.read().to_string() << endl;

            cout << __FILE__
                << ": Floating point multiplication "
                << tmp_op1.floating_point << " * " << tmp_op2.floating_point
                << " = " << mul.floating_point << endl;
#endif
            break;   
        }
        
        default:
        {
            // Do nothing.
            break;   
        }
    }
}