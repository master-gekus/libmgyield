#include "mgyield.h"

namespace mg {

__yield_generator_base::__priv_base::~__priv_base() noexcept
{
  if (thread_.joinable()) {
    // Send termination signal
    thread_.join();
  }
}

} // namespace mg
