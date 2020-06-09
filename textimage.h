#pragma once
#include "textdisp.cpp"

class TextImage
{
private:
	CHAR_INFO* image;
	int width;
	int height;
public:
	TextImage(int width, int height);
	TextImage(Textdisp& textdisp, int x, int y, int width, int height);
	~TextImage();

	TextImage(const TextImage& o);
	TextImage& operator=(const TextImage& o);
	
	void set(int x, int y, CHAR_INFO c);
	CHAR_INFO get(int x, int y);

	void draw(Textdisp& textdisp, int x, int y);
};