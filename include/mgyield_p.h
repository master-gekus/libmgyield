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
#include <mutex>
#include <condition_variable>

namespace mg {

class __yield_exception {
};

class __yield_generator_base
{
protected:
  class __priv_base;
  enum State {
    Waiting,
    Generating,
    Finished,
  };

protected:
  explicit __yield_generator_base(__priv_base* d) noexcept;
  ~__yield_generator_base();

private:
  __yield_generator_base(const __yield_generator_base&) = delete;
  __yield_generator_base& operator=(const __yield_generator_base&) = delete;

protected:
  bool is_empty() const noexcept;
  bool is_finished() const noexcept;

  bool next();

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
  State state() const noexcept;
  void set_state(State s) noexcept;

  void set_thread(::std::thread&& t) noexcept;

  void set_exception(const ::std::exception_ptr& e) noexcept;
  void clear_exception() noexcept;
  const ::std::exception_ptr& exception() const noexcept;

  bool next();
  void wait_for_next();

private:
  State state_;
  mutable ::std::mutex state_mutex_;
  ::std::condition_variable state_condition_variable_;

  ::std::thread thread_;
  ::std::exception_ptr exception_;
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

inline bool __yield_generator_base::is_finished() const noexcept
{
  return (nullptr == d_) ? false : (Finished == d_->state());
}

inline bool __yield_generator_base::next()
{
  return (nullptr == d_) ? false : d_->next();
}

inline __yield_generator_base::__priv_base::__priv_base() noexcept :
  state_{Waiting},
  exception_{nullptr}
{
}

inline void __yield_generator_base::__priv_base::set_thread(::std::thread&& t) noexcept
{
  thread_ = ::std::move(t);
}

inline void __yield_generator_base::__priv_base::set_exception(const ::std::exception_ptr& e) noexcept
{
  exception_ = e;
}

inline void __yield_generator_base::__priv_base::clear_exception() noexcept
{
  exception_ = nullptr;
}

inline const ::std::exception_ptr& __yield_generator_base::__priv_base::exception() const noexcept
{
  return exception_;
}

} // namespace mg

#endif // MGYIELD_P_H_INCLUDED
