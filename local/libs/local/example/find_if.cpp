
// Copyright (C) 2009-2011 Lorenzo Caminiti
// Use, modification, and distribution is subject to the Boost Software
// License, Version 1.0 (see accompanying file LICENSE_1_0.txt or a
// copy at http://www.boost.org/LICENSE_1_0.txt).

//[find_if_cpp
#include <boost/local/function.hpp>
#include <list>
#include <algorithm>
#include <iostream>

class employee {
public:
    explicit employee(const double& salary): salary_(salary) {}
    double salary() const { return salary_; }
private:
    double salary_;
};

int main() {
    std::list<employee> employees;
    employees.push_back(employee( 85000.00));
    employees.push_back(employee(100000.00));
    employees.push_back(employee(120000.00));

    double min_salary = 100000.00;
    double u_limit = 1.1 * min_salary;

    bool BOOST_LOCAL_FUNCTION_PARAMS( (const employee& e)
            (const bind& min_salary) (const bind& u_limit) ) {
        return e.salary() >= min_salary && e.salary() < u_limit;
    } BOOST_LOCAL_FUNCTION_NAME(between)
    
    // Pass local function to an STL algorithm as a template paramter (this
    // cannot be done with plain member funcitons of local classes).
    std::list<employee>::iterator i = std::find_if(
            employees.begin(), employees.end(), between);

    if (i != employees.end()) { std::cout << i->salary() << std::endl; }
    return 0;
}
//]

