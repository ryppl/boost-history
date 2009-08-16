///////////////////////////////////////////////////////////////////////////////
// tree::node.hpp                                                            //
//                                                                           //
//  Copyright 2009 Erwann Rogard. Distributed under the Boost                //
//  Software License, Version 1.0. (See accompanying file                    //
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)         //
///////////////////////////////////////////////////////////////////////////////
#ifndef BOOST_TREE_NODE_HPP_ER_2009
#define BOOST_TREE_NODE_HPP_ER_2009
#include <stdexcept>
#include <ostream>
#include <boost/operators.hpp>
#include <boost/tree/stage.hpp>

namespace boost{
namespace tree{

//                                             ( 1 )                                          n^0
//                                         ( 1 , 2 ,3)                                        n^1
//                    ( 1 , 2 , 3 )        ( 1 ,[2], 3 )            ( 1, 2 , 3 )              n^2
//            (1,2,3) (1,2,3)(1,2,3)  (1,2,3) (1,2,3) (1,2,3)  (1,2,3) (1,2,3) (1,2,3)        n^3

    template<unsigned n>
    struct node : 
        incrementable<
            node<n>,
            decrementable<
                node<n>,
                less_than_comparable<
                    node<n>, 
                    equality_comparable<
                        node<n> 
                    >
                >
            >
        >
    {
        typedef std::size_t size_type;
        typedef node<n> this_;
        typedef dynamic_stage<n> dyna_stage_;
        unsigned stage;
        unsigned position_in_stage;
        
        node();
        node(unsigned j,unsigned k);
        this_& operator++();
        this_& operator--();
        bool operator==(const this_&);
        bool operator<(const this_&);

    };

}//tree

    template<unsigned n> std::ostream& 
    operator<<(std::ostream& out, const tree::node<n>& that);

    template<unsigned n> typename tree::node<n>::size_type 
    position(const tree::node<n>& that);
    template<unsigned n> tree::node<n> root(const tree::node<n>& leaf);
    template<unsigned n> tree::node<n> leaf(const tree::node<n>& root);

    // To visit all the nodes sharing the same root:
    // node<n> node = leaf(root);
    // while(root(node++)!=the_root){...}

    template<unsigned n> 
    tree::node<n> 
    first(const tree::node<n>& root,unsigned stage);

    template<unsigned n> 
    tree::node<n> 
    back(const tree::node<n>& root,unsigned stage);

    // ++back == last. The reason I don't define last is because the 
    // for now only a fixed number of stages are available

namespace tree{

    // Implementation
    template<unsigned n>    
    node<n>::node():stage(0),position_in_stage(0){}

    template<unsigned n>    
    node<n>::node(unsigned j,unsigned k)
    :stage(j),position_in_stage(k){
        const char* msg = "node(%1%,!(%2%<%3%)),  ";
        unsigned l = dyna_stage_::position_last(this->stage);
        if(! (k <  l) ){
            format f(msg);
            f % j % k % l;
            throw std::out_of_range(
                f.str()
            );
        }
    }

    template<unsigned n>
    node<n>& 
    node<n>::operator++(){
        unsigned m = dyna_stage_::position_last(this->stage);
        if( 1 + position(*this) < m){
            ++(this->position_in_stage);
        }else{
            ++stage;
            (this->position_in_stage) = 0;
        }
        return (*this);
    }

    template<unsigned n>
    node<n>& 
    node<n>::operator--(){
        const char* msg = "--node<n> at root";
        if(this->stage == 0){
            throw std::out_of_range(
                msg
            );
        }
        unsigned m = dyna_stage_::position_last(this->stage-1);
        if( position(*this)-1 >= m){
            --(this->position_in_stage);
        }else{
            --stage;
            (this->position_in_stage) = m-1;
        }
        return (*this);
    }

    template<unsigned n>
    bool node<n>::operator==(const this_& that){
        return (this->position()) == (that.position());
    }

    template<unsigned n>
    bool node<n>::operator<(const this_& that){
        return (this->position()) < (that.position());
    }

}

    template<unsigned n> 
    std::ostream& operator<<(std::ostream& out, const tree::node<n>& that){
        out 
            << '(' << that.stage 
            << ',' << that.position_in_stage
            << ',' << position(that)
            << ')'; 
        return out;
    }

    template<unsigned n>
    tree::node<n> leaf(const tree::node<n>& that){
        typedef tree::node<n> node_;
        node_ node = that;
        ++node.stage;
        (node.position_in_stage) *= n;
        return node;
    }

    template<unsigned n>
    tree::node<n> 
    root(const tree::node<n>& that){
        typedef tree::node<n> node_;
        const char* msg = "node<n>::jump_to_root() already at root";
        if(that.stage == 0 ){
            throw std::out_of_range(
                msg
            );
        }
        node_ node = that;
        --node.stage;
        node.position_in_stage /= n;
        return node;
    }

    template<unsigned n>
    typename tree::node<n>::size_type
    position(const tree::node<n>& that){
        typedef tree::node<n> node_;
        typedef typename node_::dyna_stage_ dyna_stage_;
        return dyna_stage_::position_first(that.stage)
            +(that.position_in_stage);
    }

    template<unsigned n> tree::node<n> 
    first(const tree::node<n>& root,unsigned stage){
        typedef tree::node<n> node_;
        typedef typename node_::dyna_stage_ dyna_stage_;
        unsigned idx = dyna_stage_::position_first(stage);
        return node_(stage,idx);
    }

    template<unsigned n> tree::node<n> 
    back(const tree::node<n>& root,unsigned stage){
        typedef tree::node<n> node_;
        typedef typename node_::dyna_stage_ dyna_stage_;
        unsigned idx = dyna_stage_::position_last(stage)-1;
        return node_(stage,idx);
    }

}// boost

#endif