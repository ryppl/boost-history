/*-----------------------------------------------------------------------------+    
Copyright (c) 2007-2009: Joachim Faulhaber
Copyright (c) 1999-2006: Cortex Software GmbH, Kantstrasse 57, Berlin
+------------------------------------------------------------------------------+
   Distributed under the Boost Software License, Version 1.0.
      (See accompanying file LICENCE.txt or copy at
           http://www.boost.org/LICENSE_1_0.txt)
+-----------------------------------------------------------------------------*/
#ifndef __product_history_h_JOFA_011005_H__
#define __product_history_h_JOFA_011005_H__


#include <boost/itl/interval_map.hpp>

namespace boost{namespace itl
{

/// Adds episodes of different types into a single history of episode_products
/**    
    A class to merge episodes of different types into a common history object.

    Template parameter ItvDomTV contains the domain_type of the history. This
    will be an integer or a time type in most cases.

    Template parameter TypeDomTV denotes the kinds of episodes that can be
    used for the given product_history. TypeDomTV encapsulated an enumeration
    that denotes the different kinds of episodes.

    Every element of a product history is an interval value pairs whose
    associated value is a tuple of product of episodes, which is called
    episode_product. The intervals are giving the time periods where all
    episodes are contstant.

    On insertion of a new episode, intervals are split at those points in
    time where the new epiosode causes inequalities in the associated
    episodes.
    
    @author  Joachim Faulhaber
*/
template <class ItvDomTV, class TypeDomTV>
class product_history : public interval_map<ItvDomTV, episode_product<ItvDomTV,TypeDomTV> >
{
public:
    typedef                episode_product<ItvDomTV,TypeDomTV> EpisodeProductTD;
    typedef                interval_map<ItvDomTV, EpisodeProductTD> BaseTypeTD;
    typedef    typename    BaseTypeTD::value_type value_type;
    typedef    typename    BaseTypeTD::interval_type IntervalTD;
    typedef    typename    BaseTypeTD::domain_type DomainTD;
    
    typedef    typename    BaseTypeTD::data_type data_type;
    typedef    typename    EpisodeProductTD::EpisodePTD EpisodePTD;
    
public:

    void add(EpisodePTD epi)
    {
        EpisodeProductTD epiRec;
        epiRec.insert(epi);
        BaseTypeTD::add(value_type(epi->interval(), epiRec));
    }

    product_history& operator +=(EpisodePTD epi)
    {
        add(epi);
        return *this;
    }


protected:
    
    void add(const value_type& val)
    {
        BaseTypeTD::add(val);
    }
};

}} // namespace itl boost

#endif // __product_history_h_JOFA_011005_H__


