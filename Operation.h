#pragma once



extern "C" __declspec(dllexport)
void  Setting(int numOfOperand, int numOfQuestion, double rangeOfQuestion, bool addition, bool subtraction, bool multiplication, 
	bool division, bool power, bool brackets, int precision, bool properFraction, bool decimalFraction);
	
extern "C" __declspec(dllexport) 
void  GenerateAndCalc();

extern "C" __declspec(dllexport) 
const char*  getExpression(int count);

extern "C" __declspec(dllexport)
const char*  getAnswer(int count);