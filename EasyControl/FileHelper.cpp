//------------------------------------------------------------------------------------
///
/// @brief  Implementation of module FileHelper
///
/// @file   FileHelper.cpp
///
///
/// @coypright Ing.büro Hafer
///            Branderweg 8A
///            D-91058 Erlangen
///
/// @author    Detlef Hafer
///
//------------------------------------------------------------------------------------
#include "stdafx.h"
#include <direct.h>
#include "FileHelper.h"
#include "StringConvert.h"

//------------------------------------------------------------------------------------
///  @brief   ReadSystemPath
///
///            This method ....
///
///  @author  Detlef Hafer
///
///
///  @param[in] pszHelpFilePathof type
///  @param[in] iLicence       of type
///  @return BOOL
///
//------------------------------------------------------------------------------------
BOOL CFileHelper::ReadCMDPath ( std::string& stdsz)
{
   char path_buffer[_MAX_PATH];
   char drive[_MAX_DRIVE];
   char dir[_MAX_DIR];
   char fname[_MAX_FNAME];
   char ext[_MAX_EXT];

   ASSERT(!stdsz.empty());
   _splitpath_s( stdsz.c_str(), drive, dir, fname, ext );
   _makepath_s( path_buffer, drive, dir, "", "");
    auto sLen = _S32(strlen( path_buffer))-1;
    
    while ((sLen > 0) && (path_buffer[sLen] == '\\') )
    {
        path_buffer[sLen] = 0;
        sLen--;
    }
	stdsz = std::string(path_buffer);
    return !stdsz.empty();
}
//*******************************************************************************************************************
//*******************************************************************************************************************
BOOL CFileHelper::IsLocalDrive(const std::string& sz)
{
	BOOL result = FALSE;
	ASSERT(!sz.empty());
	if (! sz.empty())
	{
		char drive[_MAX_DRIVE];
		char dir[_MAX_DIR];
		char fname[_MAX_FNAME];
		char ext[_MAX_EXT];

		_splitpath_s(sz.c_str(), drive, dir, fname, ext);
		char_t cdrive = _C(std::toupper(drive[0]));
		result = (cdrive == 'C');
	}
	return result;
}
//*******************************************************************************************************************
//*******************************************************************************************************************
std::string CFileHelper::GetPath(const std::string& rFullPath)
{
	char path_buffer[_MAX_PATH];
	char drive[_MAX_DRIVE];
	char dir[_MAX_DIR];
	char fname[_MAX_FNAME];
	char ext[_MAX_EXT];

	ASSERT(!rFullPath.empty());
	_splitpath_s(rFullPath.c_str(), drive, dir, fname, ext);
	_makepath_s(path_buffer, drive, dir, "", "");
	return std::string(path_buffer);
}
//*******************************************************************************************************************
//*******************************************************************************************************************
std::string CFileHelper::GetFileName(const std::string& rFullPath)
{
	char drive[_MAX_DRIVE];
	char dir[_MAX_DIR];
	char fname[_MAX_FNAME];
	char ext[_MAX_EXT];

	ASSERT(!rFullPath.empty());
	_splitpath_s(rFullPath.c_str(), drive, dir, fname, ext);
	return std::string(fname) + std::string(ext);
}
//*******************************************************************************************************************
//*******************************************************************************************************************
BOOL CFileHelper::ChangeFileToNewFolder(const std::string& srcFile, const std::string& szNewFolder, std::string& destFile)
{
	BOOL result = FALSE;
	ASSERT(!srcFile.empty());
	if (!srcFile.empty())
	{
		char dest_buffer[_MAX_PATH];
		char srcDrive[_MAX_DRIVE];
		char srcDir[_MAX_DIR];
		char srcFname[_MAX_FNAME];
		char srcExt[_MAX_EXT];

		_splitpath_s(srcFile.c_str(), srcDrive, srcDir, srcFname, srcExt);
		sprintf_s(dest_buffer, "%s\\%s%s", szNewFolder.c_str(), srcFname, srcExt);
		destFile = std::string(dest_buffer);
		result = ! destFile.empty();
	}
	return result;
}
//*******************************************************************************************************************
//*******************************************************************************************************************
void CFileHelper::CreateFolder(const std::string& rFolder)
{
	CString szPath{ toCString(rFolder) };
	if ( ! PathFileExists(szPath))
	{
		SHCreateDirectoryEx(NULL, szPath, NULL);
	}
}
//*******************************************************************************************************************
//*******************************************************************************************************************
BOOL CFileHelper::MoveFilesToFolder(const std::string& rSrc, const std::string& rDest, const std::string& rExt, HandleIdentical pIdentFunc)
{
	// Networkdrive exists
	BOOL result = FALSE;

	ASSERT(rExt[0] == '.');
	struct stat buffer;
	auto fileexists = (stat(rSrc.c_str(), &buffer) == 0);
	if (fileexists)
	{
		fileexists = (stat(rDest.c_str(), &buffer) == 0);
		if (fileexists)
		{
			MoveFiles(rSrc, rDest, rExt, pIdentFunc);
			result = TRUE;
		}
	}
	return result;
}
//*******************************************************************************************************************
//*******************************************************************************************************************
BOOL CFileHelper::IsFileExtension(const std::string& sz, const std::string& rExt )
{
	BOOL result = FALSE;
	if (!sz.empty())
	{
		char drive[_MAX_DRIVE];
		char dir[_MAX_DIR];
		char fname[_MAX_FNAME];
		char ext[_MAX_EXT];

		_splitpath_s(sz.c_str(), drive, dir, fname, ext);
		CString szExt1 = toCString(ext);
		szExt1.MakeUpper();
		CString szExt2 = toCString(rExt);
		szExt2.MakeUpper();
		result = szExt1 == szExt2;
	}
	return result;
}

//***********************************************************************************************
//***********************************************************************************************
void CFileHelper::MoveFiles(const std::string& rSrcFolder, const std::string& destStdFolder, const std::string& rExt, HandleIdentical pIdentFunc)
{
	ASSERT(!rExt.empty());
	ASSERT(rExt[0] == '.');
	ASSERT(!rSrcFolder.empty());
	ASSERT(!destStdFolder.empty());

	auto CSrcFolder{ toCString(rSrcFolder) };
	auto CExt{ toCString(rExt) };
	CString szPath;
	szPath.Format(_T("%s\\*%s"), LPCTSTR(CSrcFolder), LPCTSTR(CExt));

	CFileFind finder;
	BOOL bWorking = finder.FindFile(szPath);
	while (bWorking)
	{
		bWorking = finder.FindNextFile();

		auto srcCFile{ finder.GetFilePath() };
		auto srcStdFile{ toStdString(srcCFile) };
		std::string destStdFile;
		auto result = ChangeFileToNewFolder(srcStdFile, destStdFolder, destStdFile);
		if (result)
		{
			auto destCFile{ toCString(destStdFile) };
			result = CopyFile(srcCFile, destCFile, TRUE);
			if (! result)
			{
				if (pIdentFunc)
				{
					pIdentFunc(srcStdFile, destStdFile);
				}
			}
			remove(srcStdFile.c_str());
		}
	}
}
//***********************************************************************************************
//***********************************************************************************************
void CFileHelper::ClearOldFiles(const std::string& rSrcFolder, const std::string& rExt)
{
	CTime now{ CTime::GetCurrentTime() };

	CString CSrcFolder{ toCString(rSrcFolder) };
	CString CExt{ toCString(rExt) };

	CString szPath;
	szPath.Format(_T("%s\\*%s"), LPCTSTR(CSrcFolder), LPCTSTR(CExt));

	CFileFind finder;
	BOOL bWorking = finder.FindFile(szPath);
	while (bWorking)
	{
		bWorking = finder.FindNextFile();
		CFileStatus status;
		if (CFile::GetStatus(finder.GetFilePath(), status))
		{
			CTimeSpan difftime = now - status.m_mtime;
			auto days = difftime.GetDays();
			if (days > 365)
			{
				auto szRemove{ toStdString(finder.GetFilePath()) };
				remove(szRemove.c_str());
			}
		}
	}
}

