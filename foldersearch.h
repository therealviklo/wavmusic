#pragma once
#include <windows.h>
#include <string>
#include <stdexcept>

template <typename CharType>
class FolderSearch
{
public:
	typedef std::basic_string<CharType> StringType;
	struct FolderSearchResult
	{
		StringType filename;
		bool isFolder;
		bool isValid;
		bool atEndOfDirectory;
	};
private:
	HANDLE folderSearchHandle;
	FolderSearchResult folderSearchResult;
	StringType path;
public:
	FolderSearch(StringType path);
	~FolderSearch();

	StringType getFilename();
	bool isFolder();
	StringType getDirectory() {return path;}

	bool next();
	bool atEndOfDirectory() {return folderSearchResult.atEndOfDirectory;}
};
typedef FolderSearch<char> FolderSearchA;
typedef FolderSearch<wchar_t> FolderSearchW;