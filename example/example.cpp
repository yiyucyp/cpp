// example.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include "json.hpp"
#include <iostream>
#include "utf8_codecvt/my_codecvt.hpp"

using json = nlohmann::json;

struct ErrorInfo
{
    int errcode;
    std::string errmsg;
};

void to_json(json& j, const ErrorInfo& p)
{

}
void from_json(const json& j, ErrorInfo& p)
{

}

void json_test()
{
    

    {
        ErrorInfo einfo={-3, "fuck"};
        json j = einfo;
        std::cout << j<<std::endl;
    }



    std::string str = R"({"errcode":-3,"errmsg":"117.29.177.162无匹配的数据!"})";
    std::string utf8 = R"(abcd操作)";
    std::cout << str<<std::endl;
    json j = json::parse(str.c_str());

    std::cout << j;

    int errcode = j["errcode"].get<int>();

    json j_1 = R"({
"baz":["one", "two"]})"_json;

    std::cout << j_1;
}


extern void test_codecvt();
extern void test_sqlite();
extern void test_http();

int _tmain(int argc, _TCHAR* argv[])
{
    test_http();
    test_sqlite();
    json_test();
    test_codecvt();
    json_test();
	return 0;
}

