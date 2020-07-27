#include <algorithm>

#include <gtest/gtest.h>

#include "mgyield.h"

namespace integer {

YIELD_GENERATOR_DECLARE(Infinite, int);
YIELD_GENERATOR_IMPLEMENT(Infinite, int)
{
  for (int i = 0; true; ++i) {
    yield(i);
  }
}

YIELD_GENERATOR_DECLARE(InfiniteWithParam, int, int start_from);
YIELD_GENERATOR_IMPLEMENT(InfiniteWithParam, int, int start_from)
{
  for (int i = start_from; true; ++i) {
    yield(i);
  }
}

} // namespace integer

namespace strings {

YIELD_GENERATOR_DECLARE(Infinite, ::std::string, ::std::string prefix, int start_from);
YIELD_GENERATOR_IMPLEMENT(Infinite, ::std::string, ::std::string prefix, int start_from)
{
  for (int i = start_from; true; ++i) {
    yield(prefix + ::std::to_string(i));
  }
}

} // namespace strings

TEST(GeneratorClass, Integer_01)
{
  integer::Infinite g;
  ::std::vector<int> v;
  ::std::copy_n(g.begin(), 5, ::std::back_inserter(v));
  EXPECT_EQ(v, ::std::vector<int>({0,1,2,3,4}));
}

TEST(GeneratorClass, Integer_02)
{
  integer::InfiniteWithParam g(10);
  ::std::vector<int> v;
  ::std::copy_n(g.begin(), 5, ::std::back_inserter(v));
  EXPECT_EQ(v, ::std::vector<int>({10,11,12,13,14}));
}

TEST(GeneratorClass, Integer_03)
{
  integer::InfiniteWithParam g(20);
  ::std::vector<int> v;
  ::std::copy_n(g.begin(), 6, ::std::back_inserter(v));
  EXPECT_EQ(v, ::std::vector<int>({20,21,22,23,24,25}));
}

TEST(GeneratorClass, String_01)
{
  strings::Infinite g("Value ", 10);
  ::std::vector<::std::string> v;
  ::std::copy_n(g.begin(), 3, ::std::back_inserter(v));
  EXPECT_EQ(v, ::std::vector<::std::string>({"Value 10", "Value 11", "Value 12"}));
}
