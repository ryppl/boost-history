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

#ifndef SCALE_TEST_HPP
#define SCALE_TEST_HPP

#include <assert.h>

template <class VectorX, class VectorY, class VectorZ, class VectorW, class T>
void scale_test(VectorX& x, VectorY& y, VectorZ& z, VectorW& w, const T& alpha)
{
  typename VectorX::size_type n = x.size(), i;
  assert(n == y.size() && n == z.size() && n == w.size());

  x = alpha * y;
  for (i = 0; i < n; ++i)
    assert(x[i] == alpha * y[i]);

  x = alpha * (y + w);
  for (i = 0; i < n; ++i)
    assert(x[i] == alpha * (y[i] + w[i]));

  x = y * alpha;
  for (i = 0; i < n; ++i)
    assert(x[i] == y[i] * alpha);

  x = (y + w) * alpha;
  for (i = 0; i < n; ++i)
    assert(x[i] == (y[i] + w[i]) * alpha);
}

#endif // SCALE_TEST_HPP
