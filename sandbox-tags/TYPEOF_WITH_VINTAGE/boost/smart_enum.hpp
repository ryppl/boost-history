#ifndef SMART_ENUM_HPP_INCLUDED
#define SMART_ENUM_HPP_INCLUDED

// A generic class for type-safe storage and manipulation of enum.

#include <stdexcept>
#include <functional>

namespace boost
    {
    //////////////////////////////////////////////////
    // Generic smart_enum<> class template.         //
    //////////////////////////////////////////////////

    template<typename enumT, class incrementorT>
    class smart_enum
        {
      public:
        typedef enumT         enum_type;
        typedef incrementorT  incrementor_type;

        explicit smart_enum(enum_type i, incrementor_type func = incrementor_type())
                : incrementor(func), val(i)
            {
            }
        explicit smart_enum(int i, incrementor_type func = incrementor_type())
                : incrementor(func), val(incrementor(i, 0))
            {
            }

        smart_enum& operator= (enum_type rhs) { val = rhs; return *this; }
        smart_enum& operator= (int rhs)       { val = incrementor(rhs, 0); return *this; }

        smart_enum&      operator++ ()      { return (*this = incrementor(val, 1)); }
        smart_enum&      operator-- ()      { return (*this = incrementor(val, -1)); }
        const smart_enum operator++ (int)   { smart_enum tmp(*this); ++(*this); return tmp; }
        const smart_enum operator-- (int)   { smart_enum tmp(*this); --(*this); return tmp; }
        smart_enum&      operator+= (int n) { return (*this = incrementor(val, n)); }
        smart_enum&      operator-= (int n) { return (*this = incrementor(val, -n)); }

        operator enum_type() const          { return val; }

      private:
        incrementor_type    incrementor;
        enum_type           val;
        };

    //////////////////////////////////////////////////
    // sequential_smart_enum<> specialization.      //
    //////////////////////////////////////////////////

    template<typename enumT, enumT minVal, enumT maxVal>
    struct SequentialIncrementor : public std::binary_function<int, int, enumT>
        {
        enumT operator() (int val, int n) const
            {
            if (val + n >= minVal && val + n <= maxVal)
                return enumT(val + n);
            else
                throw std::out_of_range("incrementation would yield invalid enum");
            }
        };

    template<typename enumT, enumT minVal, enumT maxVal>
    struct sequential_smart_enum : public smart_enum<enumT, SequentialIncrementor<enumT, minVal, maxVal> >
        {
        typedef smart_enum<enumT, SequentialIncrementor<enumT, minVal, maxVal> > smart_enum_type;
        typedef typename smart_enum_type::enum_type         enum_type;
        typedef typename smart_enum_type::incrementor_type  incrementor_type;
        enum { min = minVal, max = maxVal };

        sequential_smart_enum(int i) : smart_enum_type(i, incrementor_type())
            { }
        sequential_smart_enum(enum_type i) : smart_enum_type(i, incrementor_type())
            { }
        };


    //////////////////////////////////////////////////
    // wrapped_smart_enum<> specialization.         //
    //////////////////////////////////////////////////

    template<typename enumT, enumT minVal, enumT maxVal>
    struct WrappedIncrementor : public std::binary_function<int, int, enumT>
        {
        enumT operator() (int val, int n) const
            {
            return enumT(((val - minVal + n) % (maxVal - minVal + 1)) + minVal);
            }
        };

    template<typename enumT, enumT minVal, enumT maxVal>
    struct wrapped_smart_enum : public smart_enum<enumT, WrappedIncrementor<enumT, minVal, maxVal> >
        {
        typedef smart_enum<enumT, WrappedIncrementor<enumT, minVal, maxVal> > smart_enum_type;
        typedef typename smart_enum_type::enum_type         enum_type;
        typedef typename smart_enum_type::incrementor_type  incrementor_type;
        enum { min = minVal, max = maxVal };

        wrapped_smart_enum(int i) : smart_enum_type(i, incrementor_type())
            { }
        wrapped_smart_enum(enum_type i) : smart_enum_type(i, incrementor_type())
            { }
        };

    //////////////////////////////////////////////////
    // bounded_smart_enum<> specialization.         //
    //////////////////////////////////////////////////

    template<typename enumT, enumT minVal, enumT maxVal>
    struct BoundedIncrementor : public std::binary_function<int, int, enumT>
        {
        enumT operator() (int val, int n) const
            {
            int res = val + n;
            if (res < minVal)
                return minVal;
            else if (res > maxVal)
                return maxVal;
            else
                return enumT(res);
            }
        };

    template<typename enumT, enumT minVal, enumT maxVal>
    struct bounded_smart_enum : public smart_enum<enumT, BoundedIncrementor<enumT, minVal, maxVal> >
        {
        typedef smart_enum<enumT, BoundedIncrementor<enumT, minVal, maxVal> > smart_enum_type;
        typedef typename smart_enum_type::enum_type         enum_type;
        typedef typename smart_enum_type::incrementor_type  incrementor_type;
        enum { min = minVal, max = maxVal };

        bounded_smart_enum(int i) : smart_enum_type(i, incrementor_type())
            { }
        bounded_smart_enum(enum_type i) : smart_enum_type(i, incrementor_type())
            { }
        };

    } // end of namespace boost

#endif // !defined(SMART_ENUM_HPP_INCLUDED)
