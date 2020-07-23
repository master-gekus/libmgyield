#include <iostream>
#include <iomanip>
#include <string>

#include <clocale>
#include <list>
#include <gtest/gtest.h>

#include "mgyield.h"

class test_simple_class
{
public:
  test_simple_class() = delete;
  inline test_simple_class(int a, int b) noexcept :
    a_(a),
    b_(b)
  {}

public:
  inline bool operator==(const test_simple_class& other) const
  {
    return ((a_ == other.a_) && (b_ == other.b_));
  }

private:
  int a_;
  int b_;

  friend ::std::ostream& operator <<(::std::ostream& os, const test_simple_class& value);
};

::std::ostream& operator <<(::std::ostream& os, const test_simple_class& value)
{
  os << "{" << value.a_ << "," << value.b_ << "}";
  return os;
}

class test_polymorphic_class
{
public:
  test_polymorphic_class() = delete;
  inline test_polymorphic_class(int a, int b) noexcept :
    a_(a),
    b_(b)
  {}

  virtual ~test_polymorphic_class()
  {}

public:
  inline bool operator==(const test_polymorphic_class& other) const
  {
    return ((a_ == other.a_) && (b_ == other.b_));
  }

private:
  int a_;
  int b_;

  friend ::std::ostream& operator <<(::std::ostream& os, const test_polymorphic_class& value);
};

::std::ostream& operator <<(::std::ostream& os, const test_polymorphic_class& value)
{
  os << "{" << value.a_ << "," << value.b_ << "}";
  return os;
}

TEST(Next, SimpleClass_01)
{
  ::mg::yield_generator<test_simple_class> g([](const mg::yield_operator<test_simple_class>& yield) {
    yield(test_simple_class{1, 2});

    test_simple_class cl{1, 2};
    yield(cl);
  });

  EXPECT_FALSE(g.is_empty());
  EXPECT_FALSE(g.is_finished());

  EXPECT_TRUE(g.next());
  EXPECT_FALSE(g.is_finished());
  EXPECT_EQ(g.current(), test_simple_class(1,2));
}

TEST(Next, SimpleClass_02)
{
  ::mg::yield_generator<test_simple_class> g([](const mg::yield_operator<test_simple_class>& yield) {
    yield(test_simple_class{1, 2});

    test_simple_class cl{3, 4};
    yield(cl);
  });

  EXPECT_FALSE(g.is_empty());
  EXPECT_FALSE(g.is_finished());

  EXPECT_TRUE(g.next());
  EXPECT_FALSE(g.is_finished());
  EXPECT_EQ(g.current(), test_simple_class(1,2));

  EXPECT_TRUE(g.next());
  EXPECT_FALSE(g.is_finished());
  EXPECT_EQ(g.current(), test_simple_class(3,4));

  EXPECT_FALSE(g.next());
  EXPECT_TRUE(g.is_finished());
}

TEST(Next, SimpleClass_03)
{
  ::mg::yield_generator<test_simple_class> g([](const mg::yield_operator<test_simple_class>& yield) {
    for (int i = 1; true; ++i) {
      yield(test_simple_class{i, i * i});
    }
  });

  EXPECT_FALSE(g.is_empty());
  EXPECT_FALSE(g.is_finished());

  EXPECT_TRUE(g.next());
  EXPECT_FALSE(g.is_finished());
  EXPECT_EQ(g.current(), test_simple_class(1,1));

  EXPECT_TRUE(g.next());
  EXPECT_FALSE(g.is_finished());
  EXPECT_EQ(g.current(), test_simple_class(2,4));

  EXPECT_TRUE(g.next());
  EXPECT_FALSE(g.is_finished());
  EXPECT_EQ(g.current(), test_simple_class(3,9));

  EXPECT_FALSE(g.is_finished());
}

TEST(Next, PolymorphicClass_01)
{
  ::mg::yield_generator<test_polymorphic_class> g([](const mg::yield_operator<test_polymorphic_class>& yield) {
    yield(test_polymorphic_class{1, 2});

    test_polymorphic_class cl{1, 2};
    yield(cl);
  });

  EXPECT_FALSE(g.is_empty());
  EXPECT_FALSE(g.is_finished());

  EXPECT_TRUE(g.next());
  EXPECT_FALSE(g.is_finished());
  EXPECT_EQ(g.current(), test_polymorphic_class(1,2));
}

TEST(Next, PolymorphicClass_02)
{
  ::mg::yield_generator<test_polymorphic_class> g([](const mg::yield_operator<test_polymorphic_class>& yield) {
    yield(test_polymorphic_class{1, 2});

    test_polymorphic_class cl{3, 4};
    yield(cl);
  });

  EXPECT_FALSE(g.is_empty());
  EXPECT_FALSE(g.is_finished());

  EXPECT_TRUE(g.next());
  EXPECT_FALSE(g.is_finished());
  EXPECT_EQ(g.current(), test_polymorphic_class(1,2));

  EXPECT_TRUE(g.next());
  EXPECT_FALSE(g.is_finished());
  EXPECT_EQ(g.current(), test_polymorphic_class(3,4));

  EXPECT_FALSE(g.next());
  EXPECT_TRUE(g.is_finished());
}

TEST(Next, PolymorphicClass_03)
{
  ::mg::yield_generator<test_polymorphic_class> g([](const mg::yield_operator<test_polymorphic_class>& yield) {
    for (int i = 1; true; ++i) {
      yield(test_polymorphic_class{i, i * i});
    }
  });

  EXPECT_FALSE(g.is_empty());
  EXPECT_FALSE(g.is_finished());

  EXPECT_TRUE(g.next());
  EXPECT_FALSE(g.is_finished());
  EXPECT_EQ(g.current(), test_polymorphic_class(1,1));

  EXPECT_TRUE(g.next());
  EXPECT_FALSE(g.is_finished());
  EXPECT_EQ(g.current(), test_polymorphic_class(2,4));

  EXPECT_TRUE(g.next());
  EXPECT_FALSE(g.is_finished());
  EXPECT_EQ(g.current(), test_polymorphic_class(3,9));

  EXPECT_FALSE(g.is_finished());
}
