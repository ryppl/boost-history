// Boost.Print library

// Copyright Jared McIntyre 2007. Use, modification and
// distribution is subject to the Boost Software License, Version
// 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

// For more information, see http://www.boost.org

#include <windows.h>

#include <string>
#include <boost/algorithm/string.hpp>
#include <boost/algorithm/string/find.hpp>
#include <boost/filesystem/operations.hpp>
#include <boost/filesystem/fstream.hpp>

using namespace std;
using namespace boost;

string getPathForModule(const char* szModuleName)
{
	TCHAR buf[MAX_PATH] = "";
	HMODULE hModule = ::GetModuleHandle(szModuleName);
	::GetModuleFileName(hModule, buf, sizeof(buf));
	string strExePath(buf);
	return strExePath;
}

string getParentDirectory(int nRelativePathLevel)
{
	string strExePath = getPathForModule(NULL); 

	filesystem::path full_path = filesystem::system_complete( filesystem::path(strExePath.c_str(), filesystem::native ));
	for(int nLevel = 0; nLevel < nRelativePathLevel; nLevel++)
	{
		full_path = full_path.branch_path();
	}
	string strFullPathLocal = full_path.string();
	string strParent = strFullPathLocal + "\\";
	return strParent;
}

string getTestDataDirectory()
{
	string strBranchPath = getParentDirectory(3); 
	string strTestDataDirectory = strBranchPath + "UnitTests\\TestProperties\\Test Data Files\\";
	return strTestDataDirectory;
}

class MscCurrentDirectoryHelper
{
public:
	MscCurrentDirectoryHelper(const char* newDir)
	{
		GetCurrentDirectory(MAX_PATH, m_oldCurrentPath);
		SetCurrentDirectory(newDir);
	}
	~MscCurrentDirectoryHelper()
	{
		SetCurrentDirectory(m_oldCurrentPath);
	}
private:
	char m_oldCurrentPath[MAX_PATH];
};
