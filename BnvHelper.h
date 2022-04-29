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


using namespace std;
namespace fs = std::filesystem;

class BnvHelper
{
public:
	static void consolePrint(list<string> text, string title);
	static void consolePrint(list<string> text);
	static void consolePrint(string text);
	static void createConfigFile();
	static void createDir(string path);
	static void deleteDir(string path);
	static void copyDirTo(string pathIn, string pathTo);

	static string  getCurDir();
	static string  sendTrueFalse(bool value);
	static string  sendCommand(string cmd);
	static string  strToLower(string str);
	static bool  existDir(string path);

	template <typename T>
	static bool hasElementInList(list<T> eList, T el);

	static string& ltrim(std::string& str, const std::string& chars);

	static string& rtrim(std::string& str, const std::string& chars);

	static string& trim(std::string& str, const std::string& chars);

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


