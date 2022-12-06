#pragma once   
#include <iostream>																
#define constant 2

template<typename T>
class Vector {
	size_t size;
	T* data;
	size_t capacity;
public:
	Vector() :size(0), capacity(0), data(0) {}
	Vector(size_t n) {
		data = new T[constant * n]();
		capacity = constant * n;
		size = n;
	}

	~Vector() {
		delete[] data;
	}

	Vector(const Vector& v) {
		resize(v.size);
		for (size_t i = 0; i < v.size; i++) {
			this->data[i] = v.data[i];
		}
	}

	void resize(size_t newSize) {
		if (newSize < this->capacity)
		{
			this->size = newSize;
		}
		else
		{
			T* copy = new T[constant * newSize]();
			for (size_t i = 0; i < this->size; i++)
				copy[i] = data[i];
			delete[] this->data;
			this->data = copy;
			this->size = newSize;
			this->capacity = constant * newSize;
		}
	}

	Vector& operator=(const Vector& v) {
		if (this == &v) return *this;
		this->resize(v.size);
		for (size_t i = 0; i < v.size; i++)
			this->data[i] = v.data[i];
		return *this;
	}

	void push_back(T a) {
		this->resize(size + 1);
		this->data[size - 1] = a;
	}

	void pop_back() {
		this->resize(size - 1);
	}

	bool empty() const noexcept {
		if (size == 0) return 1;
		return 0;
	}

	T back() noexcept {
		return data[size - 1];
	}

	void erase(size_t a) {
		if (a < this->size) {
			this->resize(size - 1);
			for (size_t i = a; i < size; i++)
				this->data[i] = this->data[i + 1];
		}
		else {
			for (size_t i = a; i < size; i++)
				this->data[i] = this->data[i + 1];
		}
	}

	void erase(size_t a, size_t b) {
		if (b < this->size)
		{
			this->resize(size + a - b - 1);
			for (size_t i = a; i < b; i++)
				this->data[i] = this->data[i + b + 1];
		}
		else {
			this->resize(a);
		}
	}

	void insert(size_t i, T a) {
		if (i < size) {
			this->resize(size + 1);
			for (size_t e = size; e > i; e--) {
				this->data[e] = data[e - 1];
			}
			this->data[i] = a;
		}
		else this->push_back(a);
	}

	// test purposes  
	size_t get_size() const noexcept {
		return size;
	}

	T& operator[](size_t i) {
		return this->data[i];
	}

	T operator[](size_t i) const {
		return this->data[i];
	}

};