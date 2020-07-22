/**
 * \file mgyield.h
 * \author Evgeny V. Zalibvochkin
 */

#pragma once
#ifndef MGYIELD_H_INCLUDED
#define MGYIELD_H_INCLUDED

#include <atomic>
#include <memory>

#include "mgyield_p.h"

namespace mg {

template<typename T>
class yield_operator
{
};

template<typename T>
class yield_generator : public __yield_generator_base
{
public:
  using value_type = T;

public:
  yield_generator();
  yield_generator(yield_generator&& other) noexcept;
  yield_generator& operator=(yield_generator&& other) noexcept;

public:
  template<
      class _Fn,
      class... _Args,
      class = typename ::std::enable_if<
        ::std::is_convertible<_Fn, ::std::function<void(const yield_operator<T>&, _Args...)> >::value
        >::type
      >
  yield_generator(_Fn&& f, _Args... args) noexcept;

public:
  ~yield_generator();

public:
  const T& current() const noexcept;

public:
  using __yield_generator_base::is_empty;
  using __yield_generator_base::is_finished;
  using __yield_generator_base::next;

private:
  class __priv;
  __priv* d() noexcept;
  const __priv* d() const noexcept;
};

template<typename T>
class yield_generator<T>::__priv : public __yield_generator_base::__priv_base
{
public:
  __priv() noexcept;

  yield_operator<T> yield_;
  ::std::atomic<T*> current_;
};

template<typename T>
inline yield_generator<T>::__priv::__priv() noexcept :
  current_{nullptr}
{
}

template<typename T>
inline yield_generator<T>::yield_generator() :
  __yield_generator_base{nullptr}
{
}

template<typename T>
inline yield_generator<T>::~yield_generator()
{
}

template<typename T>
template<
    class _Fn,
    class... _Args,
    class>
inline yield_generator<T>::yield_generator(_Fn&& f, _Args... args) noexcept:
  __yield_generator_base{new __priv}
{
  d_->set_state(Waiting);
  d_->set_thread(::std::thread([this](_Fn&& f, _Args... args) {
    try {
      d_->wait_for_next();
      ::std::forward<_Fn>(f)(d()->yield_, ::std::forward<_Args>(args)...);
      d_->set_state(Finished);
    } catch (const __yield_exception&) {
    } catch (...) {
      d_->set_exception(::std::current_exception());
    }
    d_->set_state(Finished);
  }, ::std::forward<_Fn>(f), ::std::forward<_Args>(args)...));
}

template<typename T>
const T& yield_generator<T>::current() const noexcept
{
  return *(d()->current_);
}

template<typename T>
inline typename yield_generator<T>::__priv* yield_generator<T>::d() noexcept
{
  return static_cast<__priv*>(d_);
}

template<typename T>
inline const typename yield_generator<T>::__priv* yield_generator<T>::d() const noexcept
{
  return static_cast<const __priv*>(d_);
}

} // namespace mg

#endif // MGYIELD_H_INCLUDED
