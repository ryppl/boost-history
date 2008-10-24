// Copyright Kevin Sopp 2008.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#include <cmath>
#include <fstream>
#include <iostream>
#include <numeric> // accumulate
#include <stdexcept>
#include <sstream>
#include "benchmark_runner.hpp"


void benchmark_runner::run(const std::vector<std::string>& ops,
                           double sample_time)
{
  init_libs();

  for (std::vector<std::string>::const_iterator op = ops.begin(); op != ops.end(); ++op)
    execute_bench_op(*op, sample_time);

  std::cout << "creating data files..." << std::endl;
  create_data_files();
}

unsigned long
benchmark_runner::get_operand1_size(unsigned int sample_number) const
{
  // linear growth
  const double m = static_cast<double>(end_ - beg_) /
                   static_cast<double>(num_input_samples_);
  return (m * sample_number) + beg_;
}

unsigned long
benchmark_runner::get_operand2_size(unsigned int sample_number) const
{
  // modified sine curve
  const double pi = 3.141592654;
  const double sample_num_to_rad = num_input_samples_ / (4.5 * pi);
  
  // scale sample_number into the range [0...4.5*PI]
  const double x = sample_number / sample_num_to_rad;

  const double y_val = std::sin(x) + 1.0;

  const double y_top = get_operand1_size(sample_number);
  const double p0 = pi;
  double y_bottom;
  if (x <= p0)
    y_bottom = y_top / (x / p0 * 2.0 + 1.0) + 1.0;
  else
    y_bottom = y_top / 3.0;

  // we want to scale the curve at the point p1 to be above y_top at x = 2.5*pi
  const double p1 = 2.5 * pi;
  
  // here we use a function of the form y = -ax^2 + b to scale the curve at (and
  // around) p1
  const double a = 1.0/(p1 * p1);
  double adjust = -a * ((x-p1) * (x-p1)) + 1.0;
  // scale to be 10% above p1
  adjust *= 1.1;
  // stop scaling once we're past p1 and 'adjust' falls below 1.0
  if (x > p1 && adjust < 1.0)
    adjust = 1.0;
  
  return static_cast<unsigned long>(y_bottom * adjust * y_val + y_bottom);
}

benchmark_runner::benchmark_runner(unsigned int num_input_samples,
                                   unsigned long operand_size_beg,
                                   unsigned long operand_size_end)
:
  num_input_samples_(num_input_samples),
  beg_(operand_size_beg ? operand_size_beg : 1), // zero length not allowed
  end_(operand_size_end)
{
}

void benchmark_runner::add_library(const boost::shared_ptr<benchmark_base>& b)
{
  libs_.push_back(b);
}

void benchmark_runner::create_input_vectors(unsigned long min, unsigned long max)
{
  std::ofstream iv_file("input_vecs.dat");
  if (!iv_file.is_open())
    throw std::runtime_error("couldn't open file input_vecs.dat");

  std::cout << "Creating " << num_input_samples_
            << " operand indices for numbers between "
            << min << " and " << max << " bits."
            << std::endl;
  
  const std::string rn = create_random_hex_string(max);

  std::cout << "creating input vectors";
  std::cout.flush();

  // create input vector where number size is determined by the sine function
  const unsigned int stepsize = num_input_samples_ / 10;
  for (unsigned int i = 0; i < num_input_samples_; ++i)
  {
    const unsigned long size1 = get_operand1_size(i);
    const unsigned long size2 = get_operand2_size(i);

    // divide by 4 because rn has max/4 hex digits
    string_vec1_.push_back(rn.substr(0, size1/4));
    string_vec2_.push_back(rn.substr(0, size2/4));
    
    // output the input sizes for a graph here
    iv_file << size1 << "\t" << size2 << "\n";
    
    if (((i+1) % stepsize) == 0)
    {
      std::cout << ".";
      std::cout.flush();
    }      
  }

  std::cout << std::endl;

  iv_file.close();
}

void benchmark_runner::write_input_vector_plotfile(unsigned int x, unsigned int y) const
{
  std::ofstream pf("input_vecs.plt");
  if (!pf.is_open())
    throw std::runtime_error("couldn't open plotfile");
  pf << "set term png size " << x << "," << y << "\n"
        "set output \"input_vecs.png\"\n"
        "set title \"input size of operands\"\n"
        "set key left\n"
        "set xlabel \"operand number\"\n"
        "set ylabel \"size in bits\"\n"
        "plot \"input_vecs.dat\" using 0:1 title \"operand 1 size\" with lines,\\\n"
        "     \"input_vecs.dat\" using 0:2 title \"operand 2 size\" with lines";
}

void benchmark_runner::init_libs()
{
  const unsigned long min_size = 
    std::min(get_operand1_size(0),
             get_operand2_size(0));
  
  const unsigned long max_size = 
    std::max(get_operand1_size(num_input_samples_),
             get_operand2_size(num_input_samples_));

  create_input_vectors(min_size, max_size);

  const unsigned int stepsize = num_input_samples_ / 10;
  for (library_vec::iterator lib = libs_.begin(); lib != libs_.end(); ++lib)
  {
    (*lib)->set_num_input_samples(num_input_samples_);
    
    // init ctor strings
    std::string hex_s = create_random_hex_string(max_size);
    std::string dec_s = create_random_dec_string(max_size);

    for (unsigned int i = 0; i < num_input_samples_; ++i)
    {
      (*lib)->hex_str.push_back(hex_s.substr(0, get_operand1_size(i)/4));
      (*lib)->dec_str.push_back(dec_s.substr(0, get_operand1_size(i)));
    }

    
    std::cout << (*lib)->name() << "->"
              << "constructing operand 1 input vector";
    std::cout.flush();

    for (unsigned int i = 0; i < num_input_samples_; ++i)
    {
      (*lib)->construct_operand_1(string_vec1_[i], i);
      if (((i+1) % stepsize) == 0)
      {
        std::cout << ".";
        std::cout.flush();
      }
    }
    std::cout << std::endl;

    std::cout << (*lib)->name() << "->"
              << "constructing operand 2 input vector";
    std::cout.flush();

    for (unsigned int i = 0; i < num_input_samples_; ++i)
    {
      (*lib)->construct_operand_2(string_vec2_[i], i);
      if (((i+1) % stepsize) == 0)
      {
        std::cout << ".";
        std::cout.flush();
      }
    }
    std::cout << std::endl;
  }
}

void benchmark_runner::execute_bench_op(const std::string& op, double sample_time)
{
  for (library_vec::iterator lib = libs_.begin(); lib != libs_.end(); ++lib)
  {
    benchmark_result r;
    
    r.libname = (*lib)->name() + "-" + (*lib)->version();
    r.op = op;
    r.sample_time = sample_time;

    (*lib)->run(r, op, sample_time);

    results_.push_back(r);
  }
}

void benchmark_runner::create_data_files()
{
  result_list::const_iterator r = results_.begin();
  
  // expects result list to be sorted by op name
  while (r != results_.end())
  {
    data_file d;
    result_list::const_iterator cur = r;
    while (r != results_.end() && r->op == cur->op)
    {
      data_file::column c;

      c.libname = r->libname;
      c.data = r->ops;
      
      // scale from ops per sample_time_ to ops per second
      /*const double scale = 1.0 / r->sample_time;
      for (std::vector<double>::iterator it = c.data.begin();
          it != c.data.end(); ++it)
        *it *= scale;*/

      c.total_ops = std::accumulate(r->ops.begin(), r->ops.end(), 0.);
      d.cols.push_back(c);
      ++r;
    }

    d.op = cur->op;

    d.cols.sort();
    dfiles_.push_back(d);
  }
}

void benchmark_runner::write_data_files() const
{
  for (std::list<data_file>::const_iterator it = dfiles_.begin();
      it != dfiles_.end(); ++it)
    it->write();
}

void benchmark_runner::write_gnuplot_scripts(unsigned int x, unsigned int y) const
{
  write_input_vector_plotfile(x, y);
  for (std::list<data_file>::const_iterator it = dfiles_.begin();
      it != dfiles_.end(); ++it)
    it->write_gnuplot_script(x, y);
}

void benchmark_runner::write_summary_file() const
{
  std::ostringstream s;
  for (std::list<data_file>::const_iterator d = dfiles_.begin();
      d != dfiles_.end(); ++d)
  {
    s << d->op << "\n";
    for (std::list<data_file::column>::const_iterator c = d->cols.begin();
        c != d->cols.end(); ++ c)
    {
      s << c->libname << " total ops = " << c->total_ops << "\n";
    }
  }

  std::cout << s.str();

  std::ofstream file("summary.txt");
  if (!file.is_open())
    throw std::runtime_error("couldn't open summary.txt file");
  file << s.str();
}

void benchmark_runner::write_results(unsigned int x, unsigned int y) const
{
  std::cout << "writing data files..." << std::endl;
  write_data_files();
  std::cout << "writing gnuplot scripts..." << std::endl;
  write_gnuplot_scripts(x, y);
  write_summary_file();
}


void data_file::write() const
{
  const std::string filename(op + ".dat");
  std::ofstream file(filename.c_str());
  if (!file.is_open())
    throw std::runtime_error("couldn't open data file");

  const unsigned int col_len = cols.front().data.size();
  for (unsigned int i = 0; i < col_len; ++i)
  {
    std::list<column>::const_iterator it = cols.begin();
    
    while (it != cols.end())
    {
      file << it->data.at(i);
        
      if (++it != cols.end())
        file << "\t";
    }
    file << "\n";
  }
}

void data_file::write_gnuplot_script(unsigned int x, unsigned int y) const
{
  const std::string filename(op + ".plt");
  std::ofstream file(filename.c_str());
  if (!file.is_open())
    throw std::runtime_error("couldn't open plot file");

  file << "set title \"" << op << "\"\n"
          "set xlabel " << "\"operand index\"\n"
          "set ylabel " << "\"ops/msec\"\n"
          "set autoscale\n"
          "set term png size " << x << "," << y << "\n"
          "set output \"" << op << ".png\"\n";
  file << "plot \\\n";
  
  std::list<column>::const_iterator it = cols.begin();
  int count = 1;
  while (it != cols.end())
  {
    file << '"' << op << ".dat\" using 0:" << count++ << " title \""
         << it->libname << "\" with lines";
    if (++it != cols.end())
      file << ",\\\n";
  }
  file << std::endl;
}

