#include <iostream>
#include <string>
#include <map>
#include <cstdlib>
#include <cstring>
#include <unistd.h>
#include <dirent.h>
#include <sys/resource.h>
#include <sys/wait.h>
#include <sys/ptrace.h>

#include "Database.h"
#include "Compare.h"

#define BUFF_SIZE 2048
#define STD_MB 1048576LL // 标准1MB数据长度

using namespace std;
using namespace lazyoj;

static string LAZYOJ_DB_HOST; // 数据库地址
static string LAZYOJ_DB_USER; // 数据库用户名
static string LAZYOJ_DB_PASS; // 数据库密码
static string LAZYOJ_DB_NAME; // 数据库名
static string LAZYOJ_DB_PORT; // 端口

static string LAZYOJ_RUN_RESULT; // 运行结果
static string LAZYOJ_RUN_SID; // Solution ID
static string LAZYOJ_RUN_NID; // Node ID
static double LAZYOJ_RUN_AC; // 运行通过数目 
static string LAZYOJ_RUN_ALL_RESULT = "AC"; // 总体运行结果

long long LAZYOJ_RUN_TIME; // 运行总时间
long long LAZYOJ_RUN_MEMORY; // 运行最大内存

static string LAZYOJ_DIR_WORK; // 工作目录
static string LAZYOJ_DIR_HOME; // 运行目录
static string LAZYOJ_DIR_DATA; // 数据所在目录

static Database* DB; // 数据库实例

map<string, string> settings; // 配置信息
map<string, string> solution; // 题解信息
map<string, string> problem; // 问题信息
map<string, string> language; // 语言信息
map<string, string> contest; // 竞赛信息
map<string, int> testFileList; // 测试文件列表

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

void run_cmd_format(const char *fmt, ...) {
  va_list ap;
  bool flag = false;
  char cmd_buffer[BUFF_SIZE];
  va_start(ap, fmt);
  vsprintf(cmd_buffer, fmt, ap);
  flag = system(cmd_buffer);
  va_end(ap);
}

void run_cmd_argv(string cmd) {
  trimString(cmd);
  cmd.push_back(' ');
  char* vlist[30];
  memset((void*)vlist, 0, sizeof(vlist));
  string::size_type start = 0;
  string::size_type index = cmd.find_first_of(' ', start);
  int t = 0;
  while (index != cmd.npos)
  {
    string param = cmd.substr(start, index-start);
    char *temp = new char[param.size()+1];
    strcpy(temp, param.c_str());
    vlist[t++] = temp;
    start = index + 1;
    index = cmd.find_first_of(' ', start);
  }
  execvp(vlist[0], (char *const *)vlist);
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

void makeWorkDir() {
    char buff[BUFF_SIZE];
    getcwd(buff, BUFF_SIZE);
    LAZYOJ_DIR_HOME = buff;
    LAZYOJ_DIR_WORK = LAZYOJ_DIR_HOME + "/run/node_" + LAZYOJ_RUN_NID;
    LAZYOJ_DIR_DATA = LAZYOJ_DIR_HOME + "/data/" + problem["pid"];
    run_cmd_format("mkdir -p %s", LAZYOJ_DIR_WORK.c_str());
    run_cmd_format("rm -rf %s/*", LAZYOJ_DIR_WORK.c_str());
    // 改变工作目录
    chdir(LAZYOJ_DIR_WORK.c_str());
    cout << "当前工作目录: " <<  LAZYOJ_DIR_WORK << endl;
    fstream file;
    file.open(LAZYOJ_DIR_WORK + "/Main." + language["ext"], ios::ate | ios::out);
    cout << "写入用户代码文件: " << LAZYOJ_DIR_WORK << "/Main." + language["ext"] << endl;
    file << solution["code"];
    file.close();
}

void clearWorkDir() {
    run_cmd_format("rm -rf %s/*", LAZYOJ_DIR_WORK);
}

bool compare_solution(string stdFileName, string userFileName) {
  lazyoj::Compare cmp(stdFileName, userFileName);
  if (contest["format"] == "1") {
      cmp.setStrict(true);
  }
  bool flag = cmp.compare();
  LAZYOJ_RUN_RESULT = cmp.getCompareInfo();
  if (LAZYOJ_RUN_RESULT != "AC") {
    int code = cmp.getCompareCode();
    switch (code)
    {
    case 0:
        LAZYOJ_RUN_ALL_RESULT = "WA";
        break;
    case 1:
        LAZYOJ_RUN_ALL_RESULT = "PE";
        break;
    default:
        break;
    }
    fstream file;
    file.open("rutimeInfo.txt", ios::ate | ios::app);
    file << LAZYOJ_RUN_RESULT ;
    file.close();
  }
  return flag;
}

bool compile_solution() {
    pid_t pidCompiler = fork();
    if (pidCompiler < 0) {
        cout << "启动编译进程失败..." << endl;
    }
    else if (pidCompiler == 0) {
        struct rlimit LIMIT;
        // 限制CPU 使用时间 最大5秒
        LIMIT.rlim_cur = 5;
        LIMIT.rlim_max = 5;
        setrlimit(RLIMIT_CPU, &LIMIT);
        alarm(5);

        // 限制可以创建的文件的最大字节 最大长度 50MB
        LIMIT.rlim_cur = 50 * STD_MB;
        LIMIT.rlim_max = 50 * STD_MB;
        setrlimit(RLIMIT_FSIZE, &LIMIT);

        // 限制子程序内存 最大512M
        LIMIT.rlim_cur = 512 * STD_MB;
        LIMIT.rlim_max = 512 * STD_MB;
        setrlimit(RLIMIT_AS, &LIMIT);

        // 重定向输入输出到文件 
        freopen("compile.txt", "w+", stdout);
        freopen("compile.txt", "w+", stderr);
        // 执行编译
        run_cmd_argv(language["cmd"]);
        // 子进程退出
        exit(0);
    }
    else {
        int status = 0;
        waitpid(pidCompiler, &status, 0);
        cout << "编译执行结束..." << endl;
        cout << "编译状态: " << status << endl;
        return status == 0;
    }
    return 0;
}

void get_test_file() {
    // 为测试点复制文件
    cout << "复制文件测试点文件... " << endl;
    DIR *dir;
    struct dirent *ptr;
    dir = opendir(LAZYOJ_DIR_DATA.c_str());
    cout << LAZYOJ_DIR_DATA << endl;
    while ((ptr = readdir(dir)) != NULL) {
        string fullFileName = ptr->d_name;
        if (fullFileName.find(".in") != fullFileName.npos || fullFileName.find(".out") != fullFileName.npos)
        {
            string fromFilePath = LAZYOJ_DIR_DATA + "/" + fullFileName;
            string toFilePath = LAZYOJ_DIR_WORK + "/" + fullFileName;
            string filename = fullFileName.substr(0, fullFileName.find_last_of("."));
            run_cmd_format("cp %s %s ", fromFilePath.c_str(), toFilePath.c_str());
            if (testFileList.count(filename) == 0) {
                testFileList[filename] = 1;
            } else {
                testFileList[filename] = 2;
            }
        }
    }
    closedir(dir);
}

long long get_proc_status(pid_t pidApp, string key) {
    stringstream sstream;
    sstream << "/proc/" << pidApp << "/status";
    string filename = sstream.str();
    sstream.str("");
    long long ret = 0;
    fstream file;
    file.open(filename, ios::in | ios::binary);
    if (!file.is_open())
        return 0;
    while (!file.eof())
    {
        string line;
        getline(file, line);
        if (line.find(key) != line.npos)
        {
            sscanf(line.c_str() + line.find_first_of(key) + key.size() + 1, "%lld", &ret);
        }
    }
    file.close();
    return ret;
}

size_t get_file_size(string filename) {
    fstream file;
    file.open(filename, ios::in);
    if (!file.is_open())
        return 0;
    file.seekg(0, file.end);
    size_t fileSize = file.tellg();
    return fileSize;
}

void run_solution(string filename) {
    // 资源限制结构体
    struct rlimit LIMIT;

    // 设置进程优先级 19 最高为19
    nice(19);
    chdir(LAZYOJ_DIR_WORK.c_str());

    // 设置跟随
    ptrace(PTRACE_TRACEME, 0, NULL, NULL);
    
    // 限制CPU 使用时间 最多为限制时间的2倍 单位S
    LIMIT.rlim_cur = atoi(problem["time"].c_str()) * 3;
    LIMIT.rlim_max = atoi(problem["time"].c_str()) * 3;
    setrlimit(RLIMIT_CPU, &LIMIT);
    alarm(atoi(problem["time"].c_str()) * 2);
    cout << "时间限制: " << LIMIT.rlim_cur << endl;

    // 限制可以创建的文件的最大字节 最大长度 50MB 输入输出限制大小 100MB
    LIMIT.rlim_cur = 100 * STD_MB;
    LIMIT.rlim_max = 100 * STD_MB;
    setrlimit(RLIMIT_FSIZE, &LIMIT);
    cout << "文件限制: " << LIMIT.rlim_cur << endl;

    // 限制子程序内存 最大为限制内存的2倍 单位MB
    LIMIT.rlim_cur = atoi(problem["memory"].c_str()) * 2 * STD_MB ;
    LIMIT.rlim_max = atoi(problem["memory"].c_str()) * 2 * STD_MB;
    setrlimit(RLIMIT_AS, &LIMIT);
    cout << "内存限制: " << LIMIT.rlim_cur << endl;

    // 限制子程序进程 最大进程数 1
    LIMIT.rlim_cur = 1;
    LIMIT.rlim_max = 1;
    setrlimit(RLIMIT_NPROC, &LIMIT);
    cout << "进程限制: " << LIMIT.rlim_cur << endl;

    // 限制堆栈
    LIMIT.rlim_cur = STD_MB << 7;
    LIMIT.rlim_max = STD_MB << 7;
    setrlimit(RLIMIT_STACK, &LIMIT);
    cout << "堆栈限制: " << LIMIT.rlim_cur << endl;

    // 重定向输入输出到文件
    string inFile = filename + ".in";
    string outFile = filename + ".user.out";
    string errFile = filename + ".user.err";
    freopen(inFile.c_str(), "r+", stdin);
    freopen(outFile.c_str(), "w+", stdout);
    freopen(errFile.c_str(), "w+", stderr);

    // 启动进程
    run_cmd_argv("./Main");

    // 清空错误缓冲区
    fflush(stderr);

    // 退出进程
    exit(0);
}

void listen_solution(pid_t pidRunner, string filename, long long & time_used, long long & memory_used) {
    int status = 0;
    struct rusage rused;
    bool firstFlag = true;
    LAZYOJ_RUN_RESULT = "AC";
    int count = 0;
    while(true) {
        pid_t pidRet = wait4(pidRunner, &status, __WALL, &rused);
        if (firstFlag) {
            ptrace(PTRACE_SETOPTIONS, pidRunner, NULL, PTRACE_O_TRACESYSGOOD | PTRACE_O_TRACEEXIT | PTRACE_O_EXITKILL | PTRACE_O_TRACECLONE | PTRACE_O_TRACEFORK | PTRACE_O_TRACEVFORK);
            firstFlag = false;
        }
        // 如果子进程退出
        if (pidRet == pidRunner) {
            if (WIFEXITED(status)) {
                int sig = WEXITSTATUS(status);
                // 正常退出
                if (sig == 0) {
                    break;
                }
                // 如果是AC 进入 否则保持原来的状态
                if (sig != SIGTRAP && sig != 0x00 && LAZYOJ_RUN_RESULT == "AC") {
                    switch (sig)
                    {
                    case SIGALRM: // 超时警告 结束
                        alarm(0); // 取消闹钟
                    case SIGXCPU:
                    case SIGCHLD: // 子进程已经停止或退出
                    case SIGKILL: // KILL 信号结束
                        LAZYOJ_RUN_RESULT = "TLE";
                        break;
                    case SIGXFSZ: // 单个文件大小超出限制
                        LAZYOJ_RUN_RESULT = "OLE";
                        break;
                    default:
                        LAZYOJ_RUN_RESULT = "RE";
                        break;
                    }
                    // 发送结束信号
                    ptrace(PTRACE_KILL, pidRunner, NULL, NULL);
                    break;
                }
                
            }
            // 如果子进程因为信号而结束
            if (WIFSIGNALED(status)) {
                int sig = WTERMSIG(status);
                if (sig != SIGTRAP && sig != 0x00 && LAZYOJ_RUN_RESULT == "AC") {
                    switch (sig)
                    {
                    case SIGALRM: // 超时警告 结束
                        alarm(0); // 取消闹钟
                    case SIGXCPU:
                    case SIGCHLD: // 子进程已经停止或退出
                    case SIGKILL: // KILL 信号结束
                        LAZYOJ_RUN_RESULT = "TLE";
                        break;
                    case SIGXFSZ: // 单个文件大小超出限制
                        LAZYOJ_RUN_RESULT = "OLE";
                        break;
                    default:
                        LAZYOJ_RUN_RESULT = "RE";
                        break;
                    }
                }
                break;
            }

            if (WIFSTOPPED(status)) {
                int sig = WSTOPSIG(status);
                switch (sig)
                {
                case SIGTRAP:
                    ptrace(PTRACE_SYSCALL, pidRunner, NULL, NULL);
                    break;
                case SIGALRM:
                    LAZYOJ_RUN_RESULT = "TLE";
                    alarm(0);
                    ptrace(PTRACE_KILL, pidRunner, NULL, NULL);
                    break;
                }
            }
        }

        // 子进程未退出 

        // 读取最大使用内存
        memory_used = max(memory_used, get_proc_status(pidRunner, "VmSize:"));
        if (memory_used > atoi(problem["memory"].c_str()) * STD_MB * 1024) {
            LAZYOJ_RUN_RESULT = "MLE";
            // 发送结束信号
            ptrace(PTRACE_KILL, pidRunner, NULL, NULL);
            break;
        }

        // 输出超过限制 运行时错误
        long long file_used = get_file_size(filename + ".user.err");
        if (file_used > 0) {
            LAZYOJ_RUN_RESULT = "RE";
            // 发送结束信号
            ptrace(PTRACE_KILL, pidRunner, NULL, NULL);
            break;
        }

        ptrace(PTRACE_SYSCALL, pidRunner, NULL, NULL);
    }

    long long timeCost = rused.ru_utime.tv_sec * 1000 + rused.ru_utime.tv_usec / 1000;
    timeCost += rused.ru_stime.tv_sec * 1000 + rused.ru_stime.tv_usec / 1000;
    time_used += timeCost;
    cout << "测试结束: " << filename << endl;
    cout << "时间: " << timeCost  << endl;
}

void update_solution() {
    if (LAZYOJ_RUN_ALL_RESULT == "AC") {
        DB->changeToAC(solution["sid"], LAZYOJ_RUN_TIME, LAZYOJ_RUN_MEMORY);
    }
    else if (LAZYOJ_RUN_ALL_RESULT == "PE" || LAZYOJ_RUN_ALL_RESULT == "WA" || LAZYOJ_RUN_ALL_RESULT == "TLE" || LAZYOJ_RUN_ALL_RESULT == "MLE" || LAZYOJ_RUN_ALL_RESULT == "OLE" || LAZYOJ_RUN_ALL_RESULT == "RE") {
        stringstream runtimeInfo;
        fstream file;
        file.open(LAZYOJ_DIR_WORK+"/runtimeinfo.txt", ios::in);
        runtimeInfo << file.rdbuf();
        file.close();
        double passRate = testFileList.size() > 0 ? LAZYOJ_RUN_AC / double(testFileList.size()) : LAZYOJ_RUN_AC;
        cout << "passRate:" << passRate << endl;
        if (LAZYOJ_RUN_ALL_RESULT == "PE") {
            DB->changeToPE(solution["sid"], LAZYOJ_RUN_TIME, LAZYOJ_RUN_MEMORY, runtimeInfo.str(), passRate);
        }
        else if (LAZYOJ_RUN_ALL_RESULT == "WA") {
            DB->changeToWA(solution["sid"], LAZYOJ_RUN_TIME, LAZYOJ_RUN_MEMORY, runtimeInfo.str(), passRate);
        }
        else if (LAZYOJ_RUN_ALL_RESULT == "TLE") {
            DB->changeToTLE(solution["sid"], LAZYOJ_RUN_TIME, LAZYOJ_RUN_MEMORY, runtimeInfo.str(), passRate);
        }
        else if (LAZYOJ_RUN_ALL_RESULT == "MLE") {
            DB->changeToMLE(solution["sid"], LAZYOJ_RUN_TIME, LAZYOJ_RUN_MEMORY, runtimeInfo.str(), passRate);
        }
        else if (LAZYOJ_RUN_ALL_RESULT == "OLE") {
            DB->changeToOLE(solution["sid"], LAZYOJ_RUN_TIME, LAZYOJ_RUN_MEMORY, runtimeInfo.str(), passRate);
        }
        else if (LAZYOJ_RUN_ALL_RESULT == "RE") {
            DB->changeToRE(solution["sid"], LAZYOJ_RUN_TIME, LAZYOJ_RUN_MEMORY, runtimeInfo.str(), passRate);
        }
        runtimeInfo.str("");
    }
    else if (LAZYOJ_RUN_ALL_RESULT == "CE") {
        stringstream compileInfo;
        fstream file;
        file.open(LAZYOJ_DIR_WORK+"/compile.txt", ios::in);
        compileInfo << file.rdbuf();
        file.close();
        DB->changeToCE(solution["sid"], compileInfo.str());
    }
}

void initialize(int argc, char* argv[]) {
    if (argc < 2) {
        cout << "usage: cmd [solution_id] [node_id]" << endl;
        exit(0);
    }
    LAZYOJ_RUN_SID = argv[1];
    LAZYOJ_RUN_NID = argv[2];
    readDatabseConf();
    DB = new Database(LAZYOJ_DB_HOST, LAZYOJ_DB_USER, LAZYOJ_DB_PASS, LAZYOJ_DB_NAME, LAZYOJ_DB_PORT);
    if (!DB->initDatabase()) {
        cout << DB->getErrorInfo() << endl;
        exit(0);
    }
    // 读取配置数据
    settings = DB->getOptions();
    // 读取提交信息
    solution = DB->getSlutionInfo(LAZYOJ_RUN_SID);
    // 读取竞赛信息
    if (solution["cid"] != "0") {
        contest = DB->getContestInfo(solution["cid"]);
    }
    // 读取问题信息
    problem = DB->getProblemInfo(solution["pid"]);
    // 读取问题信息
    language = DB->getLanguageInfo(solution["language"]);
    // 创建运行目录
    makeWorkDir();
}

void destory() {
    clearWorkDir();
    delete DB;
}

void judge_solution() {
    DB->changeToBUILD(solution["sid"]);
    bool status = compile_solution();
    if (!status) {
        LAZYOJ_RUN_RESULT = "CE";
        update_solution();
        cout << "编译程序失败..." << endl;
        return;
    }
    get_test_file();
    DB->changeToRUN(solution["sid"]);
    for (map<string, int>::iterator it = testFileList.begin(); it != testFileList.end(); it++)
    {
        if (it->second == 2)
        {
            string filename = it->first;
            
            pid_t pidRunner = fork();
            if (pidRunner == 0)
            {
                // 子进程执行
                cout << "开始测试: " << filename << endl;
                run_solution(filename);
            }
            else
            {
                // 父进程监控
                cout << "Pid: " << pidRunner << endl;
                listen_solution(pidRunner, filename, LAZYOJ_RUN_TIME, LAZYOJ_RUN_MEMORY);
            }
            // 对比输出文件

            // 如果是ACM赛制 并且运行过程出错 跳出循环 不进行文件比较
            if (contest["type"] == "ACM" && LAZYOJ_RUN_RESULT != "AC") {
                break;
            }
            // 如果运行过程中没有出错 比较文件
            if (LAZYOJ_RUN_RESULT == "AC") {
                bool flag = compare_solution(filename + ".out", filename + ".user.out");
                cout << "匹配结果：" << endl << LAZYOJ_RUN_RESULT << endl;
                if (LAZYOJ_RUN_RESULT == "AC") {
                    LAZYOJ_RUN_AC += 1.0;
                }
                // 如果比较结果未非AC 并且是ACM赛制 跳出循环
                if (LAZYOJ_RUN_RESULT != "AC" && contest["type"] == "ACM") {
                    break;
                }
            }
        }
    }
    cout << "测试点个数：" << testFileList.size() << endl;
    cout << "通过个数：" << LAZYOJ_RUN_AC << endl;
    cout << "最终结果: " << LAZYOJ_RUN_ALL_RESULT << endl;
    update_solution();
}

int main(int argc, char* argv[]) {
    initialize(argc, argv);
    judge_solution();
    
    destory();
    return 0;
}