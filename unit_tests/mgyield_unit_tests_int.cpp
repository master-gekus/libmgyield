#include <iostream>
#include <iomanip>
#include <string>
#include <algorithm>
#include <vector>

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

TEST(Iterators, MultipleBegins)
{
  ::mg::yield_generator<int> g([](const mg::yield_operator<int>& yield) {
    int i = 0;
    while (true) {
      yield(++i);
    }
  });

  auto it1 = g.begin();
  auto it2 = g.begin();
  auto it3 = g.begin();

  EXPECT_EQ(it1, it2);
  EXPECT_EQ(it1, it3);

  EXPECT_EQ(*(it1++), 1);
  EXPECT_EQ(*(it2++), 1);
  EXPECT_EQ(*it3, 1);

  EXPECT_EQ(*(it1++), 2);
  EXPECT_EQ(*it2, 2);
  EXPECT_EQ(*it3, 1);

  EXPECT_EQ(*it1, 3);
  EXPECT_EQ(*it2, 2);
  EXPECT_EQ(*it3, 1);
}

TEST(Container, IntegerFinite_01)
{
  ::mg::yield_generator<int> g([](const mg::yield_operator<int>& yield, int count) {
    for (int i = 0; i < count; ++i) {
      yield(i);
    }
  }, 5);

  EXPECT_FALSE(g.is_empty());
  EXPECT_FALSE(g.is_finished());

  auto b = g.begin();
  auto e = g.end();

  EXPECT_EQ(::std::vector<int>(b,e), ::std::vector<int>({0,1,2,3,4}));

  ::std::vector<int> v;
  ::std::copy(b, e, ::std::back_inserter(v));
  EXPECT_EQ(v, ::std::vector<int>({0,1,2,3,4}));

  v.clear();
  ::std::copy_if(b, e, ::std::back_inserter(v), [](int x){return (1 == (x % 2));});
  EXPECT_EQ(v, ::std::vector<int>({1,3}));

  EXPECT_TRUE(g.is_finished());
}

TEST(Container, IntegerInfinite_01)
{
  ::mg::yield_generator<int> g([](const mg::yield_operator<int>& yield) {
    int i = 0;
    while (true) {
      yield(i++);
    }
  });

  EXPECT_FALSE(g.is_empty());
  EXPECT_FALSE(g.is_finished());

  auto b = g.begin();

  ::std::vector<int> v;
  ::std::copy_n(g.begin(), 5, ::std::back_inserter(v));
  EXPECT_EQ(v, ::std::vector<int>({0,1,2,3,4}));

  v.clear();
  ::std::copy_n(++g.begin(), 5, ::std::back_inserter(v));
  EXPECT_EQ(v, ::std::vector<int>({5,6,7,8,9}));

  v.clear();
  ::std::copy_n(b, 6, ::std::back_inserter(v));
  EXPECT_EQ(v, ::std::vector<int>({0,1,2,3,4,5}));
}
