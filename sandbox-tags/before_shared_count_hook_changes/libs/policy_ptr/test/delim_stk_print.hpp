#ifndef LIBS_POLICY_PTR_TEST_DELIM_STK_PRINT_HPP_LJE20050523
#define LIBS_POLICY_PTR_TEST_DELIM_STK_PRINT_HPP_LJE20050523
#include "next_stk_abs.hpp"
#include "boost/io/filters/mout.hpp"
#include <stack>

  struct
delim_stk_print
  : public next_stk_abs
/**@brief
 *  Outputs delimiters during print of a nested data structure:
 *  "{ "  - begin delimiter
 *  ", "  - between delimiter
 *  "}\n" - end delimiter
 */
{
      struct
    delim_print
      : public next_abs
    /**@brief
     *  Outputs 'between delimiter'
     */
    {
        delim_print(void)
          : my_count(0)
        {}
        
          void
        next_op(void)
        {
            if(my_count)
            {
                mout()<<", ";
            }
            ++my_count;
        }
          unsigned
        get_count(void)const
        {
            return my_count;
        }
     private:
          unsigned
        my_count
        ;
    };
    
      std::stack<delim_print>
    my_stack
    ;
    
      delim_print&
    top(void)
    {
        return my_stack.top();
    }
      delim_print&
    push_next(void)
    /**@brief
     *  Outputs 'begin delimiter'
     */
    {
        ++mout();
        mout()<<"{ ";
        my_stack.push(delim_print());
        return top();
    }
      bool
    pop_next(void)
    /**@brief
     *  Outputs 'end delimiter'
     */
    {    
        mout()<<"}\n";
        --mout();
        my_stack.pop();
        return my_stack.empty();
    }
      bool
    empty(void)const
    {    
        return my_stack.empty();
    }
};       
#endif
