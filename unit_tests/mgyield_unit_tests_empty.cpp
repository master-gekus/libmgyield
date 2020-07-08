#include <iostream>
#include <iomanip>

#include <clocale>
#include <gtest/gtest.h>

#include "mgyield.h"

TEST(ConstructorDestructor, Empty)
{
  ::mg::yield_generator<int> g;
}
