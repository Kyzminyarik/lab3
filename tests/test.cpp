// Copyright 2021 Your Name <your_email>

#include <gtest/gtest.h>
#include <SharedPtr.hpp>
#include "string.h"

TEST(SharedPtr, DefaultConstructor) {
  SharedPtr <int> p;
  EXPECT_EQ(p.get(), nullptr);
  EXPECT_EQ(p.use_count(), 0);
}

TEST(SharedPtr, InitConstructor) {
  SharedPtr p(new int(5));
  EXPECT_EQ(*p, 5);
  EXPECT_EQ(p.use_count(), 1);
}

TEST(SharedPtr, CopyConstructor) {
  const SharedPtr p1(new int(-1));
  SharedPtr p2 = SharedPtr (p1);
  EXPECT_EQ(*p2, *p1);
  EXPECT_EQ(p1.use_count(), 2);
  EXPECT_EQ(p2.use_count(), 2);
}



TEST(SharedPtr, Equal) {
  SharedPtr p(new int(10));
  SharedPtr p1 = p;
  EXPECT_EQ(*p, *p1);
  EXPECT_EQ(p.use_count(), 2);
  EXPECT_EQ(p1.use_count(), 2);
  SharedPtr p2 = std::move(p);
  EXPECT_EQ(*p2, 10);
  EXPECT_EQ(p.get(), nullptr);
  EXPECT_EQ(p2.use_count(), 2);
}

TEST(SharedPtr, Bool) {
  SharedPtr p(new int(10));
  EXPECT_TRUE(p);
  SharedPtr <int> p1;
  EXPECT_FALSE(p1);
}

TEST(SharedPtr, GetObject) {
  SharedPtr p (new std::string("abc"));
  EXPECT_EQ(p->length(), 3);
}

TEST(SharedPtr, Reset) {
  int* a = new int(10);
  SharedPtr p(a);
  p.reset();
  EXPECT_FALSE(p);
  SharedPtr p1(new int(15));
  int* b = new int(20);
  p1.reset(b);
  EXPECT_EQ(*p1, 20);
}

TEST(SharedPtr, Swap) {
  SharedPtr p1(new int(10));
  SharedPtr p2(new int(4));
  SharedPtr p3(p2);
  p1.swap(p2);
  EXPECT_EQ(*p1, 4);
  EXPECT_EQ(p1.use_count(), 2);
  EXPECT_EQ(*p2, 10);
  EXPECT_EQ(p2.use_count(), 1);
}
