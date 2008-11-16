
#ifndef BOOST_GRAPHS_ADJMATRIX_COLUMN_TRAITS_HPP
#define BOOST_GRAPHS_ADJMATRIX_COLUMN_TRAITS_HPP

namespace boost { namespace graphs { namespace adjacency_matrix {

namespace detail
{
    // Specialize the case of matrix references and various operations on them.
    // Note that the generic case is empty to force compiler errors.
    template <typename T>
    struct column_traits { };

    // Delegate operations onto a vector of optionals.
    template <typename T>
    struct column_traits<std::vector<optional<T>>>
    {
        typedef std::vector<optional<T>> container;
        typedef typename container::size_type size_type;
        typedef typename container::reference reference;
        typedef typename container::const_reference const_reference;

        typedef T const& get_result;

        static optional<T> make_value(T const& x)
        { return optional<T>(x); }

        // This can easily segfault c[i] is not valid.
        get_result get(container const& c, size_type i)
        { return c[i].get(); }

        /* Set the optional to a default-constructed value. */
        static void set(container& c, size_type i)
        { c[i] = optional<T>(T()); }

        /* Set the element to a valid value x. */
        static void set(container&c, size_type i, T const& x)
        { c[i] = x; }

        static void clear(container& c, size_type i)
        { c[i].reset(); }
    };

    template <typename T, typename A>
    struct column_traits<std::vector<optional_value<T,A>>>
    {
        typedef std::vector<optional_value<T,A>> container;
        typedef typename container::size_type size_type;
        typedef typename container::reference reference;
        typedef typename container::const_reference const_reference;

        typedef T const& get_result;

        static optional_value<T,A> make_value(T const& x)
        { return optional_value<T,A>(x); }

        // This can easily segfault c[i] is not valid.
        get_result get(container const& c, size_type i)
        { return c[i].get(); }

        static void set(container& c, size_type i)
        { c[i] = optional_value<T>(T()); }

        static void set(container&c, size_type i, T const& x)
        { c[i] = x; }

        static void clear(container& c, size_type i)
        { c[i].reset(); }
    };

    // Delegate operations onto the yea-olde bit vector.
    template <>
    struct column_traits<std::vector<bool>>
    {
        typedef std::vector<bool> container;
        typedef container::size_type size_type;
        typedef container::reference reference;
        typedef container::const_reference const_reference;

        typedef const_reference get_result;

        static bool make_value(none)
        { return false; }

        static get_result get(container const& c, size_type i)
        { return c[i]; }

        static void set(container& c, size_type i)
        { c[i] = true; }

        static void clear(container& c, size_type i)
        { c[i] = false; }
    };
}

} } } /* namespace boost::graphs::adjacency_matrix */

#endif
