/*
  A generic class for storing and manipulating enum-based state
  information.
*/

#ifndef STATE_HH_INCLUDED
#define STATE_HH_INCLUDED

#include <stdexcept>

template<typename enumT>
struct enum_traits { enum { min = 0, max = -1, init = 0 }; };

template<typename enumT>
class State
    {
  public:
    typedef enumT enum_type;

    // Having an implicit constructor with parameter will also serve
    // as assignment operator via implicit temporary objects and copy
    // construction.

    State(int init = enum_traits<enum_type>::init)
        { current = State::checked_cast(init); }

    // Add "typesafe" increment and decrement operators.

    State& operator+= (int n)     { increment(n); return *this; }
    State& operator-= (int n)     { increment(-n); return *this; }
    State& operator++ ()          { increment(1); return *this; }
    State& operator-- ()          { increment(-1); return *this; }
    const State operator++ (int)  { State tmp(*this); increment(1); return tmp; }
    const State operator-- (int)  { State tmp(*this); increment(-1); return tmp; }

    // Allow implicit conversion to enum_type, which saves us the
    // hastle of defining all those operators.

    operator const enum_type () const { return current; }

    // Disable bound checking for the default traits.

    enum { is_bound_checked = (enum_traits<enum_type>::max > enum_traits<enum_type>::min) };

    // The trait-based bound checker.

    static enum_type checked_cast(int val)
        {
        if (is_bound_checked)
            {
            if (val < enum_traits<enum_type>::min || val > enum_traits<enum_type>::max)
                throw std::out_of_range("invalid state");
            }
        return enum_type(val);
        }

  private:
    void increment(int n) { current = checked_cast(current + n); }
    enum_type current;
    };

#endif // !defined(STATE_HH_INCLUDED)
