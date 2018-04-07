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
	//�ж��Ƿ�����
	if (IsFull()){
		//������������
		ExtendCapacity();
	}
	//������Ԫ��
	buffer[current++] = obj;
}

template<typename T>
bool MyVector<T>::Insert(T obj, int index){
	//�ж�index�Ƿ񳬽�
	if (index<0 || index>current){//���磬����ʧ��
		return false;
	}
	//�ж��Ƿ�����
	if (IsFull()){
		//������������
		ExtendCapacity();
	}
	//������Ԫ��
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
	//ȡ����index��
	T obj = buffer[index];
	//֮�����ǰ��һ��
	for (int i = index + 1; i < current; i++){
		buffer[i - 1] = buffer[i];
	}
	current--;
	//����ȡ����
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
	//����maxֵ
	max += EXTEND_SIZE;
	//���������飬ʹ����max
	T* objects = new T[max];
	//����ԭ����
	for (int i = 0; i < current; i++){
		objects[i] = buffer[i];
	}
	//ɾ��ԭ����
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