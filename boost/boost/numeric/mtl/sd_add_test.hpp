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

#ifndef SD_ADD_TEST_HPP
#define SD_ADD_TEST_HPP

#include <iostream>
#include <iterator>
#include <assert.h>

// sparse-dense combinations

template <class SVectorX, class DVectorY, class DVectorZ, class SVectorW,
          class DVectorX, class DVectorW>
void sd_add_test(SVectorX& x, DVectorY& y, DVectorZ& z, SVectorW& w,
		 DVectorX& dx, DVectorW& dw)
{
  typename SVectorX::size_type n = x.size(), i;
  assert(n == y.size() && n == z.size() && n == w.size());  

  z = x + y;
  for (i = 0; i < n; ++i)
    assert(z[i] == dx[i] + y[i]);

  z = y + x;
  for (i = 0; i < n; ++i)
    assert(z[i] == y[i] + dx[i]);

  z += w;
  for (i = 0; i < n; ++i)
    assert(z[i] == dx[i] + y[i] + dw[i]);

  z -= w;
  for (i = 0; i < n; ++i)
    assert(z[i] == dx[i] + y[i]);

  z += x + y;
  for (i = 0; i < n; ++i)
    assert(z[i] == 2*(dx[i] + y[i]));

  z += y + x;
  for (i = 0; i < n; ++i)
    assert(z[i] == 3*(dx[i] + y[i]));

  z -= y + x;
  for (i = 0; i < n; ++i)
    assert(z[i] == 2*(y[i] + dx[i]));

  z -= x + y;
  for (i = 0; i < n; ++i)
    assert(z[i] == dx[i] + y[i]);

  z = x - y;
  for (i = 0; i < n; ++i)
    assert(z[i] == dx[i] - y[i]);

  z = y - x;
  for (i = 0; i < n; ++i)
    assert(z[i] == y[i] - dx[i]);

  z = (x + y) + w;
  for (i = 0; i < n; ++i)
    assert(z[i] == (dx[i] + y[i]) + dw[i]);

  z = (x + y) - w;
  for (i = 0; i < n; ++i)
    assert(z[i] == (dx[i] + y[i]) - dw[i]);

  z = w + (x + y);
  for (i = 0; i < n; ++i)
    assert(z[i] == dw[i] + (dx[i] + y[i]));

  z = w - (x + y);
  for (i = 0; i < n; ++i)
    assert(z[i] == dw[i] - (dx[i] + y[i]));

  z = (x + w) + (x + y);
  for (i = 0; i < n; ++i)
    assert(z[i] == (dx[i] + dw[i]) + (dx[i] + y[i]));

  z = (x - w) + (x - y);
  for (i = 0; i < n; ++i)
    assert(z[i] == (dx[i] - dw[i]) + (dx[i] - y[i]));

  z = (x + w) - (x + y);
  for (i = 0; i < n; ++i)
    assert(z[i] == (dx[i] + dw[i]) - (dx[i] + y[i]));

  z = -x;
  for (i = 0; i < n; ++i)
    assert(z[i] == -dx[i]);

  z = -(x + y);
  for (i = 0; i < n; ++i)
    assert(z[i] == -(dx[i] + y[i]));
}

#endif // SPARSE_ADD_TEST_HPP
