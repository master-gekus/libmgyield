#include <iostream>
#include <iomanip>
#include <string>

#include <clocale>
#include <string>
#include <gtest/gtest.h>

#include "mgyield.h"

TEST(Next, String_01)
{
  ::mg::yield_generator<::std::string> g([](const mg::yield_operator<::std::string>& yield) {
    yield("String 1");

    ::std::string s{"String 2"};
    yield(s);
  });

  EXPECT_FALSE(g.is_empty());
  EXPECT_FALSE(g.is_finished());

  EXPECT_TRUE(g.next());
  EXPECT_FALSE(g.is_finished());
  EXPECT_EQ(g.current(), ::std::string{"String 1"});
}

TEST(Next, String_02)
{
  ::mg::yield_generator<::std::string> g([](const mg::yield_operator<::std::string>& yield) {
    yield("String 1");

    ::std::string s{"String 2"};
    yield(s);
  });

  EXPECT_FALSE(g.is_empty());
  EXPECT_FALSE(g.is_finished());

  EXPECT_TRUE(g.next());
  EXPECT_FALSE(g.is_finished());
  EXPECT_EQ(g.current(), ::std::string{"String 1"});

  EXPECT_TRUE(g.next());
  EXPECT_FALSE(g.is_finished());
  EXPECT_EQ(g.current(), ::std::string{"String 2"});

  EXPECT_FALSE(g.next());
  EXPECT_TRUE(g.is_finished());
}

TEST(Next, String_03)
{
  ::mg::yield_generator<::std::string> g([](const mg::yield_operator<::std::string>& yield) {
    int i = 0;
    while (true) {
      yield("Value " + ::std::to_string(++i));
    }
  });

  EXPECT_FALSE(g.is_empty());
  EXPECT_FALSE(g.is_finished());

  EXPECT_TRUE(g.next());
  EXPECT_FALSE(g.is_finished());
  EXPECT_EQ(g.current(), ::std::string{"Value 1"});

  EXPECT_TRUE(g.next());
  EXPECT_FALSE(g.is_finished());
  EXPECT_EQ(g.current(), ::std::string{"Value 2"});

  EXPECT_TRUE(g.next());
  EXPECT_FALSE(g.is_finished());
  EXPECT_EQ(g.current(), ::std::string{"Value 3"});

  EXPECT_FALSE(g.is_finished());
}
