//abstract "next" operation (e.g. used as part of iterator )
#ifndef LIBS_POLICY_PTR_TEST_NEXT_ABS_HPP_LJE20050520
#define LIBS_POLICY_PTR_TEST_NEXT_ABS_HPP_LJE20050520

  struct
next_abs
{
        virtual
      void
    next_op(void)
    =0
    ;
      void
    operator++(void)
    {
        next_op();
    }
};
    
#endif
