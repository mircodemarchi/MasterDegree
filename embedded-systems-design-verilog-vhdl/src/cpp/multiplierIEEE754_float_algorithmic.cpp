/**
 * @file multiplierIEEE754_float_algorithmic.h
 * @author Mirco De Marchi
 * @date 30-12-2019
 * @copyright University of Verona
 * @brief Floating point multiplier IEEE754 using float params 
 * in algorithmic level.
 * 
 * Project Name: First Assignment;
 * Design Name: Multiplier floating point IEEE754 float params algorithmic;
 * Module Name: multiplierIEEE754;
 * Target Devices: PYNQ;
 */

/**
 * @brief Floating-point multiplication of op1 and op2 saved in res.
 * @param op1 First floating-point operand.
 * @param op2 Second floating-point operand.
 * @param res Floaing-point pointer where save the result of multiplication.
 */
void multiplierIEEE754(float op1, float op2, float *res) {
    *res = op1 * op2;
}