#include "MyException.h"

MyException::MyException(std::string reason) {
	_reason = reason;
}

std::string MyException::GetReason() {
	return _reason;
}