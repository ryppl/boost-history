//=======================================================================
// Copyright 2001 University of Notre Dame.
// Authors: Jeremy G. Siek, Andrew Lumsdaine
//
// This file is part of the Matrix Template Library
//
// You should have received a copy of the License Agreement for the
// Matrix Template Library along with the software; see the file LICENSE.
// If not, contact Office of Research, University of Notre Dame, Notre
// Dame, IN 46556.
//
// Permission to modify the code and to distribute modified code is
// granted, provided the text of this NOTICE is retained, a notice that
// the code was modified is included with the above COPYRIGHT NOTICE and
// with the COPYRIGHT NOTICE in the LICENSE file, and that the LICENSE
// file is distributed with the modified code.
//
// LICENSOR MAKES NO REPRESENTATIONS OR WARRANTIES, EXPRESS OR IMPLIED.
// By way of example, but not limitation, Licensor MAKES NO
// REPRESENTATIONS OR WARRANTIES OF MERCHANTABILITY OR FITNESS FOR ANY
// PARTICULAR PURPOSE OR THAT THE USE OF THE LICENSED SOFTWARE COMPONENTS
// OR DOCUMENTATION WILL NOT INFRINGE ANY PATENTS, COPYRIGHTS, TRADEMARKS
// OR OTHER RIGHTS.
//=======================================================================

#ifndef MTL_TAGS_HPP
#define MTL_TAGS_HPP

#include <mtl/pair.hpp>

// Orientation
struct row { };
struct column { };
struct diagonal { };

template <class Orientation>
struct trans_orien { };

template <>
struct trans_orien<row> { typedef column type; };

template <>
struct trans_orien<column> { typedef row type; };

// dims is (major,minor) or (2D_size, 1D_size)
template <class I>
inline I nrows(mtl::pair<I,I>& dims, row) { return dims.first; }

template <class I>
inline I nrows(mtl::pair<I,I>& dims, column) { return dims.second; }

template <class I>
inline I ncols(mtl::pair<I,I>& dims, row) { return dims.second; }

template <class I>
inline I ncols(mtl::pair<I,I>& dims, column) { return dims.first; }


// Sparsity
struct dense { };
struct sparse { };
struct static_sparse { }; // sparse, and the index structure is immutable

template <class LS, class RS>
struct or_sparsity { };

template <>
struct or_sparsity<dense,dense> { typedef dense type; };

template <>
struct or_sparsity<dense,sparse> { typedef dense type; };

template <>
struct or_sparsity<sparse,dense> { typedef dense type; };

template <>
struct or_sparsity<sparse,sparse> { typedef sparse type; };


#endif // MTL_TAGS_HPP
