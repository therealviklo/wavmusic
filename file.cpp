#pragma once
#include "file.h"
#include <exception>

int32_t maxValue(unsigned int numBytes)
{
	int32_t value = 0b01111111;
	for (int i = 1; i < numBytes; i++) //börjar på ett för att det ska köras en gång mindre
	{
		value <<= 8;
		value += 0xff;
	}
	return value;
}

template <typename T>
int8_t getByte(T number, int byteNumber)
{
	return (number >> (8*byteNumber)) & 0xff;
}

template <typename T>
T getSignBit(int numBytes)
{
	return (0b10000000 << (8 * (numBytes-1)));
}

template <typename T>
T fixSignFromSmaller(T number, int numBytes)
{
	if (number & getSignBit<T>(numBytes))
	{
		number ^= getSignBit<T>(numBytes);
		number = maxValue(numBytes) - number + 1;
		number *= -1;
		return number;
	}
	else
	{
		return number;
	}
}

template <typename T>
T fixSignToSmaller(T number, int numBytes)
{
	if (number < 0)
	{
		number *= -1;
		number = maxValue(numBytes) - number + 1;
		number |= getSignBit<T>(numBytes);
		return (number | getSignBit<T>(numBytes));
	}
	else
	{
		return number;
	}
}

File::File(const std::string& name, const std::string& mode)
{
	file = fopen(name.c_str(), mode.c_str());
	if (file == nullptr) throw std::runtime_error("Can't open file");
}

File::~File()
{
	fclose(file);
}

int File::get()
{
	int byte = getc(file);
	if (byte == EOF) throw std::runtime_error("Reached end of file");
	return byte;
}

template <typename T, bool littleEndian>
T File::read(unsigned int numBytes)
{
	if (littleEndian)
	{
		T value = 0;
		for (int i = 0; i < numBytes; i++)
		{
			int byte = getc(file);
			if (byte == EOF) throw std::runtime_error("Reached end of file");
			value += byte << (8*i);
		}
		return fixSignFromSmaller(value, numBytes);
	}
	else
	{
		T value = 0;
		for (int i = numBytes - 1; i >= 0; i--)
		{
			int byte = getc(file);
			if (byte == EOF) throw std::runtime_error("Reached end of file");
			value += byte << (8*i);
		}
		return fixSignFromSmaller(value, numBytes);
	}
}

template <typename T, bool littleEndian>
T File::readUnsigned(unsigned int numBytes)
{
	if (littleEndian)
	{
		T value = 0;
		for (int i = 0; i < numBytes; i++)
		{
			int byte = getc(file);
			if (byte == EOF) throw std::runtime_error("Reached end of file");
			value += byte << (8*i);
		}
		return value;
	}
	else
	{
		T value = 0;
		for (int i = numBytes - 1; i >= 0; i--)
		{
			int byte = getc(file);
			if (byte == EOF) throw std::runtime_error("Reached end of file");
			value += byte << (8*i);
		}
		return value;
	}
}

std::string File::readString(unsigned int numChars)
{
	std::string result;
	for (int i = 0; i < numChars; i++)
	{
		result += read<char>();
	}
	return result;
}

template <typename T, bool littleEndian>
void File::write(T number, unsigned int numBytes)
{
	number = fixSignToSmaller(number, numBytes);
	if (littleEndian)
	{
		for (int i = 0; i < numBytes; i++)
		{
			putc(getByte(number, i), file);
		}
	}
	else
	{
		for (int i = numBytes - 1; i >= 0; i--)
		{
			putc(getByte(number, i), file);
		}
	}
}

template <typename T, bool littleEndian>
void File::writeUnsigned(T number, unsigned int numBytes)
{
	if (littleEndian)
	{
		for (int i = 0; i < numBytes; i++)
		{
			putc(getByte(number, i), file);
		}
	}
	else
	{
		for (int i = numBytes - 1; i >= 0; i--)
		{
			putc(getByte(number, i), file);
		}
	}
}

void File::writeString(const std::string& string)
{
	for (auto i = string.begin(); i != string.end(); i++)
	{
		write(*i);
	}
}

void File::skip(unsigned int bytes)
{
	if (fseek(file, bytes, SEEK_CUR) != 0) throw std::runtime_error("Could not skip");
}