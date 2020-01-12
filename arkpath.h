#pragma once
#include <iostream>

class arkpath
{
	public:
		arkpath();
		void savePath(const char* newPath);
		void getPath();
	private:
		std::string txtPath;
};

