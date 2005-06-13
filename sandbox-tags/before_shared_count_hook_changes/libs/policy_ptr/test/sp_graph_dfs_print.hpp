//traverses a graph while printing vertices in nested heirarchy fashion
#ifndef LIBS_POLICY_PTR_TEST_SP_GRAPH_DFS_PRINT_HPP_LJE20050520
#define LIBS_POLICY_PTR_TEST_SP_GRAPH_DFS_PRINT_HPP_LJE20050520
#include "sp_graph_dfs_abs.hpp"
#include "delim_stk_print.hpp"

  template
  < class VertexType
  >
  struct
sp_graph_dfs_print
/**@brief
 *  Print graph in 'depth-first-search' order.
 */
  : public sp_graph_dfs_abs<VertexType,delim_stk_print>
{
    sp_graph_dfs_print(void)
    {}
 private:
        typedef
      sp_graph_dfs_abs<VertexType,delim_stk_print>
    super_type
    ;
        typedef
      VertexType
    vertex_type
    ;
      void
    visit_vertex_null
      ( void
      )
    {
        mout()<<"(null vertex)\n";
    }
      void
    visit_vertex_non_null
      ( vertex_type const& a_vertex
      )
    {
        mout()<<a_vertex.id_get();
    }
      void
    visit_vertex_first
      ( vertex_type const& a_vertex
      )
    {
        visit_vertex_non_null(a_vertex);
        mout()<<"\n";
    }
      void
    visit_vertex_repeat
      ( vertex_type const& a_vertex
      )
    {
        visit_vertex_non_null(a_vertex);
        mout()<<"<>\n";
    }
};

#endif
