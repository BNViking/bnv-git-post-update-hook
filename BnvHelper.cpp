#include "BnvHelper.h"

void BnvHelper::consolePrint(list<string> text, string title = string(""))
{
    string preStr = "";

    if (title != "") {
        preStr = "    ";
        cout << title << endl;
    }
    
    for (string n : text) {
        cout << preStr << n << endl;
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

void BnvHelper::createConfigFile()
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

    FILE* ini;
    if ((ini = fopen("./config.ini", "w")) == NULL) {
        cout << "Error creating config.ini file" << endl;
    }
    else {
        fprintf(ini,
            "################\n"
            "# BNV GIT HOOK #\n"
            "################\n"
            "\n"
            "[Main]\n"
            "\n"
            ";Path to the project \n"
            "PathToProject = %s \n"
            "\n"
            ";Path where new projects will be created \n"
            "PathToStand = %s \n"
            "\n"
            ";Regular expression to find folder name for new project \n"
            "Pattern = %s \n"
            "\n"
            ";Branches that will not be tracked \n"
            "ExcludeBranches = %s \n"
            "\n"
            "\n", pathToProject.c_str(), pathToStand.c_str(), pattern.c_str(), excludeBranches.c_str());
        fclose(ini);
    }

    cout << "config.ini created" << endl;
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