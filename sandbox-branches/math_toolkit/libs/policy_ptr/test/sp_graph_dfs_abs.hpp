//traverses a graph applying abstract operations to Vertices, possibly smart_ptrs.
#ifndef LIBS_POLICY_PTR_TEST_SP_GRAPH_DFS_ABS_HPP_LJE20050520
#define LIBS_POLICY_PTR_TEST_SP_GRAPH_DFS_ABS_HPP_LJE20050520
#include <set>
  template
  < class Vertex
  , class NextStk
  >
  struct
sp_graph_dfs_abs
/**@brief
 *  traverse graph in 'depth-first-search' order.
 */
  : public NextStk
{
        typedef
      Vertex
    vertex_type
    ;
        typedef
      typename vertex_type::sp_type
    arc_type
    ;
        typedef
      std::set<vertex_type const*>
    vertex_ptr_set_type
    ;
        virtual
    ~sp_graph_dfs_abs(void)
    {}
    
    sp_graph_dfs_abs(void)
    {}
    
      vertex_ptr_set_type const&
    get_visited_vertices(void)const
    {
        return my_visited_vertices;
    }
    
      void
    clear_visited_vertices(void)
    {
        my_visited_vertices.clear();
    }
    
      void
    visit_arc_check
      ( arc_type const& a_arc
      )
    {
        vertex_type const* target_ptr=get_arc_target(a_arc);
        if(target_ptr)
        {  
            visit_arc_target(a_arc);
            vertex_type const& target_ref=*target_ptr;
            if(is_visited(target_ref))
            {
                visit_vertex_repeat();
            }
            else
            {
                visit_vertex_first();
                mark_visited(target_ref);
                traverse_arcs(target_ref);
            }
        }
        else
        {
            visit_vertex_absent();
        }
    }
    
 private:
 
        virtual
      vertex_type const*
    get_arc_target
      ( arc_type const& a_arc
      )
    =0
    ;
        virtual
      void
    visit_arc_target
      ( arc_type const& a_arc
      )
    =0
    ;
        virtual
      void
    visit_vertex_absent
      ( void
      )
    =0
    ;
        virtual
      void
    visit_vertex_first
      ( void
      )
    =0
    ;
        virtual
      void
    visit_vertex_repeat
      ( void
      )
    =0
    ;

      bool
    is_visited(vertex_type const&a_vertex)const
    {
        typename vertex_ptr_set_type::const_iterator found
          =my_visited_vertices.find(&a_vertex);
        bool result = (found != my_visited_vertices.end());
        return result;
    }
    
      void
    mark_visited
      ( vertex_type const& a_vertex
      )
    {
        my_visited_vertices.insert(&a_vertex);
    }
    
      void
    traverse_arcs
      ( vertex_type const& a_vertex
      )
    {
        typedef typename vertex_type::const_iterator iter_type;
        iter_type iter_end(a_vertex.end());
        typename NextStk::next_abs_type& next=NextStk::push_next();
        for
          ( iter_type iter_cur(a_vertex.begin())
          ; iter_cur!=iter_end
          ; ++iter_cur
          )
        {
            ++next;
            arc_type const& arc_cur=*iter_cur;
            visit_arc_check(arc_cur);
        }
        NextStk::pop_next();
    }
    
      vertex_ptr_set_type
    my_visited_vertices
    ;
    
};
#endif
