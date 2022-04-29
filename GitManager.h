#ifndef GIT_MANAGER_INCLUDED
#define GIT_MANAGER_INCLUDED

#include "INIReader.h"
#include "BnvHelper.h"


using namespace std;

class GitManager
{
public:
    
	GitManager(string branch, string dirName, string curDir, bool isGitHook, bool isDebug);
	~GitManager();

    bool isDebug = false;

    bool hasError();
    list<string> getErrors();

    
private:
    /* Путь до эталонного сайта */
    const string PATH_TO_SITE_DEFAULT = "/home/web/bnvtest/project"; //"/home/web/sites/test";
    /* Путь где будет создана новая площадка */
    const string PATH_TO_STAND_DEFAULT = "/home/web/bnvtest";
    /* Поиск названия новой папки для площадки */
    const string PATTERN_FOR_DIR_NAME = "((WT|wt)-[0-9]+)";

    enum ScenarioSet { SCENARIO_NONE, SCENARIO_NEW_STAND, SCENARIO_DELETE_STAND, SCENARIO_UPDATE_STAND };

    /* Ошибки */
    list<string> errors = list<string>();

    string curDir;

    string branch = "";
    string dirName = "";

    string pathToProject = "";
    string pathToStand = "";
    string pattern = "";
    string excludeBranches = "";

    ScenarioSet scenario = ScenarioSet::SCENARIO_NONE;

    string vGit = "";

    bool hasGitInProject = false;
    bool isGitHook = false;
    
    void setVGit();

    void initConfig();

    void validate();
    void serDirName(string dirName);
    void setBranch(string branch);
    
    void setScenario();
    string getScenarioByString();

    void cloneProject();
    void copyProject();
    
    void createDir(string path);
    void deleteDir(string path);


    void updateStand();
    void checkoutStandBranch();

    bool hasBranchProject();
    bool hasBranchStand();
    
    void run();
};
#endif