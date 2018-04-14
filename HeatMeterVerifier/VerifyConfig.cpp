#include "stdafx.h"
#include "VerifyConfig.h"


VerifyConfig::VerifyConfig()
{
}


VerifyConfig::~VerifyConfig()
{
}

double VerifyConfig::GetVerifyRate(){
	return verifyRate;
}
void VerifyConfig::SetVerifyRate(double verifyRate){
	this->verifyRate = verifyRate;
}
