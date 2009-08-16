///////////////////////////////////////////////////////////////////////////////
// tree::example::tree.cpp                                                   //
//                                                                           //
//  Copyright 2009 Erwann Rogard. Distributed under the Boost                //
//  Software License, Version 1.0. (See accompanying file                    //
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)         //
///////////////////////////////////////////////////////////////////////////////
#include <boost/tree/stage.hpp>
#include <boost/tree/node.hpp>
#include <libs/tree/example/tree.h>

void example_tree(std::ostream& out){
    out << "-> example_tree ";

    using namespace boost;
    
    const unsigned n = 3;
    typedef tree::dynamic_stage<n> stage_;
    typedef tree::node<n> node_;

    using namespace boost;
    const unsigned n_j = 3;
    const node_ root_node;

    for(unsigned j = 0; j<n_j; j++){
        out 
            << '(' 
            << stage_::position_first(j)
            << ',' 
            << stage_::position_last(j)
            << ')';
    }
    out << std::endl;

    unsigned nn = stage_::position_last(n_j)-1;

    {
        node_ node = root_node;
        unsigned old_stage = node.stage;
        do{
            if(node.stage != old_stage){
                out << std::endl;
                old_stage = node.stage;
            }
            out << node;
        }while(
            position(++node)<nn    
        );
    }
    out << std::endl << std::endl;
    {
        node_ node = root_node;
        ++node;
        unsigned old_stage = node.stage;
        do{
            if(node.stage != old_stage){
                out << std::endl;
                old_stage = node.stage;
            }
            out << root(node);
        }while(
            position(++node)<nn    
        );
    }
    out << std::endl << std::endl;
    {
        node_ node = back(root_node,n_j);
        
        unsigned old_stage = node.stage;
        do{
            if(node.stage != old_stage){
                out << std::endl;
                old_stage = node.stage;
            }
            out << node;
        }while(
            (--node).stage > 0    
        );
    }

    out << " <- ";
}