#include <boost/tree_view/depth_first.hpp>
#include <libs/tree_view/example/depth_first.h>

void example_depth_first(std::ostream& os){
    os << "-> example_depth_first " << std::endl;

    using namespace boost;
    
    const unsigned n_branches = 3;
    typedef tree_view::dynamic_stage<n_branches>     stage_;
    typedef tree_view::node<n_branches>              node_;
    typedef tree_view::depth_first<n_branches>       depth_first_;

    using namespace boost;
    const unsigned n_stages = 4;
    const node_ root_node;

    depth_first_ visit(n_stages);

    do{
        bool b = visit.is_subtree();
        if(b){
            os << '[' << visit.node() << ']';
        }else{
            os << visit.node();
        }
        ++visit;
    }while(
        !(visit.is_subtree() && (visit.node()==root_node))
    );
    os << "<-"; 
}


