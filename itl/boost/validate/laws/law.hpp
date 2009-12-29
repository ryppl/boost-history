/*-----------------------------------------------------------------------------+    
A Law Based Test Automaton 'LaBatea'
Author: Joachim Faulhaber
Copyright (c) 2007-2009: Joachim Faulhaber
+------------------------------------------------------------------------------+
   Distributed under the Boost Software License, Version 1.0.
      (See accompanying file LICENCE.txt or copy at
           http://www.boost.org/LICENSE_1_0.txt)
+-----------------------------------------------------------------------------*/
#ifndef BOOST_ITL_LAW_HPP_JOFA_070411
#define BOOST_ITL_LAW_HPP_JOFA_070411

#include <string>
#include <set>
#include <boost/validate/loki_xt/Tuple.h>

namespace boost{namespace itl
{

    /** \brief An abstract class template Law

        A Law can hold for a given set of variables.
        A Law together with the set of input variables is an instance of the law.
        The evaluation of the law results in a set of output variables.
        Output variables hold interim and final results of the evaluation of the law.
    */
    template 
    <
        class SubType,
        typename InputTypes, typename OutputTypes
    >
    class Law
    {
    public:
        typedef SubType                           sub_type;
        typedef InputTypes                        input_types;
        typedef OutputTypes                       output_types;
        typedef typename Loki::tuple<InputTypes>  input_tuple;
        typedef typename Loki::tuple<OutputTypes> output_tuple;

    public:
        /** Function \c holds yields true if the law holds for a given instantiation
            of the input variables. */
        bool holds(){ return that()->holds(); }

        /** Function \c debug_holds is called at the end of a series of randomly
            generated tests on the smallest law instantiation that violates the
            law. Specific laws derived from class \c Law shall be coded to provide
            informations useful for debugging. */
        bool debug_holds(){ return that()->debug_holds(); }

        /** Set input variables for a law. */
        void setInstance(const input_tuple& inVars)
        { _inputTuple = inVars; }

        /** Get input and output variables. */
        void getInstance(input_tuple& inVars, output_tuple& outVars)const
        { inVars = _inputTuple; outVars = _outputTuple; }

        /** Get input variables. */
        void getInputInstance(input_tuple& inVars)const
        { inVars = _inputTuple; }

        /** Get output variables. */
        void getOutputInstance(output_tuple& outVars)const
        { outVars = _outputTuple; }

        /** Get the size of a laws instantiation, that is a function of it's 
            input variables. The size function is used to filter small and simple
            violations of laws in order to facilitate debugging. */
        size_t size()const{ return that()->size(); }

        /** A size equivalence on laws. */
        bool operator == (const Law& rhs)const
        { return size() == rhs.size(); }

        /** A size order on laws. This ordering servers filter small and simple
            law instances that violate the law. */
        bool operator < (const Law& rhs)const
        { return size() < rhs.size(); }

        /// The name of the law
        std::string name()const       { return that()->name(); }
        /// A formula for the law
        std::string formula()const    { return that()->formula(); }
        /// A string that represents the Laws type instantiation.
        std::string typeString()const { return that()->typeString(); }

        /** Set a value for a laws input variable, indicated by it's \index in
            the tuple of input variables */
        template<unsigned int index>
        typename Loki::TL::TypeAt<InputTypes, index>::Result 
            setInputValue(const typename Loki::TL::TypeAt<InputTypes, index>::Result& value) 
        { return Loki::tup::refer<index>(_inputTuple)=value; }

        /** Get the value for a laws input variable, indicated by it's \index in
            the tuple of input variables */
        template<unsigned int index>
        typename Loki::TL::TypeAt<InputTypes, index>::Result getInputValue()const 
        { return Loki::tup::get<index>(_inputTuple); }

        /** Set a value for a laws output variable, indicated by it's \index in
            the tuple of output variables */
        template<unsigned int index>
        typename Loki::TL::TypeAt<OutputTypes, index>::Result 
            setOutputValue(const typename Loki::TL::TypeAt<OutputTypes, index>::Result& value) 
        { return Loki::tup::refer<index>(_outputTuple)=value; }

        /** Get the value for a laws output variable, indicated by it's \index in
            the tuple of output variables */
        template<unsigned int index>
        typename Loki::TL::TypeAt<OutputTypes, index>::Result getOutputValue()const 
        { return Loki::tup::get<index>(_outputTuple); }

    protected:
              sub_type* that()      { return static_cast      <sub_type*>(this); }
        const sub_type* that()const { return static_cast<const sub_type*>(this); }

    private:
        input_tuple  _inputTuple;
        output_tuple _outputTuple;
    };

    enum InputVarIndex  { operand_a, operand_b, operand_c, operand_d, operand_e };
    enum OutputVarIndex { lhs_result, rhs_result };

}} // namespace itl boost

#endif //BOOST_ITL_LAW_HPP_JOFA_070411

