#pragma once

#include <mysql/mysql.h>
#include <string>
#include <sstream>
#include <map>

namespace lazyoj
{

    using std::string;
    using std::stringstream;
    using std::map;
    using std::endl;

    class Database {
    private:
    string DB_HOST;
    string DB_USER; 
    string DB_PASS;
    string DB_NAME;
    string DB_PORT;
    stringstream errorInfo;
    stringstream sstream;
    string sql;
    int errorCode;
    MYSQL *mysqlConn;
    MYSQL_RES *mysqlRes;
    MYSQL_ROW mysqlRow;
    public:
    Database(string hostname = "127.0.0.1", string username = "root", string password = "root", string database = "root", string hostport = "3306");
    bool initDatabase();
    void setHost(string hostname = "127.0.0.1");
    void setUser(string username = "root");
    void setPass(string password = "root");
    void setName(string database = "root");
    void setPort(string hostport = "3306");
    int getErrorCode();
    string getErrorInfo();
    map<string, string> getOptions();
    map<string, string> getSlutionInfo(string solutionId);
    map<string, string> getProblemInfo(string problemId);
    map<string, string> getLanguageInfo(string languageId);
    map<string, string> getContestInfo(string contestId);
    map<string, string> getJobList(string nums);
    void changeToAC(string solutionId, long long time_used, long long memory_used);
    void changeToPE(string solutionId, long long time_used, long long memory_used, string runTimeInfo, double passRate);
    void changeToWA(string solutionId, long long time_used, long long memory_used, string runTimeInfo, double passRate);
    void changeToTLE(string solutionId, long long time_used, long long memory_used, string runTimeInfo, double passRate);
    void changeToMLE(string solutionId, long long time_used, long long memory_used, string runTimeInfo, double passRate);
    void changeToOLE(string solutionId, long long time_used, long long memory_used, string runTimeInfo, double passRate);
    void changeToRE(string solutionId, long long time_used, long long memory_used, string runTimeInfo, double passRate);
    void changeToCE(string solutionId, string compilInfo);
    void changeToWT0(string solutionId);
    void changeToWT1(string solutionId);
    void changeToBUILD(string solutionId);
    void changeToRUN(string solutionId);


    };


}