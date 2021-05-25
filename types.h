#pragma once

#include <string.h>
#include <string>
namespace muduo {

using std::string;

inline void mem_zero(void *p, size_t n) { memset(p, 0, n); }

template <typename To, typename From> inline To implicit_cast(const From &f) {
  return f;
}

template <typename To, typename From> inline To down_cast(From *f) {
  if (false) {
    implicit_cast<From *, To>(0);
  }
  return static_cast<To>(f);
}

} // namespace muduo
