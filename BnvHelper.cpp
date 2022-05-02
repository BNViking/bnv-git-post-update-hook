#include "BnvHelper.h"

void BnvHelper::consolePrintInfo()
{
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
    "-v             - prit version",
    "--git-hook     - mode of operation for Git hook",
    "--debug        - output of debug information",
    "-h             - help",
    };

    BnvHelper::consolePrint(info,"");

}

void BnvHelper::consolePrint(map<string,string> mapText, string title = string(""))
{
    string preStr = "";

    if (title != "") {
        preStr = "    ";
        cout << title << endl;
    }

    for (auto iter : mapText) {
        cout << preStr << iter.first << " = " << iter.second << endl;
    }
}

void BnvHelper::consolePrint(list<string> text, string title = string(""))
{
    string preStr = "";

    if (title != "") {
        preStr = "    ";
        cout << title << endl;
    }
    
    for (string text : text) {
        cout << preStr << text << endl;
    }
}

void BnvHelper::consolePrint(list<string> text)
{
    BnvHelper::consolePrint(text, "");
}

void BnvHelper::consolePrint(string text)
{
    cout << text << endl;
}

void BnvHelper::createConfigFile(string path)
{
    string pathToProject = "";
    string pathToStand = "";
    string pattern = "";
    string excludeBranches = "";

    cout << "Let up project with git: ";
    cin >> pathToProject;
    cout << endl;

    cout << "Path where new projects will be created: ";
    cin >> pathToStand;
    cout << endl;

    cout << "Pattern for searching for a folder name from a branch: ";
    cin >> pattern;
    cout << endl;

    cout << "Exclude branches [example: master, br1, br2]: ";
    cin >> excludeBranches;
    cout << endl;

    string congigPath = path + "/config.ini";

    FILE* ini;
    if ((ini = fopen(congigPath.c_str(), "w")) == NULL) {
        cout << "Error creating config.ini file" << endl;
    }
    else {
        fprintf(ini,
            "#\n"
            "# %s \n"
            "#\n"
            "\n"
            "[Main]\n"
            "#Path to the project \n"
            "PathToProject = %s \n"
            "\n"
            "#Path where new projects will be created \n"
            "PathToStand = %s \n"
            "\n"
            "#Regular expression to find folder name for new project \n"
            "Pattern = %s \n"
            "\n"
            "#Branches that will not be tracked \n"
            "ExcludeBranches = %s \n"
            "\n"
            "\n"
            "[Messages]\n"
            "#Aliase text: \n"
            "#   @app - replace to app name \n"
            "#   @version - replace to app version \n"
            "#   @dir - replace to folder name for new project \n"
            "#   @branch - branch name \n"
            "#   @pathTo - path to a new project \n"
            "#   @pathFrom - path to the main project \n"
            "\n"
            ";TextAfterCreateStand = \n"
            ";TextAfterUpdateStand = \n"
            ";TextAfterDeleteStand = \n"
            "\n"
            "\n"
            "[CommandsAfterCreate]\n"
            "#Commands to be executed after the script ends\n"
            "#Commands will be taken from the Command-N parameter, where N is the sequence number of the command\n"
            "#Aliase text: \n"
            "#   @app - replace to app name \n"
            "#   @version - replace to app version \n"
            "#   @dir - replace to folder name for new project \n"
            "#   @branch - branch name \n"
            "#   @pathTo - path to a new project \n"
            "#   @pathFrom - path to the main project \n"
            "\n"
            ";Command-1 = \n"
            "\n"
            "\n", 
            BnvHelper::getAppFullName().c_str(), pathToProject.c_str(), pathToStand.c_str(), pattern.c_str(), excludeBranches.c_str());
        fclose(ini);
    }

    cout << congigPath << " created" << endl;
}

void BnvHelper::createDir(string path)
{
    fs::create_directory(path);
}

void BnvHelper::deleteDir(string path)
{
    fs::remove_all(path);
}

void BnvHelper::copyDirTo(string pathIn, string pathTo)
{
    const auto copyOptions = fs::copy_options::skip_existing
        | fs::copy_options::recursive
        ;
    fs::copy(pathIn, pathTo, copyOptions);
}

string BnvHelper::getAppName()
{
    return string("BNV GIT HOOK");
}

string BnvHelper::getAppVersion()
{
    return string("1.1.0");
}

string BnvHelper::getAppFullName()
{
    return string(BnvHelper::getAppName() + " [v"+ BnvHelper::getAppVersion() + "]");
}

string BnvHelper::getCurDir()
{
    return fs::current_path();
}

string BnvHelper::sendTrueFalse(bool value)
{
    string result = "false";
    if (value) {
        result = "true";
    }
    return result;
}

string BnvHelper::sendCommand(string cmd)
{
    return BnvHelper::sendCommand(cmd, false);
}

string BnvHelper::sendCommand(string cmd, bool printCmd = false)
{
    if (printCmd) {
        BnvHelper::consolePrint("exec: " + cmd);
    }
    FILE* fp;

    int status;
    char path[128];
    string result = "";

    fp = popen(cmd.c_str(), "r");
    if (fp == NULL) {
        return "error";
    }

    while (fgets(path, 128, fp) != NULL) {
        result += path;
    }

    status = pclose(fp);
    if (status == -1) {
        return "error";
    }

    if (printCmd) {
        BnvHelper::consolePrint(result + "\n");
    }

    return result;
}

string BnvHelper::sendCommand(list<string> listCmd)
{
    return BnvHelper::sendCommand(listCmd, false);
}

string BnvHelper::sendCommand(list<string> listCmd, bool printCmd = false)
{
    string result = "";

    for (string cmd : listCmd) {
        if (auto s = BnvHelper::sendCommand(cmd, printCmd) != "") {
            result = s + "\n";
        }
    }

    return result;
}

string BnvHelper::strToLower(string str)
{
    for (int i = 0; i < str.length(); i++) {
        if (str[i] >= 'A' && str[i] <= 'Z') {        
            str[i] = ((char)(str[i] - 'A' + 'a'));
        }
    }

    return str;
}

bool BnvHelper::existDir(string path)
{
    return fs::is_directory(fs::status(path.c_str()));
}

string& BnvHelper::ltrim(std::string& str, const std::string& chars = "\t\n\v\f\r ")
{
    str.erase(0, str.find_first_not_of(chars));
    return str;
}

string& BnvHelper::rtrim(std::string& str, const std::string& chars = "\t\n\v\f\r ")
{
    str.erase(str.find_last_not_of(chars) + 1);
    return str;
}

string& BnvHelper::trim(std::string& str, const std::string& chars = "\t\n\v\f\r ")
{
    return BnvHelper::ltrim(BnvHelper::rtrim(str, chars), chars);
}

string& BnvHelper::strReplace(const string& search, const string& replace, string& subject)
{
    size_t pos = 0;
    int n = 0;

    while ((pos = subject.find(search)) != string::npos && n <= 100)
    {
        subject.replace(pos, search.size(), replace);
        n++;
    }

    return subject;
}

list<string> BnvHelper::stringToList(string str, string delimiter)
{
    size_t pos = 0;
    string token;
    list<string> result = {};
    int n = 0;

    BnvHelper::trim(str);
    BnvHelper::trim(str,",");
    str += ",";
    //path.substr(0, path.find_last_of("/"));

    do {
        pos = str.find(delimiter);
        token = str.substr(0, pos);
        str.replace(0, token.length() + 1, "");
        BnvHelper::trim(token);
        if (token != "") {
            result.push_back(token);
        }
        n++;
    } while (str != "" && n < 100);

    return result;
}