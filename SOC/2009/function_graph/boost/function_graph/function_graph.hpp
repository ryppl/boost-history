/**
 * Copyright (C) 2009
 * Michael Lopez
 *
 * Issued under Boost Software License - Version 1.0
 * http://www.boost.org/LICENSE_1_0.txt
 *
 */

#ifndef FUNCTION_GRAPH_HPP_
#define FUNCTION_GRAPH_HPP_

#include <boost/graph/graph_traits.hpp>
#include <boost/function.hpp>
#include <utility>
#include <boost/range.hpp>
#include <boost/mpl/bool.hpp>
#include <boost/type_traits/is_same.hpp>

/** @todo: How to grill me on the next code audit
 * -The equality comparison on the iterators should now take the Graph* g. But
 *      they always come in pairs, so should g be considered.
 * -I have a stray meta-function. It needs a better home. Right now the odd bits
 *      lie between the iterators and the functions.
 * -Iterators should use the iterator adaptor.
 */
namespace boost {

/** Out Edge Iterator */
template<typename Graph>
struct function_graph_out_edge_iterator {
private:
    typedef function_graph_out_edge_iterator<Graph> This;

public:
    typedef Graph graph_type;
    typedef typename graph_type::vertex_iterator vertex_iterator;
    typedef typename graph_type::edge_descriptor edge_descriptor;
    typedef typename graph_type::vertex_descriptor vertex_descriptor;

    /** Iterator traits */
    typedef std::input_iterator_tag iterator_category;
    typedef edge_descriptor value_type;
    typedef int difference_type;
    typedef value_type* pointer;
    typedef value_type& reference;

    /** @name Constructors */
    //@{
    function_graph_out_edge_iterator()
        : g_(0)
    { }

    function_graph_out_edge_iterator(graph_type const& g,
                                    vertex_descriptor const& vertex,
                                    vertex_iterator const& i_at)
        : g_(&g),
          vertex_(vertex),
          i_at_(i_at)
    {
        const vertex_iterator i_end = end(g.range);

        while((i_at_ != i_end) && !has_edge(g.edge(vertex_, *i_at_)))
        { ++i_at_; }
    }

    // Constructs an end iterator without computation
    function_graph_out_edge_iterator(graph_type const& g,
                                    vertex_descriptor const& vertex)
        : g_(&g),
          vertex_(vertex),
          i_at_(end(g.range))
    { }

    function_graph_out_edge_iterator(This const& cp)
        : g_(cp.g_),
          vertex_(cp.vertex_),
          i_at_(cp.i_at_)
    { }
    //@}

    /** Input Iterator operator overloads */
    This& operator++()
    {
        const vertex_iterator i_end = end(g_->range);

        // Cycle through the range until an edge is found,
        // or the end of the list is found
        do {
            ++i_at_;
        } while((i_at_ != i_end) && !has_edge(g_->edge(vertex_, *i_at_)));

        return *this;
    }

    This operator++(int)
    {
         const This temp = *this;
         const vertex_iterator i_end = end(g_->range);

        // Cycle through the range until an edge is found,
        // or the end of the list is found
        do {
            ++i_at_;
        } while((i_at_ != i_end) && !has_edge(g_->edge(vertex_, *i_at_)));

        return temp;
    }

    edge_descriptor operator*()
    { return edge_descriptor(g_->edge(vertex_, *i_at_), vertex_, *i_at_); }

    const graph_type* g_;
    vertex_descriptor vertex_;
    vertex_iterator i_at_;
};

template<typename Graph>
bool operator==(function_graph_out_edge_iterator<Graph> const& lhs,
                function_graph_out_edge_iterator<Graph> const& rhs)
{
    return //lhs.g_ == rhs.g_ &&  // g_ is an odd case, consider this line
           lhs.vertex_ == rhs.vertex_ &&
           lhs.i_at_ == rhs.i_at_;
}

template<typename Graph>
bool operator!=(function_graph_out_edge_iterator<Graph> const& lhs,
                function_graph_out_edge_iterator<Graph> const& rhs)
{
    return //lhs.g_ != rhs.g_ ||  // g_ is an odd case, consider this line
           lhs.vertex_ != rhs.vertex_ ||
           lhs.i_at_ != rhs.i_at_;
}



/** In Edge Iterator */
template<typename Graph>
struct function_graph_in_edge_iterator {
private:
    typedef function_graph_in_edge_iterator<Graph> This;

public:
    typedef Graph graph_type;
    typedef typename graph_type::vertex_iterator vertex_iterator;
    typedef typename graph_type::edge_descriptor edge_descriptor;
    typedef typename graph_type::vertex_descriptor vertex_descriptor;
    typedef typename graph_type::function_type function_type;

    /** Iterator traits */
    typedef std::input_iterator_tag iterator_category;
    typedef edge_descriptor value_type;
    typedef int different_type;
    typedef value_type* pointer;
    typedef value_type& reference;

    /** @name Constructors */
    //@{
    function_graph_in_edge_iterator()
        : g_(0)
    { }

    function_graph_in_edge_iterator(graph_type const& g,
                                    vertex_descriptor const& vertex,
                                    vertex_iterator const& i_at)
        : g_(&g),
          vertex_(vertex),
          i_at_(i_at)
    {
        const vertex_iterator i_end = end(g.range);

        while((i_at_ != i_end) && !has_edge(g.edge(*i_at_, vertex_)))
        { ++i_at_; }
    }

    // Constructs an end iterator without computation
    function_graph_in_edge_iterator(graph_type const& g,
                                    vertex_descriptor const& vertex)
        : g_(&g),
          vertex_(vertex),
          i_at_(end(g.range))
    { }

    function_graph_in_edge_iterator(This const& cp)
        : g_(cp.g_),
          vertex_(cp.vertex_),
          i_at_(cp.i_at_)
    { }
    //@}

    /** Input Iterator operator overloads */
    This& operator++()
    {
        const vertex_iterator i_end = end(g_->range);

        // Cycle through the range until an edge is found,
        // or the end of the list is found
        do {
            ++i_at_;
        } while((i_at_ != i_end) && !has_edge(g_->edge(*i_at_, vertex_)));

        return *this;
    }

    This operator++(int)
    {
        const This temp = *this;
        const vertex_iterator i_end = end(g_->range);

        do {
            ++i_at_;
        } while((i_at_ != i_end) && !has_edge(g_->edge(*i_at_, vertex_)));

        return temp;
    }

    edge_descriptor operator*()
    {
        return edge_descriptor(g_->edge(*i_at_, vertex_), *i_at_, vertex_);
    }

    const graph_type* g_;
    vertex_descriptor vertex_;
    vertex_iterator i_at_;
};

template<typename Graph>
bool operator==(function_graph_in_edge_iterator<Graph> const& lhs,
                function_graph_in_edge_iterator<Graph> const& rhs)
{
    return //lhs.g_ == rhs.g_ &&  // g_ is an odd case, consider this line
           lhs.vertex_ == rhs.vertex_ &&
           lhs.i_at_ == rhs.i_at_;
}

template<typename Graph>
bool operator!=(function_graph_in_edge_iterator<Graph> const& lhs,
                function_graph_in_edge_iterator<Graph> const& rhs)
{
    return //lhs.g_ != rhs.g_ ||  // g_ is an odd case, consider this line
           lhs.vertex_ != rhs.vertex_ ||
           lhs.i_at_ != rhs.i_at_;
}



/** Adjacency Iterator */
template<typename Graph>
struct function_graph_adjacency_iterator {
private:
    typedef function_graph_adjacency_iterator<Graph> This;

public:
    typedef Graph graph_type;
    typedef typename graph_type::vertex_iterator vertex_iterator;
    typedef typename graph_type::edge_descriptor edge_descriptor;
    typedef typename graph_type::vertex_descriptor vertex_descriptor;

    /** Iterator traits */
    typedef std::input_iterator_tag iterator_category;
    typedef edge_descriptor value_type;
    typedef int different_type;
    typedef value_type* pointer;
    typedef value_type& reference;

    /** @name Constructor */
    //@{
    function_graph_adjacency_iterator(graph_type const& g,
                                      vertex_descriptor const& vertex)
        : g_(&g), vertex_(vertex), i_at_(begin(g.range)), in_edge_check_(true)
    {
        const vertex_iterator i_begin = begin(g.range);
        const vertex_iterator i_end = end(g.range);

        while(i_at_ != i_end && !has_edge(g.edge(*i_at_, vertex_)))
        { ++i_at_; }
        if(i_at_ == i_end)
        {
            in_edge_check_ = false;
            i_at_ = begin(g.range);
            while(i_at_ != i_end && !has_edge(g.edge(vertex_, *i_at_)))
            {  ++i_at_; }
        }
    }

    function_graph_adjacency_iterator(graph_type const& g,
                                      vertex_descriptor const& vertex,
                                      bool)
        : g_(&g),
          vertex_(vertex),
          i_at_(end(g.range)),
          in_edge_check_(false)
    { }

    function_graph_adjacency_iterator(This const& cp)
        : g_(cp.g_),
          vertex_(cp.vertex_),
          i_at_(cp.i_at_),
          in_edge_check_(cp.in_edge_check_)
    { }
    //@}

    This& operator++()
    {
        const vertex_iterator i_end = end(g_->range);

        if(in_edge_check_)
        {
            do {
                ++i_at_;
            } while(i_at_ != i_end && !has_edge(g_->edge(*i_at_, vertex_)));
            if(i_at_ == i_end)
            {
                in_edge_check_ = false;
                i_at_ = begin(g_->range);
                if(has_edge(g_->edge(vertex_, *i_at_)))
                { return *this; }
            }
        }
        if(!in_edge_check_)
        {
            do {
                ++i_at_;
            } while(i_at_ != i_end && !has_edge(g_->edge(vertex_, *i_at_)));
        }

        return *this;
    }

    This operator++(int)
    {
        const This temp = *this;
        const vertex_iterator i_end = end(g_->range);

        if(in_edge_check_)
        {
            do {
                ++i_at_;
            } while(i_at_ != i_end && !has_edge(g_->edge(*i_at_, vertex_)));
            if(i_at_ == i_end)
            {
                in_edge_check_ = false;
                i_at_ = begin(g_->range);
                if(has_edge(g_->edge(vertex_, *i_at_)))
                { return *this; }
            }
        }
        if(!in_edge_check_)
        {
            do {
                ++i_at_;
            } while(i_at_ != i_end && !has_edge(g_->edge(vertex_, *i_at_)));
        }

        return temp;
    }

    vertex_descriptor operator*()
    {
        return *i_at_;
    }

    const graph_type* g_;
    vertex_descriptor vertex_;
    vertex_iterator i_at_;
    bool in_edge_check_;
};

template<typename Graph>
bool operator==(function_graph_adjacency_iterator<Graph> const& lhs,
                function_graph_adjacency_iterator<Graph> const& rhs)
{
    return //lhs.g_ == rhs.g_ &&  // g_ is an odd case, consider this line
           lhs.vertex_ == rhs.vertex_ &&
           lhs.i_at_ == rhs.i_at_ &&
           lhs.in_edge_check_ == rhs.in_edge_check_;
}

template<typename Graph>
bool operator!=(function_graph_adjacency_iterator<Graph> const& lhs,
                function_graph_adjacency_iterator<Graph> const& rhs)
{
    return //lhs.g_ != rhs.g_ ||  // g_ is an odd case, consider this line
           lhs.vertex_ != rhs.vertex_ ||
           lhs.i_at_ != rhs.i_at_ ||
           lhs.in_edge_check_ != rhs.in_edge_check_;
}



/** Edge Iterator */
template<typename Graph>
struct function_graph_edge_iterator {
private:
    typedef function_graph_edge_iterator<Graph> This;

public:
    typedef Graph graph_type;
    typedef typename graph_type::vertex_iterator vertex_iterator;
    typedef typename graph_type::edge_descriptor edge_descriptor;
    typedef typename graph_type::vertex_descriptor vertex_descriptor;

    /** Iterator traits */
    typedef std::input_iterator_tag iterator_category;
    typedef edge_descriptor value_type;
    typedef int different_type;
    typedef value_type* pointer;
    typedef value_type& reference;

    /** @name Constructors */
    //@{
    function_graph_edge_iterator()
        : g_(0)
    { }

    function_graph_edge_iterator(graph_type const& g)
        : g_(&g), i_at_1_(begin(g.range)), i_at_2_(begin(g.range))
    {
        const vertex_iterator i_begin = begin(g.range);
        const vertex_iterator i_end = end(g.range);
        bool done = true;

        for(; i_at_1_ != i_end; ++i_at_1_)
        {
            for(; i_at_2_ != i_end; ++i_at_2_)
            {
                if(has_edge(g.edge(*i_at_1_, *i_at_2_)))
                {
                    done = true;
                    break;
                }
            }
            if(done) break;
        }
    }

    // initialize to the end
    function_graph_edge_iterator(graph_type const& g, bool)
        : g_(&g), i_at_1_(end(g.range)), i_at_2_(end(g.range))
    { }

    function_graph_edge_iterator(This const& cp)
        : g_(cp.g_), i_at_1_(cp.i_at_1_), i_at_2_(cp.i_at_2_)
    { }
    //@}

    This& operator++()
    {
        const vertex_iterator i_begin = begin(g_->range);
        const vertex_iterator i_end = end(g_->range);
        bool done = false;

        
        if(++i_at_2_ == i_end)
        {
            ++i_at_1_;
            if(i_at_1_ != i_end) i_at_2_ = i_begin;
        }

        for(; i_at_1_ != i_end; ++i_at_1_)
        {
            for(; i_at_2_ != i_end; ++i_at_2_)
            {
                if(has_edge(g_->edge(*i_at_1_, *i_at_2_)))
                {
                    done = true;
                    break;
                }
            }
            if(done) break;
        }

        return *this;
    }

    This operator++(int)
    {
        const This temp = *this;
        const vertex_iterator i_begin = begin(g_->range);
        const vertex_iterator i_end = end(g_->range);
        bool done = false;

        
        if(++i_at_2_ == i_end)
        {
            ++i_at_1_;
            if(i_at_1_ != i_end) i_at_2_ = i_begin;
        }

        for(; i_at_1_ != i_end; ++i_at_1_)
        {
            for(; i_at_2_ != i_end; ++i_at_2_)
            {
                if(has_edge(g_->edge(*i_at_1_, *i_at_2_)))
                {
                    done = true;
                    break;
                }
            }
            if(done) break;
        }

        return temp;
    }

    edge_descriptor operator*()
    {
        return edge_descriptor(g_->edge(*i_at_1_, *i_at_2_),
                               *i_at_1_,
                               *i_at_2_);
    }

    const graph_type* g_;
    vertex_iterator i_at_1_;
    vertex_iterator i_at_2_;
};

template<typename Graph>
bool operator==(function_graph_edge_iterator<Graph> const& lhs,
                function_graph_edge_iterator<Graph> const& rhs)
{
    return //lhs.g_ == rhs.g_ &&  // g_ is an odd case, consider this line
           lhs.i_at_1_ == rhs.i_at_1_ &&
           lhs.i_at_2_ == rhs.i_at_2_;
}

template<typename Graph>
bool operator!=(function_graph_edge_iterator<Graph> const& lhs,
                function_graph_edge_iterator<Graph> const& rhs)
{
    return //lhs.g_ != rhs.g_ ||  // g_ is an odd case, consider this line
           lhs.i_at_1_ != rhs.i_at_1_ ||
           lhs.i_at_2_ != rhs.i_at_2_;
}



/** function_graph tags */
struct finite_domain_tag { };
struct infinite_domain_tag { };
struct function_graph_traversal_tag
    : public virtual incidence_graph_tag,
      public virtual bidirectional_graph_tag,
      public virtual adjacency_graph_tag,
      public virtual vertex_list_graph_tag,
      public virtual edge_list_graph_tag,
      public virtual adjacency_matrix_tag
{ };


/** has_domain determines whether or not a function graph has a fintie domain */
template<typename Function_Graph>
struct has_finite_domain
    : mpl::bool_<
          is_same<
              typename Function_Graph::domain_category,
              finite_domain_tag
          >::value
      >
{ };




/** @name Edge Descriptor */
template<typename Result, typename Vertex>
struct function_graph_edge_descriptor {
    typedef Result result_type;
    typedef Vertex vertex_descriptor;

    function_graph_edge_descriptor()
    { }

    function_graph_edge_descriptor(result_type result,
                                   vertex_descriptor source,
                                   vertex_descriptor target)
        : result(result), source(source), target(target)
    { }

    result_type result;
    vertex_descriptor source;
    vertex_descriptor target;
};

template<typename Result, typename Vertex>
bool operator==(function_graph_edge_descriptor<Result, Vertex> const& lhs,
                function_graph_edge_descriptor<Result, Vertex> const& rhs)
{
    return lhs.result == rhs.result &&
           lhs.source == rhs.source &&
           lhs.target == rhs.target;
}

template<typename Result, typename Vertex>
bool operator!=(function_graph_edge_descriptor<Result, Vertex> const& lhs,
                function_graph_edge_descriptor<Result, Vertex> const& rhs)
{
    return lhs.result != rhs.result ||
           lhs.source != rhs.source ||
           lhs.target != rhs.target;
}



/** @name has_edge(r)
 *  @description Determine the existence 
 */
//@{
template<typename Result>
bool
has_edge(Result)
{ return true; }

bool
has_edge(bool r)
{ return r; }
//@}



/** @name Function Graph Base
 *
 *  @description function_graph_base handles the edge function. A user can
 *  define the function as a boost::function, but the user may only have access
 *  to this function through the function graph via edge().
 */

//@{
template <typename Func>
struct function_graph_base {
    typedef Func function_type;
    typedef typename function_type::first_argument_type vertex_type;
    typedef typename function_type::result_type result_type;
    typedef function_graph_edge_descriptor<result_type, vertex_type> edge_type;

    /** Constructors to allow for initialization of edge */
    function_graph_base(function_type const& f)
        : edge_(f)
    { }

    // Allow access to the function edge_ holds, not edge_ itself.
    result_type edge (vertex_type v1, vertex_type v2) const
    { return edge_(v1, v2); }

    function_type edge_;
};

template <typename Func, typename Rng>
struct function_graph_base_range {

    typedef Func function_type;
    typedef typename function_type::first_argument_type vertex_type;
    typedef typename function_type::result_type result_type;
    typedef function_graph_edge_descriptor<result_type, vertex_type> edge_type;
    typedef Rng vertex_iterator_range;

    /** Constructors to allow for initialization of edge */
    function_graph_base_range(function_type const& f,
                              vertex_iterator_range const& r)
        : edge_(f), range(r)
    { }

    // Allow access to the function edge_ holds, not edge_ itself.
    result_type edge (vertex_type v1, vertex_type v2) const
    { return edge_(v1, v2); }

    function_type edge_;
    vertex_iterator_range range;
};
//@}



/**
 * @name Function Graph
 * @description function_graph is a data structure that implements implicit
 * graphs and more.
 * @note This is a trap for poor template parameters
 */

template <typename Function, typename Range = infinite_domain_tag>
struct function_graph;



/**
 * function_graph is a data structure that implements implicit graphs and more.
 * Better documentation and explanation of the data structure to come.
 *
 * @internal The typical user of function graph may not need to change edge
 * function during execution. However, since the code needed is trivial,
 * set_edge is part of the interface. Paired with it is the default constructor.
 */
template <typename Result, typename Vertex, typename Range>
struct function_graph<Result(Vertex, Vertex), Range>
    : private function_graph_base_range<function<Result(Vertex, Vertex)>, Range>
{
private:
    typedef function_graph_base_range<
                function<Result(Vertex, Vertex)>,
                Range
            > Base;
    typedef function_graph<Result(Vertex, Vertex), Range> This;

public:
    typedef typename Base::function_type function_type;
    typedef typename Base::vertex_type vertex_descriptor;
    typedef typename Base::edge_type edge_descriptor;
    typedef typename Base::result_type result_type;
    typedef unsigned int degree_size_type;
    typedef unsigned int vertices_size_type;
    typedef unsigned int edges_size_type;
    typedef directed_tag directed_category;
    typedef disallow_parallel_edge_tag edge_parallel_category;
    typedef function_graph_traversal_tag traversal_category;
    typedef Range vertex_iterator_range;
    typedef typename range_iterator<vertex_iterator_range>::type
                         vertex_iterator;
    typedef function_graph_in_edge_iterator<This> in_edge_iterator;
    typedef function_graph_out_edge_iterator<This> out_edge_iterator;
    typedef function_graph_adjacency_iterator<This> adjacency_iterator;
    typedef function_graph_edge_iterator<This> edge_iterator;
    typedef finite_domain_tag domain_category;

    /** Constructor: takes a functor and range */
    function_graph(function_type const& f, vertex_iterator_range const& r)
        : Base(f, r)
    { }

    using Base::edge;
    using Base::range;
};

// Specialization of function_graph without range
template <typename Result, typename Vertex>
struct function_graph<Result(Vertex, Vertex), infinite_domain_tag>
    : public function_graph_base<function<Result(Vertex, Vertex)> >
{
private:
    typedef function_graph_base<function<Result(Vertex, Vertex)> > Base;
    typedef function_graph<function<Result(Vertex, Vertex)> > This;

public:
    typedef typename Base::function_type function_type;
    typedef typename Base::vertex_type vertex_descriptor;
    typedef typename Base::edge_type edge_descriptor;
    typedef typename Base::result_type result_type;
    typedef std::size_t degree_size_type;
    typedef std::size_t vertices_size_type;
    typedef directed_tag directed_category;
    typedef disallow_parallel_edge_tag edge_parallel_category;
    typedef adjacency_matrix_tag traversal_category;
    typedef infinite_domain_tag domain_category;

    /** Constructor: takes a boost::function or functor */
    function_graph(function_type const& f)
        : Base(f)
    { }

    using Base::edge;
};



#define FUNC_GRAPH \
    function_graph<Result(Vertex, Vertex), Range>

////////////////////////////////////////////////////////////////////////////////
// IncedenceGraph Functions                                                   

template<typename Result, typename Vertex, typename Range>
std::pair<
    typename FUNC_GRAPH::out_edge_iterator,
    typename FUNC_GRAPH::out_edge_iterator
>
out_edges(typename FUNC_GRAPH::vertex_descriptor const& u, FUNC_GRAPH const& g)
{
    typedef function_graph<Result(Vertex, Vertex), Range> Graph;
    typedef typename Graph::out_edge_iterator out_edge_iterator;

    out_edge_iterator out_edge_begin(g, u, begin(g.range));
    out_edge_iterator out_edge_end(g, u);

    return std::make_pair(out_edge_begin, out_edge_end);
}

template <typename Result, typename Vertex, typename Range>
Vertex source(function_graph_edge_descriptor<Result, Vertex> const& e,
              function_graph<Result(Vertex, Vertex), Range>)
{ return e.source; }

template <typename Result, typename Vertex, typename Range>
Vertex target(function_graph_edge_descriptor<Result, Vertex> const& e,
              function_graph<Result(Vertex, Vertex), Range>)
{ return e.target; }

template<typename Result, typename Vertex, typename Range>
typename FUNC_GRAPH::degree_size_type
out_degree(typename FUNC_GRAPH::vertex_descriptor const& u, FUNC_GRAPH const& g)
{
    typedef FUNC_GRAPH Graph;
    typedef typename Graph::vertex_iterator vertex_iterator;
    typedef typename FUNC_GRAPH::degree_size_type degree_size_type;

    degree_size_type out_edges = 0;
    vertex_iterator vertex_at(begin(g.range));
    vertex_iterator vertex_end(end(g.range));
    for(;vertex_at != vertex_end; ++vertex_at)
    {
        if(has_edge(g.edge(u, *vertex_at))) ++out_edges;
    }

    return out_edges;
}



////////////////////////////////////////////////////////////////////////////////
// BidirectionalGraph Functions 

template<typename Result, typename Vertex, typename Range>
std::pair<
    typename FUNC_GRAPH::in_edge_iterator,
    typename FUNC_GRAPH::in_edge_iterator
>
in_edges(typename FUNC_GRAPH::vertex_descriptor const& v, FUNC_GRAPH const& g)
{
    typedef function_graph<Result(Vertex, Vertex), Range> Graph;
    typedef typename Graph::in_edge_iterator in_edge_iterator;

    in_edge_iterator in_edge_begin(g, v, begin(g.range));
    in_edge_iterator in_edge_end(g, v);

    return std::make_pair(in_edge_begin, in_edge_end);
}

template<typename Result, typename Vertex, typename Range>
typename FUNC_GRAPH::degree_size_type
in_degree(typename FUNC_GRAPH::vertex_descriptor const& v, FUNC_GRAPH const& g)
{
    typedef FUNC_GRAPH Graph;
    typedef typename Graph::vertex_iterator vertex_iterator;
    typedef typename FUNC_GRAPH::degree_size_type degree_size_type;

    degree_size_type in_edges = 0;
    vertex_iterator vertex_at(begin(g.range));
    vertex_iterator vertex_end(end(g.range));
    for(;vertex_at != vertex_end; ++vertex_at)
    {
        if(has_edge(g.edge(*vertex_at, v))) ++in_edges;
    }

    return in_edges;
}

template<typename Result, typename Vertex, typename Range>
typename FUNC_GRAPH::degree_size_type
degree(typename FUNC_GRAPH::vertex_descriptor const& v, FUNC_GRAPH const& g)
{
    typedef FUNC_GRAPH graph_type;
    typedef typename graph_type::degree_size_type degree_size_type;
    typedef typename graph_type::vertex_iterator vertex_iterator;

    const vertex_iterator i_begin = begin(g.range);
    const vertex_iterator i_end = end(g.range);
    vertex_iterator i_at = i_begin;
    degree_size_type degree_count = 0;

    while(i_at != i_end)
    {
        if(has_edge(g.edge(v, *i_at))) ++degree_count;
        ++i_at;
    }
    i_at = i_begin;
    while(i_at != i_end)
    {
        if(has_edge(g.edge(*i_at, v))) ++degree_count;
        ++i_at;
    }

    return degree_count;
}



////////////////////////////////////////////////////////////////////////////////
// AdjacencyGraph Functions 

template <typename Result, typename Vertex, typename Range>
std::pair<
    typename FUNC_GRAPH::adjacency_iterator,
    typename FUNC_GRAPH::adjacency_iterator
>
adjacent_vertices(typename FUNC_GRAPH::vertex_descriptor const& v,
                  FUNC_GRAPH const& g)
{
    typedef FUNC_GRAPH graph_type;
    typedef typename graph_type::adjacency_iterator adjacency_iterator;

    return std::make_pair(adjacency_iterator(g, v),
                          adjacency_iterator(g, v, true));
}



////////////////////////////////////////////////////////////////////////////////
// VertexListGraph Functions 

template <typename Result, typename Vertex, typename Range>
std::pair<
    typename FUNC_GRAPH::vertex_iterator,
    typename FUNC_GRAPH::vertex_iterator
>
vertices(function_graph<Result(Vertex, Vertex), Range> const& g)
{ return std::make_pair(begin(g.range), end(g.range)); }

template<typename Result, typename Vertex, typename Range>
typename FUNC_GRAPH::vertices_size_type
num_vertices(FUNC_GRAPH const& g)
{ return size(g.range); }



////////////////////////////////////////////////////////////////////////////////
// EdgeListGraph Functions 

template<typename Result, typename Vertex, typename Range>
std::pair<
    typename FUNC_GRAPH::edge_iterator,
    typename FUNC_GRAPH::edge_iterator
>
edges(FUNC_GRAPH const& g)
{
    typedef FUNC_GRAPH graph_type;
    typedef typename graph_type::edge_iterator edge_iterator;
    typedef typename graph_type::vertex_iterator vertex_iterator;

    return std::make_pair(edge_iterator(g),edge_iterator(g,true));
}

template<typename Result, typename Vertex, typename Range>
typename FUNC_GRAPH::edges_size_type
num_edges(FUNC_GRAPH const& g)
{
    typedef FUNC_GRAPH graph_type;
    typedef typename FUNC_GRAPH::edges_size_type edges_size_type;
    typedef typename FUNC_GRAPH::vertex_iterator vertex_iterator;

    const vertex_iterator i_begin = begin(g.range);
    const vertex_iterator i_end = end(g.range);

    edges_size_type edges_count = 0;

    for(vertex_iterator i_at_1 = i_begin; i_at_1 != i_end; ++i_at_1)
    {
        for(vertex_iterator i_at_2 = i_begin; i_at_2 != i_end; ++i_at_2)
        {
            if(has_edge(g.edge(*i_at_1, *i_at_2))) ++edges_count;
        }
    }

    return edges_count;
}

// Note: source and target have been defined in the IncedenceGraph portion above



////////////////////////////////////////////////////////////////////////////////
// AdjacencyMatrix Functions 

template <typename Result, typename Vertex, typename Range>
std::pair<typename FUNC_GRAPH::edge_descriptor, bool>
edge(typename FUNC_GRAPH::vertex_descriptor u,
     typename FUNC_GRAPH::vertex_descriptor v,
     FUNC_GRAPH const& g)
{
    typedef FUNC_GRAPH graph_type;
    typedef typename graph_type::result_type result_type;
    typedef typename graph_type::edge_descriptor edge_descriptor;

    const result_type r = g.edge(u, v);
    return std::make_pair(edge_descriptor(r, u, v), has_edge(r));
}

#undef FUNC_GRAPH

}   // boost namespace

#endif /*FUNCTION_GRAPH_HPP_*/

