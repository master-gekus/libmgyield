/**
 * \file mgyield_p.h
 * \author Evgeny V. Zalibvochkin
 */

#pragma once
#ifndef MGYIELD_P_H_INCLUDED
#define MGYIELD_P_H_INCLUDED

#include <type_traits>
#include <functional>
#include <thread>

namespace mg {

class __yield_exception {
};

class __yield_generator_base
{
protected:
  class __priv_base;

protected:
  explicit __yield_generator_base(__priv_base* d) noexcept;
  ~__yield_generator_base();

private:
  __yield_generator_base(const __yield_generator_base&) = delete;
  __yield_generator_base& operator=(const __yield_generator_base&) = delete;

protected:
  bool is_empty() const noexcept;
  bool next() noexcept;

protected:
  __priv_base* d_;
};

class __yield_generator_base::__priv_base
{
protected:
  __priv_base() noexcept;
public:
  virtual ~__priv_base() noexcept;

public:
  void set_thread(::std::thread&& t);
  bool next() noexcept;

private:
  ::std::thread thread_;
};

inline __yield_generator_base::__yield_generator_base(__priv_base* d) noexcept :
  d_{d}
{
}

inline __yield_generator_base::~__yield_generator_base()
{
  delete d_;
}

inline bool __yield_generator_base::is_empty() const noexcept
{
  return (nullptr == d_);
}

inline bool __yield_generator_base::next() noexcept
{
  return (nullptr == d_) ? false : d_->next();
}

inline __yield_generator_base::__priv_base::__priv_base() noexcept
{
}

inline void __yield_generator_base::__priv_base::set_thread(::std::thread&& t)
{
  thread_ = ::std::move(t);
}

inline bool __yield_generator_base::__priv_base::next() noexcept
{
  return false;
}

} // namespace mg

#endif // MGYIELD_P_H_INCLUDED
