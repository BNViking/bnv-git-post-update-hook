#include "GitManager.h"

using namespace std;

void GitManager::setVGit()
{
    string result = "";
    string comRes = BnvHelper::sendCommand("git --version");

    if (comRes.find("git version") != string::npos) {
        result = comRes.substr(comRes.find_last_of("git version") + 1);
    }
    this->vGit = result;
}

void GitManager::initConfig()
{
    string configPath = this->curDir + "/config.ini";
    INIReader config(configPath);
    if (config.ParseError() < 0) {
        this->errors.push_front(configPath + " not found [run the program with the -i option to create a new config.ini]");
    }
    else {
        this->pathToProject = config.Get("Main", "PathToProject", "");
        this->pathToStand = config.Get("Main", "PathToStand", "");
        this->pattern = config.Get("Main", "Pattern", "");
        this->excludeBranches = config.Get("Main", "ExcludeBranches", "");
    }
}

GitManager::GitManager(string branch,string dirName, string curDir, bool isGitHook, bool isDebug)
{
    
    this->isDebug = isDebug;
    this->curDir = curDir;
    this->isGitHook = isGitHook;

    if (this->isGitHook) {
        BnvHelper::consolePrint("BNV-GIT-HOOK");
    }

    initConfig();
    setVGit();
    setBranch(branch);
    serDirName(dirName);

    string ss = BnvHelper::sendCommand("cd " + this->pathToProject + " && env -i git branch -l");
    this->hasGitInProject = !(ss.find("fatal") != string::npos);

    auto exBrList = BnvHelper::stringToList(this->excludeBranches, ",");
    if (!BnvHelper::hasElementInList(exBrList,this->branch) || !this->isGitHook) {
        validate();
        setScenario();

        if (!hasError()) {
            run();
        }
    } else {
        BnvHelper::consolePrint("Branch " + this->branch + " is not tracking [ExcludeBranches: "+ this->excludeBranches +"]");
    }

    if (isDebug) {
        list<string> debugList = {};
        
        debugList.push_back("CurDir = " + this->curDir);
        debugList.push_back("PathToProject = " + this->pathToProject);
        debugList.push_back("PathToStand = " + this->pathToStand);
        debugList.push_back("Pattern = " + this->pattern);
        debugList.push_back("Branch = " + this->branch);
        debugList.push_back("DirName = " + this->dirName);
        debugList.push_back("HasGitInProject = " + BnvHelper::sendTrueFalse(this->hasGitInProject));
        debugList.push_back("Scenario = " + this->getScenarioByString());
        debugList.push_back("GitHookMode = " + BnvHelper::sendTrueFalse(this->isGitHook));
        debugList.push_back("vGit = " + this->vGit);

        BnvHelper::consolePrint(debugList, "Debug:");
        BnvHelper::consolePrint(exBrList, "ExcludeBranches:");

    }
}

GitManager::~GitManager()
{
}

bool GitManager::hasError()
{
    return this->errors.size()>0;
}

list<string> GitManager::getErrors()
{
    return this->errors;
}

void GitManager::validate()
{
    if (this->pathToProject == "") {
        this->errors.push_front("Project directory not set in config.ini [PathToProject]");
    }

    if (this->pathToStand == "") {
        this->errors.push_front("Path not set for new project in config.ini [PathToStand]");
    }

    if (this->branch == "") {
        this->errors.push_front("Branch name not set [-b]");
    }

    if (this->dirName == "") {
        this->errors.push_front("The folder for the new project is not set [-d]");
    }

    if (this->vGit == "") {
        this->errors.push_front("Git not found on the system [git --version]");
    }

    if (!BnvHelper::existDir(this->pathToProject))
    {
        this->errors.push_front(this->pathToProject + " - folder not found");
    }

    if (!BnvHelper::existDir(this->pathToStand))
    {
        this->errors.push_front(this->pathToStand + " - folder not found");
    }

    if (!this->hasGitInProject) {
        this->errors.push_front(this->pathToProject + " - not a git repository");
    }
}

void GitManager::serDirName(string dirName)
{
    string result = dirName;
    if (dirName == "") {
        if (this->pattern != "") {
            regex regexp(this->pattern);

            smatch sm;
            regex_search(this->branch, sm, regexp);

            if (sm.size() > 0) {
                result = string(sm[0]);
            }
        }
        else {
            result = this->branch;
        }

    }

    this->dirName = BnvHelper::strToLower(result);
}

void GitManager::setBranch(string branch)
{
    string result = branch;
    if (branch != "") {
        result = branch.substr(branch.find_last_of("/") + 1);
    }

    this->branch = result;
}

void GitManager::setScenario()
{
    if (this->hasBranchProject() && !this->hasBranchStand() && this->dirName != "") {
        this->scenario = ScenarioSet::SCENARIO_NEW_STAND;
    }

    if (this->hasBranchProject() && this->hasBranchStand()) {
        this->scenario = ScenarioSet::SCENARIO_UPDATE_STAND;
    }

    if (!this->hasBranchProject() && (this->hasBranchStand() || (BnvHelper::existDir(this->pathToStand+"/"+this->dirName) && this->dirName != ""))) {
        this->scenario = ScenarioSet::SCENARIO_DELETE_STAND;
    }
}

string GitManager::getScenarioByString()
{
    switch (this->scenario) {
    case ScenarioSet::SCENARIO_NONE:
        return string("None");
    case ScenarioSet::SCENARIO_NEW_STAND:
        return string("Create new project");
    case ScenarioSet::SCENARIO_UPDATE_STAND:
        return string("Update project");
    case ScenarioSet::SCENARIO_DELETE_STAND:
        return string("Delete project");
    }
    return "";
}

void GitManager::cloneProject()
{
    BnvHelper::consolePrint("Clone project" + this->pathToProject);
    BnvHelper::sendCommand("git clone " + this->pathToProject + " " + this->pathToStand + "/" + this->dirName);
}

void GitManager::copyProject()
{
    BnvHelper::consolePrint("Coppy settings project " + this->pathToProject + " to " + this->pathToStand + "/" + this->dirName);
    BnvHelper::copyDirTo(this->pathToProject, this->pathToStand + "/" + this->dirName);
}

void GitManager::updateStand()
{
    BnvHelper::consolePrint("Update project " + this->pathToStand + "/" + this->dirName);
    BnvHelper::sendCommand("cd " + this->pathToStand + "/" + this->dirName + " && env -i git reset --hard && env -i git pull origin " + this->branch);
}

void GitManager::createDir(string path)
{
    BnvHelper::consolePrint("Create dir: " + path);
    BnvHelper::createDir(path);
}

void GitManager::deleteDir(string path)
{
    BnvHelper::consolePrint("Delete dir: " + path);
    BnvHelper::deleteDir(path);
}

void GitManager::checkoutStandBranch()
{
    BnvHelper::consolePrint("Checkout to " + this->branch + " in " + this->pathToStand + "/" + this->dirName);
    BnvHelper::sendCommand("cd " + this->pathToStand + "/" + this->dirName + " && env -i git reset --hard && env -i git checkout " + this->branch);
}

bool GitManager::hasBranchProject()
{
    string ss = BnvHelper::sendCommand("cd " + this->pathToProject + " && env -i git branch -l");
    bool result = (ss.find(this->branch) != string::npos) && this->branch != "";
    if (this->isDebug) {
        BnvHelper::consolePrint("Has branch " + this->branch + " in project = " + BnvHelper::sendTrueFalse(result));
    }
    return result;
}

bool GitManager::hasBranchStand()
{
    bool result = false;
    if (this->dirName != "" && BnvHelper::existDir(this->pathToStand + "/" + this->dirName)) {
        string ss = BnvHelper::sendCommand("cd " + this->pathToStand + "/" + this->dirName + " && env -i git branch -l");
        result = (ss.find(this->branch) != string::npos) && this->branch != "";
        if (this->isDebug) {
            BnvHelper::consolePrint("Has branch " + this->branch + " in stand = " + BnvHelper::sendTrueFalse(result));
        }
    }

    return result;
}

void GitManager::run()
{
    switch (this->scenario) {
        case ScenarioSet::SCENARIO_NEW_STAND:
            if (BnvHelper::existDir(this->pathToStand + "/" + this->dirName)) {
                this->deleteDir(this->pathToStand + "/" + this->dirName);
            }
            this->createDir(this->pathToStand + "/" + this->dirName);
            this->cloneProject();
            this->copyProject();
            this->checkoutStandBranch();
            break;
        case ScenarioSet::SCENARIO_UPDATE_STAND:
            this->updateStand();
            break;
        case ScenarioSet::SCENARIO_DELETE_STAND:
            this->deleteDir(this->pathToStand + "/" + this->dirName);
            break;
        default: {
            this->errors.push_front("Scenario not defined");
        }
    }
}
