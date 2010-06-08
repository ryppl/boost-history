/** This is some example code for a disassembler for using the bit_masks 
 *  library.
 *  The instructions are bassed on the 6502 processor's instruction set.
 */


#include <boost/cstdint.hpp>
#include <boost/integer/compound_mask.hpp>
#include <cstddef>

using namespace std;
using namespace boost;

// each instruction is 16 bits wide.
// All opcodes are 8 bits in length.

// type which is the width of an op code.
typedef uint8_t op_code_width;

typedef uint16_t instruction_width;

// uesd to get the opcode out of the instruction.  1111 1111  0000 0000
typedef bits_mask<instruction_width,8,8>  retrieve_op_code_mask; 

typedef bits_mask<op_code_width, 7>     bit_8;
typedef bits_mask<op_code_width, 6>     bit_7;
typedef bits_mask<op_code_width, 5>     bit_6;
typedef bits_mask<op_code_width, 4>     bit_5;
typedef bits_mask<op_code_width, 3>     bit_4;
typedef bits_mask<op_code_width, 2>     bit_3;
typedef bits_mask<op_code_width, 1>     bit_2;
typedef bits_mask<op_code_width, 0>     bit_1;

int main() {
    return 0;
}
