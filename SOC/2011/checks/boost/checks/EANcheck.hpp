// EAN.hpp

// European Article Numbering
// similar to 
// UPC Symbol Specification Manual, Uniform Code Council, Dayton, Ohio, USA.
// M. Blocksma, Reading the Numbers, Penguin, New York 1989.
// Joseph A Gallian, ACM Computing Surveys, 28(3), 504-517 (Sep 1996)
// Section 2, 506
// 13 decimal digit code, like UPC
// uses a two digits that identify the country in which the item was produced.
// C Harmon & R Adams, Reading between the Lines, Helmers publishing NH (1989)
// Stephen Barnett, discrete Mathematics, Numbers and Beyond, Addison-Wesley.
// ISBN 0201342928  EAN 9 78201 342925

#include <string>

bool EANcheck(std::string s);
char EANcompute(std::string s);
