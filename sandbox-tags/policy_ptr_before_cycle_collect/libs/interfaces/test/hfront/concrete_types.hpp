/*
  Copyright(c) 2004, Christopher Diggins
  Available at http://www.heron-language.com
  licensed under the Open Software Licence version 2.0

  C:\dev\hfront>bcc32 hfront-test.cpp -Ic:\borland\bcc55\include -Lc:\borland\bcc55\lib -O3
  bcc32 -IC:\Borland\BCC55\Include\ -LC:\Borland\BCC55\Lib\ heron-test.cpp
*/

#ifndef BOOST_INTERFACES_TEST_HFRONT_CONCERTE_TYPES_HPP_INCLUDED
#define BOOST_INTERFACES_TEST_HFRONT_CONCERTE_TYPES_HPP_INCLUDED

#include <iostream>

struct Point {
  int mx, my; 
  Point() { mx = 0; my = 0; }; 
  Point(int x, int y) { mx = x; my = y; };
  Point Plus(const Point& x) { return Point(x.mx + mx, x.my + my); }; 
  void Draw() const { std::cout << "x: " << mx << " y: " << my; };
  void MoveTo(const Point& x) { mx = x.mx; my = x.my;  };
};

struct Circle { 
  Circle() { mSize = 0; };
  void Draw() const { /* noop */ };
  void MoveTo(const Point& x) { mCenter.MoveTo(x); Draw(); }; 
  void MoveBy(const Point& x) { MoveTo(GetPos().Plus(x)); }; 
  Point GetPos() const { return mCenter; };
  void SetSize(int x) { mSize = x; };
  int GetSize() const { return mSize; };
  Point mCenter;
  int mSize;
};

#endif // #ifndef BOOST_INTERFACES_TEST_HFRONT_CONCERTE_TYPES_HPP_INCLUDED
