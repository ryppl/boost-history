//          Copyright Stefan Strasser 2009 - 2010.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)


#ifndef BOOST_TRANSACT_DETAIL_SYNCING_FILE_HPP
#define BOOST_TRANSACT_DETAIL_SYNCING_FILE_HPP

#include <string>
#include <cstring>
#include <boost/filesystem.hpp>
#include <boost/mpl/size_t.hpp>
#include <boost/static_assert.hpp>
#include <boost/assert.hpp>
#include <boost/config.hpp>

#ifdef WIN32

//TODO

#else

#include <unistd.h>
#include <fcntl.h>

#ifndef _POSIX_SYNCHRONIZED_IO
#error no POSIX synchronized IO available
#endif

#ifndef _LARGEFILE64_SOURCE
#error need POSIX Large File Support extension
#endif

#endif


namespace boost{
namespace transact{
namespace detail{

class syncing_seq_ofile{
public:
	typedef unsigned long long size_type;
	explicit syncing_seq_ofile(std::string const &name);
	void save_binary(void const *data,std::size_t size);
	template<class T>
	void save(T const &t){
		this->save_binary(&t,mpl::size_t<sizeof(T)>());
	}
	size_type position() const{ return this->pos; }
	void flush();
	void sync();
private:
	size_type pos;
#ifdef WIN32
#else
public:
	~syncing_seq_ofile(){
		if(this->filedes != -1) ::close(this->filedes);
	}
private:
	void write_ahead(size_type const &s){
		BOOST_ASSERT(s % write_ahead_size == 0);
		if(this->pos != s){
			if(::lseek64(this->filedes,s,SEEK_SET) != off64_t(s)) throw io_failure();
		}
		char data[write_page_size]; memset(data,0,write_page_size);
		BOOST_STATIC_ASSERT(write_ahead_size % write_page_size == 0);
		for(std::size_t c=0;c<write_ahead_size / write_page_size;++c){
			if(::write(this->filedes,data,write_page_size) != ssize_t(write_page_size)) throw io_failure();
		}
		if(::fsync(this->filedes) != 0) throw io_failure();
		if(::lseek64(this->filedes,this->pos,SEEK_SET) != off64_t(this->pos)) throw io_failure();
	}
	void write_ahead(size_type const &start,size_type const &end){
		BOOST_ASSERT(start % write_ahead_size == 0);
		BOOST_ASSERT(end % write_ahead_size == 0);
		for(size_type off=start;off < end;off+=write_ahead_size) this->write_ahead(off);
	}

	static std::size_t const write_ahead_size=10*1024*1024;
	static std::size_t const write_page_size=4096;

	int filedes;
#endif
};

#ifdef WIN32
#else

inline syncing_seq_ofile::syncing_seq_ofile(std::string const &name)
	: pos(0)
	, filedes(-1){
	int flags=O_CREAT | O_WRONLY;
#ifdef linux
	flags|=O_NOATIME;
#endif
	this->filedes=::open(name.c_str(),flags,S_IRUSR | S_IWUSR);
	if(this->filedes==-1) throw io_failure();
	{ //make sure the directory entry has reached the disk:
		std::string dirname=filesystem::path(name).directory_string();
		if(dirname.empty()) dirname=".";
		int dirfd=::open(dirname.c_str(),O_RDONLY);
		if(dirfd==-1) throw io_failure();
		int ret=::fsync(dirfd);
		if(::close(dirfd) != 0 || ret != 0) throw io_failure();
	}
	this->write_ahead(0);
}

void syncing_seq_ofile::save_binary(void const *data,std::size_t size){
	size_type const s=this->pos % write_ahead_size;
	if(s + size >= write_ahead_size){ //there must be at least one 0 at the and, so also write ahead if this is equal.
		size_type start=this->pos - s + write_ahead_size;
		size_type end=start+((s + size)/write_ahead_size) * write_ahead_size; //usually == start + write_ahead_size, but "size" can theoretically span a whole write_ahead_size
		BOOST_ASSERT(end > start);
		this->write_ahead(start,end);
	}

	ssize_t ret=::write(this->filedes,data,size);
	if(ret > 0) this->pos+=ret;
	if(ret != ssize_t(size)) throw io_failure();
}


inline void syncing_seq_ofile::flush(){}
inline void syncing_seq_ofile::sync(){
#ifdef linux
	if(::fdatasync(this->filedes) != 0) throw io_failure();
#else //multiple sources say fdatasync is not safe on other systems
	if(::fsync(this->filedes) != 0) throw io_failure();
#endif
}


#endif

}
}
}



#endif
