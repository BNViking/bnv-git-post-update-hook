#ifndef BNV_HELPER_INCLUDED
#define BNV_HELPER_INCLUDED

#define  _XOPEN_SOURCE

#include <string>
#include <list>
#include <iostream>
#include <fstream>
#include <iterator>
#include <regex>
#include <stdexcept>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <filesystem>
#include <map>


using namespace std;
namespace fs = std::filesystem;

class BnvHelper
{
public:
	static void consolePrintInfo();
	static void consolePrint(list<string> text, string title);
	static void consolePrint(map<string,string> mapText, string title);
	static void consolePrint(list<string> text);
	static void consolePrint(string text);
	static void createConfigFile(string path);
	static void createDir(string path);
	static void deleteDir(string path);
	static void copyDirTo(string pathIn, string pathTo);

	static string  getAppName();
	static string  getAppVersion();
	static string  getAppFullName();

	static string  getCurDir();
	static string  sendTrueFalse(bool value);
	static string  sendCommand(string cmd);
	static string  sendCommand(string cmd, bool printCmd);
	static string  sendCommand(list<string> listCmd);
	static string  sendCommand(list<string> listCmd, bool printCmd);
	static string  strToLower(string str);
	static bool  existDir(string path);

	template <typename T>
	static bool hasElementInList(list<T> eList, T el);

	static string& ltrim(string& str, const string& chars);

	static string& rtrim(string& str, const string& chars);

	static string& trim(string& str, const string& chars);

	static string& strReplace(const string& search, const string& replace, string& subject);

	static list<string> stringToList(string str, string delimiter);

private:

};

template<typename T>
inline bool BnvHelper::hasElementInList(list<T> eList, T el)
{
	for (T value : eList) {
		if (value == el) {
			return true;
		}
	}

	return false;
}

#endif


