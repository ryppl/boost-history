// Copyright Stjepan Rajko 2007. Use, modification and
// distribution is subject to the Boost Software License, Version
// 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#include <boost/dataflow/signals/component/filter_base.hpp>

#include <boost/test/included/test_exec_monitor.hpp>

using namespace boost;

template<typename Signals, typename InSignatures>
class my_filter
    : public signals::filter_base<
        my_filter<Signals, InSignatures>,
        Signals,
        InSignatures >
{    
public:
	my_filter(const my_filter &) {}
	my_filter(){}
    const my_filter &operator = (const my_filter &) {return *this;}

	///	Returns the default out signal.
  	Signals &signals() const
	{	return m_signals; }

protected:
	mutable Signals m_signals;
}; // class filter

template<typename InSignatures>
class my_filter<void, InSignatures>
    : public signals::filter_base<
        my_filter<void, InSignatures>,
        void,
        InSignatures >
{    
public:
	my_filter(const my_filter &) {}
	my_filter(){}
    const my_filter &operator = (const my_filter &) {return *this;}
}; // class filter


int test_main(int, char* [])
{    
    namespace df=boost::dataflow;

    my_filter<boost::signal<void()>, mpl::vector<void()> >
        void_filter;
        
    my_filter<void, mpl::vector<void()> >
        input_only_filter;

//    fusion::vector<boost::signal<void()> > test;
    
    return 0;
} // int test_main(int, char* [])
