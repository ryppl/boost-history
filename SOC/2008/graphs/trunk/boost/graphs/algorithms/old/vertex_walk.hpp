
#ifndef BREADTH_FIRST_VERTEX_WALK_HPP
#define BREADTH_FIRST_VERTEX_WALK_HPP

/**
 * The breadth first walk
 */
template <typename Graph, typename ColorMap, typename Queue>
struct breadth_first_vertex_walk
{
    // Grab the color and traits from the map
    typedef typename ColorMap::value_type Color;
    typedef color_traits<Color> ColorTraits;

    typedef typename Graph::vertex_descriptor Vertex;
    typedef typename Graph::edge_descriptor Edge;
    typedef typename Graph::adjacent_vertex_range AdjacencyRange;

    breadth_first_vertex_walk(Graph& g, Queue& queue, ColorMap color, Vertex start)
         : graph(g), queue(queue), color(color), start(start)
    { initialize(); }

    ~breadth_first_vertex_walk()
    { finalize(); }

    /**
     * Initialize the walk. This does not initialize the data structures passed
     * to the walk individually. It simply sets the starting state of the walk.
     */
    void initialize()
    {
        color(start) = ColorTraits::gray();
        queue.push(start);
    }

    /** Finish the algorithm. */
    void finalize()
    { }

    /**
     * Move to the next vertex in the walk. At each step, this pops the current
     * vertex and looks for unvisited adjacent vertices.
     */
    void next()
    {
        // Remvoe the top element from the queue and insert all unvisited
        // adjacent vertices.
        Vertex u = queue.front();
        queue.pop();

        AdjacencyRange ar = graph.adjacent_vertices(u);
        for( ; ar.first != ar.second; ++ar.first) {
            Vertex v = *ar.first;
            if(color(v) == ColorTraits::white()) {
                color(v) = ColorTraits::gray();

                // TODO: Visit the discovered vertex here?
                queue.push(v);
            }
        }

        color(u) = ColorTraits::black();
    }

    /** Return the current vertex. */
    Vertex vertex()
    { return queue.empty() ? Vertex() : queue.front(); }

    Graph& graph;
    Queue& queue;
    ColorMap color;
    Vertex start;
};

/**
 * Provide an iterator abstraction over the walk. The result of dereferencing
 * an iterator is the current vertex of the underlying walk. 
 */
template <typename Graph, typename ColorMap, typename Queue>
struct breadth_first_vertex_iterator
{
    typedef breadth_first_vertex_walk<Graph, ColorMap, Queue> walk_type;

    typedef std::forward_iterator_tag iterator_category;
    typedef std::size_t difference_type;
    typedef typename Graph::edge_descriptor value_type;
    typedef typename Graph::edge_descriptor reference;
    typedef walk_type* pointer;

    breadth_first_vertex_iterator(walk_type& walk)
        : walk(walk)
    { }

    inline breadth_first_vertex_iterator& operator++()
    { walk.next(); return *this; }

    inline bool operator==(breadth_first_vertex_iterator const& x) const
    { return walk.vertex() == x.walk.vertex(); }

    inline bool operator!=(breadth_first_vertex_iterator const& x) const
    { return !operator=(x); }

    inline reference operator*() const
    { return walk.vertex(); }

    walk_type& walk;
};

#endif
