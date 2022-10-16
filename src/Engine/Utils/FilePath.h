// ----------------------------------------------------------------------------
#ifndef FILE_PATH_H_
#define FILE_PATH_H_
// ----------------------------------------------------------------------------

#include <string>
// ----------------------------------------------------------------------------
// FilePath
// ----------------------------------------------------------------------------
struct FilePath
{
	std::string mDirectory;
	std::string mExtension;
	std::string mFilename;
	std::string mFullPath;

	// CTOR
	FilePath();
	FilePath(const char * path);

	// PARSE OPERATION
	void ParsePath();
};

// ----------------------------------------------------------------------------
#endif
