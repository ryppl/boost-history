# Copyright 2005 The Trustees of Indiana University.

# Use, modification and distribution is subject to the Boost Software
# License, Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
# http://www.boost.org/LICENSE_1_0.txt)

#  Authors: Douglas Gregor
#           Andrew Lumsdaine

import bgl

g = bgl.Graph("biconnected_components.dot", bgl.file_kind.graphviz)
bgl.circle_graph_layout(g, radius=200)
g.write_graphviz("circle_graph_layout_out.dot")
