#ifndef BOOST_GRAPH_EXCEPTION_HPP
#define BOOST_GRAPH_EXCEPTION_HPP

#include <stdexcept>

namespace boost {

  struct bad_graph : public std::invalid_argument {
    bad_graph(const std::string& what_arg) : invalid_argument(what_arg) { }
  };

  struct not_a_dag : public bad_graph {
    not_a_dag()
      : bad_graph("The graph must be a DAG.") { } 
  };

  struct negative_edge : public bad_graph {
    negative_edge()
      : bad_graph("The graph may not contain an edge with negative weight."){ }
  };

  struct negative_cycle : public bad_graph {
    negative_cycle()
      : bad_graph("The graph may not contain negative cycles.") { }
  };
  struct not_connected : public bad_graph {
    not_connected()
      : bad_graph("The graph must be connected.") { }
  };

} // namespace boost

#endif // BOOST_GRAPH_EXCEPTION_HPP
