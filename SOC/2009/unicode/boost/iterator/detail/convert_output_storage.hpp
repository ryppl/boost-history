#ifndef BOOST_ITERATOR_DETAIL_CONVERT_OUTPUT_STORAGE_HPP
#define BOOST_ITERATOR_DETAIL_CONVERT_OUTPUT_STORAGE_HPP

#include <boost/utility/enable_if.hpp>
#include <boost/mpl/has_xxx.hpp>
#include <vector>

#include <boost/iterator/converter_concept.hpp>

namespace boost
{
namespace detail
{
    BOOST_MPL_HAS_XXX_TRAIT_DEF(max_output)

    template<typename P, typename Enable = void>
    struct convert_output_storage;

    template<typename P>
    struct convert_output_storage<P, typename ::boost::disable_if< has_max_output<P> >::type>
    {
        BOOST_CONCEPT_ASSERT((ConverterConcept<P>));
private:
        typedef std::vector<typename P::output_type> Values;
public:
        typedef std::back_insert_iterator<Values> output_iterator;
        
        const typename P::output_type& operator[](size_t i) const
        {
            return values[i];
        }
        
        size_t last_index() const
        {
            return values.size() - 1;
        }
        
        output_iterator out()
        {
            values.clear();
            return std::back_inserter(values);
        }
        
        void update(output_iterator)
        {
        }
        
    private:
        Values values;
    };
    
    template<typename P>
    struct convert_output_storage<P, typename boost::enable_if< has_max_output<P> >::type>
    {
        BOOST_CONCEPT_ASSERT((ConverterConcept<P>));
private:
        typedef typename P::output_type Value;
public:
        typedef Value* output_iterator;
        
        const Value& operator[](size_t i) const
        {
            return values[i];
        }
        
        size_t last_index() const
        {
            return last;
        }
        
        output_iterator out()
        {
            return values;
        }
        
        void update(output_iterator u)
        {
            last = u - values - 1;
        }
        
    private:
        Value values[P::max_output::value];
        size_t last;
    };
}
}
#endif
