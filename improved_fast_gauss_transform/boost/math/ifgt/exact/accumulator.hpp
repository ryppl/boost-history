//////////////////////////////////////////////////////////////////////////////
// accumulator.hpp
//  (C) Copyright 2009 Erwann Rogard
//  Use, modification and distribution are subject to the
//  Boost Software License, Version 1.0. (See accompanying file
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
#ifndef BOOST_MATH_IFGT_EXACT_ACCUMULATOR_HPP_ER_2009
#define BOOST_MATH_IFGT_EXACT_ACCUMULATOR_HPP_ER_2009
#include <vector>
#include <algorithm>
#include <functional>
#include <iterator>
#include <vector>
#include <cmath>
#include <boost/range.hpp>
#include <boost/static_assert.hpp>
#include <boost/bind.hpp>
#include <boost/iterator/zip_iterator.hpp>
#include <boost/iterator/iterator_traits.hpp>
#include <boost/tuple/tuple.hpp>
#include <boost/math/ifgt/keyword.hpp>
#include <boost/math/ifgt/crtp/accumulator.hpp>
#include <boost/math/ifgt/exact/detail/contribution.hpp>
namespace boost{
namespace math{
namespace ifgt{
namespace exact{

    template<
        unsigned SourceSz,
        unsigned WeightSize,
        typename Cont = std::vector<double> >
    class accumulator
    : public crtp::accumulator<
        accumulator<SourceSz,WeightSize,Cont>
    >{
        public:
        typedef exact::contribution<SourceSz,WeightSize,Cont>
                                                        contribution_type;
        typedef std::vector<contribution_type>          contributions_type;
        typedef typename contribution_type::value_type      value_type;
        typedef typename contributions_type::size_type      size_type;
        static const std::size_t source_size = SourceSz;
        static const std::size_t weight_size = WeightSize;
        typedef size_type                           sources_count_type;
        typedef const contributions_type&           cref_contributions_type;

        accumulator(value_type bandwidth);
        template<typename ArgPack> accumulator(const ArgPack& arg);
        accumulator(const accumulator&);
        accumulator& operator=(const accumulator&);

        // Access
        value_type active_bandwidth()const; //>models Accumulator
        cref_contributions_type contributions()const;
        sources_count_type sources_count()const;

        // Update
        void set(value_type bandwidth);

        // Primarily intended for use internally
        template<typename R0,typename R1>
        void push_back_contribution(
            const R0& source, const R1& weight,value_type bandwidth);

        /// models Accumulator
        template<typename R0,typename R1>
        void operator()(const R0& source, const R1& weight);

        private:
        accumulator();//dont implem
        static size_type default_contributions_reserve_size(){
            static size_type sz = 1e4; return sz; }
        value_type              bandwidth_;
        contributions_type      contributions_;
    };
    template<unsigned SourceSz, unsigned WeightSize, typename Cont>
    accumulator<SourceSz,WeightSize, Cont>::accumulator(
        value_type bandwidth):bandwidth_(bandwidth),contributions_(){
            contributions_.reserve(default_contributions_reserve_size());
        }

    //TODO replace unsigned SourceSz by std::size_t SourceSz
    template<unsigned SourceSz, unsigned WeightSize, typename Cont>
    template<typename ArgPack>
    accumulator<SourceSz,WeightSize, Cont>::accumulator(
        const ArgPack& arg):bandwidth_(arg[kwd<>::bandwidth]),
        contributions_(){
        contributions_.reserve(arg[kwd<>::contributions_reserve_size|
        default_contributions_reserve_size()]);
    }

    template<unsigned SourceSz, unsigned WeightSize, typename Cont>
    accumulator<SourceSz,WeightSize, Cont>::accumulator(
        const accumulator& that):bandwidth_(that.bandwidth_),
        contributions_(that.contributions_){}

    template<unsigned SourceSz, unsigned WeightSize, typename Cont>
    typename accumulator<SourceSz,WeightSize, Cont>::accumulator&
    accumulator<SourceSz,WeightSize, Cont>::operator=(
        const accumulator& that){
            if(&that!=this){
                bandwidth_ = that.bandwidth_;
                contributions_ = that.contributions_; }
             return *this; }

    template<unsigned SourceSz, unsigned WeightSize, typename Cont>
    typename accumulator<SourceSz,WeightSize, Cont>::value_type
    accumulator<SourceSz,WeightSize, Cont>::active_bandwidth()const{
        return bandwidth_; };

    template<unsigned SourceSz, unsigned WeightSize, typename Cont>
    typename
    accumulator<SourceSz,WeightSize, Cont>::cref_contributions_type
    accumulator<SourceSz,WeightSize, Cont>::contributions()const{
        return contributions_; };

    template<unsigned SourceSz, unsigned WeightSize, typename Cont>
    typename accumulator<SourceSz,WeightSize, Cont>::sources_count_type
    accumulator<SourceSz,WeightSize, Cont>::sources_count()const{
        return contributions_.size();
    }

    template<unsigned SourceSz, unsigned WeightSize, typename Cont>
    void accumulator<SourceSz,WeightSize, Cont>::set(
        value_type bandwidth){ bandwidth_ = bandwidth; };

    template<unsigned SourceSz, unsigned WeightSize, typename Cont>
        template<typename R0,typename R1>
    void accumulator<SourceSz,WeightSize, Cont>
        ::push_back_contribution(
        const R0& source,
        const R1& weight,
        value_type bandwidth){
        contributions_.push_back(
            contribution_type(source,weight,bandwidth)
        );
    }

    template<unsigned SourceSz,unsigned WeightSize,  typename Cont>
        template<typename R0,typename R1>
    void accumulator<SourceSz,WeightSize, Cont>::operator()(
        const R0& source, const R1& weight){
        push_back_contribution(source,weight,active_bandwidth()); }

}//exact
}//ifgt
}//math
}//boost
#endif
