#pragma once

#include <initializer_list>
#include "Math.h"


template<typename T>
using InitializerList = std::initializer_list<T>;

template<typename T>
class DynamicArray {

  //Дружественная функция для отладки
  friend void dynamicArrayFriend();

  //Дружественны объекты этого-же класса другой специализации
  friend class DynamicArray;

private:

  //Размер области
  size_t size;
  //Указатель на область
  T* ptr;

  //Специальный конструктор
  DynamicArray(size_t size, T* ptr);

public:

  //Деструктор
  ~DynamicArray();


  //Конструктор по умолчанию
  DynamicArray();

  //Конструктор с указанием размера области
  DynamicArray(const size_t arrSize);
  //Конструктор через список элементов
  DynamicArray(const InitializerList<T>& list);
  //Конструктор через список массивов
  DynamicArray(const InitializerList<DynamicArray<T>>& list);
  //Конструктор копирования
  DynamicArray(const DynamicArray<T>& list);


  //Оператор присваивания другого массив
  DynamicArray<T>& operator=(const DynamicArray<T>& other);
  ///Оператор присваивания через {T, T}
  DynamicArray<T>& operator=(const InitializerList<T>& list);
  //Оператор присваивания через {DynamicArray<T>, DynamicArray<T>}
  DynamicArray<T>& operator=(const InitializerList<DynamicArray<T>>& list);


  //Присваивание массива с уничтожением массива-операнда
  DynamicArray<T>& assign(DynamicArray<T>& other);


  //Оператор сложения массивов
  DynamicArray<T> operator+(const DynamicArray<T>& other) const;
  //Оператор доприбавления
  DynamicArray<T>& operator+=(const DynamicArray<T>& other);


  //Изменение размера массива без копирования элементов
  DynamicArray<T>& setSize(const size_t newSize);
  //Изменение размера массива с копированием элементов
  DynamicArray<T>& resize(const size_t newSize);


  //Равны ли массивы
  bool operator==(const DynamicArray<T>& other) const;
  //Не равны ли массивы
  bool operator!=(const DynamicArray<T>& other) const;

  //Меньше ли массив
  bool operator<(const DynamicArray<T>& other) const;
  //Больше ли массив
  bool operator>(const DynamicArray<T>& other) const;



  //Получение доступа к ссылки на элемент
  T& operator[](const size_t index);

  //Получение доступа к копии элемента
  T get(const size_t index) const;


  //Вставить элемент
  DynamicArray<T>& insert(const T element, const size_t index);
  //Вставить массив
  DynamicArray<T>& insert(const DynamicArray<T> array, const size_t index);


  //Вырезать элемент
  DynamicArray<T>& erase(const size_t index);
  //Вырезать часть массива
  DynamicArray<T>& erase(const size_t fIndex, const size_t lIndex);

  //Очистить массив
  DynamicArray<T>& clear();


  //Обменяться элементами с другим массивом
  DynamicArray& swap(DynamicArray<T>& other);

  //Поменять местами элементы
  DynamicArray& swap(size_t aIndex, size_t bIndex);


  //Развернуть массив
  DynamicArray& reverse();

  //Получить массив элементов нового типа
  template<typename nT>
  DynamicArray<nT> cast() const;


  //Получить копию части массива
  DynamicArray<T> getPart(const size_t fIndex, const size_t lIndex) const;
  //Получить копию части массива
  DynamicArray<T> getPart(const DynamicArray<T> indices) const;


  //Получить массив константных указателей на элементы части массива
  DynamicArray<T*> selectPart(const size_t fIndex, const size_t lIndex);
  //Получить массив константных указателей на элементы части массива
  DynamicArray<T*> selectPart(const DynamicArray<T> indices);


  //Итерационное прохождение по массиву с применением функции
  //Параметром функции будет элемент массива
  DynamicArray<T>& applyFunction(void (*function)(const T&)) const;

  //Итерационное прохождение по массиву с применением функции
  //элемент будет заменён вовращаемым функцией значением
  DynamicArray<T>& applyFunction(T (*function)());

  //Итерационное прохождение по массиву с применением функции
  //Параметром функции будет элемент массива, элемент будет
  //заменён вовращаемым функцией значением
  DynamicArray<T>& applyFunction(T (*function)(const T&));

public:

  //Получение размера массива
  size_t getSize() const;

  //Пустой ли массив
  bool isEmpty() const;
  //Не пустой ли массив
  bool isNotEmpty() const;


  //Получение указателей
  //на границы массива
  T* begin();
  T* end();

  //Получение readonly
  //указателей на границы массива
  T const* begin() const;
  T const* end() const;
};



template<typename T>
inline DynamicArray<T>::~DynamicArray() {
  if (size)
    delete[] ptr;
}



template<typename T>
inline DynamicArray<T>::DynamicArray() {
  size = 0;
  ptr = nullptr;
}

template<typename T>
inline DynamicArray<T>::DynamicArray(size_t size, T* ptr) {
  this->size = size;
  this->ptr = ptr;
}

template<typename T>
inline DynamicArray<T>::DynamicArray(const size_t arrSize) {
  size = arrSize;

  if (size)
    ptr = new T[size];
  else
    ptr = nullptr;
}

template<typename T>
inline DynamicArray<T>::DynamicArray(const InitializerList<T>& list)
  : DynamicArray(list.size()) {
  for (size_t i = 0; i < size; i++) {
    ptr[i] = *(list.begin() + i);
  }
}

template<typename T>
inline DynamicArray<T>::DynamicArray(const InitializerList<DynamicArray<T>>& list) {
  size = 0;

  for (size_t i = 0; i < list.size(); i++) {
    size += (list.begin() + i)->size;
  }

  if (size)
    ptr = new T[size];
  else
    ptr = nullptr;

  for (size_t i = 0, k = 0; i < list.size(); i++) {
    for (size_t j = 0; j < (list.begin() + i)->size; j++, k++) {
      this->ptr[k] = (list.begin() + i)->ptr[j];
    }
  }
}

template<typename T>
inline DynamicArray<T>::DynamicArray(const DynamicArray<T>& other)
  : DynamicArray(other.size) {
  for (size_t i = 0; i < this->size; i++) {
    this->ptr[i] = other.ptr[i];
  }
}



template<typename T>
inline DynamicArray<T>& DynamicArray<T>::operator=(const DynamicArray<T>& other) {
  this->setSize(other.size);

  for (size_t i = 0; i < this->size; i++) {
    this->ptr[i] = other.ptr[i];
  }

  return *this;
}

template<typename T>
inline DynamicArray<T>& DynamicArray<T>::operator=(const InitializerList<T>& list) {
  this->setSize(list.size());

  for (size_t i = 0; i < this->size; i++) {
    this->ptr[i] = *(list.begin() + i);
  }
  return *this;
}

template<typename T>
inline DynamicArray<T>& DynamicArray<T>::operator=(const InitializerList<DynamicArray<T>>& list) {
  size_t size = 0;

  for (size_t i = 0; i < list.size(); i++) {
    size += (list.begin() + i)->size;
  }

  setSize(size);

  for (size_t i = 0, k = 0; i < list.size(); i++) {
    for (size_t j = 0; j < (list.begin() + i)->size; j++, k++) {
      this->ptr[k] = (list.begin() + i)->ptr[j];
    }
  }
  return *this;
}



template<typename T>
inline DynamicArray<T>& DynamicArray<T>::assign(DynamicArray<T>& other) {
  if (this->size)
    delete[] this->ptr;

  this->size = other.size;
  this->ptr = other.ptr;

  other.size = 0;
  other.ptr = nullptr;

  return *this;
}



template<typename T>
inline DynamicArray<T> DynamicArray<T>::operator+(const DynamicArray<T>& other) const {
  DynamicArray<T> result(this->size + other.size);

  for (size_t i = 0; i < this->size; i++) {
    result.ptr[i] = this->ptr[i];
  }

  for (size_t i = 0; i < other.size; i++) {
    result.ptr[i + this->size] = other.ptr[i];
  }

  return result;
}

template<typename T>
inline DynamicArray<T>& DynamicArray<T>::operator+=(const DynamicArray<T>& other) {
  DynamicArray<T> tempArray(this->size + other.size);

  for (int i = 0; i < this->size; i++) {
    tempArray.ptr[i] = this->ptr[i];
  }

  for (int i = 0; i < other.size; i++) {
    tempArray.ptr[i + this->size] = other.ptr[i];
  }

  this->assign(tempArray);

  return *this;
}



template<typename T>
inline DynamicArray<T>& DynamicArray<T>::setSize(const size_t newSize) {
  if (size != newSize) {
    if (size)
      delete[] ptr;

    if (newSize > 0)
      ptr = new T[newSize];

    size = newSize;
  }

  return *this;
}

template<typename T>
inline DynamicArray<T>& DynamicArray<T>::resize(const size_t newSize) {
  if (size != newSize) {
    T* temp = nullptr;

    if (newSize)
      temp = new T[newSize];

    for (size_t i = 0; i < getMin(size, newSize); i++) {
      temp[i] = ptr[i];
    }

    if (size)
      delete[] ptr;

    ptr = temp;
    size = newSize;
  }

  return *this;
}



template<typename T>
inline bool DynamicArray<T>::operator==(const DynamicArray<T>& other) const {
  if (this->size != other.size)
    return false;

  for (size_t i = 0; i < this->size; i++) {
    if (this->ptr[i] != other.ptr[i])
      return false;
  }

  return true;
}

template<typename T>
inline bool DynamicArray<T>::operator!=(const DynamicArray<T>& other) const {
  if (this->size != other.size)
    return true;

  for (size_t i = 0; i < this->size; i++) {
    if (this->ptr[i] != other.ptr[i])
      return true;
  }

  return false;
}

template<typename T>
inline bool DynamicArray<T>::operator<(const DynamicArray<T>& other) const {
  return this->size < other.size;
}

template<typename T>
inline bool DynamicArray<T>::operator>(const DynamicArray<T>& other) const {
  return this->size > other.size;
}



template<typename T>
inline T& DynamicArray<T>::operator[](const size_t index) {
  return ptr[index];
}

template<typename T>
inline T DynamicArray<T>::get(const size_t index) const {
  return ptr[index];
}



template<typename T>
inline DynamicArray<T>& DynamicArray<T>::insert(const T element, const size_t index) {
  DynamicArray<T> tempArray(size + 1);

  for (int i = 0; i < tempArray.size; i++) {
    if (i < index)
      tempArray.ptr[i] = this->ptr[i];
    else if (i > index)
      tempArray.ptr[i] = this->ptr[i - 1];
    else
      tempArray.ptr[i] = element;
  }

  this->assign(tempArray);

  return *this;
}

template<typename T>
inline DynamicArray<T>& DynamicArray<T>::insert(const DynamicArray<T> array, const size_t index) {
  if (array.size) {
    DynamicArray<T> tempArray(this->size + array.size);

    for (int i = 0; i < tempArray.size; i++) {
      if (i < index)
        tempArray.ptr[i] = this->ptr[i];
      else if (i >= index + array.size)
        tempArray.ptr[i] = this->ptr[i - array.size];
      else
        tempArray.ptr[i] = array.ptr[i - index];
    }
    this->assign(tempArray);
  }

  return *this;
}



template<typename T>
inline DynamicArray<T>& DynamicArray<T>::erase(const size_t index) {
  DynamicArray<T> newArray(this->size - 1);

  for (size_t i = 0; i < newArray.size; i++) {
    if (i < index)
      newArray.ptr[i] = ptr[i];
    else
      newArray.ptr[i] = ptr[i + 1];
  }

  this->assign(newArray);

  return *this;
}

template<typename T>
inline DynamicArray<T>& DynamicArray<T>::erase(const size_t fIndex, const size_t lIndex) {
  DynamicArray<T> newArray(this->size - lIndex + fIndex - 1);

  for (size_t i = 0; i < size; i++) {
    if (i < fIndex)
      newArray.ptr[i] = ptr[i];
    else if (i > lIndex)
      newArray.ptr[i - lIndex + fIndex - 1] = ptr[i];
  }

  this->assign(newArray);

  return *this;
}



template<typename T>
inline DynamicArray<T>& DynamicArray<T>::clear() {

  if (size) {
    size = 0;
    delete[] ptr;
  }

  return *this;
}



template<typename T>
inline DynamicArray<T>& DynamicArray<T>::swap(DynamicArray<T>& other) {
  DynamicArray<T> temp(this->size, this->ptr);

  this->size = other.size;
  this->ptr = other.ptr;

  other.size = temp.size;
  other.ptr = temp.ptr;

  temp.size = 0;
  temp.ptr = nullptr;

  return *this;
}

template<typename T>
inline DynamicArray<T>& DynamicArray<T>::swap(size_t aIndex, size_t bIndex) {
  T temp = ptr[aIndex];

  ptr[aIndex] = ptr[bIndex];

  ptr[bIndex] = temp;

  return *this;
}



template<typename T>
inline DynamicArray<T>& DynamicArray<T>::reverse() {
  DynamicArray array(this->size);

  for (int i = 0, j = this->size - 1; i < this->size; i++, j--) {
    array[i] = this->operator[](j);
  }

  this->assign(array);

  return *this;
}



template<typename T>
template<typename nT>
inline DynamicArray<nT> DynamicArray<T>::cast() const {

  DynamicArray<nT> array(this->size);

  for (int i = 0; i < this->size; i++) {
    array[i] = this->ptr[i];
  }

  return array;
}



template<typename T>
inline DynamicArray<T> DynamicArray<T>::getPart(const size_t fIndex, const size_t lIndex) const {

  size_t firstElement = getMin(fIndex, size - 1);
  size_t lastElement = getMin(lIndex, size - 1);

  size_t partSize = lIndex - fIndex + 1;

  DynamicArray<T> part(partSize);

  for (size_t i = firstElement, j = 0; i <= lastElement; i++, j++) {
    part.ptr[j] = this->ptr[i];
  }

  return part;
}

template<typename T>
inline DynamicArray<T> DynamicArray<T>::getPart(const DynamicArray<T> indices) const {
  DynamicArray<T> part(indices.size);

  for (int i = 0; i < indices.size; i++) {
    part.ptr[i] = this->ptr[indices.get(i)];
  }

  return part;
}



template<typename T>
inline DynamicArray<T*> DynamicArray<T>::selectPart(const size_t fIndex, const size_t lIndex) {
  size_t firstElement = getMin(fIndex, size - 1);
  size_t lastElement = getMin(lIndex, size - 1);

  size_t partSize = lIndex - fIndex + 1;

  DynamicArray<T*> part(partSize);

  for (size_t i = firstElement, j = 0; i <= lastElement; i++, j++) {
    part.ptr[j] = this->ptr + i;
  }

  return part;
}

template<typename T>
inline DynamicArray<T*> DynamicArray<T>::selectPart(const DynamicArray<T> indices) {
  DynamicArray<T*> part(indices.size);

  for (int i = 0; i < indices.size; i++) {
    part.ptr[i] = this->ptr + indices.get(i);
  }

  return part;
}



template<typename T>
inline DynamicArray<T>& DynamicArray<T>::applyFunction(void (*function)(const T&)) const {
  for (size_t i = 0; i < this->size; i++) {
    function(ptr[i]);
  }

  return *this;
}

template<typename T>
inline DynamicArray<T>& DynamicArray<T>::applyFunction(T (*function)()) {
  for (size_t i = 0; i < this->size; i++) {
    ptr[i] = function();
  }

  return *this;
}

template<typename T>
inline DynamicArray<T>& DynamicArray<T>::applyFunction(T (*function)(const T&)) {
  for (size_t i = 0; i < this->size; i++) {
    ptr[i] = function(ptr[i]);
  }

  return *this;
}



template<typename T>
inline size_t DynamicArray<T>::getSize() const {
  return size;
}



template<typename T>
inline bool DynamicArray<T>::isEmpty() const {
  return !size;
}

template<typename T>
inline bool DynamicArray<T>::isNotEmpty() const {
  return size;
}



template<typename T>
inline T* DynamicArray<T>::begin() {
  return ptr;
}

template<typename T>
inline T* DynamicArray<T>::end() {
  return ptr + size;
}



template<typename T>
inline T const* DynamicArray<T>::begin() const {
  return ptr;
}

template<typename T>
inline T const* DynamicArray<T>::end() const {
  return ptr + size;
}
