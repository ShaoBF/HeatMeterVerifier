#pragma once

#define EXTEND_SIZE 32

template < typename T >
class MyVector
{
public:
	MyVector(int initCount=32);
	virtual ~MyVector();
	

protected:
	T* buffer;
	int current;
	int max;

public:
	void Add(T obj);
	T Get(int index);
	T Remove(int index);
	int GetSize();
	void Clear();
	T operator[](int index);

protected:
	bool IsFull();
	void ExtendCapacity();
};
