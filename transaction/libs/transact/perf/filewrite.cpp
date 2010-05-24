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

static const int num_sets=100;
static const int txns_per_set=1000;
static const int txn_size=3000;  // typical transaction log size
static int total_txns = 0;
static int total_bytes = 0;

static char txn_buffer[ txn_size ]; // random data set

using namespace boost::transact;

typedef detail::sectorizing_seq_ofile<
			detail::aligning_seq_ofile<
				detail::buffering_seq_ofile<
					detail::syncing_seq_ofile,
					8192
				>
			>
		>
ofile_t;


void log_a_set(ofile_t &outfile) {
	for (int i=0; i<txns_per_set; i++) {
		outfile.write(txn_buffer, txn_size);
	}
	total_txns += txns_per_set;
	total_bytes += (txns_per_set * txn_size); 
	std::cout << "Written " << total_txns << " txns, " << total_bytes << " bytes" << std::endl;
}

void filetest1() {
	ofile_t outfile("filetest1.out");
	for (int i=0; i<num_sets; i++) {
		log_a_set(outfile);
	}
}

int main(int, const char *[]){
    filetest1();
    return 0;
}
