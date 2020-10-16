#pragma once

#include <Print.h>

class PrintBuffer
{
public:
	PrintBuffer(Print& out) : out(out)
	{
	}

	~PrintBuffer()
	{
		flushAll();
	}

	char* curPtr()
	{
		return &buf[len];
	}

	void inc()
	{
		++len;
		assert(len < bufSize);
	}

	void flush()
	{
		constexpr auto min = bufSize - 4;
		if(len >= min) {
			count_ += out.write(buf, min);
			len -= min;
			memmove(buf, &buf[min], len);
		}
	}

	void flushAll()
	{
		if(len > 0) {
			count_ += out.write(buf, len);
			len = 0;
		}
	}

	unsigned count()
	{
		return count_ + len;
	}

private:
	Print& out;
	static constexpr unsigned bufSize = 64;
	char buf[bufSize];
	uint16_t count_ = 0;
	uint8_t len = 0;
};

class PrintIterator
{
public:
	PrintIterator(PrintBuffer& buffer) : buffer(&buffer)
	{
	}

	operator char*()
	{
		return buffer->curPtr();
	}

	PrintIterator& operator++()
	{
		buffer->inc();
		buffer->flush();
		return *this;
	}

	char* operator++(int)
	{
		buffer->flush();
		char* result = buffer->curPtr();
		buffer->inc();
		return result;
	}

private:
	PrintBuffer* buffer;
};
