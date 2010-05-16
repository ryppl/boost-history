/*
 *  Creating process with child_create()
 */

#include <boost/process.hpp>
#include <vector> 
#include <iostream> 

using namespace boost::process; 

int main() 
{
        //simple usage
        child c1 = create_child( find_executable_in_path("hostname") ); 

        //usage with arguments
        std::vector<std::string> args;

        args.push_back("-a");
        args.push_back("-a");
        child c2 = create_child(find_executable_in_path("hostname"), args);

        //complete usage
        struct context ctx;
        ctx.environment.insert(std::make_pair("NEW_ENV_VARIABLE", "VALUE"));
        ctx.process_name = "My_process_name";
        ctx.stdin_behavior = inherit;
        ctx.stdout_behavior = closed;
        ctx.stderr_behavior = closed;

        child c3 = create_child(find_executable_in_path("hostname"), args,ctx);

        return 0;
} 

