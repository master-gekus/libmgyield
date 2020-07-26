#include <iostream>
#include <iomanip>
#include <string>

#include <clocale>
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

TEST(Next, Integer_03)
{
  ::mg::yield_generator<int> g([](const mg::yield_operator<int>& yield) {
    int i = 0;
    while (true) {
      yield(++i);
    }
  });

  EXPECT_FALSE(g.is_empty());
  EXPECT_FALSE(g.is_finished());

  EXPECT_TRUE(g.next());
  EXPECT_FALSE(g.is_finished());
  EXPECT_EQ(g.current(), 1);

  EXPECT_TRUE(g.next());
  EXPECT_FALSE(g.is_finished());
  EXPECT_EQ(g.current(), 2);

  EXPECT_TRUE(g.next());
  EXPECT_FALSE(g.is_finished());
  EXPECT_EQ(g.current(), 3);

  EXPECT_FALSE(g.is_finished());
}

TEST(Iterators, Integer_01)
{
  ::mg::yield_generator<int> g([](const mg::yield_operator<int>& yield) {
    yield(1);

    int i{2};
    yield(i);
  });

  EXPECT_FALSE(g.is_empty());
  EXPECT_FALSE(g.is_finished());

  auto it = g.begin();
  ASSERT_NE(it, g.end());
  EXPECT_EQ(*it, 1);
  EXPECT_FALSE(g.is_finished());

  ++it;
  ASSERT_NE(it, g.end());
  EXPECT_EQ(*it, 2);
  EXPECT_FALSE(g.is_finished());

  ++it;
  EXPECT_EQ(it, g.end());
  EXPECT_TRUE(g.is_finished());
}

TEST(Iterators, Integer_02)
{
  ::mg::yield_generator<int> g([](const mg::yield_operator<int>& yield) {
    yield(1);

    int i{2};
    yield(i);
  });

  EXPECT_FALSE(g.is_empty());
  EXPECT_FALSE(g.is_finished());

  auto it = g.begin();
  ASSERT_NE(it, g.end());
  EXPECT_EQ(*it, 1);
  EXPECT_FALSE(g.is_finished());

  it++;
  ASSERT_NE(it, g.end());
  EXPECT_EQ(*it, 2);
  EXPECT_FALSE(g.is_finished());

  it++;
  EXPECT_EQ(it, g.end());
  EXPECT_TRUE(g.is_finished());
}

TEST(Iterators, Integer_03)
{
  ::mg::yield_generator<int> g([](const mg::yield_operator<int>& yield) {
    int i = 0;
    while (true) {
      yield(++i);
    }
  });

  EXPECT_FALSE(g.is_empty());
  EXPECT_FALSE(g.is_finished());

  auto it = g.begin();
  ASSERT_NE(it, g.end());
  EXPECT_EQ(*it, 1);
  EXPECT_FALSE(g.is_finished());

  it++;
  ASSERT_NE(it, g.end());
  EXPECT_EQ(*it, 2);
  EXPECT_FALSE(g.is_finished());

  it++;
  EXPECT_NE(it, g.end());
  EXPECT_FALSE(g.is_finished());
}

TEST(Iterators, Integer_04)
{
  ::mg::yield_generator<int> g([](const mg::yield_operator<int>& yield) {
    int i = 0;
    while (true) {
      yield(++i);
    }
  });

  EXPECT_FALSE(g.is_empty());
  EXPECT_FALSE(g.is_finished());

  auto it = g.begin();
  ASSERT_NE(it, g.end());
  EXPECT_EQ(*it, 1);
  EXPECT_FALSE(g.is_finished());

  auto it1 = it;
  EXPECT_EQ(it1, it);
  ASSERT_NE(it, g.end());
  ASSERT_NE(it1, g.end());

  auto it2 = ::std::move(it);
  EXPECT_NE(it2, it);
  EXPECT_EQ(it, g.end());
  ASSERT_NE(it1, g.end());
  EXPECT_EQ(it1, it2);

  EXPECT_EQ(*(it1++), 1);
  EXPECT_EQ(*(it1++), 2);
  EXPECT_EQ(*(it1++), 3);

  EXPECT_NE(it1, it2);

  EXPECT_EQ(*(it2++), 1);
  EXPECT_EQ(*(it2++), 2);
  EXPECT_EQ(*(it2++), 3);

  EXPECT_EQ(it1, it2);
}
