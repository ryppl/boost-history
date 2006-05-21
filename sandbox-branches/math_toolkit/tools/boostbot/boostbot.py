
# Copyright Redshift Software, Inc. 2005
#
# Distributed under the Boost Software License, Version 1.0. 
# (See accompanying file LICENSE_1_0.txt or copy at 
# http://www.boost.org/LICENSE_1_0.txt)

import os
import os.path
import sys
boostbot_root = os.path.abspath( os.path.dirname( sys.argv[0] ) )
sys.path.insert(0, boostbot_root)
import boost.script
boost.script.run()
