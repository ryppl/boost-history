
// Copyright (C) 2009-2011 Lorenzo Caminiti
// Use, modification, and distribution is subject to the Boost Software
// License, Version 1.0 (see accompanying file LICENSE_1_0.txt or a
// copy at http://www.boost.org/LICENSE_1_0.txt).

#include <boost/local/function.hpp>
#include <vector>
#include <algorithm>
#include <iostream>

class a {
public:
    a(const std::vector<double>& numbers): v_(numbers) {}

    void change_sign_all(const std::vector<int>& indices) {
        void BOOST_LOCAL_FUNCTION_PARAMS(int i, bind this) { // Bind this.
            // Local function uses special name `this_` to access bound object.
            this_->v_[i] = -this_->v_[i];
        } BOOST_LOCAL_FUNCTION_NAME(complement)
        std::for_each(indices.begin(), indices.end(), complement);
    }

    friend std::ostream& operator<<(std::ostream& s, const a& obj) {
        for (std::vector<double>::const_iterator i = obj.v_.begin();
                i != obj.v_.end(); ++i) {
            s << *i << " ";
        }
        return s;
    }

private:
    std::vector<double> v_;
};

int main() {
    std::vector<double> n(3, 0);
    n[0] = 1.11; n[1] = 2.22; n[2] = 3.33;

    std::vector<int> i(2, 0);
    i[0] = 0; i[1] = 2;
    
    a an(n);
    an.change_sign_all(i);
    std::cout << an << std::endl;
    
    return 0;
}

