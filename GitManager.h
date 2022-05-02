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
    const string PATH_TO_SITE_DEFAULT = "/home/web/bnvtest/project";
    /* Путь где будет создана новая площадка */
    const string PATH_TO_STAND_DEFAULT = "/home/web/bnvtest";
    /* Поиск названия новой папки для площадки */
    const string PATTERN_FOR_DIR_NAME = "((WT|wt)-[0-9]+)";

    enum ScenarioSet { SCENARIO_NONE, SCENARIO_NEW_STAND, SCENARIO_DELETE_STAND, SCENARIO_UPDATE_STAND };

    /* Errors */
    list<string> errors = list<string>();

    string curDir;

    /* git branch name */
    string branch = "";

    /* Dir name for new project */
    string dirName = "";

    /* The path to the project with git */
    string pathToProject = "";

    /* Path to the folder where the new project will be created - pathToStand/dirName */
    string pathToStand = "";
    
    /* pattern for finding the name of a new folder from a git branch [, separated]*/
    string pattern = "";

    /* Do not track changes for branches */
    string excludeBranches = "";

    /* work scenarios */
    ScenarioSet scenario = ScenarioSet::SCENARIO_NONE;

    map<string, string> messages;

    list<string> commandsAfterCreate;

    /* git version */
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
    string getMessageByKey(string key);

    string replaseAliaseText(string text);
    list<string> getCommandsAfterCreate();

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