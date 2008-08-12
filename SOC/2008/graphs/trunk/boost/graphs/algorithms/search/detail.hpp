
#ifndef BOOST_GRAPHS_ALGORITHMS_SEARCH_DETAIL_HPP
#define BOOST_GRAPHS_ALGORITHMS_SEARCH_DETAIL_HPP


namespace detail
{
    // TODO: We need a concept that abstracts stacks and queues into a in/out
    // search buffer. Concept maps can easily provide default functions for
    // these things - or build a concept-class. For now, we're just sticking
    // a couple of overloads in this namespace.
#if BOOST_HAS_CONCEPTS
concept InOutBuffer<typename Container>
{
    typename value_type;

    void push(Container& c, value_type const& x)
    { c.push(x); }

    void pop(Container& c)
    { c.pop(); }

    value_type const& peek(Container&);
};

template <typename T, typename Container>
concept_map InOutBuffer<std::stack<T, Container>>
{
    typedef std::stack<T, Container>::value_type value_type;

    value_type const& peek(std::stack<T, Container>& c)
    { return c.top(); }
};

template <typename T, typename Container>
concept_map InOutBuffer<std::queue<T, Container>>
{
    typedef std::queue<T, Container>::value_type value_type;

    value_type const& peek(std::queue<T, Container>& c)
    { return c.front(); }
};
#endif

    // Adapters for getting the current element of a buffer.
    template <typename T, typename Seq>
    inline T& peek(std::stack<T, Seq>& s)
    { return s.top(); }

    template <typename T, typename Seq>
    inline T& peek(std::queue<T, Seq>& s)
    { return s.front(); }
}

#endif
