
#ifndef BOOST_OPTIONAL_VALUE_HPP
#define BOOST_OPTIONAL_VALUE_HPP

#include <algorithm>
#include <limits>
#include <stdexcept>

namespace boost {

// Forward declarations
template <typename> class absent;

/**
 * An optional value is like an optional class, but designates a particular
 * value as an indicator for the absence of value. A common example is to use
 * -1u to indicate the absence of values or infinity for unsigned integers.
 *
 * Note that T must equality comparable in order to reason about the presence
 * or absence of its value.
 */
template <typename T, typename Absent = absent<T>>
class optional_value
{
    typedef optional_value<T, Absent> this_type;
    typedef T this_type::* unspecified_bool_type;
public:
    typedef T value_type;

    /** @name Constructors */
    //@{
    optional_value(T const& x = absent())
        : _val(x)
    { }

    optional_value(optional_value const& x)
        : _val(x._val)
    { }

    optional_value(optional_value&& x)
        : _val(std::move(x._val))
    { }
    //@}

    /** @name Operators */
    //@{
    inline optional_value& operator=(optional_value const& x)
    { return swap(optional_value(x)); }

    inline optional_value& operator=(optional_value&& x)
    { return swap(x); }

    inline bool operator==(optional_value const& x) const
    { return _val == x._val; }

    inline bool operator!=(optional_value const& x) const
    { return !(*this == x); }

    inline T& operator*()
    { return get(); }

    inline T const& operator*() const
    { return get(); }

    inline T* operator->()
    { return &get(); }

    inline T const* operator->() const
    { return &get(); }

    inline operator unspecified_bool_type() const
    { return valid() ? &this_type::_val : 0; }
    //@}

    inline bool valid() const
    { return _val != absent(); }

    inline T& get()
    {
        if(!valid()) throw std::runtime_error("Invalid object");
        return _val;
    }

    inline T const& get() const
    {
        if(!valid()) throw std::runtime_error("Invalid object");
        return _val;
    }

    inline optional_value& swap(optional_value&& x)
    {
        using namespace std;
        std::swap(_val, x._val);
        return *this;
    }

    static T absent()
    { return Absent::value(); }

    T _val;
};

// Canned absentee values

/** Use a type's minimum value as an absentee. */
template <typename T>
struct absent_min
{
    static T value()
    { return std::numeric_limits<T>::min(); }
};

/** Use 0 as an absentee value. */
template <typename T>
struct absent_zero
{
    static T value()
    { return T(0); }
};

/** Use a type's maximum value as an absentee. */
template <typename T>
struct absent_max
{
    static T value()
    { return std::numeric_limits<T>::max(); }
};

/**
 * Use positive infinity as an absentee value. Here, T is essentially required
 * to be a float, double, or long double. If we had concepts and a generic
 * infinity() function, the requirements could be easily reduced.
 */
template <typename T>
struct absent_inf
{
    static T value()
    { return std::numeric_limits<T>::infinity(); }
};

/** Use negative infinity as an absentee value. See absent_inf for comments. */
template <typename T>
struct absent_neg_inf
{
    static T value()
    { return -std::numeric_limits<T>::infinity(); }
};

/**
 * The absent structure is used to provide default absent values for the
 * optional_value template. There are two ways to extend this functionality.
 * The first is to specialize this structure over your type and the second
 * is to simply define a new one and use it as the Absent parameter for the
 * optional_value class. This class provides a number of default specializations.
 *
 * Note that the default values of these simply inherit specific canned
 * instances absent values.
 */
template <typename T> struct absent { static T value(); };
template <typename T> struct absent<T*> : absent_zero<T*> { };
template <> struct absent<int> : absent_max<int> { };
template <> struct absent<unsigned int> : absent_max<unsigned int> { };
template <> struct absent<float> : absent_inf<float> { };
template <> struct absent<double> : absent_inf<double> { };

} /* namespace boost */

#endif