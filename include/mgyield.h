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
class yield_generator : public __yield_generator_base
{
public:
  using value_type = T;

public:
  yield_generator();
  yield_generator(yield_generator&& other) noexcept;
  yield_generator& operator=(yield_generator&& other) noexcept;

public:
  ~yield_generator();

private:
  class __priv;
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

} // namespace mg

#endif // MGYIELD_H_INCLUDED
