//! \file UPCcheck.hpp

// Copyright Paul A. Bristow 2011.

// Use, modification and distribution are subject to the
// Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt
// or copy at http://www.boost.org/LICENSE_1_0.txt)

// UPCcheck.hpp

// Universal Product Code UPC check
// UPC Symbol Specification Manual, Uniform Code Council, Dayton, Ohio, USA.
// M. Blocksma, Reading the Numbers, Penguin, New York 1989.
// Joseph A Gallian, ACM Computing Surveys, 28(3), 504-517 (Sep 1996)
// Section 2

#include <string>

bool UPCcheck(std::string s);
char UPCcompute(std::string s);
