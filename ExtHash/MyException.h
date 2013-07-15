#include <string>

#ifndef MY_EXCEPTION_H
#define MY_EXCEPTION_H
class MyException {
private:
	std::string _reason;

public:
	MyException(std::string reason);
	std::string GetReason();
};
#endif