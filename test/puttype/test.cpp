/*
 * main.cpp
 * */

#include <gtest/gtest.h>

#include "puttype.h"

// first parameter is testcase name, second parameter is test name
TEST(TestGetPattern, first) {
  ASSERT_EQ(expected, actual);
}

TEST(TestGetIndex, floory) {
  Kumipuyo p;
  p.x = 0.0, p.y = 0.5;
  constexpr int expected = 8;
  ASSERT_EQ(expected, p.GetIndex());
}

TEST(TestGetIndex, allx) {
  Kumipuyo p;
  p.x = 0.0, p.y = 1.5;
  constexpr int expected = 15;
  for (int i = 0; i < Field::COLUMN; i++) {
    ASSERT_EQ(expected + i, p.GetIndex());
    p.x += 1.0;
  }
}

TEST(TestGetIndex, last) {
  Kumipuyo p;
  p.x = 5.0, p.y = 11.5;
  constexpr int expected = 90;
  ASSERT_EQ(expected, p.GetIndex());
}

TEST(TestGetIndex, alwaysfail) {
  Kumipuyo p;
  p.x = 2.0, p.y = 3.0;
  constexpr int expected = 20;
  ASSERT_EQ(expected, p.GetIndex());
}