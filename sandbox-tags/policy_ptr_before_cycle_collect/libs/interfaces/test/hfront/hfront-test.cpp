/*
  Copyright(c) 2004, Christopher Diggins
  Available at http://www.heron-language.com
  licensed under the Open Software Licence version 2.0

  C:\dev\hfront>bcc32 hfront-test.cpp -Ic:\borland\bcc55\include -Lc:\borland\bcc55\lib -O3
  bcc32 -IC:\Borland\BCC55\Include\ -LC:\Borland\BCC55\Lib\ heron-test.cpp
*/

#include <iostream>
#include <vector>
#include <list>

#include "shapes_idl.hpp"
#include "timeit.hpp"

using namespace std; 

struct AbcDrawable {
  virtual void Draw() const = 0;
};

struct AbcPosition {
  virtual Point GetPos() const = 0;
};

struct AbcMoveable {
  virtual void MoveTo(const Point& x) = 0;
  virtual void MoveBy(const Point& x) = 0;
};

struct AbcSizeable {
  virtual void SetSize(int x) = 0;
  virtual int GetSize() const = 0;
};

struct AbcShape : public AbcDrawable, public AbcPosition, public AbcMoveable, public AbcSizeable {
};

struct NaiveCircle : public AbcShape {
  NaiveCircle() { mSize = 0; };
  void Draw() const { /* noop */ };
  void MoveTo(const Point& x) { mCenter.MoveTo(x); Draw(); }; 
  void MoveBy(const Point& x) { MoveTo(GetPos().Plus(x)); }; 
  Point GetPos() const { return mCenter; }; 
  void SetSize(int x) { mSize = x; };
  int GetSize() const { return mSize; };
  Point mCenter;
  int mSize;
}; 

template<typename T> void RunTest(T x, const char* s) {
  const int ITERS = 50000000;
  Point pt(0, 0);
  cout << "sizeof(" << s << ") = " << sizeof(T) << endl;
  cout << "timing " << ITERS << " calls to " << s << "::MoveBy() ... ";
  TimeIt t;
  x.MoveTo(pt);
  for (int i=0; i < ITERS; i++)  {
    x.MoveBy(Point(i, i));
  }
}

int main() 
{  
  Circle c1;   
  NaiveCircle c2;
  
  RunTest<IShape>(IShape(c1), "IShape");
  RunTest<AbcShape&>(c2, "AbcShape&");
  RunTest<IMoveable>(IMoveable(c1), "IMoveable");
  RunTest<AbcMoveable&>(c2, "AbcMoveable&");
  RunTest<Circle&>(c1, "Circle&");
  RunTest<NaiveCircle&>(c2, "NaiveCircle&");  
  RunTest<Circle>(c1, "Circle");
  RunTest<NaiveCircle>(c2, "NaiveCircle");  
      
  cin.get();
  return 0;
}
