#pragma once
#include <iostream>

class inipath
{
	public:
		inipath();
		void savePath(const char* newPath);
		const char* getPath();
		bool validatePath();
		std::string getProgramPath()
		{
			return programPath;
		}
	private:
		std::string progTxtPath;
		std::string fullPath;
		std::string programPath;
};

