#include <iostream>
#include <boost/iostreams/utility/indent_scoped_ostreambuf.hpp>
#include <boost/array_stepper/stepper_offset_print.hpp>
#include <boost/array_stepper/mk_iota.hpp>
#include <boost/array_stepper/vector_print.hpp>
#include <algorithm>

using namespace boost::array_stepper;

int main()
{
    boost::iostreams::indent_scoped_ostreambuf<char> indent_outbuf(std::cout,2);
    typedef stepper_offset<> sto_t;  
    typedef sto_t::lengths_t lengths_t;
    lengths_t lengths_v({-2,3,4});
    unsigned rank=lengths_v.size();
    std::cout<<"lengths_v="<<lengths_v;
    std::cout<<"***permut CTOR\n";
    std::cout<<indent_buf_in;
    sto_t::axes_permutation_t permut_v=mk_iota<sto_t::axis_t>( 0, rank);
    unsigned count=0;
    do
    {
        std::cout<<":count="<<count;
        std::cout<<":permut_v="<<permut_v;
        sto_t sto_v( lengths_v, permut_v);
        std::cout<<"stepper_ones=\n"<<sto_v.stepper_ones();
        std::cout<<"offset="<<sto_v.offset()<<"\n";
    }while 
      (  ++count< 10
      && std::next_permutation
         ( permut_v.begin()
         , permut_v.end()
         )
      );
    std::cout<<indent_buf_out;
    std::cout<<":permut_v="<<permut_v;
    std::cout<<"***permut VALU\n";
    std::cout<<indent_buf_in;
    sto_t::axes_permutation_t const permut_c=permut_v;
    count=0;
    do
    {
        std::cout<<":count="<<count;
        std::cout<<":permut_v="<<permut_v;
        sto_t sto_v( lengths_v, permut_c);
        sto_v.permute(permut_v);
        std::cout<<"stepper_ones=\n"<<sto_v.stepper_ones();
        std::cout<<"offset="<<sto_v.offset()<<"\n";
    }while 
      (  ++count< 10
      && std::next_permutation
         ( permut_v.begin()
         , permut_v.end()
         )
      );
    std::cout<<indent_buf_out;
    std::cout<<":permut_v="<<permut_v;
    return 0;
}    
