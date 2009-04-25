// Copyright Christopher Schmidt 2009.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
#pragma once

//Based on these concepts the convenience functions are implemented!

/*
auto concept ForwardIterator<class Type>
{
}

auto concept BidirectionalIterator<class Type>
{
	requires ForwardIterator<Type>;
}

auto concept RandomAccessIterator<class Type>
{
	requires BidirectionalIterator<Type>;
}

auto concept ForwardSequence<class Type>
{
}

auto concept BidirectionalSequence<class Type>
{
	requires ForwardSequence<Type>;
}

auto concept RandomAccessSequence<class Type>
{
	requires BidirectionalSequence<Type>;
}
*/
