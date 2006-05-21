#include <boost/sequence_algo/non_modifying.hpp>
#include <boost/concept_check.hpp>
#include <boost/concept_architypes.hpp>

int main(int, char*[])
{
  //===========================================================================
  // Non-mutating Algorithms

  // Check that the old iterators work with the new algorithms
  {
    input_iterator_archetype< 
      convertible_to_archetype< null_archetype<>  > > in;
    unary_function_archetype< null_archetype<> , null_archetype<> > 
      f(dummy_cons);
    boost::for_each(in, in, f);
  }
  {
    typedef equality_comparable2_first_archetype<> Left;
    input_iterator_archetype< Left > in;
    equality_comparable2_second_archetype<> value(dummy_cons);
    in = boost::find(in, in, value);
  }
  {
    input_iterator_archetype< 
      convertible_to_archetype< null_archetype<>  > > in;
    unary_predicate_archetype< null_archetype<>  > pred(dummy_cons);
    in = boost::find_if(in, in, pred);
  }
  {
    forward_iterator_archetype< equality_comparable_archetype<> > fo;
    fo = boost::adjacent_find(fo, fo);
  }
  {
    forward_iterator_archetype< 
      convertible_to_archetype< null_archetype<>  > > fo;
    binary_predicate_archetype<null_archetype<> , null_archetype<> > 
      pred(dummy_cons);
    fo = boost::adjacent_find(fo, fo, pred);
  }
  {
    typedef equal_op_first_archetype<> Left;
    input_iterator_archetype<Left> in;
    typedef equal_op_second_archetype<> Right;
    forward_iterator_archetype<Right> fo;
    in = boost::find_first_of(in, in, fo, fo);
  }
  {
    typedef equal_op_first_archetype<> Left;
    typedef input_iterator_archetype<Left> InIter;
    InIter in;
    function_requires< InputIteratorConcept<InIter> >();
    equal_op_second_archetype<> value(dummy_cons);
    boost::iterator_traits<InIter>::difference_type
      n = boost::count(in, in, value);
    ignore_unused_variable_warning(n);
  }
#if !defined(__KCC)
  {
    typedef equal_op_first_archetype<> Left;
    typedef input_iterator_archetype<Left> InIter;
    InIter in;
    equal_op_second_archetype<> value(dummy_cons);
    unsigned long n;
    boost::count(in, in, value, n);
  }
  {
    input_iterator_archetype< convertible_to_archetype<null_archetype<> > > in;
    unary_predicate_archetype<null_archetype<> > pred(dummy_cons);
    unsigned long n;
    boost::count_if(in, in, pred, n);
  }
#endif
  {
    typedef input_iterator_archetype<
      convertible_to_archetype<null_archetype<> > > InIter;
    InIter in;
    unary_predicate_archetype<null_archetype<> > pred(dummy_cons);
    boost::iterator_traits<InIter>::difference_type
      n = boost::count_if(in, in, pred);
    ignore_unused_variable_warning(n);
  }
  {
    typedef equal_op_first_archetype<> Left;
    typedef input_iterator_archetype<Left> InIter1;
    InIter1 in1;
    typedef equal_op_second_archetype<> Right;
    typedef input_iterator_archetype<Right> InIter2;
    InIter2 in2;
    std::pair<InIter1, InIter2> p = boost::mismatch(in1, in1, in2);
    ignore_unused_variable_warning(p);
  }
  {
    typedef input_iterator_archetype<
      convertible_to_archetype<null_archetype<> > > InIter;
    InIter in1, in2;
    binary_predicate_archetype<null_archetype<> , null_archetype<> > 
      pred(dummy_cons);
    std::pair<InIter, InIter> p = boost::mismatch(in1, in1, in2, pred);
    ignore_unused_variable_warning(p);
  }
  {
    typedef equality_comparable2_first_archetype<> Left;
    input_iterator_archetype<Left> in1;
    typedef equality_comparable2_second_archetype<> Right;
    input_iterator_archetype<Right> in2;
    bool b = boost::equal(in1, in1, in2);
    ignore_unused_variable_warning(b);
  }
  {
    input_iterator_archetype< convertible_to_archetype<null_archetype<> > >
      in1, in2;
    binary_predicate_archetype<null_archetype<> , null_archetype<> > 
      pred(dummy_cons);
    bool b = boost::equal(in1, in1, in2, pred);
    ignore_unused_variable_warning(b);
  }
  {
    typedef equality_comparable2_first_archetype<> Left;
    forward_iterator_archetype<Left> fo1;
    typedef equality_comparable2_second_archetype<> Right;
    forward_iterator_archetype<Right> fo2;
    fo1 = boost::search(fo1, fo1, fo2, fo2);
  }
  {
    typedef equality_comparable2_first_archetype< 
      convertible_to_archetype<null_archetype<> > > Left;
    forward_iterator_archetype<Left> fo1;
    typedef equality_comparable2_second_archetype<
      convertible_to_archetype<null_archetype<> > > Right;
    forward_iterator_archetype<Right> fo2;
    binary_predicate_archetype<null_archetype<> , null_archetype<> > 
      pred(dummy_cons);
    fo1 = boost::search(fo1, fo1, fo2, fo2, pred);
  }
  {
    typedef equality_comparable2_first_archetype<> Left;
    forward_iterator_archetype<Left> fo;
    equality_comparable2_second_archetype<> value(dummy_cons);
    int n = 1;
    fo = boost::search_n(fo, fo, n, value);
  }
  {
    forward_iterator_archetype< 
      convertible_to_archetype<null_archetype<> > > fo;
    convertible_to_archetype<null_archetype<> > value(dummy_cons);
    binary_predicate_archetype<null_archetype<> , null_archetype<> > 
      pred(dummy_cons);
    int n = 1;
    fo = boost::search_n(fo, fo, n, value, pred);
  }
  {
    typedef equality_comparable2_first_archetype<> Left;
    forward_iterator_archetype<Left> fo1;
    typedef equality_comparable2_second_archetype<null_archetype<> > Right;
    forward_iterator_archetype<Right> fo2;
    fo1 = boost::find_end(fo1, fo1, fo2, fo2);
  }
  {
    // equality comparable required because find_end() calls search
    typedef equality_comparable2_first_archetype<
      convertible_to_archetype<null_archetype<> > > Left;
    forward_iterator_archetype<Left> fo1;
    typedef equality_comparable2_second_archetype<
      convertible_to_archetype<null_archetype<> > > Right;
    forward_iterator_archetype<Right> fo2;
    binary_predicate_archetype<null_archetype<> , null_archetype<> > 
      pred(dummy_cons);
    fo1 = boost::find_end(fo1, fo1, fo2, fo2, pred);
  }


  return boost::exit_success;
}
