// IBM check used by MAstercard, VISA, and most other credit card companies.

// Is an even/odd weighted code.

// Digits in the even positions (numbering from the right) are multiplied by 2,
// then reduced to a single digit (if > 9) by "casting out nines"
// (subtracting 9, which is equivalent to adding the digits).
// All digits are them summed
// and a check digit added to make the result evenly divisble by 10.

// EAN for books is based on the ISBN and begins with the digits 978,
// and the next nine digits being the first nine digits of the ISBN,
// finally an EAN-13 check decimal digit.

#include <string>
#include <vector>
#include <cctype> // isdigit, isspace, tolower

using std::string;
using std::vector;

using std::cout;
using std::endl;

bool IBMcheck(string s)
{ // Check an existing IBM check string s
	// which includes a check digit 0 to 9.
	vector<int> v(16);
	for (int i = 0; i < 16; i++)
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
		// cout << v[i] << ' ';
	}
	int check = 0;
	for (int i = 0; i < 16; i++)
	{
		if ((i % 2) == 0)
		{ // even
			v[i] *= 2;
			if (v[i] > 9)
			{
				v[i] -= 9; // "casting out nines"
			}
		}
		// else unchanged if odd.
		check += v[i]; //
	}
	//cout << '\n' << check << endl;
	check %= 10; // modulus 10
	// cout << ' ' << check << endl;
	return (check == 0); // true if check digit is correct.
} // bool IBMcheck(string s)

char IBMcompute(string s)
{ // Compute check digit 0 to 9 for the string provided.
	// Might allow other size if remove whitespace and punctutation?
	vector<int> v(15); // To hold numeric values from character values in string s.
	for (int i = 0; i < 15; i++)
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
	for (int i = 0; i < 15; i++)
	{
		if ((i % 2) == 0)
		{ // even
			v[i] *= 2;
			if (v[i] > 9)
			{
				v[i] -= 9; // "casting out nines"
			}
		}
		// else unchanged if odd.
		check += v[i]; //
	}
	// cout << '\n' << check << endl;

	check %= 10; // modulus 10
	// cout << ' ' << check << endl;
	if (check == 0) return '0';
	check = 10 - check;
	return char(check + '0');
} // char IBMcompute(string s)

