///////////////////////////////////////////////////////////////////////////////
// tree_view::example::tree.cpp                                              //
//                                                                           //
//  Copyright 2009 Erwann Rogard. Distributed under the Boost                //
//  Software License, Version 1.0. (See accompanying file                    //
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)         //
///////////////////////////////////////////////////////////////////////////////
#include <boost/format.hpp>
#include <boost/tree_view/stage.hpp>
#include <boost/tree_view/node.hpp>
#include <boost/tree_view/dynamic_stage.hpp>
#include <libs/tree_view/example/tree.h>

void example_tree(std::ostream& out){
    out << "-> example_tree " << std::endl;

    using namespace boost;
    
    const unsigned n_branches = 3;
    typedef tree_view::dynamic_stage<n_branches>     stage_;
    typedef tree_view::node<n_branches>              node_;

    using namespace boost;
    const unsigned n_stages = 4;
    const node_ root_node;

    out << (format("{[i(j),i(j+1)) : j=0,...,%1%}:")%(n_stages-1)).str();
    for(unsigned j = 0; j<n_stages; j++){
        out 
            << '(' 
            << stage_::position_first(j)
            << ',' 
            << stage_::position_last(j)
            << ')';
    }
    out << std::endl;
    node_ the_last = last( root_node, n_stages-1);

    {   // Breadth-first visitation
        out << node_::header << std::endl;
        node_ node = root_node;
        unsigned old_stage = node.stage;
        do{
            if(node.stage != old_stage){
                out << std::endl;
                old_stage = node.stage;
            }
            out << node;
        }while(
            ++node< the_last    
        );
    }

    out << std::endl;
    {   // Breadth-first visitation, in reverse
        out << node_::header;
        node_ node = the_last;
        
        unsigned old_stage = node.stage;
        while(node.stage > 0){
            --node;
            if(node.stage != old_stage){
                old_stage = node.stage;
                out << std::endl;
            }
            out << node;
        }
    }

    out << " <- ";
}