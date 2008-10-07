
#ifndef BOOST_GRAPHS_ADJMATRIX_MATRIX_ELEMENT_HPP
#define BOOST_GRAPHS_ADJMATRIX_MATRIX_ELEMENT_HPP

namespace boost { namespace graphs { namespace adjacency_matrix {

namespace detail
{
    // Metafunctions to generate the value type of a basic matrix.
    // TODO: This facility probably needs to be exposed to other matrix
    // implementations (e.g., triangular matrix, compressed matrix, etc.).
    template <typename T>
    struct matrix_element
    {
        typedef T value_type;
        typedef optional<T> element_type;
    };

    // When parameterized over optional values, we simply store those objects.
    // Note that this reduces the storage cost by using the absentee value of
    // the optional_value to represent missing values.
    template <typename T, typename A>
    struct matrix_element<optional_value<T,A>>
    {
        typedef T value_type;
        typedef optional_value<T,A> element_type;
    };

    // For unparameterized matrix elements, we can simply revert to using a
    // boolean value to indicate the presence or absence of edge types.
    template <>
    struct matrix_element<none>
    {
        typedef none value_type;
        typedef bool element_type;
    };
}

} } } /* namespace boost::graphs::adjacency_matrix */

#endif
