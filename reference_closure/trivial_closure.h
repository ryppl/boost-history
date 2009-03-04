#ifndef TRIVIAL_CLOSURE2_DWA200933_H
# define TRIVIAL_CLOSURE2_DWA200933_H

struct frame { double a, b, c; };

inline void work( frame* f )
{
# if !DISABLE_WORK
    f->a += f->b * f->c;
# endif 
}

#if REFERENCE_CLOSURE

inline void nested( frame* f ) { work(f); }

struct closure {
    void (*func)( frame* );
    frame* scope;
    void operator()() { func( scope ); }
};

#elif STD_FUNCTION

struct lambda
{
  frame* scope;
  void operator()() { work( scope ); }
};

#include <tr1/functional>
namespace std
{ 
  namespace tr1
  {

    template <>
      struct __is_location_invariant<lambda>
      : integral_constant<bool,true>
      {};

  }
}

#endif

#endif // TRIVIAL_CLOSURE2_DWA200933_H
