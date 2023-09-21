// SPDX-License-Identifier: LGPLv3
// Copyright (C) 2023 - depbox

#pragma once

// C
#include <stdio.h>
// C++
#include <iostream>
// thirdparty (testing)
#include <cln/integer.h>     // NOLINT
#include <cln/integer_io.h>  // NOLINT
#include <cln/io.h>          // NOLINT
#include <cln/number.h>      // NOLINT
#include <ginac.h>           // NOLINT
#include <gmp.h>             // NOLINT

using namespace GiNaC;  // NOLINT
using namespace cln;    // NOLINT

class my_visitor : public visitor,           // this is required
                   public add::visitor,      // visit add objects
                   public numeric::visitor,  // visit numeric objects
                   public power::visitor,
                   public basic::visitor  // visit basic objects
{
  void visit(const add& x) override {
    std::cout << "called with an add object" << std::endl;
    for (size_t i = 0; i < x.nops(); i++) {
      x.op(i).accept(*this);
    }
  }

  void visit(const numeric& x) override {
    std::cout << "called with a numeric object" << std::endl;
  }

  void visit(const power& x) override {
    std::cout << "called with a power object" << std::endl;
    const ex& base = x.op(0);
    const ex& exponent = x.op(1);

    if (is_a<numeric>(exponent)) {
      numeric n = ex_to<numeric>(exponent);
      std::cout << "Exponent is numeric: " << n << std::endl;
      if (n == 2) std::cout << "TWO!" << std::endl;
    }

    base.accept(*this);      // Visit the base expression
    exponent.accept(*this);  // Visit the exponent expression
  }

  void visit(const basic& x) override {
    std::cout << "called with a basic object" << std::endl;

    if (is_a<symbol>(x)) {
      std::cout << "variable name: " << ex_to<symbol>(x) << std::endl;
    }
  }
};

int basic_testing() {
  unsigned result = 0;
  {
    ex e;
    symbol x("x");
    lst syms = {x};
    e =
        ex("((x * x) + ((x - (x)) * {((x) + {(({-7^2} + ((((-1 - "
           "((-1))))))))^2})^2}))",
           syms);
    std::cout << "equacao: " << std::flush;
    std::cout << latex << e << std::endl;  // x^2
    //
    my_visitor v;
    e.accept(v);
    //
    // Substitute values
    ex substituted_expr = e.subs(x == 3);

    // Evaluate the substituted expression
    numeric result = ex_to<numeric>(substituted_expr);
    std::cout << e.subs(x == 3) << std::endl;
    std::cout << e.subs(x == 5) << std::endl;
    //
    auto ex2 = ex("x+1/x", syms);
    std::cout << ex2.subs(x == 3) << std::endl;
    std::cout << ex2.subs(x == 1) << std::endl;
  }
  std::cout << "OK" << std::endl;
  return 0;
}
