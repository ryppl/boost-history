//components treated "specially" in a composite.
#ifndef BOOST_COMPOSITE_STORAGE_SPECIAL_COMPONENTS_HPP_INCLUDED
#define BOOST_COMPOSITE_STORAGE_SPECIAL_COMPONENTS_HPP_INCLUDED

namespace boost
{
namespace composite_storage
{
namespace special_components
{
    enum special_id
    { nothing_id //signifies error, something like haskell's Nothing.
    , empty_id //signifies empty class.
    };
    template<special_id Id>
    struct special_type
    {
            static
          special_type*
        _(void)
        {
            static special_type a;
            return &a;
        }
        special_type(void)
        {}
        special_type(special_type const&)
        {}
        void operator=(special_type const&)
        {}
        bool operator==(special_type const&)const
        {
            return true;
        }
    };
    typedef special_type<empty_id> empty;
    typedef special_type<nothing_id> nothing;

}//exit special_components namespace
}//exit composite_storage namespace
}//exit boost namespace
#endif
