
# Copyright Aleksey Gurtovoy 2001-2006
#
# Distributed under the Boost Software License, Version 1.0. 
# (See accompanying file LICENSE_1_0.txt or copy at 
# http://www.boost.org/LICENSE_1_0.txt)
#
# See http://www.boost.org/libs/mpl for documentation.

# $Source$
# $Date$
# $Revision$

import preprocess
import os.path

preprocess.main(
      [ "no_ctps", "plain", "typeof_based" ]
    , "vector"
    , os.path.join( "boost", "mpl", "vector", "aux_", "preprocessed" )
    )
