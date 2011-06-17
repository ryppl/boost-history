// IBM check used by Mastercard, VISA, and most other credit card companies.
// Version using Joseph A. Gallian permutations, rather than 'casting out nines'
// See Error Detection Methods, Joseph A. Gallian,
// ACM computing Surveys, 28(3) 504-517 (Sep 1996)
// ISSN 0360-0300

// Is an even/odd weighted code.

// Digits in the even positions (numbering from the right) are multiplied by 2,
// then reduced to a single digit (if > 9) by "casting out nines"
// (subtracting 9, which is equivalent to adding the digits).
// All digits are them summed
// and a check digit added to make the result evenly divisble by 10.

#include <string>
#include <vector>
#include <cctype> // isdigit, isspace, tolower

using std::string;
using std::vector;

using std::cout;
using std::endl;

int perm[10] = 
{ // J A Gallian & S Winters, (1988), Amer. Math. Monthly 95, 548-551.
	// A. Ecker & G. Poch, Check Characeter system, Computing, 37, 277-301 (1986)
	// if digit == 0, add 0, if digit is 1, add 2 to check.
	0, // s(0) = 0 page 506 section 3, Credit card scheme.
	2, // s(1) = 2 
	4, // s(2) = 4
	6, // s(3) = 6
	8, // s(4) = 8
	1, // s(5) = 1
	3, // s(6) = 3
	5, // s(7) = 5
	7, // s(8) = 7
	9, // s(9) = 9
};

bool VISAcheck(string s)
{ // Check an existing IBM check string s
	// which includes a check digit 0 to 9.
	// const int n = 8; // n-1 digits, plus 1 nth check digit.
		const int n = int(s.size()); // including check digit, so do all n digits.
	// cout << "Check n = " << n << endl;
	// 16 for credit card numbers.
	vector<int> v(n);
	for (int i = 0; i < n; i++)
	{ // Convert decimal digit characters in string to integers in vector.
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
		//cout << v[i] << ' ';
	}
	//cout << endl;
	int check = 0;
	for (int i = 0; i < n; i++)
	{ // Assumes n (total including check digit) is even.
		// Permute the even numbered digits instead if n is odd.
		if ((i % 2) == 0)
		{ // i is even, odd numbered digits (counting 1, 2, 3 ... n) are permuted.
			// cout << v[i] << ' ' << perm[v[i]] << endl;
			check += perm[v[i]];
		}
		else
		{ // even numbered digits (counting 1, 2, 3 ... n) are unchanged.
			// cout << v[i] << endl;
			check += v[i]; //
		}
	}
	// cout << '\n' << check << endl;
	check %= 10; // modulus 10
	// cout << ' ' << check << endl;
	return (check == 0); // true if check digit is correct.
} // bool IBMcheck(string s)

char VISAcompute(string s)
{ // Compute check digit 0 to 9 for the string provided.
	// Might allow other size if remove whitespace and punctutation?
//	const int n = 8; // including check digit, so only do n-1 digits.
	const int n = int(s.size() +1); // including check digit, so only do n-1 digits.
	// cout << "compute n = " << n << endl;

	vector<int> v(n-1); // To hold numeric values from character values in string s.
	for (int i = 0; i < n-1; i++)
	{
		if (isdigit(s[i]))
		{
			v[i] = s[i] - '0';
		}
		else
		{ // throw?
			//return '?';
			// ignore
		}
		// cout << v[i] << ' ';
	}
	int check = 0;
	for (int i = 0; i < n-1; i++)
	{ // Assumes n (total including check digit) is even.
		// Permute the even numbered digits instead if n is odd.
		if ((i % 2) == 0)
		{ // i is even, odd numbered digits (counting 1, 2, 3 ... n) are permuted.
			// cout << v[i] << ' ' << perm[v[i]] << endl;
			check += perm[v[i]]; // permutated.
		}
		else
		{ // even numbered digits (counting 1, 2, 3 ... n) are unchanged.
			// cout << v[i] << endl;
			check += v[i]; // unchanged.
		}
	}
	// cout << "\nComputed check " << check << endl;
	// cout << '\n' << check << endl;
	check %= 10; // modulus 10
	// cout << ' ' << check << endl;
	if (check == 0) return '0';
	check = 10 - check;
	return char(check + '0');
} // char IBMcompute(string s)

/*


Test j:\cpp\isbn\testisbn.cpp Thu Nov 28 09:46:29 2002
VISAcheck("76592146")
Check n = 8
7 6 5 9 2 1 4 6
7 5
6
5 1
9
2 4
1
4 8
6

40
 0
true
VISAcompute("7659214")
compute n = 8
7 5
6
5 1
9
2 4
1
4 8

Computed check 34
VISAcompute("7659214") 6
Check n = 16
5 8 1 8 0 0 6 1 9 1 1 4 0 0 2 7
5 1
8
1 2
8
0 0
0
6 3
1
9 9
1
1 2
4
0 0
0
2 4
7

50
 0
VISAcheck("5818006191140027") true
compute n = 16
5 1
8
1 2
8
0 0
0
6 3
1
9 9
1
1 2
4
0 0
0
2 4

Computed check 43
VISAcompute("581800619114002") 7
Check n = 16
4 4 1 7 1 2 3 4 5 6 7 8 9 1 1 2
4 8
4
1 2
7
1 2
2
3 6
4
5 1
6
7 5
8
9 9
1
1 2
2

69
 9
VISAcheck("4417123456789112") false
compute n = 16
4 8
4
1 2
7
1 2
2
3 6
4
5 1
6
7 5
8
9 9
1
1 2

Computed check 67
VISAcompute("441712345678911") 3
Check n = 16
4 4 1 7 1 2 3 4 5 6 7 8 9 1 1 3
4 8
4
1 2
7
1 2
2
3 6
4
5 1
6
7 5
8
9 9
1
1 2
3

70
 0
VISAcheck("4417123456789113") true
Press any key to continue

*/