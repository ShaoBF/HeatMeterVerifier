#pragma once
class VerifyConfig
{
public:
	VerifyConfig();
	virtual ~VerifyConfig();

	double GetVerifyRate();
	void SetVerifyRate(double verifyRate);

	double verifyRate = 0.01;
};

