#pragma once
#include <stdexcept>
#include <iostream>

template <typename T>
class MyVector {
public:
	MyVector();
	MyVector(const MyVector& other);
	MyVector& operator=(const MyVector<T>& other);
	~MyVector();

	void push_back(const T& element);
	T pop_back();
	void insert(const T& element, size_t position);
	T& operator[](size_t index);
	const T& operator[](size_t index) const;
	size_t getSize() const;
	size_t getCapacity() const;
	void print() const;
private:
	T* data;
	size_t size;
	size_t capacity;

	void copyFrom(const MyVector<T>& other);
	void free();
	void resize();
};

template<typename T>
void MyVector<T>::copyFrom(const MyVector<T>& other)
{
	size = other.size;
	capacity = other.capacity;

	data = new T[capacity];
	for (size_t i = 0; i < size; i++)
	{
		data[i] = other.data[i];
	}
}

template<typename T>
void MyVector<T>::free()
{
	delete[] data;
	data = nullptr;
	size = 0;
	capacity = 0;
}

template<typename T>
void MyVector<T>::resize()
{
	capacity *= 2;
	T* temp = new T[capacity];

	for (size_t i = 0; i < size; i++)
	{
		temp[i] = data[i];
	}

	free();
	data = temp;
}

template<typename T>
MyVector<T>::MyVector() : size(0), capacity(4) {
	data = new T[capacity];
}

template<typename T>
MyVector<T>::MyVector(const MyVector<T>& other)
{
	copyFrom(other);
}

template<typename T>
MyVector<T>& MyVector<T>::operator=(const MyVector<T>& other)
{
	if (this != &other)
	{
		free();
		copyFrom(other);
	}

	return *this;
}

template<typename T>
MyVector<T>::~MyVector()
{
	free();
}

template<typename T>
void MyVector<T>::push_back(const T& element)
{
	if (size >= capacity)
	{
		resize();
	}

	data[size++] = element;
}

template<typename T>
T MyVector<T>::pop_back()
{
	if (size <= 0)
	{
		throw std::out_of_range("Size is 0, can't pop_back");
	}
	size--;
	return data[size];
}

template<typename T>
void MyVector<T>::insert(const T& element, size_t index)
{
	if (index >= size)
	{
		throw std::out_of_range("Position must be less than size");
	}

	if (size + 1 >= capacity)
	{
		resize();
	}

	T temp = data[index];
	data[index] = element;
	for (size_t i = index + 1; i < size; i++)
	{
		T temp1 = data[i];
		data[i] = temp;
		temp = temp1;
	}
	data[size++] = temp;
}

template<typename T>
T& MyVector<T>::operator[](size_t index)
{
	if (index >= size)
	{
		throw std::out_of_range("Index must be positive and less than size");
	}

	return data[index];
}

template<typename T>
const T& MyVector<T>::operator[](size_t index) const
{
	if (index >= size)
	{
		throw std::out_of_range("Index must be positive and less than size");
	}

	return data[index];
}

template<typename T>
size_t MyVector<T>::getSize() const
{
	return size;
}

template<typename T>
size_t MyVector<T>::getCapacity() const
{
	return capacity;
}

template<typename T>
void MyVector<T>::print() const
{
	if (size <= 0)
	{
		std::cout << "Vector is empty\n";
	}
	for (size_t i = 0; i < size; i++)
	{
		std::cout << data[i] << " ";
	}
	std::cout << std::endl;
}