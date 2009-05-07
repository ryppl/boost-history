#ifndef BOOST_FTMPL_ALIAS_HPP
#define BOOST_FTMPL_ALIAS_HPP

// ToDo: Remove typeof requirement
#define BOOST_FTMPL_TYPE_ALIAS( name, expr )                                   \
typedef decltype( expr ) name;

#define BOOST_FTMPL_FUN_ALIAS( name, expr )                                    \
static decltype( expr ) name();

#endif // BOOST_FTMPL_ALIAS_HPP
