/**
	@file
	T100 brain kernel.

	@note
	Copyright (c) 2008 Phil Bouchard <phil@fornux.com>.

	Distributed under the Boost Software License, Version 1.0.

	See accompanying file LICENSE_1_0.txt or copy at
	http://www.boost.org/LICENSE_1_0.txt

	See http://www.boost.org/libs/smart_ptr/doc/index.html for documentation.
*/


#ifndef T100_HPP_INCLUDED
#define T100_HPP_INCLUDED


#include <vector>
#include <string>
#include <iostream>
#include <boost/regex.hpp>
#include <boost/shifted_ptr.hpp>


namespace boost
{

namespace detail
{

namespace sh
{


struct neuron_base
{
    typedef boost::shifted_ptr<neuron_base> pointer;

    enum sense_t {sight, sound, touch, smell, taste};

    boost::regex exp_;
    std::vector<pointer> sub_;

    neuron_base(std::string const & s) : exp_(s), sub_(exp_.mark_count()) {}
    virtual ~neuron_base() {};

    virtual double operator () (std::string const & input) {};
};

template <neuron_base::sense_t>
    class neuron : public neuron_base
    {
        // disable non-"shifted<neuron>" allocations:
        void * operator new (size_t);

    public:
        typedef boost::shifted<neuron> pointee;
        typedef std::map<std::string, pointer> map_sn_t;
        
        static map_sn_t search_;
    
        neuron(std::string const & s, pointee * p1 = 0, pointee * p2 = 0, pointee * p3 = 0) : neuron_base(s)
        {
            search_[s] = reinterpret_cast<pointee *>(this); /// FIXME
        
            if (p1) sub_[0] = p1;
            if (p2) sub_[1] = p2;
            if (p3) sub_[2] = p3;
        }

        double operator () (std::string const & input)
        {
            boost::match_results<std::string::const_iterator> what;

            if (! boost::regex_match(input, what, exp_, boost::match_default | boost::match_partial))
                return 0;
              
            if (! what[0].matched)
                return 0;

            // ponderate
            double accuracy = 0;
            for (int i = 1; i < what.size(); i ++)
                if (what[i].matched)
                    accuracy += (* sub_[i])(what[i].str()) / (what.size() - 1);
            
            // learn if sounds equitable, God tells you to or "energy" spent is still low
            if (accuracy > .7)
                for (int i = 1; i < what.size(); i ++)
                    if (! what[i].matched)
                    {
                        typename map_sn_t::iterator j = search_.find(what[i].str());
                        
                        if (j != search_.end())
                        {
                            /**
                                What we should do here is to:
                                - cummulate suggestions
                                - calculate difference between all proposals
                                - create new regular expression when demand is too high
                            */
                            sub_[i] = j->second;
                        }
                        else
                        {
                            /**
                                Over here we should start guessing
                            */
                        }
                    }
            
            return accuracy;
        }
    };

template <neuron_base::sense_t I>
    typename neuron<I>::map_sn_t neuron<I>::search_;


typedef neuron<neuron_base::sight> neuron_sight;
typedef neuron<neuron_base::sound> neuron_sound;
typedef neuron<neuron_base::touch> neuron_touch;
typedef neuron<neuron_base::smell> neuron_smell;
typedef neuron<neuron_base::taste> neuron_taste;


} // namespace sh

} // namespace detail

} // namespace boost


#endif
