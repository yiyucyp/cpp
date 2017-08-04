#include <iostream>
#include <codecvt>
//#include <boost/locale/encoding.hpp>
#include "my_codecvt.hpp"

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

class chs_codecvt : public std::codecvt_byname<wchar_t, char, std::mbstate_t>
{
public:
    chs_codecvt():codecvt_byname("chs"){}
};

wstring_convert<codecvt_byname<wchar_t, char, mbstate_t>> cv1(new codecvt_byname<wchar_t, char, mbstate_t>(".936"));

void test_codecvt()
{
    {
        std::string s1 = my_codecvt::utf8(L"1234");
        my_codecvt::utf8(s1);
        chs_codecvt _11;
        wstring_convert<chs_codecvt> _936;

    }

    std::locale sys_loc("");
    //mbstocws
    std::cout<< sys_loc.name();
    {
        std::string utf81 = "(abcd操作)";
        std::string utf8 = "abcd操作";
        std::string str = R"({"errcode":-3,"errmsg":"117.29.177.162无匹配的数据!"})";

        cout << utf8<<utf81<<str;
        
        std::wstring_convert<std::codecvt_utf8<wchar_t>> conv;
        std::wstring ws = conv.from_bytes(utf8);
        
        //string gbk1 = boost::locale::conv::between(utf8, "GBK", "UTF-8");
        string gbk = cv1.to_bytes(ws);
        gbk = wstring_convert<chs_codecvt>().to_bytes(ws);
        utf8 = conv.to_bytes(ws);
    }
}

