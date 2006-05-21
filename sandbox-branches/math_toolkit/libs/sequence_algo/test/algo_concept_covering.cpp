
int main() 
{

  // SGI STL Extensions, should all be in the Boost Sequence Algorithm Library
  {
    typedef assignable_archetype<> Tout;
    typedef convertible_to_archetype<Tout> Tin;
    input_iterator_archetype<Tin> in; 
    random_access_iterator_archetype<Tout> ri_out;
    ri_out = boost::random_sample(in, in, ri_out, ri_out);
  }
  {
    typedef assignable_archetype<> Tout;
    typedef convertible_to_archetype<Tout> Tin;
    input_iterator_archetype<Tin> in; 
    random_access_iterator_archetype<Tout> ri_out;
    unary_function_archetype<std::ptrdiff_t, std::ptrdiff_t> ran(dummy_cons);
    ri_out = boost::random_sample(in, in, ri_out, ri_out, ran);
  }
  {
    typedef assignable_archetype<> Tout;
    typedef convertible_to_archetype<Tout> Tin;
    forward_iterator_archetype<Tin> in; 
    output_iterator_archetype<Tout> out(dummy_cons);
    int n = 1;
    out = boost::random_sample_n(in, in, out, n);
  }
  {
    typedef assignable_archetype<> Tout;
    typedef convertible_to_archetype<Tout> Tin;
    input_iterator_archetype<Tin> in; 
    forward_iterator_archetype<Tout> fwd_out;
    unary_function_archetype<std::ptrdiff_t, std::ptrdiff_t> ran(dummy_cons);
    int n = 1;
    fwd_out = boost::random_sample_n(in, in, fwd_out, n, ran);
  }
  {
    typedef less_than_comparable_archetype<> T;
    forward_iterator_archetype<T> fi;
    bool b = boost::is_sorted(fi, fi);
    ignore_unused_variable_warning(b);
  }
  {
    typedef null_archetype<int> Arg1;
    typedef null_archetype<char> Arg2;
#if defined(__GNUC__) || defined(__KCC)
    typedef convertible_to_archetype<Arg1,
      convertible_to_archetype<Arg2> > Tin1;
    typedef convertible_to_archetype<Arg2,
      convertible_to_archetype<Arg1> > Tin2;
#endif
    input_iterator_archetype<Tin1> in1;
    input_iterator_archetype<Tin2> in2;
    binary_predicate_archetype<Arg1, Arg2> comp(dummy_cons);
    int r = boost::lexicographical_compare_3way(in1, in1, in2, in2, comp);
    ignore_unused_variable_warning(r);
  }
  {
    typedef sgi_assignable_archetype<
      less_than_comparable_archetype<> > T;
    random_access_iterator_archetype<T> ri;
    bool b = boost::is_heap(ri, ri);
    ignore_unused_variable_warning(b);
  }
  {
    typedef null_archetype<> Arg;
    typedef sgi_assignable_archetype< 
      convertible_to_archetype<Arg> > T;
    random_access_iterator_archetype<T> ri;
    binary_predicate_archetype<Arg, Arg> comp(dummy_cons);
    bool b = boost::is_heap(ri, ri, comp);
    ignore_unused_variable_warning(b);
  }
  {
    typedef null_archetype<> FT;
    typedef assignable_archetype<
      convertible_to_archetype<FT> > T;
    forward_iterator_archetype<FT> fi;
    T value(dummy_cons);
    boost::iota(fi, fi, value);
  }

  // This one doesn't below in the algorithm library, but it will live
  // here for now until someone finds it a better home. -JGS
  {
    int n = 1;
    multipliable_archetype<power_stuff::monoid_archetype> x(dummy_cons);
    x = boost::power(x, n);
  }
  {
    int n = 1;
    typedef multipliable_archetype<power_stuff::monoid_archetype> T;
    T x(dummy_cons);
    x = boost::power(x, n, multiplies<T>());
  }

  return boost::exit_success;
}
