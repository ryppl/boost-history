#include "test_functor_impl.hpp"

p_functor_class_char::p_functor_class_char() : data(' ') { }
p_functor_class_char::p_functor_class_char(char c) : data(c) { }
char p_functor_class_char::operator()() { return data; }
void p_functor_class_char::operator()(char d) { data = d; }

p_functor_class_int::p_functor_class_int() : data(0) { }
p_functor_class_int::p_functor_class_int(int i) : data(i) { }
int p_functor_class_int::operator()() { return data; }
void p_functor_class_int::operator()(int d) { data = d; }

p_functor_class_double::p_functor_class_double() : data(0.0) { }
p_functor_class_double::p_functor_class_double(double d) : data(d) { }
double p_functor_class_double::operator()() { return data; }
void p_functor_class_double::operator()(double d) { data = d; }

p_functor_class_bool::p_functor_class_bool() : data(false) { }
p_functor_class_bool::p_functor_class_bool(bool b) : data(b) { }
bool p_functor_class_bool::operator()() { return data; }
void p_functor_class_bool::operator()(bool d) { data = d; }

p_functor_class_pointer::p_functor_class_pointer() : data(0) { }
p_functor_class_pointer::p_functor_class_pointer(int * p) : data(p) { }
int * p_functor_class_pointer::operator()() { return data; }
void p_functor_class_pointer::operator()(int * d) { data = d; }

p_functor_class_enum::p_functor_class_enum() : data(e_test_default) { }
p_functor_class_enum::p_functor_class_enum(test_enum e) : data(e) { }
test_enum p_functor_class_enum::operator()() { return data; }
void p_functor_class_enum::operator()(test_enum d) { data = d; }

p_functor_class_pod::p_functor_class_pod() { }
p_functor_class_pod::p_functor_class_pod(test_pod p) : data(p) { }
test_pod p_functor_class_pod::operator()() { return data; }
void p_functor_class_pod::operator()(test_pod d) { data = d; }
