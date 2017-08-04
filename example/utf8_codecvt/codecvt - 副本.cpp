#include <iostream>
#include <codecvt>

// http://www.cppblog.com/lf426/archive/2010/06/26/118772.html
// http://www.360doc.com/content/12/1202/22/3398926_251650422.shtml

using namespace std;

const std::string ws2utf8(const std::wstring& src)
{
    std::wstring_convert<std::codecvt_utf8<wchar_t>> conv;
    return conv.to_bytes(src);
}

const std::wstring utf8_2_ws(const std::string& src)
{
    std::wstring_convert<std::codecvt_utf8<wchar_t> > conv;
    return conv.from_bytes(src);
}

void test_codecvt()
{
    std::locale sys_loc("");
    //mbstocws
    std::cout<< sys_loc.name();
    {
        std::string utf81 = R"(abcd操作)";
        std::string utf8 = R"(abcd操作符)";
        std::string str = R"({"errcode":-3,"errmsg":"117.29.177.162无匹配的数据!"})";

        cout << utf8<<utf81<< str;
        
        std::wstring_convert<std::codecvt_utf8<wchar_t>> conv;
        std::wstring ws = conv.from_bytes(utf8);
        
        utf8 = conv.to_bytes(ws);
    }
}