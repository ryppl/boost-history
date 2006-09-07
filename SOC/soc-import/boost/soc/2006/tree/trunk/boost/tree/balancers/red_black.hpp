//  Copyright (c) 2006, Bernhard Reiter
//
//  This code may be used under either of the following two licences:
//
//  Permission is hereby granted, free of charge, to any person obtaining a copy 
//  of this software and associated documentation files (the "Software"), to deal 
//  in the Software without restriction, including without limitation the rights 
//  to use, copy, modify, merge, publish, distribute, sublicense, and/or sell 
//  copies of the Software, and to permit persons to whom the Software is 
//  furnished to do so, subject to the following conditions:
//
//  The above copyright notice and this permission notice shall be included in 
//  all copies or substantial portions of the Software.
//
//  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR 
//  IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, 
//  FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL 
//  THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER 
//  LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, 
//  OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN 
//  THE SOFTWARE. OF SUCH DAMAGE.
//
//  Or:
//
//  Distributed under the Boost Software License, Version 1.0.
//  (See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)


#ifndef BOOST_TREE_BALANCERS_RED_BLACK_HPP
#define BOOST_TREE_BALANCERS_RED_BLACK_HPP

//TODO: lots.
//templatize with bool add_data?

namespace boost {
namespace tree {

enum red_black { black, red };

class red_black_metadata {
public:	
	red_black_metadata()
	: m_color(black) {}
	
	red_black get_color()
	{
		return m_color;
	}
	
	void set_color(red_black col)
	{
		m_color = col;
	}
private:
	red_black m_color;

};

//make the following part of tree? that is, derive tree from this (as templ. arg)?
//template <class Node> //tentative approach using CRTP.
class red_black_balance {
public:
	typedef red_black_metadata metadata_type;
//	metadata_type metadata;
	
	//red_black_balance() : red_black_metadata {}
	
//	typedef Tree tree_type;
//	typedef typename tree_type::cursor cursor;
	
	//make this x's member, so we have a node and a cursor part. (??)
	
	/* well, we don't want to expose add. (Red-black info can be best be exposed 
	 * protected, just like rank? -- as a cursor-protected member probably, 
	 * that is.)
	 */
	
	
	
	template <class Cursor>
	static void add(Cursor x, Cursor header) 
	{
		x.metadata().set_color(red);
		while (x.parent().metadata().get_color() == red) {
			Cursor y = x.parent();
			y = (y == y.parent().begin() ? y.parent().end() : y.parent().begin());
			if (y.metadata().get_color() == red) {
				x.parent().metadata().set_color(black);
				x.parent().parent().metadata().set_color(red);
				y.metadata().set_color(black);
				x = x.parent().parent();
			} else {
				if (x.parity() != x.parent().parity()) {
					x.rotate();
					x = (x.parity() ? x.end() : x.begin());
				}
			x.parent().metadata().set_color(black);
			x.parent().parent().metadata().set_color(red);
			x.parent().rotate();
			}
		if (x.parent() == header)
			x.metadata().set_color(black);
		}
	}
	  
	template <class Cursor>
	static void remove(Cursor) { }
	
	template <class Cursor>
	static void read(Cursor) { }
  
};

} // namespace tree
} // namespace boost


#endif // BOOST_TREE_BALANCERS_RED_BLACK_HPP


