//abstract stack of next_abs'
#ifndef LIBS_POLICY_PTR_TEST_NEXT_STK_ABS_HPP_LJE20050523
#define LIBS_POLICY_PTR_TEST_NEXT_STK_ABS_HPP_LJE20050523
#include "next_abs.hpp"

  struct
next_stk_abs
/**@brief
 *  Stack of next_abs.
 */
{
        typedef
      next_abs
    next_abs_type
    ;
        virtual    
    ~next_stk_abs(void)
    {}

        virtual
      next_abs&
    push_next(void)
    /**@brief
     *  Pushes another next_abs onto stack, amoung possibly other
     *  things.
     */
    =0
    ;
        virtual
      bool
    pop_next(void)
    /**@brief
     *  Pops top of stack and returns value indicating emptiness.
     */
    =0
    ;
        virtual
      bool
    empty(void)const
    /**@brief
     *  Indicates if stack is empty.
     */
    =0
    ;
};       
#endif
