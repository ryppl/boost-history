//Purpose:
//  Demo array whose dimensions and sizes are defined at runtime.
//
#include <boost/iostreams/utility/indent_scoped_ostreambuf.hpp>
#include <boost/array_stepper/array_own_print.hpp>

using namespace boost::array_stepper;
typedef array<int> array_t;


void test(array_t::lengths_t const& a_lengths)
{
    unsigned const rank=a_lengths.size();
    std::cout<<"a_lengths["<<rank<<"]=\n";
    for(unsigned i=0; i<rank; ++i)
    {
        std::cout<<"  length["<<i<<"]="<<a_lengths[i]<<"\n";
    }
    for(unsigned dir_op=0; dir_op<permut_predef_num; ++dir_op)
    {
        std::cout<<"*************************\n";
        std::cout<<"dir_op="<<dir_op<<"\n";
        array_t ai( a_lengths, permut_predefined(dir_op));
        unsigned const size=ai.data().size();
        std::cout<<"my_data.size()="<<size<<"\n";
        std::cout<<"ai.offset="<<ai.offset()<<"\n";
        {
            unsigned value=1000;
            std::iota( ai.data().begin(), ai.data().end(), value);
        }
        std::cout<<"ai=\n";
        std::cout<<ai<<".\n";
    }
}    

int main(void)
{
    boost::iostreams::indent_scoped_ostreambuf<char> indent_outbuf(std::cout,2);
    {
        std::cout<<"***reorderings:\n";
        std::cout<<indent_buf_in;
        array_t::lengths_t const lengths0({ 2, 3, 4});
        test(lengths0);
        for(unsigned i=0; i< lengths0.size(); ++i)
        {
            array_t::lengths_t a_lengths(lengths0);
            a_lengths[i]=-a_lengths[i];
            test(a_lengths);
        }
        std::cout<<indent_buf_out;
    }
    {
        std::cout<<"***reshap:\n";
        std::cout<<indent_buf_in;
        array_t::lengths_t const lengths0({ 2, 3, 4});
        array_t ai( lengths0);
        {
            unsigned value=1000;
            std::iota( ai.data().begin(), ai.data().end(), value);
        }
        std::cout<<"ai before reshape=\n";
        std::cout<<indent_buf_in;
        std::cout<<ai;
        std::cout<<indent_buf_out;
        array_t::lengths_t const lengths1({6,4});
        ai.reshape(lengths1);
        std::cout<<"ai after reshape=\n";
        std::cout<<indent_buf_in;
        std::cout<<ai;
        std::cout<<indent_buf_out;
        std::cout<<indent_buf_out;
    }
    return 0;
}    
