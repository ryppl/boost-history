//////////////////////////////////////////////////////////////////////////////
// main.cpp
//  (C) Copyright 2009 Erwann Rogard
//  Use, modification and distribution are subject to the
//  Boost Software License, Version 1.0. (See accompanying file
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
#include <libs/math/ifgt/src/example/truncation_properties.h>
#include <libs/math/ifgt/src/example/cluster.h>
#include <libs/math/ifgt/src/example/find_nearest_cluster.h>
#include <libs/math/ifgt/src/example/truncation_degree_constant.h>
#include <libs/math/ifgt/src/example/coefficients.h>
#include <libs/math/ifgt/src/example/coefficients_evaluator.h>
#include <libs/math/ifgt/src/example/cluster_evaluator.h>
#include <libs/math/ifgt/src/example/clusters_evaluator.h>
#include <libs/math/ifgt/src/example/fast_accumulator.h>
#include <libs/math/ifgt/src/example/fast_evaluator.h>
#include <libs/math/ifgt/src/example/exact_gauss_transform.h>
#include <libs/math/ifgt/src/example/for_each_accumulate.h>
#include <libs/math/ifgt/src/example/for_each_evaluate.h>
#include <libs/math/ifgt/src/example/benchmark_exact.h>
#include <libs/math/ifgt/src/example/benchmark_fast.h>

int main()
{

    example_truncation_properties();
    example_coefficients();
    example_coefficients_evaluator();
    example_cluster();
    example_find_nearest_cluster();
    example_truncation_degree_constant();
    example_cluster_evaluator();
    example_clusters_evaluator();
    example_exact_gauss_transform();
    example_fast_accumulator();
    example_fast_evaluator();
    example_for_each_accumulate();
    example_for_each_evaluate();
    example_benchmark_exact();
    example_benchmark_fast();
    return 0;
}
