// type which is the width of an op code.
typedef uint8_t op_code_width;

// type used for retrieving an opcode from a 

// uesd to get the opcode out of the instruction.  1111 1111  0000 0000
typedef bits_mask<uint16_t,8,8>             retrieve_op_code_mask; 

/**********************************bpl*****************************************/
// 0001 0000
typedef bits_mask<op_code_width, 4>          bpl_op_code;

/**********************************jsr*****************************************/

// 0010 0000
typedef bits_mask<op_code_width,5>          jsr_op_code;

/**********************************brk*****************************************/

// 0000 0000
typedef integral_mask< op_code_width, 0>    brk_op_code;



/**********************************ORA*****************************************/
// 0000 0001
typedef bits_mask< op_code_width, 1>        ora_op_code;

// 0000 1001
typedef compound_mask<
    bits_mask<op_code_width,3>,
    bits_mask<op_code_width,0>
    >                                       ora2_op_code;

// 0000 1101
typedef compound_mask<
    bits_mask<op_code_width, 2,2>,
    bits_mask<op_code_width,0>
    >                                       ora3_op_code;

// 0001 0001
typedef compound_mask<
    bpl_op_code,
    bits_mask<op_code_width,0>
    >                                       ora4_op_code;

// 0001 0010
typedef compound_mask<
    bpl_op_code,
    bits_mask<op_code_width,1>
    >                                       ora5_op_code;

// 0001 0101
typedef compound_mask<
    bpl_op_code,
    bits_mask<op_code_width,1,2>,
    bits_mask<op_code_width,0>
    >                                       ora6_op_code;

// 0001 1001
typedef compound_mask<
    bpl_op_code,
    bits_mask<op_code_width,3>,
    bits_mask<op_code_width,0>
>                                           ora7_op_code;

// 0001 1101
typedef compound_mask<
    bits_mask<op_code_width,3,2>,
    bits_mask<op_code_width,0>
    >                                       ora8_op_code;

/**********************************tsb*****************************************/
// 0000 0100
typedef bits_mask<op_code_width, 2>         tsb_op_code;

// 0000 1100
typedef bits_mask< op_code_width,2,2>       tsb2_op_code;



/**********************************asl*****************************************/
// 0000 0101
typedef compound_mask<
    bits_mask<op_code_width, 2>,
    bits_mask<op_code_width, 0 >
    >                                       asl_op_code;

// 0000 1010
typedef compound_mask<
    bits_mask< op_code_width, 3>,
    bits_mask< op_code_width, 1>
    >                                       asl2_op_code;

// 0000 1110
typedef bits_mask<op_code_width, 1,3>       asl3_op_code;

// 0001 0110
typedef compound_mask<
    bpl_op_code,
    bits_mask<op_code_width,1,2>,
    bits_mask<op_code_width,1>
    >                                       asl4_op_code;

// 0001 1110
typedef bits_mask<op_code_width,4,1>        asl5_op_code;

/**********************************php*****************************************/
// 0000 1000
typedef bits_mask<op_code_width,3>          php_op_code;



/**********************************trb*****************************************/
// 0001 0100
typedef compound_mask<
    bpl_op_code,
    bits_mask<op_code_width,3>
    >                                       trb_op_code;

// 0001 1100
typedef bits_mask<op_code_width,3,2>        trb2_op_code;

/**********************************clc*****************************************/
// 0001 1000
typedef compound_mask<
    bpl_op_code,
    bits_mask<op_code_width,3>
    >                                       clc_op_code;

/**********************************inc*****************************************/
// 0001 1010
typedef compound_mask<
    bpl_op_code,
    bits_mask<op_code_width, 3>,
    bits_mask<op_code_width, 1>
    >                                       inc_op_code;


/**********************************and*****************************************/
// 0010 0001
typedef compound_mask<
    jsr_op_code, 
    bits_mask<op_code_width,0>
    >                                       and_op_code;

// 0010 0101
typedef compound_mask<
    and_op_code,
    bits_mask<op_code_width, 2>
    >                                       and2_op_code;

// 0010 0101
typedef compound_mask<
    jsr_op_code,
    bits_mask<op_code_width,2>,
    bits_mask<op_code_width,1>
    >                                       and3_op_code;

/**********************************bit*****************************************/
// 0010 0100
typedef compound_mask<
    jsr_op_code, 
    bits_mask<op_code_width,2>
    >                                       bit_op_code;

/**********************************rol*****************************************/
// 0010 0110
typedef compound_mask<
    jsr_op_code,
    bits_mask<op_code_width, 1,2>
    >                                       rol_op_code;

/**********************************plp*****************************************/
// 0010 1000
typedef compound_mask<
    jsr_op_code,
    bits_mask<op_code_width, 3>
    >                                       plp_op_code;
