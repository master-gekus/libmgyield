/**
 * \file mgyield_p.h
 * \author Evgeny V. Zalibvochkin
 */

#pragma once
#ifndef MGYIELD_P_H_INCLUDED
#define MGYIELD_P_H_INCLUDED

namespace mg {

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
  __priv_base* d_;
};

class __yield_generator_base::__priv_base
{
protected:
  __priv_base() noexcept;
public:
  virtual ~__priv_base() noexcept;
};

inline __yield_generator_base::__priv_base::__priv_base() noexcept
{
}

__yield_generator_base::__yield_generator_base(__priv_base* d) noexcept :
  d_{d}
{
}

__yield_generator_base::~__yield_generator_base()
{
  delete d_;
}

} // namespace mg

#endif // MGYIELD_P_H_INCLUDED
