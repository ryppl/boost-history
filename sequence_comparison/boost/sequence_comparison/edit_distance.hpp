// Copyright 2008 Chris Goller, Jeff Flinn, Brook Milligan and Stjepan Rajko.
// Distributed under the Boost Software License, Version 1.0. (See
// accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_SEQUENCE_COMPARISON_EDIT_DISTANCE_HPP
#define BOOST_SEQUENCE_COMPARISON_EDIT_DISTANCE_HPP

#include <boost/assert.hpp>
#include <boost/graph/graph_traits.hpp>
#include <boost/graph/properties.hpp>
#include <boost/iterator/iterator_facade.hpp>
#include <map>
#include <utility>

namespace boost { namespace sequence_comparison {

namespace graph {
    
    struct vertex_descriptor
        : public std::pair<int, int>
    {
        vertex_descriptor()
        {}
        vertex_descriptor(int first, int second)
            : std::pair<int, int>(first, second)
        {}
        bool operator == (const vertex_descriptor &rhs) const
        {   return first == rhs.first && second == rhs.second; }
        bool operator < (const vertex_descriptor &rhs) const
        {   return second < rhs.second || (second == rhs.second && first < rhs.first); }
        
        static inline vertex_descriptor null()
        {   return vertex_descriptor(-1,-1); }
    };
    
    struct edge_descriptor
        : private std::pair<vertex_descriptor, unsigned>
    {
        edge_descriptor()
        {}
        edge_descriptor(vertex_descriptor source, unsigned position)
            : m_source(source), m_position(position)
        {
            BOOST_ASSERT(m_position > 0 && m_position < 4);
        }
        const vertex_descriptor &source() const
        {
            return m_source;
        }
        vertex_descriptor target() const
        {
            return vertex_descriptor(m_source.first + (m_position & 1),
                        m_source.second + ((m_position & 2) >> 1));
        }
        unsigned position() const
        {   return m_position; }
        bool operator == (const edge_descriptor &rhs) const
        {   return m_source == rhs.m_source && m_position == rhs.m_position; }
        bool operator != (const edge_descriptor &rhs) const
        {   return !(*this == rhs); }
    private:
        vertex_descriptor m_source;
        unsigned m_position;
    };
    
    class edge_iterator;
    class vertex_iterator;
    
    template<typename RandomAccessIteratorA, typename RandomAccessIteratorB>
    class edit_distance
    {
    public:
        typedef graph::vertex_descriptor    vertex_descriptor;
        typedef graph::edge_descriptor      edge_descriptor;
        typedef void                        adjacency_iterator;
        typedef graph::edge_iterator        out_edge_iterator;
        typedef void                        in_edge_iterator;
        typedef graph::vertex_iterator      vertex_iterator;
        typedef void                        edge_iterator;
        
        typedef boost::directed_tag                 directed_category;
        typedef boost::disallow_parallel_edge_tag   edge_parallel_category;
        typedef boost::incidence_graph_tag          traversal_category;
        
        typedef size_t                      vertices_size_type;
        typedef void                        edges_size_type;
        typedef unsigned                    degree_size_type;
        
        static inline vertex_descriptor null_vertex()
        {   return vertex_descriptor::null(); }

        edit_distance(RandomAccessIteratorA begin_a, RandomAccessIteratorA end_a,
            RandomAccessIteratorB begin_b, RandomAccessIteratorB end_b)
            : m_begin_a(begin_a), m_end_a(end_a), m_begin_b(begin_b), m_end_b(end_b)
        {}
        size_t a_size() const
        {
            return m_end_a - m_begin_a;
        }
        size_t b_size() const
        {
            return m_end_b - m_end_b;
        }
        unsigned weight(const edge_descriptor &edge) const
        {
            unsigned position = edge.position();
            BOOST_ASSERT(position > 0 && position <= 3);
            
            if (position <= 2)
                return 1;
            else
            {
                std::pair<int, int> coords = edge.source();
                return m_begin_a[coords.first] != m_begin_b[coords.second];
            }
        }
        vertex_descriptor upper_left() const
        {
            return vertex_descriptor(0,0);
        }
        vertex_descriptor lower_right() const
        {
            return vertex_descriptor(a_size(), b_size());
        }
    private:
        RandomAccessIteratorA m_begin_a, m_end_a;
        RandomAccessIteratorA m_begin_b, m_end_b;
    };

    template<typename Graph>
    size_t num_vertices(const Graph &graph)
    {
        return (graph.a_size() + 1) * (graph.b_size()+1);
    }
        
    class edge_iterator
      : public boost::iterator_facade<
            edge_iterator
          , edge_descriptor
          , boost::forward_traversal_tag
          , edge_descriptor
        >
    {
    public:
        edge_iterator()
          : m_v(vertex_descriptor::null())
          , m_position(0)
          , m_mask(0)
        {}

        template<typename Graph>
        explicit edge_iterator(const vertex_descriptor &v, const Graph &g)
          : m_v(v)
          , m_position(0)
        {
            m_mask =
                (v.first < g.a_size()) +
                ((v.second < g.b_size()) << 1);
            increment();
        }
        
        struct end{};
        template<typename Graph>
        explicit edge_iterator(const vertex_descriptor &v, const Graph &g, end)
          : m_v(v)
          , m_position(4)
        {}

    private:
        friend class boost::iterator_core_access;

        void increment()
        {
            BOOST_ASSERT(m_position >= 0 && m_position < 4);
            do
            {
                m_position++;
            } while (m_position < 4 && ((m_position & m_mask) == m_position));
        }
        
        bool equal(edge_iterator const& other) const
        {
            return this->m_v == other.m_v && this->m_position == other.m_position;
        }

        edge_descriptor dereference() const
        {
            BOOST_ASSERT(m_position > 0 && m_position < 4);
            
            return edge_descriptor(m_v, m_position);             
        }

        vertex_descriptor m_v;
        unsigned m_position;
        unsigned m_mask;
    };
    
    class vertex_iterator
      : public boost::iterator_facade<
            vertex_iterator
          , vertex_descriptor
          , boost::forward_traversal_tag
          , const vertex_descriptor &
        >
    {
    public:
        vertex_iterator()
          : m_v(vertex_descriptor::null())
          , m_row_max(0)
        {}

        template<typename Graph>
        explicit vertex_iterator(const vertex_descriptor &v, const Graph &g)
          : m_v(v)
          , m_row_max(g.a_size())
        {}

    private:
        friend class boost::iterator_core_access;

        void increment()
        {
            if(++m_v.first > m_row_max)
            {
                m_v.first = 0;
                m_v.second++;
            }
        }
        
        bool equal(vertex_iterator const& other) const
        {
            return this->m_v == other.m_v;
        }

        const vertex_descriptor &dereference() const
        {            
            return m_v;             
        }

        vertex_descriptor m_v;
        size_t m_row_max;
    };

    template<typename Graph>
    std::pair<vertex_iterator, vertex_iterator> vertices(Graph &graph)
    {
        return std::make_pair(vertex_iterator(vertex_descriptor(0, 0), graph)
            , vertex_iterator(vertex_descriptor(0, graph.b_size()+2), graph));
    }

    // Returns the vertex descriptor for u of the edge (u,v) represented by e.
    // Return type: vertex_descriptor
    template<typename Graph>
    vertex_descriptor source(const edge_descriptor &e, const Graph &g)
    {
        return e.source();
    }

    // Returns the vertex descriptor for v of the edge (u,v) represented by e.
    // Return type: vertex_descriptor
    template<typename Graph>
    vertex_descriptor target(const edge_descriptor &e, const Graph &g)
    {
        return e.target();
    }
    
    // Returns an iterator-range providing access to the out-edges of vertex u in graph g.
    // The source vertex of an edge obtained via an out edge iterator is guaranteed
    // (for both directed and undirected graphs) to be the vertex u used in the call to
    // out_edges(u, g) and the target vertex must the a vertex adjacent to u.
    // Return type: std::pair<out_edge_iterator, out_edge_iterator>
    template<typename Graph>
    std::pair<edge_iterator, edge_iterator> out_edges(const vertex_descriptor &u, const Graph &g)
    {
        return std::make_pair(edge_iterator(u, g), edge_iterator(u, g, edge_iterator::end()));
    }
    
    template<typename Graph>
    unsigned out_degree(const vertex_descriptor &u, const Graph &g)
    {
        bool not_right_edge = u.first < g.a_size();
        bool not_bottom_edge = u.second < g.b_size();
        return not_right_edge + not_bottom_edge + (not_right_edge && not_bottom_edge);
    }
    
    template<typename Graph>
    class weight_map
    {
    public:
        typedef unsigned value_type;
        typedef unsigned reference;
//        typedef unsigned reference_type;
        typedef edge_descriptor key_type;
        typedef boost::readable_property_map_tag category;
        
        weight_map(const Graph &g)
            : m_graph(g)
        {}
        
        value_type get(const key_type &edge) const
        {
            return m_graph.weight(edge);
        }
    private:
        const Graph &m_graph;
    };

    class distance_map
    {
    public:
        typedef unsigned value_type;
        typedef unsigned reference;
        typedef vertex_descriptor key_type;
        typedef boost::read_write_property_map_tag category;
                
        value_type get(const key_type &key) const
        {
            return m_map.find(key)->second;
        }
        
        void put(const key_type &key, value_type value)
        {
            m_map[key] = value;
        }
    private:
        std::map<key_type, value_type> m_map;
    };
    
    template<typename Map>
    unsigned get(const Map &map, const typename Map::key_type &key)
    {
        return map.get(key);
    }
    
    template<typename Map>
    void put(Map &map, const typename Map::key_type &key, const typename Map::value_type &value)
    {
        map.put(key, value);
    }
}

} // namespace sequence_comparison
}

#endif

