// (C) Copyright Jonathan Turkanis 2004.
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt.)

// Disclaimer: Not a Boost library.

// Based on "shapes.hfront" from Listing 2 of "C++ with Interfaces", by 
// Christopher Diggins, CUJ Sept. 2004, p.37.

#ifndef BOOST_INTERFACES_TEST_HFRONT_SHAPES_IDL_HPP_INCLUDED
#define BOOST_INTERFACES_TEST_HFRONT_SHAPES_IDL_HPP_INCLUDED

#include <boost/interfaces.hpp>
#include "./concrete_types.hpp"

#define BOOST_IDL_LEAN_AND_MEAN

//interface IDrawable {
//  contract:
//    void Draw() const; 
//};
BOOST_IDL_BEGIN(IDrawable)
    BOOST_IDL_CONST_FN0(Draw, void)
BOOST_IDL_END(IDrawable)

//interface IPosition {
//  contract:
//    Point GetPos() const;
//};
BOOST_IDL_BEGIN(IPosition)
    BOOST_IDL_CONST_FN0(GetPos, Point)
BOOST_IDL_END(IPosition)

//interface IMoveable {
//  contract:
//    void MoveTo(const Point& x);
//    void MoveBy(const Point& x); 
//};
BOOST_IDL_BEGIN(IMoveable)
    BOOST_IDL_FN1(MoveTo, void, const Point&)
    BOOST_IDL_FN1(MoveBy, void, const Point&)
BOOST_IDL_END(IMoveable)

//interface ISizeable {
//  contract:
//    void SetSize(int x); 
//    int GetSize() const; 
//}; 
BOOST_IDL_BEGIN(ISizeable)
    BOOST_IDL_FN1(SetSize, void, int)
    BOOST_IDL_CONST_FN0(GetSize, int)
BOOST_IDL_END(ISizeable)

//interface IShape : IDrawable, IMoveable, ISizeable {  
//};
BOOST_IDL_BEGIN(IShape)
    BOOST_IDL_EXTENDS(IDrawable)
    BOOST_IDL_EXTENDS(IPosition)
    BOOST_IDL_EXTENDS(IMoveable)
    BOOST_IDL_EXTENDS(ISizeable)
BOOST_IDL_END(IShape)

#endif // #ifndef BOOST_INTERFACES_TEST_HFRONT_SHAPES_IDL_HPP_INCLUDED
