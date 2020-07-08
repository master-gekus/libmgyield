#ifndef MGYIELD_H_INCLUDED
#define MGYIELD_H_INCLUDED

namespace mg {

template<typename T>
class yield_generator
{
public:
  using value_type = T;

public:
  yield_generator();
  yield_generator(const yield_generator&) = delete;
  yield_generator(yield_generator&& other) noexcept;
  yield_generator& operator=(const yield_generator&) = delete;
  yield_generator& operator=(yield_generator&& other) noexcept;

public:
  ~yield_generator();

private:
  class __priv;
  __priv *d_;
};

} // namespace mg

template<typename T>
class mg::yield_generator<T>::__priv
{
};

template<typename T>
inline mg::yield_generator<T>::yield_generator() :
  d_{nullptr}
{
}

template<typename T>
inline mg::yield_generator<T>::~yield_generator()
{
  delete d_;
}

#endif // MGYIELD_H_INCLUDED
