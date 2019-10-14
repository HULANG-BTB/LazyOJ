#include "Compare.h"

#include <string>
#include <fstream>
#include <sstream>

/**
 * Class Compare
 * Author: Lang Hu
 * QQ: 774023581
 * Email: admin@oibit.cn
 * Date: 2019-10-10
 * CompareCode: -2 --- 打开文件失败
 *               0 --- 答案错误
 *               1 --- 格式错误
 *               2 --- 对比通过
 **/

namespace lazyoj
{

using std::endl;
using std::fstream;
using std::ios;
using std::size_t;
using std::string;

Compare::Compare(string fileName_1, string fileName_2)
{
    this->filename_1 = fileName_1;
    this->filename_2 = fileName_2;
    this->setStrict();
}
void Compare::setFileOne(string fileName)
{
    this->filename_1 = fileName;
}
void Compare::setFileTwo(string fileName)
{
    this->filename_2 = fileName;
}
void Compare::setStrict(bool Strict)
{
    this->strict = Strict;
}
bool Compare::compare()
{
    fstream File1, File2;
    File1.open(this->filename_1, ios::in);
    File2.open(this->filename_2, ios::in);
    if (!File1.is_open())
    {
        compareInfo << "Failed to open file1!" << std::endl;
        compareCode = -2;
        return false;
    }
    if (!File2.is_open())
    {
        compareInfo << "Failed to open file2!" << std::endl;
        compareCode = -2;
        return false;
    }
    string line1, line2;
    int count = 1;
    while (true)
    {
        line1.clear();
        line2.clear();
        // 文件1到末尾 且 文件2到末尾 正确
        if (File1.eof() && File2.eof())
        {
            compareInfo << "AC";
            compareCode = 2;
            return true;
        }
        // 如果 文件1到文件末尾 但文件2未到文件尾
        else if (File1.eof() && !File2.eof())
        {
            // 文件2 读取一行
            getline(File2, line2);
            string newLine = line2;
            // 清除换行 回车 空格
            this->deleteAllMark(newLine, "\r");
            this->deleteAllMark(newLine, "\n");
            this->deleteAllMark(newLine, " ");
            // 如果是严格模式
            if (this->strict)
            {
                // 如果清除后的字符串长度等于0 或 文件2未到文件尾 格式错误
                if (!newLine.size() && File2.eof())
                {
                    this->compareInfo << "=============" << this->filename_1 << "=============" << endl;
                    this->compareInfo << "Line: " << count << " Result:PE" << endl
                                      << "Expected:" << endl;
                    this->compareInfo << "---(NULL)---" << endl;
                    this->compareInfo << "Yours:" << endl;
                    this->compareInfo << "---" << line2 << "---" << endl;
                    this->compareInfo << "=============" << this->filename_1 << "=============" << endl;
                    this->compareInfo << endl;
                    this->compareCode = 1;
                    return false;
                }
                else
                {
                    this->compareInfo << "=============" << this->filename_1 << "=============" << endl;
                    this->compareInfo << "Line: " << count << " Result:WA" << endl
                                      << "Expected:" << endl;
                    this->compareInfo << "---(NULL)---" << endl;
                    this->compareInfo << "Yours:" << endl;
                    this->compareInfo << "---" << line2 << "---" << endl;
                    this->compareInfo << "=============" << this->filename_1 << "=============" << endl;
                    this->compareInfo << endl;
                    this->compareCode = 0;
                    return false;
                }
            }
            // 非严格模式
            else
            {
                // 如果清除后的字符串长度为0 并且 文件2未到文件尾 答案正确
                if (!newLine.size() && File2.eof())
                {
                    this->compareInfo << "AC";
                    this->compareCode = 2;
                    return true;
                }
                else
                {
                    // 否则 答案错误
                    this->compareInfo << "=============" << this->filename_1 << "=============" << endl;
                    this->compareInfo << "Line: " << count << " Result:WA" << endl
                                      << "Expected:" << endl;
                    this->compareInfo << "---(NULL)---" << endl;
                    this->compareInfo << "Yours:" << endl;
                    this->compareInfo << "---" << line2 << "---" << endl;
                    this->compareInfo << "=============" << this->filename_1 << "=============" << endl;
                    this->compareInfo << endl;
                    this->compareCode = 0;
                    return false;
                }
            }
        }
        // 文件1未到末尾 文件2到末尾
        else if (!File1.eof() && File2.eof())
        {
            // 文件1 读取一行
            getline(File1, line1);
            string newLine = line1;
            // 清除 空白字符
            this->deleteAllMark(newLine, "\r");
            this->deleteAllMark(newLine, "\n");
            this->deleteAllMark(newLine, " ");
            if (this->strict)
            {
                // 严格模式
                if (!newLine.size() && File1.eof())
                {
                    // 如果清除后的字符串长度为0 并且 文件1未到文件尾 格式错误
                    this->compareInfo << "=============" << this->filename_1 << "=============" << endl;
                    this->compareInfo << "Line: " << count << " Result:PE" << endl
                                      << "Expected:" << endl;
                    this->compareInfo << "---" << line1 << "---" << endl;
                    this->compareInfo << "Yours:" << endl;
                    this->compareInfo << "---(NULL)---" << endl;
                    this->compareInfo << "=============" << this->filename_1 << "=============" << endl;
                    this->compareInfo << endl;
                    this->compareCode = 1;
                    return false;
                }
                else
                {
                    // 否则 答案错误
                    this->compareInfo << "=============" << this->filename_1 << "=============" << endl;
                    this->compareInfo << "Line: " << count << " Result:WA" << endl
                                      << "Expected:" << endl;
                    this->compareInfo << "---" << line1 << "---" << endl;
                    this->compareInfo << "Yours:" << endl;
                    this->compareInfo << "---(NULL)---" << endl;
                    this->compareInfo << "=============" << this->filename_1 << "=============" << endl;
                    this->compareInfo << endl;
                    this->compareCode = 0;
                    return false;
                }
            }
            // 非严格模式
            else
            {
                // 如果新行长度等于0 且文件1到文件末尾 答案正确
                if (!newLine.size() && File1.eof())
                {
                    this->compareInfo << "AC";
                    this->compareCode = 2;
                    return true;
                }
                // 否则答案错误
                else
                {
                    this->compareInfo << "=============" << this->filename_1 << "=============" << endl;
                    this->compareInfo << "Line: " << count << " Result:WA" << endl
                                      << "Expected:" << endl;
                    this->compareInfo << "---" << line1 << "---" << endl;
                    this->compareInfo << "Yours:" << endl;
                    this->compareInfo << "---(NULL)---" << endl;
                    this->compareInfo << "=============" << this->filename_1 << "=============" << endl;
                    this->compareInfo << endl;
                    this->compareCode = 0;
                    return false;
                }
            }
        }
        // 两个文件均未到末尾
        else
        {
            // 分别读取一行
            getline(File1, line1);
            getline(File2, line2);
            // 删除末尾 换行 回车
            deleteAllMark(line1, "\r");
            deleteAllMark(line2, "\r");
            deleteAllMark(line1, "\n");
            deleteAllMark(line2, "\n");
            // 如果两行内容不相等
            if (line1 != line2)
            {
                // 严格模式 比较格式
                if (this->strict)
                {
                    // 删除所有空格
                    string newLineStd = line1;
                    string newLineUser = line2;
                    deleteAllMark(newLineStd, " ");
                    deleteAllMark(newLineUser, " ");
                    // 清除空格后相等 格式错误
                    if (newLineStd == newLineUser)
                    {
                        this->compareInfo << "=============" << this->filename_1 << "=============" << endl;
                        this->compareInfo << "Line: " << count << " Result:PE" << endl
                                          << "Expected:" << endl;
                        this->compareInfo << line1 << endl;
                        this->compareInfo << "Yours:" << endl;
                        this->compareInfo << line2 << endl;
                        this->compareInfo << "=============" << this->filename_1 << "=============" << endl;
                        this->compareInfo << endl;
                        this->compareCode = 1;
                        return false;
                    }
                    // 否则答案错误
                    else
                    {
                        this->compareInfo << "=============" << this->filename_1 << "=============" << endl;
                        this->compareInfo << "Line: " << count << " Result:WA" << endl
                                          << "Expected:" << endl;
                        this->compareInfo << "---" << line1 << "---" << endl;
                        this->compareInfo << "Yours:" << endl;
                        this->compareInfo << "---" << line2 << "---" << endl;
                        this->compareInfo << "=============" << this->filename_1 << "=============" << endl;
                        this->compareInfo << endl;
                        this->compareCode = 0;
                        return false;
                    }
                }
                // 非严格模式 答案错误
                else
                {
                    // 删除所有空格
                    string newLineStd = line1;
                    string newLineUser = line2;
                    deleteAllMark(newLineStd, " ");
                    deleteAllMark(newLineUser, " ");
                    // 清除空格后不相等 格式错误
                    if (newLineStd != newLineUser)
                    {
                        this->compareInfo << "=============" << this->filename_1 << "=============" << endl;
                        this->compareInfo << "Line: " << count << " Result:WA" << endl
                                          << "Expected:" << endl;
                        this->compareInfo << "---" << line1 << "---" << endl;
                        this->compareInfo << "Yours:" << endl;
                        this->compareInfo << "---" << line2 << "---" << endl;
                        this->compareInfo << "=============" << this->filename_1 << "=============" << endl;
                        this->compareInfo << endl;
                        this->compareCode = 0;
                        return false;
                    }
                }
            }
        }
        count++;
    }
    return true;
}
void Compare::deleteAllMark(string &val, string mark)
{
    string::size_type pos = val.find_first_of(mark);
    while (pos != val.npos)
    {
        val.replace(pos, mark.size(), "");
        pos = val.find_first_of(mark);
    }
}
int Compare::getCompareCode()
{
    return this->compareCode;
}
string Compare::getCompareInfo() const
{
    return this->compareInfo.str();
}

} // namespace lazyoj
