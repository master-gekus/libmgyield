#include "mgyield.h"

namespace mg {

__yield_generator_base::__priv_base::~__priv_base() noexcept
{
  if (thread_.joinable()) {
    set_state(Finished);
    thread_.join();
  }
}

__yield_generator_base::State __yield_generator_base::__priv_base::state() const noexcept
{
  ::std::lock_guard<::std::mutex> lock(state_mutex_);
  return state_;
}

void __yield_generator_base::__priv_base::set_state(State s) noexcept
{
  ::std::unique_lock<::std::mutex> lock(state_mutex_);
  state_ = s;
  lock.unlock();
  state_condition_variable_.notify_one();
}

bool __yield_generator_base::__priv_base::next()
{
  ::std::unique_lock<::std::mutex> lock(state_mutex_);
  state_condition_variable_.wait(lock, [this](){ return (state_ != Generating);});
  if (Waiting != state_) {
    return false;
  }
  state_ = Generating;
  lock.unlock();
  state_condition_variable_.notify_one();

  lock.lock();
  state_condition_variable_.wait(lock, [this](){ return (state_ != Generating);});
  if (nullptr != exception_) {
    ::std::rethrow_exception(exception_);
  }
  return (Waiting == state_);
}

void __yield_generator_base::__priv_base::wait_for_next()
{
  ::std::unique_lock<::std::mutex> lock(state_mutex_);
  state_condition_variable_.wait(lock, [this](){ return (state_ != Waiting);});
  if (Generating != state_) {
    throw __yield_exception();
  }
}

} // namespace mg
