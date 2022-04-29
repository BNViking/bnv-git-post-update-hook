#include <iostream>
#include "BnvHelper.h"
#include "GitManager.h"

using namespace std;

int main(int argc, char* argv[])
{
    setlocale(LC_ALL, "ru-RU.UTF-8");

    list<string> info = {
        "***********************************************************************************************",
        "** BNV GIT HOOK                                                                              **",
        "** Hook for post-update                                                                      **",
        "** Automatically creates a project and switches to a new branch                              **",
        "** If the project was created then updates it if there were changes in the branch            **",
        "** If Pattern and the -d option are not specified, then the folder name = name of the branch **",
        "** Deletes the project when a branch is deleted                                              **",
        "***********************************************************************************************",
        " ",
        "Option:",
        "-b             - name of the branch",
        "-d             - folder name",
        "-i             - create new config.ini",
        "--git-hook     - mode of operation for Git hook",
        "--debug        - output of debug information",
        "-h             - help",
    };

    bool isHelp = false;
    bool isCreateConfig = false;
    bool isGitHook = false;

    string dirName = "";
    string branch = "";

    bool hasError = false;
    
    string curDir = string(argv[0]);

    bool isDebug = false;

    BnvHelper::rtrim(curDir, "/");
    curDir = curDir.substr(0, curDir.find_last_of("/"));

    for (int i = 0; argv[i] != NULL; ++i)
    {

        string param = argv[i];

        if (param == "-i") {
            isCreateConfig = true;
        }
        if (param == "-h") {
            isHelp = true;
        }

        if (param == "-d") {
            try {
                dirName = string(argv[i+1]);
            }catch (const std::exception& ex) {
                hasError = true;
                cout << "The folder name for the new project is not set [-d]";
            }  
        }

        if (param == "-b") {
            try {
                branch = string(argv[i+1]);
            }
            catch (const std::exception& ex) {
                hasError = true;
                cout << "Branch name not set [-b]";
            }
        }

        if (param == "--debug") {
            isDebug = true;
        }

        if (param == "--git-hook") {
            isGitHook = true;
        }

    }
    
    if (hasError) {
        return 0;
    }

    if (isHelp) {
        BnvHelper::consolePrint(info);
        return 0;
    }

    if (isCreateConfig) {
        BnvHelper::createConfigFile();
        return 0;
    }

    GitManager manager = GitManager(branch, dirName, curDir, isGitHook, isDebug);

    if (manager.hasError()) {
        BnvHelper::consolePrint(manager.getErrors(),"Errors:");
        return 0;
    }

	return 0;
}
