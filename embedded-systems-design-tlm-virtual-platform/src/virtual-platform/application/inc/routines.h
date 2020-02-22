//-------------------------------------------------------------
// Title:	Driver Library for m6502-Splatters platform
// Author:	Stefano Centomo
// Date:	15/06/2018
//-------------------------------------------------------------
#include "mmio.h"
#include "stdbool.h"
#include "defines.h"

// Send op1 and op2 to multiplier connected to psel1, and return the result.
uint32_t mul(uint16_t op1, uint16_t op2);
// WRITE to IO module
void io_write(uint32_t data);
// READ from IO module
uint32_t io_read(void);

// Add your driver here!

/** @brief Multiplier floating-point IEEE754 connected to psel3,
 * with the implementation in Verilog.
 * @param op1 The first operand in floating-point.
 * @param op2 The second operand in floating-point.
 * @return The result of floating-point multiplication.
 */
uint32_t mulIEEE754_verilog(uint32_t op1, uint32_t op2);

/** @brief Multiplier floating-point IEEE754 connected to psel4,
 * with the implementation in VHDL.
 * @param op1 The first operand in floating-point.
 * @param op2 The second operand in floating-point.
 * @return The result of floating-point multiplication.
 */
uint32_t mulIEEE754_vhdl(uint32_t op1, uint32_t op2);


/** @brief Multiplier HW Accelerator: two parallel multiplier IEEE754.
 * @param op1 The first operand for the first multiplier floating-point.
 * @param op2 The second operand for the first multiplier floating-point.
 * @param op3 The first operand for the second multiplier floating-point.
 * @param op4 The second operand for the second multiplier floating-point.
 * @param res1 The pointer to the variable to store the result of the 
 * multiplication of the first two operands in the first multiplier.
 * @param res2 The pointer to the variable to store the result of the 
 * multiplication of the second two operands in the second multiplier.
 */
void mulHWaccelerator(
    uint32_t op1, 
    uint32_t op2, 
    uint32_t op3, 
    uint32_t op4,
    uint32_t *res1,
    uint32_t *res2
    );
