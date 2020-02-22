/**
 * @file multiplierIEEE754_float_systemc.h
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

#include <systemc.h>

#define SIZE 32

SC_MODULE(multiplierIEEE754_float)
{
    /// @brief Structure to convert uint in float bit to bit.
    typedef union {
        unsigned int uint;
        float        floating_point;
    } ieee754_single_precision;

    /// States.
    typedef enum {
        ST_0,     
        ST_1
    } TYPE_STATE;

    // Signals
    sc_signal< TYPE_STATE > state, next_state;

    /// Port IN.
	sc_in< sc_lv<SIZE> > op1;
	sc_in< sc_lv<SIZE> > op2;
	sc_in< sc_logic > in_rdy;
	sc_in< sc_logic > rst;
	sc_in_clk clk;

	/// Port OUT.
	sc_out< sc_lv<SIZE> > res;
	sc_out< sc_logic > res_rdy;

    /// @brief Do the multiplication between op1 and op2.
    void multiplierIEEE754_float::doit_fsm() 
    {
        if (rst.read() == sc_dt::SC_LOGIC_1) 
        {
            next_state.write(ST_0);
        }
        else if (clk.read() == sc_dt::SC_LOGIC_1) 
        {
            switch (next_state)
            {
                case ST_0:
                {
                    if (in_rdy.read() == sc_dt::SC_LOGIC_1)
                    {
                        next_state.write(ST_1);
                    }
                    else 
                    {
                        next_state.write(ST_0);
                    }

                    break;   
                }

                case ST_1:
                {
                    next_state.write(ST_0);
                    break;   
                }
                
                default:
                {
                    next_state.write(ST_0);
                    break;   
                }
            }
        }
    }

    void multiplierIEEE754_float::doit_datapath() 
    {
        state.write(next_state.read());
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

                break;   
            }
            
            default:
            {
                // Do nothing.
                break;   
            }
        }
    }

    // SC_HAS_PROCESS(multiplierIEEE754_float);
    // multiplierIEEE754_float(sc_core::sc_module_name module_name);
    SC_CTOR(multiplierIEEE754_float) 
        : op1("op1_mfloat")
        , op2("op2_mfloat")
        , in_rdy("in_rdy_mfloat")
        , clk("clk_mfloat")
        , rst("rst_mfloat")
        , res("res_mfloat")
        , res_rdy("res_rdy_mfloat")
    {
        SC_METHOD(doit_fsm);
        sensitive_pos << clk;
        sensitive << rst;

        SC_METHOD(doit_datapath);
        sensitive << next_state;
    }

};

