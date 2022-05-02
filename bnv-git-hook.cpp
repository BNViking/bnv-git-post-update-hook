#include <iostream>
#include "BnvHelper.h"
#include "GitManager.h"

using namespace std;

int main(int argc, char* argv[])
{
    setlocale(LC_ALL, "ru-RU.UTF-8");

    bool isGitHook = false;

    string dirName = "";
    string branch = "";
  
    string curDir = string(argv[0]);

    bool isDebug = false;

    BnvHelper::rtrim(curDir, "/");
    curDir = curDir.substr(0, curDir.find_last_of("/"));

    for (int i = 0; argv[i] != NULL; ++i)
    {

        string param = argv[i];

        if (param == "-i") {
            BnvHelper::createConfigFile(curDir);
            return 0;
        }

        if (param == "-h") {
            BnvHelper::consolePrintInfo();
            return 0;
        }

        if (param == "-v") {
            cout << BnvHelper::getAppFullName();
            return 0;
        }

        if (param == "-d") {
            try {
                dirName = string(argv[i+1]);
            }catch (const std::exception& ex) {
                cout << "The folder name for the new project is not set [-d]";
                return 0;
            }  
        }

        if (param == "-b") {
            try {
                branch = string(argv[i+1]);
            }
            catch (const std::exception& ex) {
                cout << "Branch name not set [-b]";
                return 0;
            }
        }

        if (param == "--debug") {
            isDebug = true;
        }

        if (param == "--git-hook") {
            isGitHook = true;
        }

    }


    GitManager manager = GitManager(branch, dirName, curDir, isGitHook, isDebug);

    if (manager.hasError()) {
        BnvHelper::consolePrint(manager.getErrors(),"Errors:");
        return 0;
    }

	return 0;
}
