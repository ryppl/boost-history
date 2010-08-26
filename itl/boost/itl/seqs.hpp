/*-----------------------------------------------------------------------------+
Copyright (c) 2010-2010: Joachim Faulhaber
+------------------------------------------------------------------------------+
   Distributed under the Boost Software License, Version 1.0.
      (See accompanying file LICENCE.txt or copy at
           http://www.boost.org/LICENSE_1_0.txt)
+-----------------------------------------------------------------------------*/

/*------------------------------------------------------------------------------
itl_rational provides adapter code for boost::rational.
------------------------------------------------------------------------------*/

#ifndef BOOST_ITL_SEQS_HPP_JOFA_100824
#define BOOST_ITL_SEQS_HPP_JOFA_100824


namespace boost{namespace itl
{

template<class SeqT>
struct seqs
{
    typedef          SeqT                     seq_type;
    typedef typename seq_type::domain_type    domain_type;
    typedef typename seq_type::codomain_type  codomain_type;
    typedef typename seq_type::element_type   element_type;
    typedef typename seq_type::size_type      size_type;
    typedef typename seq_type::iterator       iterator;
    typedef typename seq_type::const_iterator const_iterator;

    static const_iterator begin(const seq_type&);
    static       iterator begin(      seq_type&);
    static const_iterator end  (const seq_type&);
    static       iterator end  (      seq_type&);
    static      size_type size (const seq_type&);
    static const_iterator find (const seq_type&, const domain_type&);
    static       iterator find (      seq_type&, const domain_type&);

    static std::pair<iterator,bool> insert(seq_type&, const element_type&);
    static iterator insert(seq_type&, iterator, const element_type&);
    static void     erase (seq_type&, iterator);
    static void     erase (seq_type&, iterator, iterator);

    static void swap(seq_type&, seq_type&);

    //--------------------------------------------------------------------------
    template<typename IteratorT>
    static const domain_type& key_value(IteratorT value_);

    template<typename IteratorT>
    static const codomain_type& co_value(IteratorT value_);

    template<typename LeftIterT, typename RightIterT>
    static bool key_less(LeftIterT lhs_, RightIterT rhs_);

    static element_type make_value(const domain_type& key_value, const codomain_type& data_value);

};


}} // namespace itl boost


#endif


