#ifndef BOOST_TRANSACT_DETAIL_FILE_HPP
#define BOOST_TRANSACT_DETAIL_FILE_HPP

#include <boost/filesystem.hpp>
#include <boost/transact/exception.hpp>

namespace boost{
namespace transact{
namespace detail{

struct eof_exception : io_failure{};


#ifdef _WIN32
#include <Windows.h>
#include <WinBase.h>
	
// low-level ofile representation for WIN32
class ofile {
public:
	typedef unsigned int size_type;

	void* filedes;
	
	ofile(std::string const &name) : filedes(INVALID_HANDLE_VALUE) {
		unsigned long access = GENERIC_READ | GENERIC_WRITE;
		unsigned long creation_flags = OPEN_ALWAYS;
		unsigned long flags = FILE_FLAG_NO_BUFFERING | FILE_FLAG_WRITE_THROUGH;
		this->filedes = CreateFileA(name.c_str(), access,
										FILE_SHARE_READ | FILE_SHARE_WRITE | FILE_SHARE_DELETE,
										0, creation_flags, flags, 0);
		if (this->filedes == INVALID_HANDLE_VALUE ) {
			throw io_failure();
		}
		std::cerr << "File opened" << std::endl;
	}
	
	~ofile() {
		if(this->filedes != INVALID_HANDLE_VALUE) CloseHandle(this->filedes);
	}
	
	void seek(size_type const &s) {
		LARGE_INTEGER loc;
		loc.QuadPart = s;
		if(SetFilePointerEx(this->filedes, loc, NULL, FILE_BEGIN) == 0) {
			std::cerr << "SetFilePointerEx == 0" << std::endl;
			throw io_failure();
		}
	}
	
	size_type write(const char *data, size_type const &size) {
		DWORD written;
		if(WriteFile(this->filedes, data, size, &written, 0) == 0) {
			std::cerr << "WriteFile == 0" << std::endl;
			throw io_failure();
		}
		return (size_type)written;
	}
	
	void sync() { }
};

#else
	
#include <unistd.h>
#include <fcntl.h>
	
#ifndef _POSIX_SYNCHRONIZED_IO
#error no POSIX synchronized IO available
#endif

// low-level ofile for Linux/Unix
class ofile {
public:
	typedef unsigned int size_type;

	int filedes;
	
	ofile(std::string const &name) : filedes(-1) {
		int flags=O_CREAT | O_WRONLY;
#ifdef linux
		flags|=O_NOATIME;
#endif
		this->filedes= open(name.c_str(),flags,S_IRUSR | S_IWUSR);
		if(this->filedes==-1) throw io_failure();
		{ //make sure the directory entry has reached the disk:
			std::string dirname=filesystem::path(name).directory_string();
			if(dirname.empty()) dirname=".";
			int dirfd= open(dirname.c_str(),O_RDONLY);
			if(dirfd==-1) throw io_failure();
			int ret=::fsync(dirfd);
			if(::close(dirfd) != 0 || ret != 0) throw io_failure();
		}
	}
	
	~ofile() {
        if(this->filedes != -1) ::close(this->filedes);
    }
	
	void seek(size_type const &s) {
		if(::lseek(this->filedes,s,SEEK_SET) != off_t(s)) throw io_failure();
	}
	
	size_type write(const char *data, size_type const &size) {
		if(::write(this->filedes,data,size) != ssize_t(size)) throw io_failure();
		return size;
	}
	
	void sync() {
#ifdef linux
		if(::fdatasync(this->filedes) != 0) throw io_failure();
#else //multiple sources say fdatasync is not safe on other systems
		if(::fsync(this->filedes) != 0) throw io_failure();
#endif		
	}
};

#endif
		
}
}
}

#endif
