// Copyright 2021 Your Name <your_email>

#include <gtest/gtest.h>
#include <string>

#include "MySharedPtr.hpp"

int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}

TEST(MySharedPtr, TestDefaultConstructor) {
  MySharedPtr<int> a;
  ASSERT_EQ(a.get(), nullptr);
  ASSERT_EQ(a.get_counter(), nullptr);
}

TEST(MySharedPtr, TestPtrConstructor) {
  MySharedPtr<int> a(new int(100));
  ASSERT_EQ(*a.get(), 100);
  ASSERT_EQ(a.use_count(), 1);

  MySharedPtr<double> b(new double(100.2));
  ASSERT_EQ(*b.get(), 100.2);
  ASSERT_EQ(b.use_count(), 1);
}

TEST(MySharedPtr, TestCopyConstructor) {
  const MySharedPtr a(new std::string("Hello World!"));
  MySharedPtr b(a);
  ASSERT_EQ(*a, *b);
  ASSERT_EQ(a.use_count(), b.use_count());
}

TEST(MySharedPtr, TestMove){
  MySharedPtr<int> a(new int(1));
  EXPECT_EQ(std::is_move_constructible<MySharedPtr<int>>::value, true);
}

TEST(MySharedPtr, TestCopyAssignment) {
  const MySharedPtr a(new double(100.2));
  MySharedPtr b(new double(50.3));
  b = a;
  ASSERT_EQ(*a, *b);
  ASSERT_EQ(a.use_count(), b.use_count());
}

TEST(MySharedPtr, TestNormalAccess) {
  MySharedPtr a(new int(1));
  EXPECT_EQ(*a, 1);
}

TEST(MySharedPtr, TestReset) {
  MySharedPtr a(new double(100.2));
  a.reset();
  ASSERT_EQ(a.get(), nullptr);
  ASSERT_EQ(a.get_counter(), nullptr);
}

TEST(MySharedPtr, TestGet) {
  MySharedPtr a(new std::string("12345"));
  EXPECT_EQ(a -> length(), 5);
}

TEST(MySharedPtr, TestBool) {
  MySharedPtr<int> a;
  ASSERT_EQ(a.operator bool(), false);

  MySharedPtr b(new int(100));
  ASSERT_EQ(b.operator bool(), true);
}

TEST(MySharedPtr, TestSwap) {
  MySharedPtr a(new int(100));
  MySharedPtr b(new int(50));
  MySharedPtr c(b);
  a.swap(b);
  EXPECT_EQ(*a, 50);
  EXPECT_EQ(a.use_count(), 2);
  EXPECT_EQ(*b, 100);
  EXPECT_EQ(b.use_count(), 1);
}