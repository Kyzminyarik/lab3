// Copyright 2021 Your Name <your_email>


#ifndef INCLUDE_SHAREDPTR_HPP_
#define INCLUDE_SHAREDPTR_HPP_

#include <atomic>
#include <iostream>
#include <utility>

template <typename T>
class SharedPtr {

 private:
  T* p; //указатель
  std::atomic_uint* count; //счетчик указателей

 public:
  SharedPtr();
  SharedPtr(T* ptr);
  SharedPtr(const SharedPtr& r);
  SharedPtr(SharedPtr&& r);
  ~SharedPtr();
  auto operator=(const SharedPtr& r) -> SharedPtr&;
  auto operator=(SharedPtr&& r) -> SharedPtr&;

  // проверяет, указывает ли указатель на объект
  operator bool() const;
  auto operator*() const -> T&;
  auto operator->() const -> T*;

  auto get() -> T*;
  void reset();
  void reset(T* ptr);
  void swap(SharedPtr& r);
  // возвращает количество объектов SharedPtr,
  // которые ссылаются на тот же управляемый объект
  [[nodiscard]] auto use_count() const -> size_t;
};

template<typename T>
SharedPtr<T>::SharedPtr() { //конструктор по умолчанию
  p= nullptr;
  count = nullptr;
}

template<typename T>
SharedPtr<T>::SharedPtr(T *ptr) { //конструктор
  p= ptr;
  count = new std::atomic_uint{1}; //при создании первого объекта создается счетчик ссылок
}

template<typename T>
SharedPtr<T>::SharedPtr(const SharedPtr&r) { //конструктор копирования
  count = nullptr;
  *this=r;
}

template<typename T>
SharedPtr<T>::SharedPtr(SharedPtr&&r) { //конструктор перемещения
  count = nullptr;
  *this=std::move(r);
}

template<typename T>
SharedPtr<T>::~SharedPtr() { //деструктор
  if (count == nullptr)
    return;
  (count)--; //если счетчик ссылок не пуст, вычитаем единицу из счетчика
  if (count == 0) { //если ссылки закончились, удаляем счетчик и очищаем память
    delete p;
    delete count;
  }
}

template<typename T>
auto SharedPtr<T>::operator=(const SharedPtr&r) -> SharedPtr& { //перегрузка оператора = (копирование)
  if(this==&r)
    return *this;

  this->~SharedPtr();

  p=r.p;
  count =r.count;
  (*count)++;

  return *this;
}

template<typename T>
auto SharedPtr<T>::operator=(SharedPtr&&r) -> SharedPtr& { //перегрузка оператора = (пермещение)
  if(this==&r)
    return *this;

  this->~SharedPtr();

  p=r.p;
  count =r.count;
  r.count = nullptr;
  r.p= nullptr;

  return *this;
}
// проверяет, указывает ли указатель на объект
template<typename T>
SharedPtr<T>::operator bool() const {
  return p != nullptr;
}

template<typename T>
auto SharedPtr<T>::operator*() const -> T & { //перегрузка *
  return *p;
}

template<typename T>
auto SharedPtr<T>::operator->() const -> T * { //перегрузка ->
  return p;
}

template<typename T>
auto SharedPtr<T>::get() -> T * {//возвращение указателя
  return p;
}

template<typename T>
void SharedPtr<T>::reset() { //сброс
  *this= SharedPtr();
}

template<typename T>
void SharedPtr<T>::reset(T *ptr) { //сброс
  *this= SharedPtr(ptr);
}

template<typename T>
void SharedPtr<T>::swap(SharedPtr&r) {//подмена
  std::swap(p, r.p);
  std::swap(count, r.count);
}

// возвращает количество объектов SharedPtr,
// которые ссылаются на тот же управляемый объект

template<typename T>
auto SharedPtr<T>::use_count() const -> size_t {
  if(count != nullptr)
    return *count;
  else
    return 0;
}

#endif // INCLUDE_SHAREDPTR_HPP_
