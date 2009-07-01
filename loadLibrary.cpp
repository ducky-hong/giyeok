#include "Parser.h"
#include <windows.h>

extern Parser *prsr;

void loadLibrary() {
	HANDLE hFile;
	WIN32_FIND_DATA fileinfo;
	std::stringstream ss;

	hFile=::FindFirstFile(".\\lib\\*.*", &fileinfo);
	if (hFile!=INVALID_HANDLE_VALUE) {
		do {
			if(fileinfo.cFileName[0] != '.')
				if(!(fileinfo.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)) {
					ss.clear();
					ss<<".\\lib\\";
					ss<<fileinfo.cFileName;
					prsr->addLibrary(ss.str());
				}
		} while(::FindNextFile(hFile, &fileinfo));
		::FindClose(hFile);
	}
}
