/*
 *  filewrite.cpp
 *  transact
 *
 *  Copyright Bob Walters 2010
 *  Distributed under the Boost
 *  Software License, Version 1.0.
 *  (See accompanying file LICENSE_1_0.txt or
 *  copy at http://www.boost.org/LICENSE_1_0.txt)
 */
#include <iostream>

#include <boost/transact/detail/aligning_file.hpp>
#include <boost/transact/detail/buffering_file.hpp>
#include <boost/transact/detail/sectorizing_file.hpp>
#include <boost/transact/detail/syncing_file.hpp>
#include <boost/transact/detail/file.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>

static const int loop_size=1000;

static char txn_buffer[ 10000 ]; // random data set

using namespace boost::transact;
using namespace boost::posix_time;

typedef detail::sectorizing_seq_ofile<
			detail::aligning_seq_ofile<
				detail::buffering_seq_ofile<
					detail::syncing_seq_ofile< detail::ofile<false> >,
					8192
				>
			>
		>
ofile_t;


typedef detail::sectorizing_seq_ofile<
			detail::aligning_seq_ofile<
				detail::buffering_seq_ofile<
					detail::syncing_seq_ofile< detail::direct_ofile >,
					8192
				>
			>
		>
direct_ofile_t;

template <class file_t>
void filetest1(const char *filename, size_t txn_size) {
	file_t outfile(filename);	
	
	ptime start = microsec_clock::local_time();	
	for (int i=0; i<loop_size; i++) {
		outfile.write(txn_buffer, txn_size);
		outfile.sync();
	}
	ptime end = microsec_clock::local_time();

	std::cout << "Written " << loop_size << " txns, " 
			<< loop_size*txn_size << " bytes, in "
			<< (end-start) << " microseconds"
			<< std::endl;
}


int main(int, const char *[]){
	// write loop_size transactions to disk, each 3k in size.
    filetest1<ofile_t>("filetest1.out", 3000);
    filetest1<direct_ofile_t>("filetest2.out", 3000);

	// write loop_size transactions to disk, each 10k in size.
	filetest1<ofile_t>("filetest3.out", 10000);
    filetest1<direct_ofile_t>("filetest4.out", 10000);

    return 0;
}
