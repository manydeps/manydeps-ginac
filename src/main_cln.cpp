// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright (C) 2023 - manydeps - https://github.com/manydeps
//
// License is a combination of:
// - CLN (GPL-2.0-or-later)
// - GMP (GPL-2.0-or-later OR LGPL-3.0-or-later)

// C
#include <stdio.h>

// C++
#include <vector>
// local library
#include <cln/string.h>

// #include <manydeps-ginac-lib/Testing.hpp>

namespace cln {
extern cl_heap_string* cl_make_heap_string(const char* ptr);
}

int main() {
  //
  // testing 'cl_make_heap_string' from library
  std::string str("123");
  cln::cl_heap_string* out = cln::cl_make_heap_string(str.c_str());
  std::cout << out << std::endl;
  cl_free_heap_object(out);
  //
  std::cout << "finished successfully!" << std::endl;
  return 0;
}
