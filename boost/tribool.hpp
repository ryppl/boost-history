// Three-state boolean logic library
//
// Copyright (C) 2002 Doug Gregor (gregod@cs.rpi.edu)
//
// Permission to copy, use, sell and distribute this software is granted
// provided this copyright notice appears in all copies.
// Permission to modify the code and to distribute modified code is granted
// provided this copyright notice appears in all copies, and a notice
// that the code was modified is included with the copyright notice.
//
// This software is provided "as is" without express or implied warranty,
// and with no claim as to its suitability for any purpose.
 
// For more information, see http://www.boost.org

#ifndef BOOST_TRIBOOL_HPP
#define BOOST_TRIBOOL_HPP

namespace boost {

namespace detail {
  /**
   * A type used only to uniquely identify the 'indeterminate' 
   * function/keyword.
   */
  struct indeterminate_t {};
} // end namespace boost

// Forward declaration of tribool class
class tribool;

// Declaration for the indeterminate function
inline bool 
indeterminate(tribool x, 
              detail::indeterminate_t dummy = detail::indeterminate_t());

/**
 * The type of the 'indeterminate' keyword. This has the same type as the
 * function 'indeterminate' so that we can recognize when the keyword is
 * used.
 */
typedef bool (*indeterminate_keyword_t)(tribool, detail::indeterminate_t);

/**
 * A 3-state boolean type. 3-state boolean values are either true, false, or
 * indeterminate.
 */
class tribool
{
private:
  struct dummy {
    void nonnull() {};
  };

  typedef void (dummy::*safe_bool)();

public:
  /**
   * Construct a new 3-state boolean value with the value 'false'.
   *
   * \throws nothing
   */
  tribool() : value(false_value) {}

  /**
   * Construct a new 3-state boolean value with the given boolean value.
   *
   * \param value the value of the 3-state boolean
   * \throws nothing
   */
  tribool(bool value) : value(value? true_value : false_value) {}

  /**
   * Construct a new 3-state boolean value with an indeterminate value.
   *
   * \throws nothing
   */
  tribool(indeterminate_keyword_t) : value(indeterminate_value) {}

  /**
   * Use a 3-state boolean in a boolean context. Will evaluate true in a
   * boolean context only when the 3-state boolean is definitely true.
   *
   * \returns true if the 3-state boolean is true, false otherwise
   * \throws nothing
   */
  operator safe_bool() const
  {
    return value == true_value? &dummy::nonnull : 0;
  }

  /**
   * Return the logical negation of the given 3-state boolean. 
   *
   * \returns the logical negation of the 3-state boolean value
   * \throws nothing
   */
  tribool operator!() const
  {
    return value == false_value? tribool(true)
          :value == true_value? tribool(false)
          :tribool(indeterminate);
  }

  friend bool indeterminate(tribool x, detail::indeterminate_t);

private:
  /**
   * The actual stored value in this 3-state boolean, which may be false, true,
   * or indeterminate.
   */
  enum { false_value, true_value, indeterminate_value } value;
};

// Check if the given tribool has an indeterminate value. Also doubles as a
// keyword for the 'indeterminate' value
inline bool indeterminate(tribool x, detail::indeterminate_t)
{
  return x.value == tribool::indeterminate_value;
}

// Equality comparisons among tribools and bools ----------------------
inline tribool operator==(tribool x, bool y)
{
  return x && y || !x && !y;
}

inline tribool operator==(bool x, tribool y)
{
  return x && y || !x && !y;
}
   
inline tribool operator==(indeterminate_keyword_t, tribool x)
{
  return indeterminate(x);
}
        
inline tribool operator==(tribool x, indeterminate_keyword_t)
{
  return indeterminate(x);
}
    
inline tribool operator==(tribool x, tribool y)
{
  if (x == indeterminate || y == indeterminate)
    return indeterminate;
  else
    return x && y || !x && !y;
}
// End Equality comparisons among tribools and bools ------------------

// Inequality comparisons among tribools and bools --------------------
inline tribool operator!=(tribool x, bool y)
{
  return x && !y || !x && y;
}
               
inline tribool operator!=(bool x, tribool y)
{
  return x && !y || !x && y;
}
 
inline tribool operator!=(indeterminate_keyword_t, tribool x)
{
  return !indeterminate(x);
}

inline tribool operator!=(tribool x, indeterminate_keyword_t)
{
  return !indeterminate(x);
}

inline tribool operator!=(tribool x, tribool y)
{
  if (x == indeterminate || y == indeterminate)
    return indeterminate;
  else
    return !(x && y || !x && !y);
}
// End Inequality comparisons among tribools and bools ----------------

// Logical AND operations among tribools and bools --------------------
inline tribool operator&&(tribool x, bool y)
{
  return y? x : tribool(false);
}

inline tribool operator&&(bool x, tribool y)
{
  return x? y : tribool(false);
}

inline tribool operator&&(indeterminate_keyword_t, tribool x)
{
  return !x? tribool(false) : indeterminate;
}

inline tribool operator&&(tribool x, indeterminate_keyword_t)
{
  return !x? tribool(false) : indeterminate;
}

inline tribool operator&&(tribool x, tribool y)
{
  
  if (static_cast<bool>(!x) || static_cast<bool>(!y))
    return false;
  else if (static_cast<bool>(x) && static_cast<bool>(y))
    return true;
  else
    return indeterminate;
}
// End Logical AND operations among tribools and bools ----------------


// Logical OR operations among tribools and bools ---------------------
inline tribool operator||(tribool x, bool y)
{
  return y? tribool(true) : x;
}

inline tribool operator||(bool x, tribool y)
{
  return x? tribool(true) : y;
}

inline tribool operator||(indeterminate_keyword_t, tribool x)
{
  return x? tribool(true) : indeterminate;
}

inline tribool operator||(tribool x, indeterminate_keyword_t)
{
  return x? tribool(true) : indeterminate;
}

inline tribool operator||(tribool x, tribool y)
{
  if (static_cast<bool>(!x) && static_cast<bool>(!y))
    return false;
  else if (static_cast<bool>(x) || static_cast<bool>(y))
    return true;
  else
    return indeterminate;
}
// End Logical OR operations among tribools and bools ------------------

} // end namespace boost

#endif // BOOST_TRIBOOL_HPP
