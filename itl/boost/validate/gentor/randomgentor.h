/*----------------------------------------------------------------------------+
Interval Template Library
Author: Joachim Faulhaber
Copyright (c) 2007-2008: Joachim Faulhaber
+-----------------------------------------------------------------------------+
   Distributed under the Boost Software License, Version 1.0.
      (See accompanying file LICENCE.txt or copy at
           http://www.boost.org/LICENSE_1_0.txt)
+----------------------------------------------------------------------------*/
#pragma once


#include <boost/itl_xt/numbergentor.hpp>
#include <boost/itl_xt/setgentor.hpp>
#include <boost/itl_xt/mapgentor.hpp>
#include <boost/itl_xt/itvgentor.hpp>
#include <boost/itl/interval_set.hpp>
#include <boost/itl/separate_interval_set.hpp>
#include <boost/itl/split_interval_set.hpp>
#include <boost/itl/interval_map.hpp>
#include <boost/itl/split_interval_map.hpp>
#include <boost/validate/gentor/gentorprofile.h>


namespace boost{namespace itl
{

    // ----------------------------------------------------------
    template <class ValueT> class RandomGentor;
    template <> class RandomGentor<int> : public NumberGentorT<int> {};
    template <> class RandomGentor<double> : public NumberGentorT<double> {};

    // ----- sets ----------------------------------------------------------------
    //template <class DomainT, template<class>class Set> 
    //class RandomGentor<Set<DomainT> > :
    //    public SetGentorT<Set<DomainT> > {};

    template <class DomainT> 
    class RandomGentor<itl::set<DomainT> > :
        public SetGentorT<itl::set<DomainT> > {};

    template <class DomainT> 
    class RandomGentor<itl::interval_set<DomainT> > :
        public SetGentorT<itl::interval_set<DomainT> > {};

    template <class DomainT> 
    class RandomGentor<itl::separate_interval_set<DomainT> > :
        public SetGentorT<itl::separate_interval_set<DomainT> > {};

    template <class DomainT> 
    class RandomGentor<itl::split_interval_set<DomainT> > :
        public SetGentorT<itl::split_interval_set<DomainT> > {};

    // ----- maps -------------------------------------------------------------
    template <class DomainT, class Neutronizer> 
    class RandomGentor<itl::map<DomainT,itl::set<int>,Neutronizer> > : 
        public MapGentorT<itl::map<DomainT,itl::set<int>,Neutronizer> > {};

    template <class DomainT, class CodomainT, class Neutronizer> 
    class RandomGentor<itl::map<DomainT,CodomainT,Neutronizer> > : 
        public MapGentorT<itl::map<DomainT,CodomainT,Neutronizer> > {};


    // ----- interval_map<D,C,N> ----------------------------------------
    template <class DomainT, class Neutronizer> 
    class RandomGentor<interval_map<DomainT,itl::set<int>,Neutronizer> > : 
        public MapGentorT<interval_map<DomainT,itl::set<int>,Neutronizer> > {};

    template <class DomainT, class CodomainT, class Neutronizer> 
    class RandomGentor<interval_map<DomainT,CodomainT,Neutronizer> > : 
        public MapGentorT<interval_map<DomainT,CodomainT,Neutronizer> > {};

    // ----- split_interval_map<D,C,N> ----------------------------------------
    template <class DomainT, class Neutronizer> 
    class RandomGentor<split_interval_map<DomainT,itl::set<int>,Neutronizer> > : 
        public MapGentorT<split_interval_map<DomainT,itl::set<int>,Neutronizer> > {};

    template <class DomainT, class CodomainT, class Neutronizer> 
    class RandomGentor<split_interval_map<DomainT,CodomainT,Neutronizer> > : 
        public MapGentorT<split_interval_map<DomainT,CodomainT,Neutronizer> > {};


    // ------------------------------------------------------------------------
    // class template SomeValue:
    // TargetT is a value tuple that fits to the value generator gentor
    // of type GentorT<TargetT>. The special kind of generator can be
    // passed by the user. The only requirement is that the generator
    // implements a function 'some' which is called to generate a tuple
    // of type TargetT
    template <class TargetT, template<class>class GentorT> struct SomeValue
    {
        static void apply(TargetT& value, GentorT<TargetT>& gentor); 
    };

    template <class TargetT> 
    struct SomeValue<TargetT, RandomGentor>
    {
        static void apply(TargetT& value, RandomGentor<TargetT>& gentor) 
        {
            gentor.some(value);
        }
    };
    // ------------------------------------------------------------------------


    // ------------------------------------------------------------------------
    // ------------------------------------------------------------------------
    template <class TargetT, template<class>class GentorT> struct Calibrater
    {
        static void apply(GentorT<TargetT>& gentor); 
    };

    template <> 
    struct Calibrater<int, RandomGentor>
    {
        static void apply(RandomGentor<int>& gentor)
        {
            // Set the range within which the sizes of the generated object varies.
            gentor.setRange(GentorProfileSgl::it()->range_int());
        }
    };

    template <> 
    struct Calibrater<double, RandomGentor>
    {
        static void apply(RandomGentor<double>& gentor) 
        {
            // Set the range within which the sizes of the generated object varies.
            gentor.setRange(GentorProfileSgl::it()->range_double());
        }
    };


    template <> 
    struct Calibrater<itl::set<int>, RandomGentor>
    {
        static void apply(RandomGentor<itl::set<int> >& gentor) 
        {
            gentor.setRangeOfSampleSize(GentorProfileSgl::it()->range_ContainerSize());
            NumberGentorT<int>* intGentor = new NumberGentorT<int>;
            intGentor->setRange(GentorProfileSgl::it()->range_int());
            gentor.setDomainGentor(intGentor);
        }
    };

    template <> 
    struct Calibrater<itl::set<double>, RandomGentor>
    {
        static void apply(RandomGentor<itl::set<double> >& gentor) 
        {
            gentor.setRangeOfSampleSize(GentorProfileSgl::it()->range_ContainerSize());
            NumberGentorT<double>* elemGentor = new NumberGentorT<double>;
            elemGentor->setRange(GentorProfileSgl::it()->range_double());
            gentor.setDomainGentor(elemGentor);
        }
    };


    template <> 
    struct Calibrater<interval_set<int>, RandomGentor>
    {
        static void apply(RandomGentor<interval_set<int> >& gentor) 
        {
            gentor.setRangeOfSampleSize(GentorProfileSgl::it()->range_ContainerSize());
            ItvGentorT<int>* itvGentor = new ItvGentorT<int>;
            interval<int> valRange = GentorProfileSgl::it()->range_interval_int();
            itvGentor->setValueRange(valRange.lower(), valRange.upper());
            itvGentor->setMaxIntervalLength(GentorProfileSgl::it()->maxIntervalLength()); //JODO
            gentor.setDomainGentor(itvGentor);
        }
    };

    template <> 
    struct Calibrater<interval_set<double>, RandomGentor>
    {
        static void apply(RandomGentor<interval_set<double> >& gentor) 
        {
            gentor.setRangeOfSampleSize(GentorProfileSgl::it()->range_ContainerSize());
            ItvGentorT<double>* itvGentor = new ItvGentorT<double>;
            interval<double> valRange = GentorProfileSgl::it()->range_interval_double();
            itvGentor->setValueRange(valRange.lower(), valRange.upper());
            itvGentor->setMaxIntervalLength(GentorProfileSgl::it()->maxIntervalLength()); //JODO
            gentor.setDomainGentor(itvGentor);
        }
    };

    template <> 
    struct Calibrater<separate_interval_set<int>, RandomGentor>
    {
        static void apply(RandomGentor<separate_interval_set<int> >& gentor) 
        {
            gentor.setRangeOfSampleSize(GentorProfileSgl::it()->range_ContainerSize());
            ItvGentorT<int>* itvGentor = new ItvGentorT<int>;
            interval<int> valRange = GentorProfileSgl::it()->range_interval_int();
            itvGentor->setValueRange(valRange.lower(), valRange.upper());
            itvGentor->setMaxIntervalLength(GentorProfileSgl::it()->maxIntervalLength()); //JODO
            gentor.setDomainGentor(itvGentor);
        }
    };

    template <> 
    struct Calibrater<separate_interval_set<double>, RandomGentor>
    {
        static void apply(RandomGentor<separate_interval_set<double> >& gentor) 
        {
            gentor.setRangeOfSampleSize(GentorProfileSgl::it()->range_ContainerSize());

            // If it is a container: (Create and) Pass the generator(s) for their contents
            // NumberGentorT<int> intGentor;
            ItvGentorT<double>* itvGentor = new ItvGentorT<double>;
            interval<double> valRange = GentorProfileSgl::it()->range_interval_double();
            itvGentor->setValueRange(valRange.lower(), valRange.upper());
            itvGentor->setMaxIntervalLength(GentorProfileSgl::it()->maxIntervalLength()); //JODO
            gentor.setDomainGentor(itvGentor);
        }
    };

    template <> 
    struct Calibrater<split_interval_set<int>, RandomGentor>
    {
        static void apply(RandomGentor<split_interval_set<int> >& gentor) 
        {
            // Set the range within which the sizes of the generated object varies.
            // interval<int> range = rightOpenInterval<int>(0,10); //JODO: From SysCalibrater
            //JODO gentor.calibrate(profile);

            gentor.setRangeOfSampleSize(GentorProfileSgl::it()->range_ContainerSize());

            // If it is a container: (Create and) Pass the generator(s) for their contents
            // NumberGentorT<int> intGentor;
            ItvGentorT<int>* itvGentor = new ItvGentorT<int>;
            interval<int> valRange = GentorProfileSgl::it()->range_interval_int();
            itvGentor->setValueRange(valRange.lower(), valRange.upper());
            itvGentor->setMaxIntervalLength(GentorProfileSgl::it()->maxIntervalLength()); //JODO
            gentor.setDomainGentor(itvGentor);
        }
    };

    template <> 
    struct Calibrater<split_interval_set<double>, RandomGentor>
    {
        static void apply(RandomGentor<split_interval_set<double> >& gentor) 
        {
            gentor.setRangeOfSampleSize(GentorProfileSgl::it()->range_ContainerSize());

            // If it is a container: (Create and) Pass the generator(s) for their contents
            // NumberGentorT<int> intGentor;
            ItvGentorT<double>* itvGentor = new ItvGentorT<double>;
            interval<double> valRange = GentorProfileSgl::it()->range_interval_double();
            itvGentor->setValueRange(valRange.lower(), valRange.upper());
            itvGentor->setMaxIntervalLength(GentorProfileSgl::it()->maxIntervalLength()); //JODO
            gentor.setDomainGentor(itvGentor);
        }
    };

    //----------------------------------------------------------------------------
    // itl::map<DomainT,CodomainT,Neutronizer>
    //----------------------------------------------------------------------------
    template <typename NumericDomainT, class Neutronizer> 
    struct Calibrater<itl::map<NumericDomainT,itl::set<int>,Neutronizer>, RandomGentor>
    {
        static void apply(RandomGentor<itl::map<NumericDomainT,itl::set<int>,Neutronizer> >& gentor) 
        {
            // Set the range within which the sizes of the generated object varies.
            // interval<int> range = rightOpenInterval<int>(0,10); //JODO: From SysCalibrater
            //JODO gentor.calibrate(profile);

            gentor.setRangeOfSampleSize(GentorProfileSgl::it()->range_ContainerSize());

            // If it is a container: (Create and) Pass the generator(s) for their contents
            // NumberGentorT<int> intGentor;
            NumberGentorT<NumericDomainT>* domainGentor = new NumberGentorT<NumericDomainT>;
            domainGentor->setRange(GentorProfileSgl_numeric_range<NumericDomainT>::get());

            SetGentorT<itl::set<int> >* codomainGentor = new SetGentorT<itl::set<int> >;
            NumberGentorT<int>* elementGentor = new NumberGentorT<int>;
            elementGentor->setRange(GentorProfileSgl_numeric_range<int>::get());

            codomainGentor->setDomainGentor(elementGentor);            
            codomainGentor->setRangeOfSampleSize(GentorProfileSgl::it()->range_ContainerSize());

            gentor.setDomainGentor(domainGentor);
            gentor.setCodomainGentor(codomainGentor);
        }
    };

    template <typename NumericDomainT, typename NumericCodomainT, class Neutronizer> 
    struct Calibrater<itl::map<NumericDomainT,NumericCodomainT,Neutronizer>, RandomGentor>
    {
        static void apply(RandomGentor<itl::map<NumericDomainT,NumericCodomainT,Neutronizer> >& gentor) 
        {
            gentor.setRangeOfSampleSize(GentorProfileSgl::it()->range_ContainerSize());

            // If it is a container: (Create and) Pass the generator(s) for their contents
            // NumberGentorT<int> intGentor;
            NumberGentorT<NumericDomainT>* domainGentor = new NumberGentorT<NumericDomainT>;
            NumberGentorT<NumericCodomainT>* codomainGentor = new NumberGentorT<NumericCodomainT>;
            domainGentor->setRange(GentorProfileSgl_numeric_range<NumericDomainT>::get());
            codomainGentor->setRange(GentorProfileSgl_numeric_range<NumericCodomainT>::get());
            gentor.setDomainGentor(domainGentor);
            gentor.setCodomainGentor(codomainGentor);
        }
    };


    //----------------------------------------------------------------------------
    // itl::interval_map<DomainT,CodomainT,Neutronizer>
    //----------------------------------------------------------------------------
    template <class NumericDomainT, class Neutronizer> 
    struct Calibrater<interval_map<NumericDomainT,itl::set<int>,Neutronizer>, RandomGentor>
    {
        static void apply(RandomGentor<interval_map<NumericDomainT,itl::set<int>,Neutronizer> >& gentor) 
        {
            // Set the range within which the sizes of the generated object varies.
            // interval<int> range = rightOpenInterval<int>(0,10); //JODO: From SysCalibrater
            //JODO gentor.calibrate(profile);

            gentor.setRangeOfSampleSize(GentorProfileSgl::it()->range_ContainerSize());

            // If it is a container: (Create and) Pass the generator(s) for their contents
            // NumberGentorT<int> intGentor;
            ItvGentorT<NumericDomainT>* itvGentor = new ItvGentorT<NumericDomainT>;
            interval<NumericDomainT> valRange = GentorProfileSgl_numeric_range<NumericDomainT>::get();
            itvGentor->setValueRange(valRange.lower(), valRange.upper());
            itvGentor->setMaxIntervalLength(GentorProfileSgl::it()->maxIntervalLength()); //JODO

            SetGentorT<itl::set<int> >* codomainGentor = new SetGentorT<itl::set<int> >;
            NumberGentorT<int>* elementGentor = new NumberGentorT<int>;
            elementGentor->setRange(GentorProfileSgl::it()->range_int());

            codomainGentor->setDomainGentor(elementGentor);            
            codomainGentor->setRangeOfSampleSize(GentorProfileSgl::it()->range_element_ContainerSize());

            gentor.setDomainGentor(itvGentor);
            gentor.setCodomainGentor(codomainGentor);
        }
    };

    template <typename NumericDomainT, typename NumericCodomainT, class Neutronizer> 
    struct Calibrater<interval_map<NumericDomainT,NumericCodomainT,Neutronizer>, RandomGentor>
    {
        static void apply(RandomGentor<interval_map<NumericDomainT,NumericCodomainT,Neutronizer> >& gentor) 
        {
            // Set the range within which the sizes of the generated object varies.
            // interval<int> range = rightOpenInterval<int>(0,10); //JODO: From SysCalibrater
            //JODO gentor.calibrate(profile);

            gentor.setRangeOfSampleSize(GentorProfileSgl::it()->range_ContainerSize());

            // If it is a container: (Create and) Pass the generator(s) for their contents
            // NumberGentorT<int> intGentor;
            ItvGentorT<NumericDomainT>* itvGentor = new ItvGentorT<NumericDomainT>;
            interval<NumericDomainT> valRange = GentorProfileSgl_numeric_range<NumericDomainT>::get();
            itvGentor->setValueRange(valRange.lower(), valRange.upper());
            itvGentor->setMaxIntervalLength(GentorProfileSgl::it()->maxIntervalLength()); //JODO

            NumberGentorT<NumericCodomainT>* codomainGentor = new NumberGentorT<NumericCodomainT>;
            codomainGentor->setRange(GentorProfileSgl_numeric_range<NumericCodomainT>::get());

            gentor.setDomainGentor(itvGentor);
            gentor.setCodomainGentor(codomainGentor);
        }
    };

    //----------------------------------------------------------------------------
    // itl::split_interval_map<DomainT,CodomainT,Neutronizer>
    //----------------------------------------------------------------------------
    template <class NumericDomainT, class Neutronizer> 
    struct Calibrater<split_interval_map<NumericDomainT,itl::set<int>,Neutronizer>, RandomGentor>
    {
        static void apply(RandomGentor<split_interval_map<NumericDomainT,itl::set<int>,Neutronizer> >& gentor) 
        {
            gentor.setRangeOfSampleSize(GentorProfileSgl::it()->range_ContainerSize());

            // If it is a container: (Create and) Pass the generator(s) for their contents
            // NumberGentorT<int> intGentor;
            ItvGentorT<NumericDomainT>* itvGentor = new ItvGentorT<NumericDomainT>;
            interval<NumericDomainT> valRange = GentorProfileSgl_numeric_range<NumericDomainT>::get();
            itvGentor->setValueRange(valRange.lower(), valRange.upper());
            itvGentor->setMaxIntervalLength(GentorProfileSgl::it()->maxIntervalLength());

            SetGentorT<itl::set<int> >* codomainGentor = new SetGentorT<itl::set<int> >;
            NumberGentorT<int>* elementGentor = new NumberGentorT<int>;
            elementGentor->setRange(GentorProfileSgl::it()->range_int());

            codomainGentor->setDomainGentor(elementGentor);            
            codomainGentor->setRangeOfSampleSize(GentorProfileSgl::it()->range_element_ContainerSize());

            gentor.setDomainGentor(itvGentor);
            gentor.setCodomainGentor(codomainGentor);
        }
    };

    template <typename NumericDomainT, typename NumericCodomainT, class Neutronizer> 
    struct Calibrater<split_interval_map<NumericDomainT,NumericCodomainT,Neutronizer>, RandomGentor>
    {
        static void apply(RandomGentor<split_interval_map<NumericDomainT,NumericCodomainT,Neutronizer> >& gentor) 
        {
            // Set the range within which the sizes of the generated object varies.
            // interval<int> range = rightOpenInterval<int>(0,10); //JODO: From SysCalibrater
            //JODO gentor.calibrate(profile);

            gentor.setRangeOfSampleSize(GentorProfileSgl::it()->range_ContainerSize());

            // If it is a container: (Create and) Pass the generator(s) for their contents
            // NumberGentorT<int> intGentor;
            ItvGentorT<NumericDomainT>* itvGentor = new ItvGentorT<NumericDomainT>;
            interval<NumericDomainT> valRange = GentorProfileSgl_numeric_range<NumericDomainT>::get();
            itvGentor->setValueRange(valRange.lower(), valRange.upper());
            itvGentor->setMaxIntervalLength(GentorProfileSgl::it()->maxIntervalLength()); //JODO

            NumberGentorT<NumericCodomainT>* codomainGentor = new NumberGentorT<NumericCodomainT>;
            codomainGentor->setRange(GentorProfileSgl_numeric_range<NumericCodomainT>::get());

            gentor.setDomainGentor(itvGentor);
            gentor.setCodomainGentor(codomainGentor);
        }
    };
    // ---------------------------------------------------------------------------

}} // namespace itl boost
