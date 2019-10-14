#include "Database.h"

namespace lazyoj {

    Database::Database(string hostname, string username, string password, string database, string hostport) {
        this->DB_HOST = hostname;
        this->DB_USER = username;
        this->DB_PASS = password;
        this->DB_NAME = database;
        this->DB_PORT = hostport;
        this->sstream.precision(3);
    }

    bool Database::initDatabase() {
        this->mysqlConn = mysql_init(NULL);
        if (mysql_real_connect(this->mysqlConn, this->DB_HOST.c_str(), this->DB_USER.c_str(), this->DB_PASS.c_str(), this->DB_NAME.c_str(), atoi(this->DB_PORT.c_str()), NULL, 0) == NULL) {
            this->errorInfo << "连接数据库失败..." << endl;
            this->errorInfo << "服务启动失败！" << endl;
            this->errorCode = -1;
            return false;
        }
        return true;
    }
    
    void Database::setHost(string hostname) {
        this->DB_HOST = hostname;
    }

    void Database::setUser(string username) {
        this->DB_USER = username;
    }

    void Database::setPass(string password) {
        this->DB_PASS = password;
    }

    void Database::setName(string database) {
        this->DB_NAME = database;
    }

    void Database::setPort(string hostport) {
        this->DB_PORT = hostport;
    }

    map<string, string> Database::getOptions() {
        map<string, string> options;
        this->sstream << "SELECT `key`, `value` FROM `option` WHERE 1";
        this->sql = sstream.str();
        sstream.str("");
        mysql_real_query(this->mysqlConn, this->sql.c_str(), this->sql.size());
        this->mysqlRes = mysql_store_result(this->mysqlConn);
        while (NULL != this->mysqlRes && (this->mysqlRow = mysql_fetch_row(this->mysqlRes)) != NULL)
        {
            options[mysqlRow[0]] = mysqlRow[1];
        }
        return options;
    }

    int Database::getErrorCode() {
        return this->errorCode;
    }

    string Database::getErrorInfo() {
        return this->errorInfo.str();
    }

    map<string, string> Database::getSlutionInfo(string solutionId) {
        map<string, string> result;
        this->sstream << "SELECT `sid`, `pid`, `uid`, `cid`, `code`, `language` FROM `solution` WHERE `sid`='" << solutionId << "'";
        this->sql = sstream.str();
        this->sstream.str("");
        mysql_real_query(this->mysqlConn, this->sql.c_str(), this->sql.size());
        this->mysqlRes = mysql_store_result(this->mysqlConn);
        while (NULL != this->mysqlRes && (this->mysqlRow = mysql_fetch_row(this->mysqlRes)) != NULL)
        {
            result["sid"] = string(this->mysqlRow[0]);
            result["pid"] = string(this->mysqlRow[1]);
            result["uid"] = string(this->mysqlRow[2]);
            result["cid"] = string(this->mysqlRow[3]);
            result["code"] = string(this->mysqlRow[4]);
            result["language"] = string(this->mysqlRow[5]);
        }
        return result;
    }

    map<string, string> Database::getProblemInfo(string problemId) {
        map<string, string> result;
        this->sstream << "SELECT `pid`, `time`, `memory` FROM `problem` WHERE `pid`='" << problemId << "'";
        this->sql = sstream.str();
        this->sstream.str("");
        mysql_real_query(this->mysqlConn, this->sql.c_str(), this->sql.size());
        this->mysqlRes = mysql_store_result(this->mysqlConn);
        while (NULL != this->mysqlRes && (this->mysqlRow = mysql_fetch_row(this->mysqlRes)) != NULL)
        {
            result["pid"] = string(this->mysqlRow[0]);
            result["time"] = string(this->mysqlRow[1]);
            result["memory"] = string(this->mysqlRow[2]);
        }
        return result;
    }

    map<string, string> Database::getLanguageInfo(string languageId) {
        map<string, string> result;
        this->sstream << "SELECT `lid`, `ext`, `cmd` FROM `language` WHERE `lid`='" << languageId << "'";
        this->sql = sstream.str();
        this->sstream.str("");
        mysql_real_query(this->mysqlConn, this->sql.c_str(), this->sql.size());
        this->mysqlRes = mysql_store_result(this->mysqlConn);
        while (NULL != this->mysqlRes && (this->mysqlRow = mysql_fetch_row(this->mysqlRes)) != NULL)
        {
            result["lid"] = string(this->mysqlRow[0]);
            result["ext"] = string(this->mysqlRow[1]);
            result["cmd"] = string(this->mysqlRow[2]);
        }
        return result;
    }

    map<string, string> Database::getContestInfo(string contestId) {
        map<string, string> result;
        this->sstream << "SELECT `cid`, `type`, `format` FROM `contest` WHERE `cid`='" << contestId << "'";
        this->sql = sstream.str();
        this->sstream.str("");
        mysql_real_query(this->mysqlConn, this->sql.c_str(), this->sql.size());
        this->mysqlRes = mysql_store_result(this->mysqlConn);
        while (NULL != this->mysqlRes && (this->mysqlRow = mysql_fetch_row(this->mysqlRes)) != NULL)
        {
            result["cid"] = string(this->mysqlRow[0]);
            result["type"] = string(this->mysqlRow[1]);
            result["format"] = string(this->mysqlRow[2]);
        }
        return result;
    }

    map<string, string> Database::getJobList(string nums) {
        map<string, string> result;
        this->sstream << "SELECT `sid` FROM `solution` WHERE `result`='0' OR `result`='1' ORDER BY `create_time` ASC LIMIT " << nums << ";";
        this->sql = sstream.str();
        this->sstream.str("");
        mysql_real_query(this->mysqlConn, this->sql.c_str(), this->sql.size());
        this->mysqlRes = mysql_store_result(this->mysqlConn);
        while (NULL != this->mysqlRes && (this->mysqlRow = mysql_fetch_row(this->mysqlRes)) != NULL)
        {
            result[string(this->mysqlRow[0])] = "1";
        }
        return result;
    }
    
    void Database::changeToAC(string solutionId, long long time_used, long long memory_used) {
        this->sstream << "UPDATE `solution` SET `result`=4, `time`='" << time_used << "', `memory`='" << memory_used << "', `pass_rate`='1' WHERE `sid`=" << solutionId;
        this->sql = sstream.str();
        this->sstream.str("");
        mysql_real_query(this->mysqlConn, this->sql.c_str(), this->sql.size());
        this->mysqlRes = mysql_store_result(this->mysqlConn);
    }

    void Database::changeToPE(string solutionId, long long time_used, long long memory_used, string runTimeInfo, double passRate) {
        this->sstream << "UPDATE `solution` SET `result`=5, `time`='" << time_used << "', `memory`='" << memory_used << "', `pass_rate`='" << passRate << "' WHERE `sid`=" << solutionId;
        this->sql = sstream.str();
        this->sstream.str("");
        mysql_real_query(this->mysqlConn, this->sql.c_str(), this->sql.size());
        this->mysqlRes = mysql_store_result(this->mysqlConn);
    }

    void Database::changeToWA(string solutionId, long long time_used, long long memory_used, string runTimeInfo, double passRate) {
        this->sstream << "UPDATE `solution` SET `result`=6, `time`='" << time_used << "', `memory`='" << memory_used << "', `pass_rate`='" << passRate << "' WHERE `sid`=" << solutionId;
        this->sql = sstream.str();
        this->sstream.str("");
        mysql_real_query(this->mysqlConn, this->sql.c_str(), this->sql.size());
        this->mysqlRes = mysql_store_result(this->mysqlConn);
    }

    void Database::changeToTLE(string solutionId, long long time_used, long long memory_used, string runTimeInfo, double passRate) {
        this->sstream << "UPDATE `solution` SET `result`=7, `time`='" << time_used << "', `memory`='" << memory_used << "', `pass_rate`='" << passRate << "' WHERE `sid`=" << solutionId;
        this->sql = sstream.str();
        this->sstream.str("");
        mysql_real_query(this->mysqlConn, this->sql.c_str(), this->sql.size());
        this->mysqlRes = mysql_store_result(this->mysqlConn);
    }

    void Database::changeToMLE(string solutionId, long long time_used, long long memory_used, string runTimeInfo, double passRate) {
        this->sstream << "UPDATE `solution` SET `result`=8, `time`='" << time_used << "', `memory`='" << memory_used << "', `pass_rate`='" << passRate << "' WHERE `sid`=" << solutionId;
        this->sql = sstream.str();
        this->sstream.str("");
        mysql_real_query(this->mysqlConn, this->sql.c_str(), this->sql.size());
        this->mysqlRes = mysql_store_result(this->mysqlConn);
    }

    void Database::changeToOLE(string solutionId, long long time_used, long long memory_used, string runTimeInfo, double passRate) {
        this->sstream << "UPDATE `solution` SET `result`=9, `time`='" << time_used << "', `memory`='" << memory_used << "', `pass_rate`='" << passRate << "' WHERE `sid`=" << solutionId;
        this->sql = sstream.str();
        this->sstream.str("");
        mysql_real_query(this->mysqlConn, this->sql.c_str(), this->sql.size());
        this->mysqlRes = mysql_store_result(this->mysqlConn);
    }

    void Database::changeToRE(string solutionId, long long time_used, long long memory_used, string runTimeInfo, double passRate) {
        this->sstream << "UPDATE `solution` SET `result`=10, `time`='" << time_used << "', `memory`='" << memory_used << "', `pass_rate`='" << passRate << "' WHERE `sid`=" << solutionId;
        this->sql = sstream.str();
        this->sstream.str("");
        mysql_real_query(this->mysqlConn, this->sql.c_str(), this->sql.size());
        this->mysqlRes = mysql_store_result(this->mysqlConn);
    }

    void Database::changeToCE(string solutionId, string compilInfo) {
        this->sstream << "UPDATE `solution` SET `result`=11, `compile_info`='" << compilInfo << "' WHERE `sid`=" << solutionId;
        this->sql = sstream.str();
        this->sstream.str("");
        mysql_real_query(this->mysqlConn, this->sql.c_str(), this->sql.size());
        this->mysqlRes = mysql_store_result(this->mysqlConn);
    }

    void Database::changeToWT0(string solutionId) {
        this->sstream << "UPDATE `solution` SET `result`=0 WHERE `sid`=" << solutionId;
        this->sql = sstream.str();
        this->sstream.str("");
        mysql_real_query(this->mysqlConn, this->sql.c_str(), this->sql.size());
        this->mysqlRes = mysql_store_result(this->mysqlConn);
    }

    void Database::changeToWT1(string solutionId) {
        this->sstream << "UPDATE `solution` SET `result`=1 WHERE `sid`=" << solutionId;
        this->sql = sstream.str();
        this->sstream.str("");
        mysql_real_query(this->mysqlConn, this->sql.c_str(), this->sql.size());
        this->mysqlRes = mysql_store_result(this->mysqlConn);
    }

    void Database::changeToBUILD(string solutionId) {
        this->sstream << "UPDATE `solution` SET `result`=2 WHERE `sid`=" << solutionId;
        this->sql = sstream.str();
        this->sstream.str("");
        mysql_real_query(this->mysqlConn, this->sql.c_str(), this->sql.size());
        this->mysqlRes = mysql_store_result(this->mysqlConn);
    }

    void Database::changeToRUN(string solutionId) {
        this->sstream << "UPDATE `solution` SET `result`=3 WHERE `sid`=" << solutionId;
        this->sql = sstream.str();
        this->sstream.str("");
        mysql_real_query(this->mysqlConn, this->sql.c_str(), this->sql.size());
        this->mysqlRes = mysql_store_result(this->mysqlConn);
    }

}