#include <boost/interval.hpp>
#include <boost/test/test_tools.hpp>

typedef enum { EXPR_VAR, EXPR_NEG, EXPR_UP, EXPR_DOWN, EXPR_ADD, EXPR_SUB } e_type;

struct expr;
struct pexpr {
  expr *ptr;
  expr* operator->() { return ptr; }
  pexpr(expr *p = NULL): ptr(p) { }
};

struct expr {
  e_type type;
  int var;
  pexpr e;
  pexpr e1, e2;
};

pexpr var(int v) {
  pexpr e = new expr;
  e->type = EXPR_VAR;
  e->var = v;
  return e;
}

pexpr operator+(pexpr, pexpr);
pexpr operator-(pexpr, pexpr);
pexpr operator-(pexpr);

pexpr operator+(pexpr a, pexpr b) {
  if (a->type == EXPR_NEG) return b - a->e;
  if (b->type == EXPR_NEG) return a - b->e;
  if (a->type == EXPR_VAR && b->type == EXPR_VAR && a->var > b->var) return b + a;
  pexpr c = new expr;
  c->type = EXPR_ADD;
  c->e1 = a;
  c->e2 = b;
  return c;
}

pexpr operator-(pexpr a, pexpr b) {
  if (b->type == EXPR_NEG) return a + b->e;
  pexpr c = new expr;
  c->type = EXPR_SUB;
  c->e1 = a;
  c->e2 = b;
  return c;
}

pexpr down(pexpr a) {
  pexpr e = new expr;
  e->type = EXPR_DOWN;
  e->e = a;
  return e;
}

pexpr up(pexpr a) {
  pexpr e = new expr;
  e->type = EXPR_UP;
  e->e = a;
  return e;
}

pexpr operator-(pexpr a) {
  if (a->type == EXPR_NEG) return a->e;
  if (a->type == EXPR_UP) return down(-a->e);
  if (a->type == EXPR_DOWN) return up(-a->e);
  if (a->type == EXPR_SUB) return a->e2 - a->e1;
  if (a->type == EXPR_ADD) return -a->e1 - a->e2;
  pexpr e = new expr;
  e->type = EXPR_NEG;
  e->e = a;
  return e;
}

bool operator==(pexpr a, pexpr b) {
  if (a->type != b->type) return false;
  if (a->type == EXPR_VAR) return a->var == b->var;
  if (a->type == EXPR_DOWN || a->type == EXPR_UP || a->type == EXPR_NEG)
    return a->e == b->e;
  return a->e1 == b->e1 && a->e2 == b->e2;
}

bool operator>(pexpr, pexpr) { return false; }

namespace boost {
  namespace interval_lib {

template<>
struct rounding_control<pexpr> {
  typedef bool rounding_mode;
  bool mode;
  bool get_rounding_mode() { return mode; }
  void set_rounding_mode(bool m) { mode = m; }
  void upward()   { mode = true; }
  void downward() { mode = false; }
  pexpr force_rounding(pexpr a) { return (mode ? &up : &down)(a); }
};

  };
};

using namespace boost;
using namespace interval_lib;

typedef interval_traits<pexpr, compare_certainly<pexpr>, save_state<rounded_arith_std<pexpr> > > traits1;
typedef interval_traits<pexpr, compare_certainly<pexpr>, save_state<rounded_arith_opp<pexpr> > > traits2;

template<class I>
bool test_neg() {
  I a(var(0), var(1));
  return equal(-a, I(-var(1), -var(2)));
}

template<class I>
bool test_add() {
  I a(var(0), var(1)), b(var(2), var(3));
  return equal(a + b, I(down(var(0) + var(2)), up(var(1) + var(3))));
}

template<class I>
bool test_sub() {
  I a(var(0), var(1)), b(var(2), var(3));
  return equal(a - b, I(down(var(0) - var(3)), up(var(1) - var(2))));
}

int test_main(int, char *[]) {
  BOOST_TEST((test_neg<interval<pexpr, traits1> >()));
  BOOST_TEST((test_neg<interval<pexpr, traits2> >()));
  BOOST_TEST((test_add<interval<pexpr, traits1> >()));
  BOOST_TEST((test_add<interval<pexpr, traits2> >()));
  BOOST_TEST((test_sub<interval<pexpr, traits1> >()));
  BOOST_TEST((test_sub<interval<pexpr, traits2> >()));
  return 0;
}
