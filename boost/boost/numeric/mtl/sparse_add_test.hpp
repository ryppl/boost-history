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

#ifndef SPARSE_ADD_TEST_HPP
#define SPARSE_ADD_TEST_HPP

#include <iostream>
#include <iterator>
#include <assert.h>

template <class SVectorX, class SVectorY, class SVectorZ, class SVectorW,
          class DVectorX, class DVectorY, class DVectorZ, class DVectorW>
void sparse_add_test(SVectorX& x, SVectorY& y, SVectorZ& z, SVectorW& w,
		     DVectorX& dx, DVectorY& dy, DVectorZ& dz, DVectorW& dw)
{
  typename SVectorX::size_type n = x.size(), i;
  assert(n == y.size() && n == z.size() && n == w.size());  

  z = x + y;
  dz = z;
  for (i = 0; i < n; ++i)
    assert(dz[i] == dx[i] + dy[i]);

#if 0
  // need to add a test for when and freeze_mod
  typename DVectorZ::value_type d = 0, thresh = 0;
  freeze_mod(z, d) += when(w, greater_than(thresh));
  std::cout << d << std::endl;
#endif

  freeze(z) += w;
  for (typename SVectorZ::iterator zi = z.begin();
       zi != z.end(); ++zi) {
    i = index(*zi);
    assert(value(*zi) == dx[i] + dy[i] + dw[i]);
  }

  freeze(z) -= w;
  for (typename SVectorZ::iterator zi = z.begin();
       zi != z.end(); ++zi) {
    i = index(*zi);
    assert(value(*zi) == dx[i] + dy[i]);
  }

  z += w;
  dz = z;
  for (i = 0; i < n; ++i)
    assert(dz[i] == dx[i] + dy[i] + dw[i]);

  z -= w;
  dz = z;
  for (i = 0; i < n; ++i)
    assert(dz[i] == dx[i] + dy[i]);

  z += x + y;
  dz = z;
  for (i = 0; i < n; ++i)
    assert(dz[i] == 2*(dx[i] + dy[i]));

  z -= x + y;
  dz = z;
  for (i = 0; i < n; ++i)
    assert(dz[i] == dx[i] + dy[i]);

  z = x - y;
  dz = z;
  for (i = 0; i < n; ++i)
    assert(dz[i] == dx[i] - dy[i]);

  z = (x + y) + w;
  dz = z;
  for (i = 0; i < n; ++i)
    assert(dz[i] == (dx[i] + dy[i]) + dw[i]);

  z = (x + y) - w;
  dz = z;
  for (i = 0; i < n; ++i)
    assert(dz[i] == (dx[i] + dy[i]) - dw[i]);

  z = w + (x + y);
  dz = z;
  for (i = 0; i < n; ++i)
    assert(dz[i] == dw[i] + (dx[i] + dy[i]));

  z = w - (x + y);
  dz = z;
  for (i = 0; i < n; ++i)
    assert(dz[i] == dw[i] - (dx[i] + dy[i]));

  z = (x + w) + (x + y);
  dz = z;
  for (i = 0; i < n; ++i)
    assert(dz[i] == (dx[i] + dw[i]) + (dx[i] + dy[i]));

  z = (x - w) + (x - y);
  dz = z;
  for (i = 0; i < n; ++i)
    assert(dz[i] == (dx[i] - dw[i]) + (dx[i] - dy[i]));

  z = (x + w) - (x + y);
  dz = z;
  for (i = 0; i < n; ++i)
    assert(dz[i] == (dx[i] + dw[i]) - (dx[i] + dy[i]));

  z = -x;
  dz = z;
  for (i = 0; i < n; ++i)
    assert(dz[i] == -dx[i]);

  z = -(x + y);
  dz = z;
  for (i = 0; i < n; ++i)
    assert(dz[i] == -(dx[i] + dy[i]));
}

#endif // SPARSE_ADD_TEST_HPP
