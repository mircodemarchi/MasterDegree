/**
 * @file multiplierIEEE754_fsmd_systemc.cpp
 * @author Mirco De Marchi
 * @date 30-12-2019
 * @copyright University of Verona
 * @brief Floating point multiplier IEEE754 performing with a fsmd.
 * 
 * Project Name: First Assignment;
 * Design Name: Multiplier floating point IEEE754 fsmd perform;
 * Module Name: multiplierIEEE754_fsmd;
 * Target Devices: PYNQ;
 */

#include <systemc.h>

#define SIZE 32
#define SIZE_OF_MANTISSA 23
#define SIZE_OF_EXPONENT 8

SC_MODULE(multiplierIEEE754_fsmd)
{
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
    sc_in< sc_logic > rst; 
    sc_in_clk clk;

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
    void multiplierIEEE754_fsmd::fsm() 
    {
        if (rst.read() == sc_dt::SC_LOGIC_1) 
        {
            next_state.write(ST_0);
        }
        else if (clk.read() == sc_dt::SC_LOGIC_1) 
        {
            switch (state) 
            {
                case ST_0: 
                {
                    if (in_rdy.read() == sc_dt::SC_LOGIC_1)
                        next_state.write(ST_1);
                    else
                        next_state.write(ST_0);
                    
                    break;
                }
                case ST_1:
                {
                        
                    if ((exp1.read() == 0xFF && m1.read() != 0)
                        || (exp2.read() == 0xFF && m2.read() != 0))
                    { 
                        // op1 or op2 are NaN
                        next_state.write(ST_NANv1);
                    }
                    else if ((exp1.read() == 0xFF && m1.read() == 0
                        && exp2.read() == 0 && m2.read() == 0)
                        || (exp1.read() == 0 && m1.read() == 0
                        && exp2.read() == 0xFF && m2.read() == 0))
                    { 
                        // op1 is inf and op2 is 0 or vice versa
                        next_state.write(ST_NANv2);
                    }
                    else if ((exp1.read() == 0xFF && m1.read() == 0)
                        || (exp2.read() == 0xFF && m2.read() == 0))
                    { 
                        // op1 or op2 are inf
                        next_state.write(ST_INF);
                    }
                    else if ((exp1.read() == 0 && m1.read() == 0)
                        || (exp2.read() == 0 && m2.read() == 0))
                    { 
                        // op1 or op2 are zero
                        next_state.write(ST_ZERO);
                    }
                    else
                    { 
                        // op1 and op2 are normalized or denormalized number 
                        next_state.write(ST_2);
                    }

                    break;
                }
                case ST_2:
                {
                    if (exp1.read() != 0 && exp2.read() != 0)
                    {
                        // Both op1 and op2 are normalized
                        next_state.write(ST_3);
                    }
                    else if (exp1.read() != 0)
                    {
                        // Only op1 is normalized
                        next_state.write(ST_4);
                    }
                    else if (exp2.read() != 0)
                    {
                        // Only op2 is normalized
                        next_state.write(ST_5);
                    }
                    else 
                    {
                        // Both denormalized
                        next_state.write(ST_6);
                    } 

                    break;
                }
                case ST_3:
                {
                    next_state.write(ST_6);
                    break;
                }
                case ST_4:
                {
                    next_state.write(ST_6);
                    break;
                }
                case ST_5:
                {
                    next_state.write(ST_6);
                    break;
                }
                case ST_6:
                {
                    if (tmpm.read().range(47, 46) == "10" 
                        || tmpm.read().range(47, 46) == "11") 
                    {
                        // Only one DX shift
                        next_state.write(ST_7);
                    }
                    else if (tmpm.read().range(47, 46) == "00")
                    {
                        // SX shift
                        next_state.write(ST_8);
                    }
                    else // if (tmpm[47:46] == 2'b01)
                    {
                        // Mantissa already normalized
                        next_state.write(ST_10);
                    }

                    break;
                }
                case ST_7:
                {
                    next_state.write(ST_10);
                    break;
                }
                case ST_8:
                {
                    if (tmpm.read().range(47, 46) == "01" || tmpexp.read() == 0)
                    {
                        // Mantissa normalized or denormalized
                        next_state.write(ST_10);
                    }
                    else // if (tmpm.read().range(47, 46) == "00")
                    {
                        // Still SX shift
                        next_state.write(ST_9); 
                    }
                
                    break;
                }
                case ST_9:
                {
                    next_state.write(ST_8);
                    break;
                }
                case ST_10:
                {
                    if (tmpexp.read()[8] == 1 
                        && ((exp1.read().to_uint() + exp2.read().to_uint()) < 127))
                    {
                        // Underflow
                        next_state.write(ST_7);
                    }
                    else if (tmpexp.read()[8] == 1 
                        && ((exp1.read().to_uint() + exp2.read().to_uint()) >= 127))
                    {
                        // Overflow
                        next_state.write(ST_INF);
                    }
                    else // if (tmpexp.read()[8] == 0)
                    {
                        // Prepare exponent
                        next_state.write(ST_11);
                    }

                    break;
                }
                case ST_11:
                {
                    if (tmpm.read()[22] == 1)
                    {
                        // Round
                        next_state.write(ST_ROUND);
                    }
                    else 
                    {
                        // Prepare result
                        next_state.write(ST_13);
                    }

                    break;
                }
                case ST_ROUND:
                {
                    if (tmpm.read().range(47, 46) == "10")
                    {
                        // Prepare result
                        next_state.write(ST_12);
                    }
                    else // if (tmpm(47 downto 46) = '01' || '00')
                    {
                        next_state.write(ST_13);
                    }

                    break;
                }
                case ST_13:
                {
                    next_state.write(ST_END);
                    break;
                }
                case ST_12:
                {
                    next_state.write(ST_10);
                    break;
                }
                case ST_NANv1:
                {
                    if (exp1.read() == 0xFF && m1.read() != 0)
                    {
                        next_state.write(ST_NANv1_1);
                    }
                    else // if (exp2.read() == 0xFF && m2.read() != 0)
                    {
                        next_state.write(ST_NANv1_2);
                    }
                    break;
                }
                case ST_NANv1_1:
                {
                    next_state.write(ST_END);
                    break;
                }
                case ST_NANv1_2:
                {
                    next_state.write(ST_END);
                    break;
                }
                case ST_NANv2:
                {
                    next_state.write(ST_END);
                    break;
                }
                case ST_INF:
                {
                    next_state.write(ST_END);
                    break;
                }
                case ST_ZERO:
                {
                    next_state.write(ST_END);
                    break;
                }
                case ST_END:
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

    /// DATAPATH.
    void multiplierIEEE754_fsmd::datapath() 
    {
        state.write(next_state.read());
        switch (next_state)
        {
            case ST_0: 
            {
                exp    = 0;
                exp1   = 0;
                exp2   = 0;
                tmpexp = 0;
                m      = 0;
                m1     = 0;
                m2     = 0;
                tmpm   = 0;
                s      = 0;
                s1     = 0;
                s2     = 0;

                res_rdy.write(sc_dt::SC_LOGIC_0);
                res.write(0);
                
                break;
            }
            case ST_1:
            {
                sc_uint<1> bit_zero = 0;
                m1 = (bit_zero, op1.read().range(22, 0));
                m2 = (bit_zero, op2.read().range(22, 0));

                exp1 = op1.read().range(30, 23);
                exp2 = op2.read().range(30, 23);
                
                s1 = op1.read().get_bit(31);
                s2 = op2.read().get_bit(31);
                
                s = (bool) op1.read().get_bit(31) ^ (bool) op2.read().get_bit(31);
                
                break;
            }
            case ST_2:
            {   
                // do nothing.
                break;
            }
            case ST_3:
            {
                sc_uint<1> bit_one = 1;
                m1 = (bit_one, m1.read().range(22, 0));
                m2 = (bit_one, m2.read().range(22, 0));

                break;
            }
            case ST_4:
            {
                sc_uint<1> bit_one = 1;
                m1 = (bit_one, m1.read().range(22, 0));

                break;
            }
            case ST_5:
            {
                sc_uint<1> bit_one = 1;
                m2 = (bit_one, m2.read().range(22, 0));

                break;
            }
            case ST_6:
            {
                tmpexp = exp1.read().to_uint() + exp2.read().to_uint() - 127;
                tmpm = m1.read().to_uint64() * m2.read().to_uint64();

                break;
            }
            case ST_7:
            {
                tmpm   = tmpm.read() >> 1;
                tmpexp = tmpexp.read().to_uint() + 1;

                break;
            }
            case ST_8:
            {
                // do nothing.
                break;
            }
            case ST_9:
            {
                tmpm   = tmpm.read() << 1;
                tmpexp = tmpexp.read().to_uint() - 1;
                break;
            }
            case ST_10:
            {
                // do nothing.
                break;
            }
            case ST_11:
            {
                exp = tmpexp.read().range(7, 0);

                break;
            }
            case ST_ROUND:
            {
                // tmpm[47:22] + 1 
                // tmpm[47:22] in the form
                // 01 | .... from bit 45 to 23 .... | 1
                sc_lv<22> right = tmpm.read().range(21, 0);
                sc_lv<26> left = tmpm.read().range(47, 22).to_uint() + 1;

                tmpm = (left, right);
                
                break;
            }
            case ST_13:
            {
                m = tmpm.read().range(45, 23);
                break;
            }
            case ST_12:
            {
                tmpexp = tmpexp.read().to_uint() + 1;
                break;
            }
            case ST_NANv1:
            {
                // do nothing.
                break;
            }
            case ST_NANv1_1:
            {
                sc_lv<SIZE_OF_MANTISSA> mantissa_nan = m1.read();
                mantissa_nan[22] = 1;

                exp = 0xFF;
                m   = mantissa_nan;
                s   = s1.read();

                break;
            }
            case ST_NANv1_2:
            {
                sc_lv<SIZE_OF_MANTISSA> mantissa_nan = m2.read();
                mantissa_nan[22] = 1;

                exp = 0xFF;
                m   = mantissa_nan;
                s   = s2.read();

                break;
            }
            case ST_NANv2:
            {
                sc_lv<SIZE_OF_MANTISSA> mantissa_nan = 0;
                mantissa_nan[22] = 1;

                exp = 0xFF;
                m   = mantissa_nan;
                s   = 1;

                break;
            }
            case ST_INF:
            {
                exp = 0xFF;
                m   = 0;

                break;
            }
            case ST_ZERO:
            {
                exp = 0x00;
                m   = 0;

                break;
            }
            case ST_END:
            {
                res_rdy.write(sc_dt::SC_LOGIC_1);
                res.write((s.read(), exp.read(), m.read()));

                break;
            }
            default: 
            { 
                // do nothings
                break;
            }
        }
    }

    // SC_HAS_PROCESS(multiplierIEEE754_fsmd);
    // multiplierIEEE754_fsmd(sc_core::sc_module_name module_name);
    SC_CTOR(multiplierIEEE754_fsmd)
        : op1("op1_mfsmd")
        , op2("op2_mfsmd")
        , in_rdy("in_rdy_mfsmd")
        , clk("clk_mfsmd")
        , rst("rst_mfsmd")
        , res("res_mfsmd")
        , res_rdy("res_rdy_mfsmd")
        // , state(ST_0)
        // , next_state(ST_0)
    {
        SC_METHOD(fsm);
        sensitive_pos << clk;
        sensitive << rst;

        SC_METHOD(datapath);
        sensitive << next_state;
    }

};


