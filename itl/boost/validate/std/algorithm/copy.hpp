/*-----------------------------------------------------------------------------+    
A Law Based Test Automaton 'LaBatea'
Author: Joachim Faulhaber
Copyright (c) 2009-2009: Joachim Faulhaber
+------------------------------------------------------------------------------+
   Distributed under the Boost Software License, Version 1.0.
      (See accompanying file LICENCE.txt or copy at
           http://www.boost.org/LICENSE_1_0.txt)
+-----------------------------------------------------------------------------*/
#ifndef BOOST_VALIDATE_STD_ALGORITHM_COPY_HPP_JOFA_091124
#define BOOST_VALIDATE_STD_ALGORITHM_COPY_HPP_JOFA_091124

#include <algorithm>

namespace boost{namespace itl
{

template<class SourceT, class TargetT, template<class>class InsertIterator>
struct std_copy_forward
{
	static void apply(const SourceT& source, TargetT& target)
	{
		std::copy(source.begin(), source.end(), 
			InsertIterator<TargetT>(target, typename TargetT::iterator(target.end())));
	}

	static void apply_elemental(const SourceT& source, TargetT& target)
	{
		std::copy(source.elements_begin(), source.elements_end(), 
			InsertIterator<TargetT>(target, typename TargetT::iterator(target.end())));
	}

	static std::string struct_abbreviation(){ return "cpy_f"; }
};

template<class SourceT, class TargetT, template<class>class InsertIterator>
struct std_copy_backward
{
	static void apply(const SourceT& source, TargetT& target)
	{
		std::copy(source.rbegin(), source.rend(), 
			InsertIterator<TargetT>(target, typename TargetT::iterator(target.end())));
	}

	static void apply_elemental(const SourceT& source, TargetT& target)
	{
		std::copy(source.elements_rbegin(), source.elements_rend(), 
			InsertIterator<TargetT>(target, typename TargetT::iterator(target.end())));
	}

	static std::string struct_abbreviation(){ return "cpy_b"; }
};

template<class SourceT, class TargetT, template<class>class InsertIterator>
struct std_reverse_copy_forward
{
	static void apply(const SourceT& source, TargetT& target)
	{
		std::reverse_copy(source.begin(), source.end(), 
			InsertIterator<TargetT>(target, typename TargetT::iterator(target.end())));
	}

	static void apply_elemental(const SourceT& source, TargetT& target)
	{
		std::reverse_copy(source.elements_begin(), source.elements_end(), 
			InsertIterator<TargetT>(target, typename TargetT::iterator(target.end())));
	}

	static std::string struct_abbreviation(){ return "rcpy_f"; }
};

template<class SourceT, class TargetT, template<class>class InsertIterator>
struct std_reverse_copy_backward
{
	static void apply(const SourceT& source, TargetT& target)
	{
		std::reverse_copy(source.rbegin(), source.rend(), 
			InsertIterator<TargetT>(target, typename TargetT::iterator(target.end())));
	}

	static void apply_elemental(const SourceT& source, TargetT& target)
	{
		std::reverse_copy(source.elements_rbegin(), source.elements_rend(), 
			InsertIterator<TargetT>(target, typename TargetT::iterator(target.end())));
	}

	static std::string struct_abbreviation(){ return "rcpy_b"; }
};


}} // namespace itl boost

#endif BOOST_VALIDATE_STD_ALGORITHM_COPY_HPP_JOFA_091124

