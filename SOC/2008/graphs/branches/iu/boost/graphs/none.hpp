
#ifndef NONE_HPP
#define NONE_HPP

// The canonical none type.
struct none { };

// A little weird, but there are times when it makes sense to create noop
// operations. This takes a single parameter.
template <typename T>
struct noop
{
    typedef void result_type;
    void operator()(T const&)
    { }
};

#endif

