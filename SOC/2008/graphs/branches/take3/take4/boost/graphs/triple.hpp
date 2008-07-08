
#ifndef TRIPLE_HPP
#define TRIPLE_HPP

/**
 * Like a pair, but with three elements. This gets around having to use the
 * Boost.Tuple library and generating exceedingly long type names in compiler
 * errors.
 */
template <typename First, typename Second, typename Third>
struct triple
{
    typedef First first_type;
    typedef Second second_type;
    typedef Third third_type;

    triple()
        : first(), second(), third()
    { }

    triple(first_type const& f, second_type const& s, third_type const& t)
        : first(f), second(s), third(t)
    { }

    first_type first;
    second_type second;
    third_type third;
};

template <typename F, typename S, typename T>
inline bool
operator==(triple<F,S,T> const& a, triple<F,S,T> const& b)
{ return (a.first == b.first) && (a.second == b.second) && (a.third == b.third); }

template <typename F, typename S, typename T>
inline bool
operator!=(triple<F,S,T> const& a, triple<F,S,T> const& b)
{ return !(a == b); }

template <typename F, typename S, typename T>
inline bool
operator<(triple<F,S,T> const& a, triple<F,S,T> const& b)
{
    // TODO There is definitely a prettier way to write this, but I'm too lazy
    // to figure it out right now.
    if(a.first == b.first) {
        if(a.second == b.second) {
            return a.third < b.third;
        }
        else {
            return a.second < b.second;
        }
    }
    else {
        return a.first < b.first;
    }
}

template <typename F, typename S, typename T>
inline bool
operator>(triple<F,S,T> const& a, triple<F,S,T> const& b)
{ return b < a; }

template <typename F, typename S, typename T>
inline bool
operator<=(triple<F,S,T> const& a, triple<F,S,T> const& b)
{ return !(b < a); }

template <typename F, typename S, typename T>
inline bool
operator>=(triple<F,S,T> const& a, triple<F,S,T> const& b)
{ return !(a < b); }

template <typename F, typename S, typename T>
inline triple<F,S,T>
make_triple(F const& f, S const& s, T const& t)
{ return triple<F,S,T>(f, s, t); }

#endif
