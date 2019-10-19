#pragma once
#include <string>
#include <sstream>

namespace std
{
#ifdef _UNICODE
	typedef wstring tstring;
	typedef wfstream tfstream;
	typedef wistringstream tistringstream;

	#define to_tstring(val) to_wstring(val)
#elif
	typedef string tstring;
	typedef fstream tfstream;
	typedef istringstream tistringstream;

	#define to_tstring(val) to_string(val)
#endif
}
