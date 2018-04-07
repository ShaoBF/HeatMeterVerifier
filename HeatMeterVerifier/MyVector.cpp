#include "stdafx.h"
#include "MyVector.h"

template<typename T>
MyVector<T>::MyVector(int initCount)
{
	max = initCount;
	buffer = new T[max];
	current = 0;
}


template<typename T>
MyVector<T>::~MyVector()
{
	if (buffer != nullptr)
		delete buffer;
}

template<typename T>
void MyVector<T>::Add(T obj){
	//判断是否已满
	if (IsFull()){
		//若已满则扩容
		ExtendCapacity();
	}
	//加入新元素
	buffer[current++] = obj;
}

template<typename T>
bool MyVector<T>::Insert(T obj, int index){
	//判断index是否超界
	if (index<0 || index>current){//超界，加入失败
		return false;
	}
	//判断是否已满
	if (IsFull()){
		//若已满则扩容
		ExtendCapacity();
	}
	//插入新元素
	for (int i = current; i > index; i--){
		buffer[i] = buffer[i - 1];
	}
	buffer[index] = obj;

}

template<typename T>
T MyVector<T>::Get(int index){
	return buffer[index];
}

template<typename T>
T MyVector<T>::Remove(int index){
	//取出第index个
	T obj = buffer[index];
	//之后的向前移一个
	for (int i = index + 1; i < current; i++){
		buffer[i - 1] = buffer[i];
	}
	current--;
	//返回取出的
	return obj;
}

template<typename T>
T MyVector<T>::operator[](int index){
	return buffer[index];
}

template<typename T>
bool MyVector<T>::IsFull(){
	return current == max;
}

template<typename T>
void MyVector<T>::ExtendCapacity(){
	//增加max值
	max += EXTEND_SIZE;
	//创建新数组，使用新max
	T* objects = new T[max];
	//复制原数组
	for (int i = 0; i < current; i++){
		objects[i] = buffer[i];
	}
	//删除原数组
	delete buffer;
	buffer = objects;
}

template<typename T>
int MyVector<T>::GetSize(){
	return current;
}

template<typename T>
void MyVector<T>::Clear(){
	current = 0;
}