/**
 * \file mgyield.h
 * \author Evgeny V. Zalibvochkin
 */

#pragma once
#ifndef MGYIELD_H_INCLUDED
#define MGYIELD_H_INCLUDED

#include <memory>
#include <iterator>

#include "mgyield_p.h"

namespace mg {

template<typename T>
class yield_operator;

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

public:
  class iterator;
  iterator begin();
  iterator end();

private:
  class __priv;
  __priv* d() noexcept;
  const __priv* d() const noexcept;

  struct element
  {
    ::std::shared_ptr<element> next_;
    T data_;
  };

  friend class yield_operator<T>;
};

template<typename T>
class yield_operator
{
private:
  explicit yield_operator(typename yield_generator<T>::__priv* owner) noexcept;

public:
  template<typename U>
  typename ::std::enable_if<::std::is_convertible<U,T>::value>::type
  operator() (U value) const;

private:
  typename yield_generator<T>::__priv* owner_;

  friend class yield_generator<T>;
};

template<typename T>
class yield_generator<T>::iterator : public ::std::iterator<::std::forward_iterator_tag, T>
{
private:
  iterator() noexcept;
  iterator(yield_generator<T>* generator, const ::std::shared_ptr<element>& source) noexcept;

public:
  iterator(const iterator& other) noexcept;
  iterator(iterator&& other) noexcept;

  iterator& operator=(const iterator& other) noexcept;
  iterator& operator=(iterator&& other) noexcept;

public:
  iterator& operator++() noexcept;
  iterator operator++(int) noexcept;
  bool operator==(const iterator& other) const;
  bool operator!=(const iterator& other) const;
  const T& operator*() const;
  const T* operator->() const;

private:
  ::std::shared_ptr<element> e_;
  typename yield_generator<T>::__priv* g_;

  friend class yield_generator<T>;
};

template<typename T>
class yield_generator<T>::__priv : public __yield_generator_base::__priv_base
{
public:
  __priv() noexcept;
  ~__priv() noexcept;

public:
  yield_operator<T> yield_;
  ::std::shared_ptr<element> current_;
};

template<typename T>
inline yield_generator<T>::__priv::__priv() noexcept :
  yield_{this}
{
}

template<typename T>
inline yield_generator<T>::__priv::~__priv() noexcept
{
}

template<typename T>
inline yield_generator<T>::yield_generator() :
  __yield_generator_base{nullptr}
{
}

template<typename T>
inline yield_generator<T>::yield_generator(yield_generator&& other) noexcept :
  __yield_generator_base{other.d_}
{
  other.d_ = nullptr;
}

template<typename T>
inline yield_generator<T>& yield_generator<T>::operator=(yield_generator&& other) noexcept
{
  ::std::swap(d_, other.d_);
  return *this;
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
yield_generator<T>::yield_generator(_Fn&& f, _Args... args) noexcept:
  __yield_generator_base{new __priv}
{
  d_->set_state(Waiting);
  d_->set_thread(::std::thread([](__priv* p, _Fn&& f, _Args... args) {
    try {
      p->wait_for_next();
      ::std::forward<_Fn>(f)(p->yield_, ::std::forward<_Args>(args)...);
      p->set_state(Finished);
    } catch (const __yield_exception&) {
    } catch (...) {
      p->set_exception(::std::current_exception());
    }
    p->set_state(Finished);
  }, d(), ::std::forward<_Fn>(f), ::std::forward<_Args>(args)...));
}

template<typename T>
inline const T& yield_generator<T>::current() const noexcept
{
  const __priv* p = d();
  if ((nullptr == p) || (!p->current_)) {
    return *(static_cast<const T*>(nullptr));
  }
  return p->current_->data_;
}

template<typename T>
typename yield_generator<T>::iterator yield_generator<T>::begin()
{
  __priv* p = d();
  if (nullptr == p) {
    return iterator();
  }

  if (!p->current_) {
    p->next();
  }

  return iterator{this, p->current_};
}

template<typename T>
typename yield_generator<T>::iterator yield_generator<T>::end()
{
  return iterator();
}

template<typename T>
inline yield_generator<T>::iterator::iterator() noexcept :
  g_{nullptr}
{
}

template<typename T>
inline yield_generator<T>::iterator::iterator(yield_generator<T>* generator,
                                              const ::std::shared_ptr<element>& source) noexcept :
  e_{source},
  g_{generator->d()}
{
}

template<typename T>
inline yield_generator<T>::iterator::iterator(const iterator& other) noexcept :
  e_{other.e_},
  g_{other.g_}
{
}

template<typename T>
inline yield_generator<T>::iterator::iterator(iterator&& other) noexcept:
  e_{::std::move(other.e_)},
  g_{::std::move(other.g_)}
{
  other.g_ = nullptr;
}

template<typename T>
inline typename yield_generator<T>::iterator& yield_generator<T>::iterator::operator=(const iterator& other) noexcept
{
  e_.operator=(other.e_);
  g_ = other.g_;
  return *this;
}

template<typename T>
inline typename yield_generator<T>::iterator& yield_generator<T>::iterator::operator=(iterator&& other) noexcept
{
  e_.operator=(::std::move(other.e_));
  g_ = ::std::move(other.g_);
  other.g_ = nullptr;
  return *this;
}

template<typename T>
typename yield_generator<T>::iterator& yield_generator<T>::iterator::operator++() noexcept
{
  if (g_) {
    g_->next();
  }

  if (e_) {
    e_ = e_->next_;
  }

  return *this;
}

template<typename T>
typename yield_generator<T>::iterator yield_generator<T>::iterator::operator++(int) noexcept
{
  if (g_) {
    g_->next();
  }

  if (!e_) {
    return iterator();
  }

  iterator ret{*this};
  e_ = e_->next_;
  return ret;
}

template<typename T>
inline bool yield_generator<T>::iterator::operator==(const iterator& other) const
{
  return (e_ == other.e_);
}

template<typename T>
inline bool yield_generator<T>::iterator::operator!=(const iterator& other) const
{
  return (e_ != other.e_);
}

template<typename T>
inline const T& yield_generator<T>::iterator::operator*() const
{
  return static_cast<bool>(e_) ? e_->data_ : *(static_cast<const T*>(nullptr));
}

template<typename T>
inline const T* yield_generator<T>::iterator::operator->() const
{
  return static_cast<bool>(e_) ? &(e_->data_) : static_cast<const T*>(nullptr);
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

template<typename T>
inline yield_operator<T>::yield_operator(typename yield_generator<T>::__priv* owner) noexcept :
  owner_{owner}
{
}

template<typename T>
template<typename U>
typename ::std::enable_if<::std::is_convertible<U,T>::value>::type
yield_operator<T>::operator() (U value) const
{
  using element = typename yield_generator<T>::element;
  auto new_current = ::std::make_shared<element>(element{nullptr, ::std::forward<U>(value)});
  if (owner_->current_) {
    owner_->current_->next_ = new_current;
  }
  owner_->current_ = new_current;

  owner_->set_state(yield_generator<T>::Waiting);
  owner_->wait_for_next();
}

} // namespace mg

#endif // MGYIELD_H_INCLUDED
