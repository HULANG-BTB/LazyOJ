#pragma once

#include <fstream>
#include <string>
#include <sstream>

namespace lazyoj {

    using std::string;
    using std::fstream;
    using std::stringstream;

    class Compare {
    private:
        string filename_1;
        string filename_2;
        stringstream compareInfo;
        int compareCode; // 打开文件错误 -2 答案错误0 格式错误1 通过2
        bool strict;
        void deleteAllMark(string& val, string mark = "");

    public:
        Compare(string fileName_1 = "", string fileName_2 = "");
        void setFileOne(string fileName = "");
        void setFileTwo(string fileName = "");
        void setStrict(bool Strict = false);
        bool compare();
        int getCompareCode();
        string getCompareInfo() const;

    };

}
