///////////////////////////////////////////////////////////////////////////////
// tree_view::doc::readme.hpp                                                //
//                                                                           //
//  Copyright 2009 Erwann Rogard. Distributed under the Boost                //
//  Software License, Version 1.0. (See accompanying file                    //
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)         //
///////////////////////////////////////////////////////////////////////////////

[ Contact ]

erwann.rogard@gmail.com

[ Overview ]

    Maps a vector to a tree view, where each node has a fixed number of
    branches. 
    
[ Useful links ]

    A comprehensive tree framework that is unrelated to this package:
        http://raz.or.at/soc2006/doc/html/

[ Compiler ]

gcc version i686-apple-darwin9-gcc-4.0.1 (GCC) 4.0.1

[ Dependencies ]

/usr/local/boost_1_39_0/ 
/switch 

[ Links ]
Here's the current link to switch:
http://www.boostpro.com/vault/index.php?action=downloadfile&filename=switch.zip&directory=&


[ History ]

Sep 01, 2009    : Addressed some bugs and added dynamic_stage.hpp 
July 2009       : Current version

[ Output ]

main.cpp

-> example_tree 
{[i(j),i(j+1)) : j=0,...,3}:(0,1)(1,4)(4,13)(13,40)
(stage,k,i)
(0,0,0)
(1,0,1)(1,1,2)(1,2,3)
(2,0,4)(2,1,5)(2,2,6)(2,3,7)(2,4,8)(2,5,9)(2,6,10)(2,7,11)(2,8,12)
(3,0,13)(3,1,14)(3,2,15)(3,3,16)(3,4,17)(3,5,18)(3,6,19)(3,7,20)(3,8,21)(3,9,22)(3,10,23)(3,11,24)(3,12,25)(3,13,26)(3,14,27)(3,15,28)(3,16,29)(3,17,30)(3,18,31)(3,19,32)(3,20,33)(3,21,34)(3,22,35)(3,23,36)(3,24,37)(3,25,38)(3,26,39)
(stage,k,i)
(3,26,39)(3,25,38)(3,24,37)(3,23,36)(3,22,35)(3,21,34)(3,20,33)(3,19,32)(3,18,31)(3,17,30)(3,16,29)(3,15,28)(3,14,27)(3,13,26)(3,12,25)(3,11,24)(3,10,23)(3,9,22)(3,8,21)(3,7,20)(3,6,19)(3,5,18)(3,4,17)(3,3,16)(3,2,15)(3,1,14)(3,0,13)
(2,8,12)(2,7,11)(2,6,10)(2,5,9)(2,4,8)(2,3,7)(2,2,6)(2,1,5)(2,0,4)
(1,2,3)(1,1,2)(1,0,1)
(0,0,0) <- 

