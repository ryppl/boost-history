// Copyright Christopher Schmidt 2009.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
#pragma once

//Based on these concepts the convenience functions are implemented!

/*auto concept MetaFunction<class Type>
{
	typename type;
}

auto concept ForwardIterator<class Type>
{
	MetaFunction get_next;
}

auto concept BidirectionalIterator<class Type>
{
	requires ForwardIterator<Type>;

	MetaFunction get_prior;
}

auto concept RandomAccessIterator<class Type>
{
	MetaFunction advance;
	MetaFunction get_distance;
}

auto concept FusionSequence<class Type>
{
	class get_begin;
	//MetaFunction get_begin;
	//requires MetaFunction<get_next>;
	MetaFunction get_end;
}

auto concept ForwardSequence<class Type>
{
	requires FusionSequence<Type>;

	MetaFunction get_size;
}

auto concept BidirectionalSequence<class Type>
{
	requires ForwardSequence<Type>;
}

auto concept RandomAccessSequence<class Type>
{
	requires FusionSequence<Type>;
}*/
