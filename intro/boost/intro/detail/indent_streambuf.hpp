#ifndef BOOST_INTRO_INDENT_STREAMBUF_HPP
#define BOOST_INTRO_INDENT_STREAMBUF_HPP

#include <streambuf>

namespace boost{
namespace intro{
namespace detail{

//TODO doesn't set pointers correctly on failure
class indent_streambuf : public std::streambuf{
    typedef std::streambuf base_type;
public:
    explicit indent_streambuf(std::streambuf &base,bool indent)
        : base(base),indent(indent){}
    virtual int overflow(int c){
        if(!this->flush()) return EOF;
        return this->write(c) ? c : EOF;
    }
    virtual int sync(){
        return this->flush() ? 0 : -1;
    }
private:
    bool write(char c){
        if(this->indent){
            if(base.sputn("  ",2) != 2) return false;
            this->indent=false;
        }
        if(base.sputc(c) == EOF) return false;
        if(c == '\n') this->indent=true;
        return true;
    }
    bool flush(){
        for(char *c=pbase();c != pptr();++c){
            if(!this->write(*c)) return false;
        }
        setp(pbase(),pbase());
        return true;
    }

    std::streambuf &base;
    bool indent;
};


}
}
}

#endif
