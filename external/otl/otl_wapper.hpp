#pragma once


//http://otl.sourceforge.net/otl4_ex700.htm
#define OTL_ODBC
#define OTL_STL // Turn on STL features
#include "otl/otlv4.h"


using namespace std;

template<typename date_struct>
class otl_interface
{
public:
    static void create(otl_connect& db)
    {
        db.direct_exec(date_struct::create_table);
    }
    void insert(otl_connect& db)
    {
        date_struct& data = static_cast<date_struct&>(*this);
        data.insert(db);
    }
    //template<typename select_type>
    //void select(select_type& st, otl_connect& db)
    //{
    //    date_struct& data = static_cast<date_struct&>(*this);      
    //    data.select(st, db);
    //}

    // lambda
    template<typename hander>
    static void handle_action(otl_connect& db, hander h)
    {
        h(db);
    }
};

//template<typename data_struct>
//otl_stream& operator << (otl_stream& s, data_struct data)
//{
//    data.in(s);
//    return s;
//}


