#include "FilePath.h"
// ----------------------------------------------------------------------------
// ----------------------------------------------------------------------------
// FilePath
FilePath::FilePath()
{}
FilePath::FilePath(const char * path)
{
	mFullPath = path;

	// get the info
	ParsePath();
}
void FilePath::ParsePath()
{
	// safeguard
	if (mFullPath.length() == 0)
		return;

	// parse the mFullPath and extract

	// mDirectory
	std::size_t dir_offset = mFullPath.find_last_of("\\");
	mDirectory = mFullPath.substr(0, dir_offset + 1);

	// mExtension
	std::size_t ext_offset = mFullPath.find_last_of(".");
	mExtension = mFullPath.substr(ext_offset);

	// mFilename
	mFilename = mFullPath.substr(dir_offset + 1, ext_offset - dir_offset - 1);
}