// Boost.SafeBool
//
// Copyright (c) 2009-2011 Vladimir Batov.
// With valuable input from Steven Watanabe and Robert Stewart.
// Use, modification and distribution are subject to the Boost Software License,
// Version 1.0. See http://www.boost.org/LICENSE_1_0.txt.

#ifndef __BOOST_SAFE_BOOL_PUBLIC_HPP__
#define __BOOST_SAFE_BOOL_PUBLIC_HPP__

namespace boost
{
/// @brief Generalization of the Safe-Bool Technique
/// @details An implicit conversion to bool (operator bool() const) is very much
/// idiomatic and is often deployed in constructs like "if (foo)" and "if (!foo)"
/// (with no explicit op!() defined). However, sadly, implementing "operator bool()"
/// is *wrong* as that conversion kicks in far too often and unexpectedly. Like in
/// "foo == 1", "foo+1", "1+foo" or potentially during lexical_cast<string>(foo)
/// (if there are no op>>() and op<<() defined). Consequently, that "implicit
/// conversion to bool" functionality has to be implemented in an indirect and
/// somewhat awkward way via an implicit conversion to some other type. The best
/// type for the purpose appears to be a pointer to a member function. For more
/// see the chapter 7.7 in Alexandrescu's "Modern C++ Design" and the article at
/// http://www.artima.com/cppsource/safebool.html by Bjorn Karlsson.\n\n
/// Deployment:
/// @code
/// struct Foo
/// { ...
///     operator safebool<Foo>::type() const { return safebool<Foo>(condition); }
/// };
/// template<class T>
/// struct Moo // for a template class
/// { ...
///     operator typename safebool<Moo>::type() const { return safebool<Moo>(condition); }
/// };
/// template<class T>
/// struct Zoo // with convenience typedefs
/// { ...
///     typedef safebool<Zoo> safebool;
///     typedef typename safebool::type safebool_type;
///
///     operator safebool_type() const { return safebool(condition); }
/// };
/// @endcode
/// safebool needs to be a template to make the returned safebool<Foo>::type type
/// unique. Without it different classes would return the same safebool::type type
/// that would make possible relational operators between unrelated types. Like
/// @code
///     struct Foo { operator safebool::type() const { return safebool(...); }};
///     struct Moo { operator safebool::type() const { return safebool(...); }};
///     Foo foo;
///     Zoo zoo;
///     if (foo == zoo) Valid (but wrong) comparison between unrelated types.
/// @endcode
template<class T>
struct safebool
{
    typedef void (safebool::*type)() const;

    explicit safebool(bool v) : value_(v) {}

    operator type() const { return value_ ? &safebool::true_ : 0; }

    private:

    void  true_ () const {};
    bool value_;
};

} // End of the namespace

#endif // __BOOST_SAFE_BOOL_PUBLIC_HPP__
