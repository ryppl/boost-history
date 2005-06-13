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
      std::set<Vertex const*>
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
    visit_sp
      ( typename Vertex::sp_type const& a_sp
      )
    {
        Vertex const* ref=get_impl(a_sp);
        if(ref)
        {  
            visit_tree(*ref);
        }
        else
        {
            visit_vertex_null();
        }
    }
    
 private:
 
        virtual
      void
    visit_vertex_null
      ( void
      )
    =0
    ;
        virtual
      void
    visit_vertex_first
      ( Vertex const& a_vertex
      )
    =0
    ;
        virtual
      void
    visit_vertex_repeat
      ( Vertex const& a_vertex
      )
    =0
    ;

      bool
    is_visited(Vertex const&a_vertex)const
    {
        typename vertex_ptr_set_type::const_iterator found
          =my_visited_vertices.find(&a_vertex);
        bool result = (found != my_visited_vertices.end());
        return result;
    }
    
      void
    mark_visited
      ( Vertex const& a_vertex
      )
    {
        my_visited_vertices.insert(&a_vertex);
    }
    
      void
    visit_tree
      ( Vertex const& a_vertex
      )
    {
        if(is_visited(a_vertex))
        {
            visit_vertex_repeat(a_vertex);
        }
        else
        {
            visit_vertex_first(a_vertex);
            mark_visited(a_vertex);
            visit_children(a_vertex);
        }
    }
      void
    visit_children
      ( Vertex const& a_vertex
      )
    {
        typedef typename Vertex::const_iterator iter_type;
        iter_type iter_end(a_vertex.end());
        typename NextStk::next_abs_type& next=NextStk::push_next();
        for
          ( iter_type iter_cur(a_vertex.begin())
          ; iter_cur!=iter_end
          ; ++iter_cur
          )
        {
            ++next;
            typename Vertex::sp_type const& sp_cur=*iter_cur;
            visit_sp(sp_cur);
        }
        NextStk::pop_next();
    }
    
      vertex_ptr_set_type
    my_visited_vertices
    ;
    
};
#endif
