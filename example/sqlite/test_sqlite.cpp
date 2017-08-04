#include <iostream>

#include "hiberlite.h"
#include "otl/otl_wapper.hpp"


struct test_db : public otl_interface<test_db>
{
    // data
    int id;
    string name;

    // create table just set the sql string
    static constexpr char* create_table = "create table if not exists test_db (id int(16) not null primary key, name varchar(32))";
    // insert set the property sql string once, and set the property data, only once
    void insert(otl_connect& db)
    {
        otl_stream o(1, "insert into test_db values(:id<int>, :name<char[33]>)", db);
        o << id;
        o << name;
        o.flush();
    }

    // select这种写法有点蠢了
    //template<typename select_actor>
    //void select(select_actor& st, otl_connect& db)
    //{
    //    st.action(db);
    //}
    //struct select_type_a
    //{
    //    char* select;
    //    select_type_a(char* str) : select(str) {}
    //};
    //template<>
    //void select(select_type_a& st, otl_connect& db)
    //{
    //}

    // lambda
    void get_all_id(otl_connect& db)
    {
        test_db::handle_action(db, [&](otl_connect& db)
        {
            otl_stream select(1, "select * from test_db where id>=10", db);
            while (!select.eof())
            {
                select >> id >> name;
                std::cout << id << name<<std::endl;
            }
        });
    }
};


void test_sqlite()
{
    //otld
    //hiberlite::Database db("sample.db");
    otl_connect db;

    int ret = otl_connect::otl_initialize();

    try 
    {
        //db.rlogon("UID=root;PWD=root;DSN=testsys");
        db.rlogon("root/root@testsys");

        //otl_cursor::direct_exec(db, "create table test_ab(f1 int, f2 varchar(11))");

        test_db t;
        t.id = 111;
        t.name = "上海";
        t.create(db);

        test_db::handle_action(db, [](otl_connect& db) {
            db.direct_exec("delete from test_db");
        });
        //t.insert(db);

        for (size_t i = 0; i < 100; i++)
        {
            t.id = i;
            t.insert(db);
        }

        t.get_all_id(db);



        std::list<test_db> ary_set;
        test_db::handle_action(db, [&](otl_connect& db) {
            otl_stream o(1, "select * from test_db", db);

            test_db tt;
            while (!o.eof())
            {
                o >> tt.id >> tt.name;
                ary_set.push_back(tt);
            }

        });

    }
    catch (otl_exception& p)
    {
        cout << p.msg << p.var_info;

    }
     db.logoff();
}