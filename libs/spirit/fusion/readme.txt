Hi,

See: http://spirit.sourceforge.net/dl_more/fusion.zip This is a re-
implementation of the TR tuples with iterators, views and algorithms. The
structure is somewhat modeled after MPL. I code-named it "fusion" because
the library reminds me of the "fusion" of compile time metaprogramming with
runtime programming. Right now, for the purpose of testing and
experimenting, I have placed it under the Spirit directory (like Phoenix).

Here's the overall structure:

Tuples:

    tuple       : the basic tuple structure
    tuple-i/o   : tuple I/O
    make-tuple  : make_tuple and tie
    operator    : tuple operators
    generator   : generate a tuple given a begin/end iterators

    * Basically the same as the TR1 tuples in the boost-sandbox
      with the ability to construct and generate tuples from
      iterators.

Iterators:

    Generic iteration for heterogeneous types. This is not at all tied to
    tuples; the tuple-iterator is just a specific type. The library is
    based on iterators. There are iterators for MPL containers, filtering
    iterators, transforming iterators, etc.

Sequences and Views:

    Like in MPL, holds a begin/end iterator. Sequences and views may be
    composed to form more complex sequences and views. View/sequence
    composition is a very nice concept. These are extremely lighweight
    classes and can be passed around by value quite inexpensively. For
    instance, rather than working directly on tuples, the algorithms work
    on views and return views.

    const_value_sequence
        Wraps an MPL-like constant value sequence (e.g. iterators to
        mpl::vector_c and mpl::list_c).

    type_sequence
        Wraps an MPL-like type sequence (e.g. iterators to
        mpl::vector and mpl::list).

    tuple_view
        Wraps a reference to a tuple. I would like to propose the addition
        of begin() end() functions to the tuple class API so that it can
        participate directly as a Sequence. Right now, it can't -you have to
        make a tuple_view first (easy).

    filter_view
        given an MPL predicate, filters the view to present only the items
        that passes the predicate.

    single_view
        a single element view (similar to MPL's)

    joint_view
        a two-sequence view (similar to MPL's). Quite nifty, for example
        here's how push back is implemented:

        template <typename ViewT, typename T>
        inline typename push_back_result<ViewT, T>::type
        push_back(ViewT const& view, T const& x)
        {
            typedef typename make_single_view<T>::type insert_type;
            typedef joint_view<ViewT, insert_type> result;
            return result(view, insert_type(x));
        }

    slice_view
        a sliced sequence/view.

    transform_view
        Like, MPL's: a sequence wrapper that allows one to operate on the
        transformed sequence without actually creating one. However, this
        uses half-runtime transform functions. Here's an example of a
        transform:

        struct square
        {
            template <typename T>
            struct result
            {
                typedef T type;
            };

            template <typename T>
            T operator()(T x) const
            {
                return x * x;
            }
        };

        << The nice thing is that you can use Lambda/Phoenix polymorphic
        functors for the transform >>

Algorithms:

    With very complex composition of algorithms, it is not desirable to
    work directly on tuples. Like MPL, and unlike STL, the algorithms take
    in sequences/views and *return* the result by value; the algorithms are
    purely functional and do not (cannot) have side effects. We cannot have
    an out parameter that is passed by reference where the result is
    placed. The algorithms, instead, work on sequences and views and
    generate views.

    This strategy is very efficient. You can think of the algorithms as
    "lazy". The values are generated only wnen it is needed -for example
    when generating a tuple. When you are sure that you need a tuple, you
    can "generate" it from the view: generate(view);

    Currently, these algorithms are defined:

        fold
        find
        find_if
        transform
        filter
        count
        count_if
        contains
        append
        push_back
        push_front
        insert
        remove
        replace
        remove_if
        for_each

    Each of these algorithms has its result metaprogram. For instance,
    here's the transform algorithm:

        template <typename ViewT, typename F>
        struct transform_result
        {
            typedef transform_view<ViewT, F> type;
        };

        template <typename ViewT, typename F>
        inline transform_view<ViewT, F>
        transform(ViewT const& view, F const& f)
        {
            return transform_view<ViewT, F>(view, f);
        }

    transform_result<ViewT, F>::type is the result type when transform is
    called given a view ViewT and a transform functor F.

Example, squaring an MPL list_c and storing it in a tuple:

    typedef boost::mpl::range_c<int, 5, 9> mpl_list1;
    typedef const_value_sequence<mpl_list1> sequence_type;
    sequence_type sequence;

    std::cout << generate(transform(sequence, square())) << std::endl;
    BOOST_TEST((generate(transform(sequence, square()))
        == make_tuple(25, 36, 49, 64)));

More tests/examples can be found in the test directory.

There you go. Comments welcome... Dave's insistence on iterators is
godsend! I only had to add 2 extensions to Jaakko's original TR1 proposal:
a constructor that takes in an iterator and a "generate" function (similar
to make_tuple) that returns a tuple given a begin/end iterator pair. The
whole stuff is built on iterators.

Regards,
--
Joel de Guzman
http://www.boost-consulting.com
http://spirit.sf.net
