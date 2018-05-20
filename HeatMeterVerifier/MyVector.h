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
	bool Insert(T obj, int index);
	T Get(int index);
	int Find(T obj);
	T Remove(int index);
	T Remove(T obj);
	int GetSize();
	void Clear();
	T operator[](int index);

protected:
	bool IsFull();
	void ExtendCapacity();
};
