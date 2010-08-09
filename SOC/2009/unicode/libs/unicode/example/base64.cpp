#include <boost/iterator/convert_iterator.hpp>
#include <boost/iterator/converter_codecvt.hpp>

#include <boost/range/as_literal.hpp>

#include <fstream>

struct base64_encoder
{
    typedef char input_type;
    typedef char output_type;
    
    typedef boost::mpl::int_<4> max_output;
    
    template<typename In, typename Out>
    Out ltr(In& begin, In end, Out out)
    {
        const char * lookup_table = 
            "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
            "abcdefghijklmnopqrstuvwxyz"
            "0123456789"
            "+/";
        
        char in[3];
        
        in[0] = *begin++;
        *out++ = lookup_table[in[0] >> 2];
        if(begin == end)
        {
            *out++ = lookup_table[(in[0] & 0x03) << 4];
            *out++ = '=';
            *out++ = '=';
            return out;
        }
        
        in[1] = *begin++;
        *out++ = lookup_table[((in[0] & 0x03) << 4) | (in[1] >> 4)];
        if(begin == end)
        {
            *out++ = lookup_table[(in[1] & 0x0f) << 2];
            *out++ = '=';
            return out;
        }
        
        in[2] = *begin++;
        *out++ = lookup_table[((in[1] & 0x0f) << 2) | (in[2] >> 6)];
        *out++ = lookup_table[in[2] & 0x3f];
        return out;
    }
    
    template<typename In, typename Out>
    Out rtl(In begin, In& end, Out out)
    {
        size_t to_read = std::distance(begin, end) % 3;
        if(!to_read)
            to_read = 3;
        
        char in[3];
        for(size_t i=0; i<to_read; i++)
            in[to_read-i-1] = *--end;
            
        char* b = in;
        return ltr(b, in+to_read, out);
    }
};

template<std::size_t N>
struct fixed_boundary
{
    typedef char input_type;
    
    template<typename In>
    bool operator()(In begin, In end, In pos)
    {
        return !(std::distance(begin, pos) % N);
    }
};

typedef boost::converter_codecvt<
    char,
    fixed_boundary<3>,
    base64_encoder,
    fixed_boundary<3>,
    base64_encoder
> base64_codecvt;

#define CHECK_EQUAL(a, b) if(a != b) std::unexpected();

int main()
{
    char data_in_[] = "fooba";
    char data_out_[] = "Zm9vYmE=";
    
    boost::iterator_range<const char*> data_in = boost::as_literal(data_in_);
    boost::iterator_range<const char*> data_out = boost::as_literal(data_out_);
    
    std::locale old_loc;
    std::locale loc(old_loc, new base64_codecvt(boost::size(data_in)));
    {
        std::ofstream ofs("test.base64");
        ofs << data_in;
    }
    
    std::ifstream ifs("test.base64");
    ifs.imbue(loc);
    
    char c;
    size_t i = 0;
    while(ifs.get(c))
    {
        CHECK_EQUAL(c, data_out[i]);
        ++i;
    }
    CHECK_EQUAL(i, (size_t)boost::size(data_out));
}
