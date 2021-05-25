#ifndef LOG_STREAM_H
#define LOG_STREAM_H

#include "noncopyable.h"
#include "string_piece.h"
#include "types.h"

namespace muduo {

namespace detail {

const int k_small_buffer = 4000;
const int k_large_buffer = 4000 * 1000;

template <int SIZE> class fixed_buffer : public noncopyable {
public:
  fixed_buffer() : cur_(data_) { set_cookie(cookie_start); }

  ~fixed_buffer() { set_cookie(cookie_stop); }

  void append(const char *buf, size_t len) {
    if (implicit_cast<size_t>(avail()) > len) {
      memcpy(cur_, buf, len);
      cur_ += len;
    }
  }

  const char *data() const { return data_; }

  int length() const { return static_cast<int>(cur_ - data_); }

  char *current() { return cur_; }

  int avail() const { return static_cast<int>(end() - cur_); }

  void add(size_t len) { cur_ += len; }

  void reset() { cur_ = data_; }

  void bzero() { mem_zero(data_, sizeof(data_)); }

  const char *debug_string() {
    *cur_ = '\0';
    return data_;
  }

  void set_cookie(void (*cookie)()) { this->cookie_ = cookie; }

  string to_string() const { return string(data_, length()); }

  string_piece to_string_piece() const { return string_piece(data_, length()); }

private:
  const char *end() const { return data_ + sizeof(data_); }

  static void cookie_start() {}
  static void cookie_stop() {}

  void (*cookie_)();
  char data_[SIZE];
  char *cur_;
};

} // namespace detail

class log_stream : public noncopyable {
  typedef log_stream self;

public:
  typedef detail::fixed_buffer<detail::k_small_buffer> Buffer;

  self &operator<<(bool v) {
    buffer_.append(v ? "1" : "0", 1);
    return *this;
  }

  self &operator<<(short);
  self &operator<<(unsigned short);
  self &operator<<(int);
  self &operator<<(unsigned int);
  self &operator<<(long);
  self &operator<<(unsigned long);
  self &operator<<(long long);
  self &operator<<(unsigned long long);

  self &operator<<(const void *);

  self &operator<<(float v) {
    *this << static_cast<double>(v);
    return *this;
  }
  self &operator<<(double);
  // self& operator<<(long double);

  self &operator<<(char v) {
    buffer_.append(&v, 1);
    return *this;
  }

  // self& operator<<(signed char);
  // self& operator<<(unsigned char);

  self &operator<<(const char *str) {
    if (str) {
      buffer_.append(str, strlen(str));
    } else {
      buffer_.append("(null)", 6);
    }
    return *this;
  }

  self &operator<<(const unsigned char *str) {
    return operator<<(reinterpret_cast<const char *>(str));
  }

  self &operator<<(const string &v) {
    buffer_.append(v.c_str(), v.size());
    return *this;
  }

  self &operator<<(const string_piece &v) {
    buffer_.append(v.data(), v.size());
    return *this;
  }

  self &operator<<(const Buffer &v) {
    *this << v.to_string_piece();
    return *this;
  }

  void append(const char *data, int len) { buffer_.append(data, len); }

  const Buffer &buffer() const { return this->buffer_; }

  void reset_buffer() { this->buffer_.reset(); }

private:
  void static_check();

  template <typename T> void format_integer(T);

  Buffer buffer_;
  static const int kMaxNumericSize = 32;
};

class Fmt
{
public:

    template<typename T>
    Fmt(const char* fmt, T val);



    const char* data() const {
        return buf_;
    }

    int length() const
    {
        return length_;
    }

private:
    char buf_[32];
    int length_;
};

inline log_stream & operator<<(log_stream &s, const Fmt& fmt)
{
    s.append(fmt.data(), fmt.length());
    return s;
}

string formatSI(int64_t n);
string formatIEC(int64_t n);
} // namespace muduo

#endif // LOG_STREAM_H
