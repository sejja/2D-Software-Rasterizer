// ----------------------------------------------------------------------------
//
//	\file	OpenSaveFile.h 
//	\brief	Header for Utility class OpenSaveFileDlg
//
//	Authors:  Thomas Komair
//	Copyright 2011, Digipen Institute of Technology
//
// ----------------------------------------------------------------------------

#ifndef OPEN_SAVE_FILE_H
#define OPEN_SAVE_FILE_H

// STL containers
#include <vector>
#include <string>

// ----------------------------------------------------------------------------
// Class:	OpenSaveFileDlg
// Desc:	Utility class wrapper around Windows open/save file dialogs.
//			
// ----------------------------------------------------------------------------
class OpenSaveFileDlg
{
public:
	typedef std::vector<std::string> StringArray;

	// ----------------------------------------------------------------------------
	/// \fn		Destructor
	OpenSaveFileDlg();

	// ----------------------------------------------------------------------------
	/// \fn		Destructor
	~OpenSaveFileDlg();

	
	// ----------------------------------------------------------------------------
	/// \fn		Open
	/// \brief	Shows an open file dialog with the specified title
	/// \param	title - The title of the dialog
	/// \param	valid_extensions - A null-terminated string that contains
	///			the valid extensions for filtering the files to open. Each extension
	///			is separated by a ";"e.g.: "png;jpg;tga". If null, no filtering
	///			will be applied.
	bool Open(const char * title, const char * valid_extensions = NULL);

	// ----------------------------------------------------------------------------
	/// \fn		Save
	/// \brief	Shows a save file dialog with the specified title
	/// \param	title - The title of the dialog
	/// \param	valid_extensions - A null-terminated string that contains
	///			the available extensions to save the file to. e.g.: "png;jpg;tga" 
	///			If null, no filtering will be applied
	bool Save(const char * title, const char * valid_extensions = NULL);

	// ----------------------------------------------------------------------------
	/// \fn		GetFiles
	/// \brief	Returns all the files selected by the user since the last call
	///			to open/save.
	/// \ret	A const vector of std::string containing the full paths of all
	///			the files selected by the user.
	const StringArray & GetFiles();

	// ----------------------------------------------------------------------------
	/// \fn		GetNextFilePath
	/// \brief	Retrieves the next file full path.
	/// \param	filepath - Out parameter
	/// \param	advance_to_next - if true, will move to the next file (or end).
	///			if false, then it doesn't advance the cursor
	bool GetNextFilePath(std::string & filepath, bool advance_to_next = true);

	// ----------------------------------------------------------------------------
	/// \fn		GetNextFileName
	/// \brief	Retrieves the next file name (no directory included). 
	/// \param	filename - Out parameter
	/// \param	advance_to_next - if true, will move to the next file (or end).
	///			if false, then it doesn't advance the cursor
	bool GetNextFileName(std::string &filename, bool advance_to_next = true);

	// ----------------------------------------------------------------------------
	/// \fn		GetNextFileDirectory
	/// \brief	Retrieves the directory of the next file. 
	/// \param	directory - Out parameter
	/// \param	advance_to_next - if true, will move to the next file (or end).
	///			if false, then it doesn't advance the cursor
	bool GetNextFileDirectory(std::string &directory, bool advance_to_next = true);

	// ----------------------------------------------------------------------------
	/// \fn		ResetNextFileCursor
	/// \brief	Resets the cursor so that the next call to GetNextFile... functions
	///			starts from the begininng again.
	void ResetNextFileCursor(); 
private:
	StringArray mFiles;
	StringArray::iterator mCursor;	// used when calling GetNextFile
};

#endif