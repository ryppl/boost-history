# template file

def extract_optional_list ( option ):
    result = template.options.get( option, None, '')
    if result=='':
        result = []
    else:
        result = result.split(",")
    return result

def typename ( list ):
    return [("typename " + x) for x in list]

filename = os.path.basename(template.item.destination)
include_guard = "BOOST__DATAFLOW__GENERIC__" + filename.rpartition("/")[2].replace(".", "_").upper()

enable = ["typename Enable=void"]

operation = template.options.get('NAME')
operation_impl = operation + '_impl'
operation_will_succeed = template.options.get('NAME') + '_will_succeed'
operation_will_succeed_impl = operation_will_succeed + '_impl'
operation_check = template.options.get('CHECK')
has_traits = template.options.get('HAS_TRAITS')

extra_args = extract_optional_list('EXTRA_ARGS')

includes = extract_optional_list('INCLUDES')

templates = extract_optional_list('TEMPLATES')
inner_templates = extract_optional_list('INNER_TEMPLATES')

arity = template.options.get('ARITY')
operation_check = template.options.get('CHECK')
operation_check_specialized = operation_check + '_specialized'

arity_range = [(str(x)) for x in range(1,int(arity)+1)]

traits = [("Traits" + x) for x in arity_range]
frameworks = [("Framework" + x) for x in arity_range]
ts = [("T" + x) for x in arity_range]
check_has_traits = [(has_traits + "<T"+x+",Framework"+x+">") for x in arity_range]

typename_frameworks_wdefaults = [("typename Framework" + x + "=typename default_framework_of<T" + x + ">::type") for x in arity_range]
default_frameworks = [("typename default_framework_of<T" + x + ">::type") for x in arity_range]
get_traits = [("typename traits_of<T"+x+", Framework"+x+">::type") for x in arity_range]

targs = [("T" + x + " &t" + x) for x in arity_range]
targs_names = [("t" + x) for x in arity_range]
extra_args_args = [("const " + x + " &" + x.lower()) for x in extra_args]
extra_args_names = [(x.lower()) for x in extra_args]
default_extra_args_args = [(extra_args_args[x] + '=' + extra_args[x] + '()') for x in range(len(extra_args))]

content = """
namespace boost { namespace dataflow {

namespace extension
{
    template<""" + ','.join(typename(traits) + typename(templates) + enable) + """>
    struct """ + operation_impl + """
    {
        BOOST_MPL_ASSERT(( is_same<Enable, void> ));
        
        typedef detail::not_specialized result_type;

        template<""" + ','.join(typename(ts) + typename(inner_templates))+""">
        result_type operator()(""" + ','.join(targs + extra_args_args) + """)
        {
            // Error: """ + operation_impl + """ has not been
            // specialized appropriately.
            BOOST_MPL_ASSERT(( mpl::bool_<sizeof(T1)==0> ));
            return result_type();
        }
    };
    
    template<""" + ','.join(typename(traits) + typename(templates) + enable) + """>
    struct """ + operation_will_succeed_impl + """
    {
        BOOST_MPL_ASSERT(( is_same<Enable, void> ));
        
        typedef bool result_type;

        template<""" + ','.join(typename(ts) + typename(inner_templates))+""">
        result_type operator()(""" + ','.join(targs + extra_args_args) + """)
        {
            return
                !is_same<
                        typename  extension::""" + operation_impl + """<
                            """ + ','.join(traits + templates) + """
                            >::result_type,
                    detail::not_specialized
                >::value;
        }
    };
}

namespace detail
{

    template<""" + ','.join(typename(ts) + typename(templates) + typename(inner_templates) + typename_frameworks_wdefaults + enable) + """>
    struct """ + operation_check_specialized + """
        : public mpl::true_
    {};
    
    template<""" + ','.join(typename(ts) + typename(templates) + typename(inner_templates) + typename(frameworks)) + """>
    struct """ + operation_check_specialized + """<
        """ + ','.join(ts + templates + inner_templates + frameworks) + """
        ,
        typename enable_if<
            is_same<
               typename  extension::""" + operation_impl + """<
                """ + ','.join(get_traits + templates) + """
                >::result_type,
                detail::not_specialized>
        >::type>
        : public mpl::false_
    {};

}

template<""" + ','.join(typename(ts) + typename(templates) + typename_frameworks_wdefaults + enable) + """>
struct """ + operation_check + """
    : public mpl::false_
{
    BOOST_MPL_ASSERT((mpl::and_<""" + ','.join(check_has_traits) + """ >));
};

template<""" + ','.join(typename(ts) + typename(templates) + typename(frameworks)) + """>
struct """ + operation_check + """<
    """ + ','.join(ts + templates  + frameworks) + """,
    typename enable_if<
        detail::"""+operation_check_specialized+"""<
            """ + ','.join(ts + templates + frameworks) + """>
     >::type>
    : public mpl::true_
{
    BOOST_MPL_ASSERT((mpl::and_<""" + ','.join(check_has_traits) + """ >));
};

namespace result_of {

    template<""" + ','.join(typename(ts) + typename(templates) + typename(inner_templates) + typename_frameworks_wdefaults) + """>
    struct """ + operation + """
    {
        typedef typename boost::result_of<
            extension::""" + operation_impl + """<
                """ + ','.join(get_traits + templates) + """
            >
            (""" + ','.join(ts + extra_args) + """)
        >::type type;
    };
    
}

"""

for i in range(0,2**int(arity)):

    consted_targs = [(['','const '][(i & 2**x)>0] + targs[x] ) for x in range(len(targs))]
    
    content = content + """
template<""" + ','.join(typename(templates) + typename(inner_templates) + typename(frameworks) + typename(ts)) + """>
inline typename result_of::""" + operation + """<
    """ + ','.join(ts + templates + inner_templates + frameworks) + """
    >::type 
""" + operation + "_framework("+ ','.join(consted_targs + default_extra_args_args) + """)
{
    return extension::""" + operation_impl + """<
        """ + ','.join(get_traits + templates) + """
        >()(""" + ','.join(targs_names + extra_args_names) + """);
}
"""

    content = content + """
template<""" + ','.join(typename(templates) + typename(inner_templates)+ typename(ts)) + """>
inline typename result_of::""" + operation + """<
    """ + ','.join(ts + templates + inner_templates + default_frameworks) + """
    >::type 
""" + operation + "("+ ','.join(consted_targs + default_extra_args_args) + """)
{
    """ + '\n'.join([('typedef ' + default_frameworks[x] + ' Framework' + str(x+1) + ';') for x in range(len(default_frameworks))]) + """
    return extension::""" + operation_impl + """<
        """ + ','.join(get_traits + templates) + """
        >()(""" + ','.join(targs_names + extra_args_names) + """);
}
"""

    content = content + """
template<""" + ','.join(typename(templates) + typename(inner_templates) + typename(frameworks) + typename(ts)) + """>
inline bool
""" + operation_will_succeed + "_framework("+ ','.join(consted_targs + default_extra_args_args) + """)
{
    return extension::""" + operation_will_succeed_impl + """<
        """ + ','.join(get_traits + templates) + """
        >()(""" + ','.join(targs_names + extra_args_names) + """);
}
"""

    content = content + """
template<""" + ','.join(typename(templates) + typename(inner_templates) + typename(ts)) + """>
inline bool 
""" + operation_will_succeed + "("+ ','.join(consted_targs + default_extra_args_args) + """)
{
    """ + '\n'.join([('typedef ' + default_frameworks[x] + ' Framework' + str(x+1) + ';') for x in range(len(default_frameworks))]) + """
    return extension::""" + operation_will_succeed_impl + """<
        """ + ','.join(get_traits + templates) + """
        >()(""" + ','.join(targs_names + extra_args_names) + """);
}
"""

content = content + """
}}
"""

template.append_content(
"""/*=================================---------------------------------------------
    Copyright 2007,2008 Stjepan Rajko
  
    Distributed under the Boost Software License, Version 1.0.
    (See accompanying file LICENSE_1_0.txt or copy at
    http://www.boost.org/LICENSE_1_0.txt)
-----------------------------------------------===============================*/

#ifndef """ + include_guard + """
#define """ + include_guard + """


#include <boost/mpl/assert.hpp>
#include <boost/utility/result_of.hpp>
#include <boost/type_traits/is_same.hpp>
#include <boost/dataflow/generic/framework_entity/default_framework_of.hpp>
#include <boost/dataflow/generic/framework_entity/traits_of.hpp>
#include <boost/dataflow/detail/not_specialized.hpp>


""" + "\n".join(('#include ' + x) for x in includes) + content + """

#endif // """ + include_guard + """
""")


