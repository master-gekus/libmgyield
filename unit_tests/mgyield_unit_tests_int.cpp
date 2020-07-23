#include <iostream>
#include <iomanip>
#include <string>

#include <clocale>
#include <stdexcept>
#include <gtest/gtest.h>

#include "mgyield.h"

TEST(Next, Integer_01)
{
  ::mg::yield_generator<int> g([](const mg::yield_operator<int>& yield) {
    yield(1);

    int i{2};
    yield(i);
  });

  EXPECT_FALSE(g.is_empty());
  EXPECT_FALSE(g.is_finished());

  EXPECT_TRUE(g.next());
  EXPECT_FALSE(g.is_finished());
  EXPECT_EQ(g.current(), 1);
}

TEST(Next, Integer_02)
{
  ::mg::yield_generator<int> g([](const mg::yield_operator<int>& yield) {
    yield(1);

    int i{2};
    yield(i);
  });

  EXPECT_FALSE(g.is_empty());
  EXPECT_FALSE(g.is_finished());

  EXPECT_TRUE(g.next());
  EXPECT_FALSE(g.is_finished());
  EXPECT_EQ(g.current(), 1);

  EXPECT_TRUE(g.next());
  EXPECT_FALSE(g.is_finished());
  EXPECT_EQ(g.current(), 2);

  EXPECT_FALSE(g.next());
  EXPECT_TRUE(g.is_finished());
}
