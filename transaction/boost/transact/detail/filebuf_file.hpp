//          Copyright Stefan Strasser 2009 - 2010.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)


#ifndef BOOST_TRANSACT_DETAIL_FILEBUF_FILE_HPP
#define BOOST_TRANSACT_DETAIL_FILEBUF_FILE_HPP

#include <fstream>
#include <string>
#include <boost/mpl/size_t.hpp>
#include <boost/transact/exception.hpp>

#pragma warning(push)
#pragma warning(disable:4996)

namespace boost{
namespace transact{
namespace detail{

class filebuf_seq_ofile{
public:
	typedef unsigned long long size_type;
	explicit filebuf_seq_ofile(std::string const &name) : pos(0){
		if(!this->buf.open(name.c_str(),std::ios::out | std::ios::binary)) throw io_failure();
	}
	void save_binary(void const *data,mpl::size_t<1>){
		if(this->buf.sputc(*static_cast<char const *>(data)) == EOF) throw io_failure();
		++this->pos;
	}
	void save_binary(void const *data,std::size_t size){
		std::streamsize ret=this->buf.sputn(static_cast<char const *>(data),std::streamsize(size));
		this->pos+=ret;
		if(ret != std::streamsize(size)) throw io_failure();
	}
	template<class T>
	void save(T const &t){
		this->save_binary(&t,mpl::size_t<sizeof(T)>());
	}
	size_type position() const{ return this->pos; }
	void flush(){
		if(this->buf.pubsync() != 0) throw io_failure();
	}
	void sync(){
		throw unsupported_exception();
	}
private:
	std::filebuf buf;
	size_type pos;
};

class filebuf_seq_ifile{
public:
	typedef unsigned long long size_type;
	explicit filebuf_seq_ifile(std::string const &name) : pos(0){
		if(!this->buf.open(name.c_str(),std::ios::in | std::ios::binary)) throw io_failure();
	}
	void load_binary(void *dataptr,mpl::size_t<1>){
		char &data=*static_cast<char *>(dataptr);
		int ret=this->buf.sbumpc();
		if(ret == EOF) throw eof_exception();
		++this->pos;
		data=ret;
	}
	void load_binary(void *data,std::size_t size){
		std::streamsize ret=this->buf.sgetn(static_cast<char *>(data),std::streamsize(size));
		this->pos+=ret;
		if(ret != std::streamsize(size)){
			if(ret == 0) throw eof_exception();
			else throw io_failure();
		}
	}
	template<class T>
	void load(T &t){
		this->load_binary(&t,mpl::size_t<sizeof(T)>());
	}
	size_type position() const{ return this->pos; }
private:
	std::filebuf buf;
	size_type pos;
};



}
}
}

#pragma warning(pop)

#endif
