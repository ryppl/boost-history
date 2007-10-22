namespace boost { namespace logging {

/** 
@page scenarios_code Code for the common scenarios

- @ref scenarios_code_1 
- @ref scenarios_code_2
- @ref scenarios_code_3
- @ref scenarios_code_4
- @ref scenarios_code_5
- @ref scenarios_code_6
- @ref scenarios_code_7
- @ref scenarios_code_8


\n\n\n
@section scenarios_code_1 Scenario 1, Common usage: Multiple levels, One logging class, Multiple destinations.

@include mul_levels_one_logger.cpp
\n\n\n




@section scenarios_code_2 Scenario 2: Multiple levels, Multiple logging classes, Multiple destinations

@include mul_levels_mul_logers.cpp
\n\n\n



@section scenarios_code_3 Scenario 3: No levels, One Logger, Multiple destinations, Custom route

@include no_levels_with_route.cpp
\n\n\n



@section scenarios_code_4 Scenario 4: No levels, Multiple Loggers, One Filter

@include mul_loggers_one_filter.cpp
\n\n\n



@section scenarios_code_5 Scenario 5: No levels, One Logger, One Filter

@include one_loger_one_filter.cpp
\n\n\n



@section scenarios_code_6 Scenario 6: Fastest: Multiple Loggers, One Filter, Not using Formatters/Destinations, Not using <<

@include fastest_no_ostr_like.cpp
\n\n\n



@section scenarios_code_7 Scenario 7: Fast: Multiple Loggers, One Filter, Not using Formatters/Destinations, Using <<

@include fastest_use_ostr_like.cpp
\n\n\n



@section scenarios_code_8 Scenario 8: Using custom formatters/destinations together with the existing formatters/destinations

@include custom_fmt_dest.cpp
\n\n\n


*/

}}
