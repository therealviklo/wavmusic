#pragma once
#include "foldersearch.h"

template <>
FolderSearch<char>::FolderSearch(StringType path)
{
	if (path == "") throw std::runtime_error("Invalid path");

	this->path = path;

	WIN32_FIND_DATAA win32_find_data;
	folderSearchHandle = FindFirstFileA((path + "\\*").c_str(), &win32_find_data);
	if (folderSearchHandle == INVALID_HANDLE_VALUE)
	{
		folderSearchResult.isValid = false;
		if (GetLastError() != ERROR_FILE_NOT_FOUND) throw std::runtime_error("Cannot start folder search");
		folderSearchResult.atEndOfDirectory = true;
	}
	else
	{
		folderSearchResult.filename = win32_find_data.cFileName;
		folderSearchResult.isFolder = win32_find_data.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY;
		folderSearchResult.isValid = true;
		folderSearchResult.atEndOfDirectory = false;
	}
}

template <>
FolderSearch<wchar_t>::FolderSearch(StringType path)
{
	if (path == L"") throw std::runtime_error("Invalid path");

	this->path = path;

	WIN32_FIND_DATAW win32_find_data;
	folderSearchHandle = FindFirstFileW((path + L"\\*").c_str(), &win32_find_data);
	if (folderSearchHandle == INVALID_HANDLE_VALUE)
	{
		folderSearchResult.isValid = false;
		if (GetLastError() != ERROR_FILE_NOT_FOUND) throw std::runtime_error("Cannot start folder search");
		folderSearchResult.atEndOfDirectory = true;
	}
	else
	{
		folderSearchResult.filename = win32_find_data.cFileName;
		folderSearchResult.isFolder = win32_find_data.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY;
		folderSearchResult.isValid = true;
		folderSearchResult.atEndOfDirectory = false;
	}
}

template <typename CharType>
FolderSearch<CharType>::~FolderSearch()
{
	if (folderSearchHandle != INVALID_HANDLE_VALUE) FindClose(folderSearchHandle);
}

template <typename CharType>
std::basic_string<CharType> FolderSearch<CharType>::getFilename()
{
	if (folderSearchResult.isValid)
	{
		return folderSearchResult.filename;
	}
	else
	{
		return StringType();
	}
}

template <typename CharType>
bool FolderSearch<CharType>::isFolder()
{
	if (folderSearchResult.isValid)
	{
		return folderSearchResult.isFolder;
	}
	else
	{
		return false;
	}
}

template <>
bool FolderSearch<char>::next()
{
	if (folderSearchHandle == INVALID_HANDLE_VALUE)
	{
		return false;
	}
	else
	{
		WIN32_FIND_DATAA win32_find_data;
		if (FindNextFileA(folderSearchHandle, &win32_find_data) == 0)
		{
			folderSearchResult.isValid = false;
			if (GetLastError() != ERROR_NO_MORE_FILES) throw std::runtime_error("Error when searching folder");
			folderSearchResult.atEndOfDirectory = true;
			return false;
		}
		else
		{
			folderSearchResult.filename = win32_find_data.cFileName;
			folderSearchResult.isFolder = win32_find_data.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY;
			folderSearchResult.isValid = true;
			folderSearchResult.atEndOfDirectory = false;
			return true;
		}
	}
}

template <>
bool FolderSearch<wchar_t>::next()
{
	if (folderSearchHandle == INVALID_HANDLE_VALUE)
	{
		return false;
	}
	else
	{
		WIN32_FIND_DATAW win32_find_data;
		if (FindNextFileW(folderSearchHandle, &win32_find_data) == 0)
		{
			folderSearchResult.isValid = false;
			if (GetLastError() != ERROR_NO_MORE_FILES) throw std::runtime_error("Error when searching folder");
			folderSearchResult.atEndOfDirectory = true;
			return false;
		}
		else
		{
			folderSearchResult.filename = win32_find_data.cFileName;
			folderSearchResult.isFolder = win32_find_data.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY;
			folderSearchResult.isValid = true;
			folderSearchResult.atEndOfDirectory = false;
			return true;
		}
	}
}