#include <iostream>
#include <fstream>
#include <string>
#include <unistd.h>
#include <wait.h>
#include <set>

#include "Database.h"

using namespace std;
using lazyoj::Database;

#define BUFF_SIZE 2048

static string LAZYOJ_DB_HOST; // 数据库地址
static string LAZYOJ_DB_USER; // 数据库用户名
static string LAZYOJ_DB_PASS; // 数据库密码
static string LAZYOJ_DB_NAME; // 数据库名
static string LAZYOJ_DB_PORT; // 端口

map<string, string> option; // 配置信息
set<pid_t> processPool; // 进程池

static Database* DB; // 数据库实例

void run_cmd_format(const char *fmt, ...) {
  va_list ap;
  bool flag = false;
  char cmd_buffer[BUFF_SIZE];
  va_start(ap, fmt);
  vsprintf(cmd_buffer, fmt, ap);
  flag = system(cmd_buffer);
  va_end(ap);
}

void trimString(string &val) {
    string::size_type last = val.find_last_not_of(" \r\n\t");
    if (last != val.npos)
    {
        val = val.substr(0, last + 1);
    }
    size_t first = val.find_first_not_of(" \r\n\t");
    if (first != val.npos)
    {
        val = val.substr(first, val.size() - first);
    }
}

bool readDatabseConf() {
    char buffer[BUFF_SIZE];
    string currentDir;
    getcwd(buffer, BUFF_SIZE);
    currentDir = buffer;
    string configFilePath = currentDir + "/config/database.conf";
    ifstream file;
    cout << "读取数据库配置信息..." << endl;
    file.open(configFilePath, ios::in);
    if (!file.is_open()) {
        cout << "打开数据库配置文件失败！" << endl;
        return -1;
    }
    while (!file.eof())
    {
        string line;
        getline(file, line);
        string::size_type pos = line.find_first_of("=");
        string key = line.substr(0, pos);
        string val = line.substr(pos + 1, line.size() - (pos + 1));
        trimString(key);
        trimString(val);
        if (key == "hostname") LAZYOJ_DB_HOST = val;
        if (key == "username") LAZYOJ_DB_USER = val;
        if (key == "password") LAZYOJ_DB_PASS = val;
        if (key == "database") LAZYOJ_DB_NAME = val;
        if (key == "hostport") LAZYOJ_DB_PORT = val;
    }
    file.close();
}

void initialize() {
    readDatabseConf();
    DB = new Database(LAZYOJ_DB_HOST, LAZYOJ_DB_USER, LAZYOJ_DB_PASS, LAZYOJ_DB_NAME, LAZYOJ_DB_PORT);
    if (!DB->initDatabase())
    {
        cout << DB->getErrorInfo() << endl;
        exit(0);
    }
    option = DB->getOptions();
}

void working() {
    int count = 1;
    map<string, string> joblist = DB->getJobList(option["LAZYOJ_RUN_MAX"]);
    for (map<string, string>::iterator it = joblist.begin(); it != joblist.end(); it++)
    {
        int sid = atoi(it->first.c_str());
        pid_t pidRunner = fork();
        if (pidRunner == 0) {
            run_cmd_format("./JudgeClient %s %d", it->first.c_str(), count);
        } else {
            cout << "任务号: " << count << endl;
            cout << "Pid: " << pidRunner << endl;
            cout << "Sid: " << it->first << endl;
            processPool.insert(pidRunner);
        }
        count++;
    }

    for (set<pid_t>::iterator it = processPool.begin(); it != processPool.end(); it++)
    {
        int status = 0;
        cout << "等待任务: " << *it << endl;
        waitpid(*it, &status, 0);
    }
    
}

void daemon_init() {
    string pid = "32767";
    char cmd[BUFF_SIZE];
    fstream pidFile;
    bool runFlag = false;
    FILE *fp = NULL;
    pidFile.open("./config/Judge.pid", ios::in);
    if (pidFile.is_open()) {
        pidFile >> pid;
        if (!pid.size()) {
            pid = "32767";
        }
    }
    pidFile.close();
    sprintf(cmd, "ps -p %s", pid.c_str());
    if ((fp = popen(cmd, "r")) != NULL) {
        char buff[BUFF_SIZE];
        while((fgets(buff, BUFF_SIZE, fp)) != NULL) {
            string line = buff;
            if (line.find(pid) != line.npos) {
                runFlag = true;
            }
        }
    }
    pclose(fp);
    fp = NULL;
    if (runFlag) {
        cout << "JudgeServer is already running..." << endl;
        cout << "Pid: " << pid << endl;
        exit(0);
    } else {
        daemon(true, true);
        pid_t myPid = getpid();
        cout << "JudgeServer start success..." << endl;
        cout << "Pid: " << myPid << endl;
        pidFile.open("./config/Judge.pid", ios::out | ios::ate);
        pidFile << myPid ;
        pidFile.close();
    }
    // 
    pidFile.close();
    
}

int main() {
    daemon_init();

    freopen("./log/log.log", "w+", stdout);
    freopen("./log/log.log", "w+", stderr);
    initialize();
    
    while (true) {
        working();
        sleep(atoi(option["LAZYOJ_SLEEP_TIME"].c_str()));
    }

    return 0;
}