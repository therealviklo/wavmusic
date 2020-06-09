#pragma once
#include "textimage.h"

TextImage::TextImage(int width, int height)
{
	this->width = width;
	this->height = height;
	image = new CHAR_INFO[width*height];
	for (int i = 0; i < height; i++)
	{
		for (int j = 0; j < width; j++)
		{
			set(j, i, {{L'\0'}, 0});
		}
	}
}

TextImage::TextImage(Textdisp& textdisp, int x, int y, int width, int height)
{
	this->width = width;
	this->height = height;
	image = new CHAR_INFO[width*height];
	for (int i = 0; i < height; i++)
	{
		for (int j = 0; j < width; j++)
		{
			set(j, i, {{textdisp.getChar(x+j, y+i)}, textdisp.getAttributes(x+j, y+i)});
		}
	}
}

TextImage::~TextImage()
{
	delete[] image;
}

TextImage::TextImage(const TextImage& o)
{
	this->width = o.width;
	this->height = o.height;
	image = new CHAR_INFO[width*height];
	for (int i = 0; i < width*height; i++)
	{
		image[i] = o.image[i];
	}
}

TextImage& TextImage::operator=(const TextImage& o)
{
	if (this != &o)
	{
		delete[] image;
		
		this->width = o.width;
		this->height = o.height;
		image = new CHAR_INFO[width*height];
		for (int i = 0; i < width*height; i++)
		{
			image[i] = o.image[i];
		}
	}
	return *this;
}

void TextImage::set(int x, int y, CHAR_INFO c)
{
	if (x >= 0 &&
		x < width &&
		y >= 0 &&
		y < height)
	{
		image[x+y*width] = c;
	}
}

CHAR_INFO TextImage::get(int x, int y)
{
	if (x >= 0 &&
		x < width &&
		y >= 0 &&
		y < height)
	{
		return image[x+y*width];
	}
	else
	{
		return {{L'\0'}, 0};
	}
}

void TextImage::draw(Textdisp& textdisp, int x, int y)
{
	for (int i = 0; i < height; i++)
	{
		for (int j = 0; j < width; j++)
		{
			CHAR_INFO c = get(j, i);
			textdisp.set(x+j, y+i, c.Char.UnicodeChar, c.Attributes);
		}
	}
}