#pragma once

#include <WString.h>

class StringIterator
{
public:
	StringIterator(String& string) : string(reinterpret_cast<StringBuffer*>(&string))
	{
	}

	operator char*()
	{
		if(!*string) {
			string->setLength(0);
		}
		return string->curPtr();
	}

	StringIterator& operator++()
	{
		string->inc();
		return *this;
	}

	char* operator++(int)
	{
		string->inc();
		return string->curPtr() - 1;
	}

private:
	class StringBuffer : public String
	{
	public:
		char* curPtr()
		{
			return begin() + length();
		}

		bool inc()
		{
			auto len = length() + 1;
			setlen(len);
			auto cap = capacity();
			return (length() < cap) ?: reserve(cap + 64);
		}
	};

	StringBuffer* string;
};
