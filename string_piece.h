#pragma once

#include "types.h"

namespace muduo {
class string_arg {
public:
  string_arg(const char *str) : _str(str) {}

  string_arg(const string &str) : _str(str.c_str()) {}

  const char *c_str() const { return this->_str; }

private:
  const char *_str;
};

class string_piece {
private:
  const char *_ptr;
  int _length;

public:
  string_piece() : _ptr(nullptr), _length(0) {}

  string_piece(const char *str)
      : _ptr(str), _length(static_cast<int>(strlen(_ptr))) {}

  string_piece(const unsigned char *str)
      : _ptr(reinterpret_cast<const char *>(str)),
        _length(static_cast<int>(strlen(_ptr))) {}

  string_piece(const string &str) : _ptr(str.data()), _length(str.size()) {}

  string_piece(const char *offset, int len) : _ptr(offset), _length(len) {}

  const char *data() const { return this->_ptr; }

  int size() const { return this->_length; }

  bool empty() const { return this->_length == 0; }

  const char *begin() const { return this->_ptr; }

  const char *end() const { return this->_ptr + this->_length; }

  void clear() {
    this->_ptr = nullptr;
    this->_length = 0;
  }

  void set(const char *buffer, int len) {
    this->_ptr = buffer;
    this->_length = len;
  }

  void set(const char *str) {
    this->_ptr = str;
    this->_length = static_cast<int>(strlen(str));
  }

  void set(const void *buffer, int len) {
    this->_ptr = reinterpret_cast<const char *>(buffer);
    this->_length = len;
  }

  char operator[](int i) const { return this->_ptr[i]; }

  void remove_prefix(int n) {
    this->_ptr += n;
    this->_length -= n;
  }

  void remove_suffix(int n) { this->_length -= n; }

  bool operator==(const string_piece &x) const {
    return ((this->_length == x._length) &&
            (memcmp(this->_ptr, x._ptr, this->_length) == 0));
  }

  bool operator!=(const string_piece &x) const { return !(*this == x); }

#define STRINGPIECE_BINARY_PREDICATE(cmp, auxcmp)                              \
  bool operator cmp(const string_piece &x) const {                             \
    int r = memcmp(_ptr, x._ptr, _length < x._length ? _length : x._length);   \
    return ((r auxcmp 0) || ((r == 0) && (_length cmp x._length)));            \
  }
  STRINGPIECE_BINARY_PREDICATE(<, <);
  STRINGPIECE_BINARY_PREDICATE(<=, <);
  STRINGPIECE_BINARY_PREDICATE(>=, >);
  STRINGPIECE_BINARY_PREDICATE(>, >);
#undef STRINGPIECE_BINARY_PREDICATE

  int compare(const string_piece &x) const {
    int r = memcmp(this->_ptr, x._ptr,
                   this->_length < x._length ? this->_length : x._length);

    if (r == 0) {
      if (this->_length < x._length)
        r = -1;
      else if (this->_length > x._length)
        r = +1;
      else {
      }
    }

    return r;
  }

  string as_string() const { return string(this->data(), this->size()); }

  void copy_to_string(string *target) const {
    target->assign(this->_ptr, this->_length);
  }

  bool start_with(const string_piece &x) const {
    return ((this->_length > x._length) &&
            memcmp(this->_ptr, x._ptr, x._length));
  }
};
} // namespace muduo

std::ostream &operator<<(std::ostream &o, const muduo::string_piece &piece);
