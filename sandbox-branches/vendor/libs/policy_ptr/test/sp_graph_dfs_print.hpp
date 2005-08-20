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
      VertexType
    vertex_type
    ;
        typedef
      typename vertex_type::sp_type
    arc_type
    ;
        typedef
      sp_graph_dfs_abs<vertex_type,delim_stk_print>
    super_type
    ;
      vertex_type const*
    get_arc_target
      ( arc_type const& a_arc
      )
    {
        return get_impl(a_arc);
    }
      void
    visit_arc_target
      ( arc_type const& a_arc
      )
    {
        mout()
          <<":target id="<<get_arc_target(a_arc)->id_get()
          <<":count id="<<a_arc.count_prox_con().count_ptr_con()->id_get()
          <<"\n";
    }
      void
    visit_vertex_absent
      ( void
      )
    {
        mout()<<"(absent vertex)\n";
    }
      void
    visit_vertex_first
      ( void
      )
    {
        mout()<<"\n";
    }
      void
    visit_vertex_repeat
      ( void
      )
    {
        mout()<<"<>\n";
    }
};

#endif
