#include "stdafx.h"
#include "ColumnProperty.h"


ColumnProperty::ColumnProperty(){

}

ColumnProperty::~ColumnProperty(){

}

ColumnProperty::ColumnProperty(CString name, int format, int width){
	this->name = name;
	this->format = format;
	this->width = width;
}

