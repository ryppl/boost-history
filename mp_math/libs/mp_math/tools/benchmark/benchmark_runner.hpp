// Copyright Kevin Sopp 2008.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_MP_MATH_TOOLS_BENCHMARK_RUNNER_HPP
#define BOOST_MP_MATH_TOOLS_BENCHMARK_RUNNER_HPP

#include <list>
#include "benchmark.hpp"


struct data_file
{
  struct column
  {
    std::string libname;
    std::vector<double> data;
    double total_ops;
    bool operator < (const column& rhs) const { return libname < rhs.libname; }
  };

  std::string op;

  std::list<column> cols;

  void write() const;
  void write_gnuplot_script(unsigned int x, unsigned int y) const;
};


struct benchmark_runner
{
  benchmark_runner(unsigned int num_input_samples,
                   unsigned long operand_size_beg,
                   unsigned long operand_size_end);

  // add a library
  void add_library(const boost::shared_ptr<benchmark_base>& b);
  
  // benches all operations given in ops
  void run(const std::vector<std::string>& ops, double sample_time);

  void write_data_files() const;
  void write_gnuplot_scripts(unsigned int x, unsigned int y) const;
  void write_summary_file() const;

  void write_results(unsigned int x, unsigned int y) const;

  void execute_bench_op(const std::string& op, double sample_time);

private:

  void init_libs();

  void create_data_files();

  void create_input_vectors(unsigned long min, unsigned long max);
  void write_input_vector_plotfile(unsigned int x, unsigned int y) const;

  unsigned long get_operand1_size(unsigned int) const;
  unsigned long get_operand2_size(unsigned int) const;
  
  typedef std::vector<boost::shared_ptr<benchmark_base> > library_vec;
  typedef std::list<benchmark_result>                     result_list;

  const unsigned int num_input_samples_;
  const unsigned long beg_;
  const unsigned long end_;
  std::vector<std::string> string_vec1_;
  std::vector<std::string> string_vec2_;
  library_vec libs_;
  result_list results_;

  std::list<data_file> dfiles_;
};


#endif

