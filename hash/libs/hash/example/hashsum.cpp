
//
// Copyright 2010 Scott McMurray.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
//

//
// Define HASH to one of the models of the HashAlgorithm concept.
// Do not include the boost::hashes:: namespace qualifier.
//

#ifndef HASH
#define HASH cubehash<80*4>
#endif

#include <boost/hash.hpp>

#define XSTR(s) STR(s)
#define STR(s) #s

#include <fstream>
#include <iostream>
#include <iterator>
#include <string>

#ifdef USE_MMAP
#include <fcntl.h>
#include <sys/mman.h>
#endif

typedef boost::hashes::HASH HashAlgorithm;

HashAlgorithm::digest_type
hash_streambuf(std::streambuf *sbuf) {
#ifdef BOOST_HASH_NO_OPTIMIZATION
    return boost::hashes::compute_digest<HashAlgorithm>(
               std::istreambuf_iterator<char>(sbuf),
               std::istreambuf_iterator<char>()
           );
#else
    HashAlgorithm::stream_hash<8>::type hash;
    for (;;) {
        boost::array<char, 8*1024> buf;
        std::streamsize n = sbuf->sgetn(&buf[0], buf.size());
        if (!n) break;
        hash.update_n(buf.begin(), n);
    }
    return hash.end_message();
#endif           
}

HashAlgorithm::digest_type
hash_memory(void *buf, size_t n) {
    return boost::hashes::compute_digest_n<HashAlgorithm>((char*)buf, n);
}

std::ostream &
do_istream(std::ostream &sink, std::string const &fn, std::istream &source) {
    return sink << hash_streambuf(source.rdbuf())
                << "  " << fn << std::endl;
}

#ifdef USE_MMAP
std::ostream &
do_memory(std::ostream &sink, std::string const &fn, void *buf, size_t n) {
    return sink << hash_memory(buf, n)
                << "  " << fn << std::endl;
}
#endif

std::ostream &
do_file(std::ostream &sink, std::string const &fn) {
#ifdef USE_MMAP
    int fd = open(fn.c_str(), O_RDONLY | O_NOATIME);
    size_t len = lseek(fd, 0, SEEK_END);
    void *p = mmap(NULL, len, PROT_READ, MAP_SHARED, fd, 0);
    return do_memory(sink, fn, p, len);
#else
    std::ifstream f(fn.c_str());
    return do_istream(sink, fn, f);
#endif
}

int main(int argc, char **argv) {
    std::cerr << "Using boost::hashes::" XSTR(HASH) "\n";
    if (argc < 2) {
        do_istream(std::cout, "-", std::cin);
    } else {
        while (--argc) {
            do_file(std::cout, *++argv);
        }
    }
}

