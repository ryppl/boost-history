// (C) Copyright Craig Henderson 2002
//               cdm.henderson@virgin.net
//
// Permission to use, copy, modify, distribute and sell this software
// and its documentation for any purpose is hereby granted without fee,
// provided that the above copyright notice appears in all copies and
// that both that copyright notice and this permission notice appear
// in supporting documentation.  The author makes no representations
// about the suitability of this software for any purpose.  It is
// provided "as is" without express or implied warranty.

// define BOOST_MMAP_WINDOWS or BOOST_MMAP_POSIX to determine the
// method of file management. this can be moved somewhere else
#ifdef _WIN32
#   define BOOST_MMAP_WINDOWS
#else
#   define BOOST_MMAP_POSIX
#endif

#if defined(BOOST_MMAP_WINDOWS)
#include <windows.h>
#elif defined (BOOST_MMAP_POSIX)
#include <io.h>
#include <fcntl.h>
#include <sys/mman.h>
#endif

#include <iostream>
#include "boost/config.hpp"
#include "boost/memmap.hpp"

void test1(void)
{
    typedef char mapped_file_t;
    boost::file_handle_t hFile;
    
#if defined(BOOST_MMAP_WINDOWS)
    hFile = ::CreateFile("memmap.cpp",
                         GENERIC_ALL, FILE_SHARE_READ, NULL, OPEN_EXISTING,
                         0, NULL);
#elif defined (BOOST_MMAP_POSIX)
    hFile = open("memmap.cpp", _O_RDWR);
#endif

    if (hFile == BOOST_FS_INVALID_HANDLE)
        std::cerr << "Failed to open the file" << std::endl;
    else
    {
        boost::memory_mapped_file<mapped_file_t> myfile(hFile, boost::readwrite);
        if (myfile.is_mapped())
        {
            //boost::memory_mapped_file<mapped_file_t> myfile1(hFile, boost::memory_mapped_file<mapped_file_t>::readonly);
            if (myfile.is_mapped())
            {
                mapped_file_t              *p1 = myfile.get();
                const mapped_file_t        *p2 = myfile.get();
                mapped_file_t       * const p3 = myfile.get();
                const mapped_file_t * const p4 = myfile.get();
                const boost::memory_mapped_file<mapped_file_t> *pmyfile = &myfile;
                const mapped_file_t *p5 = pmyfile->get();
                p1; p2; p3; p4; p5;

                std::cout << "Everything has worked Ok." << std::endl;

                boost::filesize_t file_size = boost::get_file_size(hFile);
                for (boost::filesize_t loop=1; loop < file_size; ++loop)
                    std::cout << *(p2++);

                myfile.release();
            }
            else
                std::cerr << "Failed to create the file mapping for read only" << std::endl;
        }
        else
            std::cerr << "Failed to create the file mapping for read write" << std::endl;

#if defined(BOOST_MMAP_WINDOWS)
        ::CloseHandle(hFile);
#elif defined (BOOST_MMAP_POSIX)
        close(hFile);
#endif
    }
    std::cout << std::endl;
}



void test2(void)
{
    typedef char mapped_file_t;
    
    boost::file<char> file("memmap.cpp", boost::readwrite);

    if (!file.is_open())
        std::cerr << "Failed to open the file" << std::endl;
    else
    {
        std::cout << "Successfully opened file: " << file.filepath().c_str() << std::endl;

        boost::memory_mapped_file<mapped_file_t> myfile(file, boost::readwrite);
        if (myfile.is_mapped())
        {
            if (myfile.is_mapped())
            {
                mapped_file_t              *p1 = myfile.get();
                const mapped_file_t        *p2 = myfile.get();
                mapped_file_t       * const p3 = myfile.get();
                const mapped_file_t * const p4 = myfile.get();
                const boost::memory_mapped_file<mapped_file_t> *pmyfile = &myfile;
                const mapped_file_t *p5 = pmyfile->get();
                p1; p2; p3; p4; p5;

                std::cout << "Everything has worked Ok." << std::endl;

                boost::filesize_t file_size = file.size();
                for (boost::filesize_t loop=1; loop < file_size; ++loop)
                    std::cout << *(p2++);

                myfile.release();
            }
            else
                std::cerr << "Failed to create the file mapping for read only" << std::endl;
        }
        else
            std::cerr << "Failed to create the file mapping for read write" << std::endl;
    }
    std::cout << std::endl;
}


void test3(void)
{
    boost::file<> newfile;
    if (!newfile.open_readonly("Hello World.txt"))
    {
        if (newfile.create("Hello World.txt"))
            std::cout << newfile.filepath().c_str() << " Created" << std::endl;

        newfile.open_readonly("Hello World.txt");
    }
}



int main(int, char **)
{
    try
    {
        test1();
        test2();
        test3();

        boost::file<> newfile;
        if (newfile.create("Hello.txt"))
        {
            std::cout << newfile.filepath().c_str() << " Created" << std::endl;
            newfile.close();
            if (newfile.open_readonly("Hello.txt"))
            {
                std::cout << newfile.filepath().c_str() << " Opened read-only" << std::endl;
                newfile.close();
                if (newfile.open_readwrite("Hello.txt"))
                    std::cout << newfile.filepath().c_str() << " Opened read-write" << std::endl;
                else
                    std::cerr << "Failed to open the file readwrite" << newfile.filepath().c_str() << ": Error " << newfile.error() << std::endl;
            }
            else
                std::cerr << "Failed to open the file readonly" << newfile.filepath().c_str() << ": Error " << newfile.error() << std::endl;
        }
        else
            std::cerr << "Failed to create file " << newfile.filepath().c_str() << ": Error " << newfile.error() << std::endl;
    }
    catch (const std::exception &e)
    {
        std::cerr << std::endl
                  << "Caught exception: "
                  << e.what()
                  << std::endl;
    }
    
    return 0;
}
