//------------------------------------------------------------------------------------
///
/// @brief  Declaration of module FileHelper
///
/// @file   FileHelper.h
///
///
/// @coypright Ing.büro Hafer
///            Branderweg 8A
///            D-91058 Erlangen
///
/// @author    Detlef Hafer
///
//------------------------------------------------------------------------------------
#pragma once

#include "BASE/Base.h"
#include <string>


class CFileHelper
{
	typedef BOOL(*HandleIdentical) (const std::string&, const std::string&);

	static void MoveFiles(const std::string& rSrcFolder, const std::string& destStdFolder, const std::string& rExt, HandleIdentical pIdentFunc = nullptr);
	static BOOL ChangeFileToNewFolder(const std::string& srcFile, const std::string& szTempFolder, std::string& destFile);
public:
	static BOOL ReadCMDPath(std::string& sz);
	static BOOL IsLocalDrive(const std::string& sz);
	static void CreateFolder(const std::string& rFolder);
	static BOOL MoveFilesToFolder(const std::string& rSrc, const std::string& rDest, const std::string& rExt, HandleIdentical pIdentFunc = nullptr);
	static BOOL IsFileExtension(const std::string& sz, const std::string& rExt);
	static void ClearOldFiles(const std::string& rPath, const std::string& rExt);
	static std::string GetPath(const std::string& rFullPath);
	static std::string GetFileName(const std::string& rFullPath);

};

