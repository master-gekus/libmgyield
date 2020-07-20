#include <iostream>
#include <iomanip>
#include <string>

#include <clocale>
#include <gtest/gtest.h>

#include "mgyield.h"

TEST(ConstructorDestructor, Empty)
{
  ::mg::yield_generator<int> g;
  EXPECT_TRUE(g.is_empty());
}

TEST(ConstructorDestructor, EmptyGenerator)
{
  {
    ::mg::yield_generator<int> g([](const mg::yield_operator<int>&){});
    EXPECT_FALSE(g.is_empty());
  }

  {
    ::mg::yield_generator<int> g([](const mg::yield_operator<int>&, int){}, 1);
    EXPECT_FALSE(g.is_empty());
  }

  {
    int i{0};
    ::mg::yield_generator<int> g([](const mg::yield_operator<int>&, int&){}, ::std::ref(i));
    EXPECT_FALSE(g.is_empty());
  }

  {
    int i{0};
    ::std::string s;
    ::mg::yield_generator<int> g([](const mg::yield_operator<int>&, int&, const ::std::string&){}, ::std::ref(i), s);
    EXPECT_FALSE(g.is_empty());
  }
}
