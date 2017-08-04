#pragma once

#include<codecvt>

class my_codecvt
{
    class chs_codecvt : public std::codecvt_byname<wchar_t, char, std::mbstate_t>
    {
    public:
        chs_codecvt() :codecvt_byname("chs") {}
    };

    static constexpr std::wstring_convert<std::codecvt_utf8<wchar_t>> cvt_();

public:



    static std::string utf8(const std::wstring& ws)
    { 
        return std::wstring_convert<std::codecvt_utf8<wchar_t>>().to_bytes(ws);
    }

    static std::wstring utf8(const std::string& s)
    {
        return std::wstring_convert<std::codecvt_utf8<wchar_t>>().from_bytes(s);
    }

    static std::wstring chs(const std::string& s)
    {
        return std::wstring_convert<chs_codecvt>().from_bytes(s);
    }

    static std::string chs(const std::wstring& ws)
    {
        return std::wstring_convert<chs_codecvt>().to_bytes(ws);
    }

};

//std::wstring_convert<std::codecvt_utf8<wchar_t>> my_codecvt::cvt_;

