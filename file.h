#pragma once
#include <cstdio>
#include <stdexcept>
#include <string>
#include <cstdint>

int32_t maxValue(unsigned int numBytes);
template <typename T>
int8_t getByte(T number, int byteNumber);
template <typename T>
T getSignBit(int numBytes);
//så att tecknet hamnar på på rätt ställe
template <typename T>
T fixSignFromSmaller(T number, int numBytes);
template <typename T>
T fixSignToSmaller(T number, int numBytes);

class File
{
private:
	FILE* file;
public:
	File(std::string name, std::string mode);
	~File();

	File(const File&) = delete;
	File& operator=(const File&) = delete;

	int get();

	template <typename T, bool littleEndian = false>
	T read(unsigned int numBytes = sizeof(T));
	template <typename T, bool littleEndian = false>
	T readUnsigned(unsigned int numBytes = sizeof(T));
	std::string readString(unsigned int numChars);

	template <typename T, bool littleEndian = false>
	void write(T number, unsigned int numBytes = sizeof(T));
	template <typename T, bool littleEndian = false>
	void writeUnsigned(T number, unsigned int numBytes = sizeof(T));
	void writeString(std::string string);

	void skip(unsigned int bytes);
};