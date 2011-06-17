// ISSN.hpp

// The Standard Book Numbering Agency Ltd, London.
// Philip M. Tuchinsky, International Standard Book Numbers, UMAP Journal 5(1985) 41-45, ISSN 0197-3622
// ISO Standard, International Standard Book Number 2108:1992.
// http://www.issn.org:8080/English/pub/faqs/barcodes
// EAN-13 codes start with 977, "land of serial publications"
// for example ISSN 1144-875X GENCOD informations(Paris)
// EAN-13 bar code is 9 771144 875007
// ISSN Check digit, in this case X is removed from the ISSN,
// 00 is price code (usually 00), 7 is EAN-13 check digit
// and finally issue number 03 for March, for example.

// Defines:
bool ISSNcheck(string s);
char ISSNcompute(string s);

#include <iostream>
#include <string>
#include <vector>
#include <cctype> // for isdigit and tolower

using std::string;
using std::vector;

using std::cout; // test only.
using std::endl;

bool ISSNcheck(string s)
{ // Check an existing ISSN string s
	// which includes a check digit 0 to 9 or X (=11)
	vector<int> v(8);
	for (int i = 0; i < 8; i++)
	{
		if (isdigit(s[i]))
		{
			v[i] = s[i] - '0';
		}
		else if (toupper(s[i] == 'X'))
		{
			v[i] = 10;
		}
		else
		{ // give up! (assumes no whitespace or punctuation).
			return false;
		}
		// cout << v[i] << ' ';
	}
	int check =  8 * v[0] + 7 * v[1] + 6 * v[2] + 5 * v[3] + 4 * v[4]
	  + 3 * v[5] + 2 * v[6] + 1 * v[7];
	//cout << ' ' << check << endl;
	check %= 11; // modulus 11
	//cout << ' ' << check << endl;
	return (check == 0); // true if check digit is correct.
} // bool ISSNcheck(string s)

char ISSNcompute(string s)
{ // Compute check digit 0 to 9 or X (=11) for the string provided.
	if (s.size() != 7)
	{
		return '?' ;  // Must be exactly 7 digits (to be a 8 digit ISSN)
	}
	// Might allow other sizes if remove whitespace and punctutation?
	vector<int> v(7);
	for (int i = 0; i < 7; i++)
	{
		if (isdigit(s[i]))
		{
			v[i] = s[i] - '0';
		}
		else if (toupper(s[i] == 'X'))
		{
			v[i] = 10;
		}
		else
		{ // throw?
			return '?';
		}
		// cout << v[i] << ' ';
	}
	int check  = 8 * v[0] + 7 * v[1] + 6 * v[2] + 5 * v[3] + 4 * v[4] + 3 * v[5] + 2 * v[6];

	check %= 11; // modulus 11
	if (check == 0) return '0';
	check = 11 - check;
	return (check == 10) ? 'X' : char(check + '0');
	// return 3 * v[0] + 4 * v[1] + 5 * v[2] + 6 * v[3] + 7 * v[4] + 8 * v[5] + 9 * v[6];
	// L. Egghe  & R. Rousseau, Mathmetical and Computer Modelling,	
	// 33 (2001), 943-955 ISSN 0895-7177  page 944,
	// equation 5 - simpler version of equation 4, the ISO standard.
} // char ISSNcomputer(string s)



