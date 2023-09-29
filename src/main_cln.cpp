// SPDX-License-Identifier: LGPL
// Copyright (C) 2023

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
