/**
 * \file mgyield.h
 * \author Evgeny V. Zalibvochkin
 */

#pragma once
#ifndef MGYIELD_H_INCLUDED
#define MGYIELD_H_INCLUDED

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
  yield_generator(const _Fn& f, _Args... args) noexcept;

public:
  ~yield_generator();

public:
  using __yield_generator_base::is_empty;

private:
  class __priv;
  yield_operator<T> yield_;
};

template<typename T>
class yield_generator<T>::__priv : public __yield_generator_base::__priv_base
{
};

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
inline yield_generator<T>::yield_generator(const _Fn& f, _Args... args) noexcept :
  __yield_generator_base{new __priv}
{
  d_->set_thread(::std::thread([this, f](_Args... args) {
    f(yield_, ::std::forward<_Args>(args)...);
  }, ::std::forward<_Args>(args)...));
}

} // namespace mg

#endif // MGYIELD_H_INCLUDED
