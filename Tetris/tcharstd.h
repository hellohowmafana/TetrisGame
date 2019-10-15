#pragma once
#include <string>
#include <sstream>

namespace std
{
#ifdef _UNICODE
	typedef wstring tstring;
	typedef wfstream tfstream;
	typedef wistringstream tistringstream;
#elif
	typedef string tstring;
	typedef fstream tfstream;
	typedef istringstream tistringstream;
#endif
}
