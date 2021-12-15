// Copyright 2021 Your Name <your_email>

#ifndef INCLUDE_MYSHAREDPTR_HPP_
#define INCLUDE_MYSHAREDPTR_HPP_

#include <atomic>
#include <iostream>
#include <utility>

template <typename T>
class MySharedPtr {
 private:
  T *_pointer;
  std::atomic_uint *_counter;

 public:
  MySharedPtr();
  explicit MySharedPtr(T *ptr);
  MySharedPtr(const MySharedPtr &r);
  MySharedPtr(MySharedPtr &&r) noexcept;
  ~MySharedPtr();
  auto operator=(const MySharedPtr &r) -> MySharedPtr&;
  auto operator=(MySharedPtr &&r) noexcept -> MySharedPtr&;

  // проверяет, указывает ли указатель на объект
  explicit operator bool() const;
  auto operator*() const -> T&;
  auto operator->() const -> T*;

  auto get() -> T*;
  [[nodiscard]] auto get_counter() const -> const std::atomic_uint*;
  void reset();
  void reset(T *ptr);
  void swap(MySharedPtr &r);
  // возвращает количество объектов SharedPtr, которые ссылаются на тот же
  // управляемый объект
  [[nodiscard]] auto use_count() const -> size_t;
};

template <typename T>
MySharedPtr<T>::MySharedPtr()
    : _pointer(nullptr)
      , _counter(nullptr) {}

template <typename T>
MySharedPtr<T>::MySharedPtr(T *ptr) {
  _pointer = ptr;
  _counter = new std::atomic_uint(1);
}

template <typename T>
MySharedPtr<T>::MySharedPtr(const MySharedPtr &r) {
  _pointer = r._pointer;
  _counter = r._counter;
  (*_counter) += 1;
}

template <typename T>
MySharedPtr<T>::MySharedPtr(MySharedPtr &&r) noexcept {
  _pointer = std::move(r._pointer);
  _counter = std::move(r._counter);
}

template <typename T>
MySharedPtr<T>::~MySharedPtr() {
  if (_counter) {
    if ((*_counter) == 1) {
      delete _pointer;
      delete _counter;
    } else {
      (*_counter) -= 1;
    }
  }
}

template <typename T>
auto MySharedPtr<T>::operator=(const MySharedPtr &r) -> MySharedPtr& {
  if (this == &r) {
    return *this;
  }
  this->~MySharedPtr();
  _pointer = r._pointer;
  _counter = r._counter;
  (*_counter) += 1;
  return *this;
}

template <typename T>
auto MySharedPtr<T>::operator=(MySharedPtr &&r) noexcept -> MySharedPtr& {
  if (this != &r) {
    this -> ~MySharedPtr();
    _pointer = std::move(r._pointer);
    _counter = std::move(r._counter);
  }
  return *this;
}

template <typename T>
MySharedPtr<T>::operator bool() const {
  return _pointer != nullptr;
}

template <typename T>
auto MySharedPtr<T>::operator*() const -> T& {
  return *_pointer;
}

template <typename T>
auto MySharedPtr<T>::operator->() const -> T* {
  return _pointer;
}

template <typename T>
auto MySharedPtr<T>::get() -> T* {
  return _pointer;
}

template <typename T>
auto MySharedPtr<T>::get_counter() const -> const std::atomic_uint * {
  return _counter;
}

template <typename T>
void MySharedPtr<T>::reset() {
  *this = std::move(MySharedPtr());
}

template <typename T>
void MySharedPtr<T>::reset(T *ptr) {
  *this = std::move(MySharedPtr<T>(ptr));
}

template <typename T>
void MySharedPtr<T>::swap(MySharedPtr &r) {
  std::swap(_pointer, r._pointer);
  std::swap(_counter, r._counter);
}

template <typename T>
auto MySharedPtr<T>::use_count() const -> size_t {
  return static_cast<size_t>(*_counter);
}

#endif  // INCLUDE_MYSHAREDPTR_HPP_
