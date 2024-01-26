#include <stdio.h>
#include <sqlite3.h>
#include <stdlib.h>

int main()
{
    /* 打开数据库: 如果数据库不存在，那么就创建 */
    sqlite3 *mydb = NULL;
    int ret = sqlite3_open("demo.db", &mydb);
    if(ret != SQLITE_OK)
    {
        perror("sqlite open error");
        exit(-1);
    }

    /*创建一个表
    create table 表名（filed, field2,...)
    create table user(id int primary key not NULL, name int not NULL, name text not NULL)
    */
   
    char * errmsg = NULL;//不存在就创建
    const char * sql = "create table if not exists user(id int primary key not NULL, age int not NULL, name text not NULL)";
    ret = sqlite3_exec(mydb, sql, NULL, NULL, &errmsg);
    if(ret != SQLITE_OK)
    {
        fprintf(stderr, "create error:%s\n", sqlite3_errmsg(mydb));
        
        exit(-1);
    }
    
    //插入数据
    sql = "insert into user values (19, 'yedou', 'nanjing')";
    ret = sqlite3_exec(mydb, sql, NULL, NULL, &errmsg);
    if(ret != SQLITE_OK)
    {
        fprintf(stderr, "insert error:%s\n", sqlite3_errmsg(mydb));
        // exit(-1);
    }

    //新增一列address的属性
    sql = "alter table user add address text";
    ret = sqlite3_exec(mydb, sql, NULL, NULL, &errmsg);
    if(ret != SQLITE_OK)
    {
        fprintf(stderr, "add error:%s\n", sqlite3_errmsg(mydb));
        // exit(-1);
    }

    //删除一列
    sql = "alter table user drop column age";
    ret = sqlite3_exec(mydb, sql, NULL, NULL, &errmsg);
    if(ret != SQLITE_OK)
    {
        fprintf(stderr, "drop error:%s\n", sqlite3_errmsg(mydb));
        // exit(-1);
    }

    //修改一列address的属性
    #if 1
    sql = "alter table user alter cloumn address int;";
    ret = sqlite3_exec(mydb, sql, NULL, NULL, &errmsg);
    if(ret != SQLITE_OK)
    {
        fprintf(stderr, "modify error:%s\n", sqlite3_errmsg(mydb));
        // exit(-1);
    }
    #else
    sql = "ALTER TABLE user alter COLUMN address INT";
    ret = sqlite3_exec(mydb, sql, NULL, NULL, &errmsg);
    if (ret != SQLITE_OK)
    {
        printf("modfity error:%s\n", errmsg);
        exit(-1);
    }
    #endif
    //删除数据
    const char *sql2 = "delete from user where id = 20";
    ret = sqlite3_exec(mydb, sql2, NULL, NULL, &errmsg);
    if(ret != SQLITE_OK)
    {
        fprintf(stderr, "delete id error:%s\n", sqlite3_errmsg(mydb));
        exit(-1);
    }
   
    //更新数据库表
    /* sql：update 表名 set field = value1 where [condition] */
    const char *sql3 = "update user set name = 'rihe' where id = 15";
    ret = sqlite3_exec(mydb, sql3, NULL, NULL, &errmsg);
    if(ret != SQLITE_OK)
    {
        fprintf(stderr, "update error:%s\n", sqlite3_errmsg(mydb));
        exit(-1);
    }

    /* 查询数据库
    sql：select *from user*/
    char ** result = NULL;
    int row = 0;//行
    int column = 0;//列

    sql  = "select *from user";
    ret = sqlite3_get_table(mydb, sql, &result, &row, &column, &errmsg);
    if(ret != SQLITE_OK)
    {
        fprintf(stderr, "sqlite3_get_table error:%s\n", sqlite3_errmsg(mydb));
        exit(-1);
    }
    // *((*result + row) + column)= result[row][column];
    #if 0
    //打印表
    for(int idx = 0; idx <= row; idx++)
    {
        for(int jdx = 0; jdx < column; jdx++)
        {
            printf("%s ", result[idx * column + jdx]);//
        }
        printf("\n");
    }
    #endif
    //模糊匹配
    sql3 = "select name from user where address LIKE 'ri%'";
    ret = sqlite3_get_table(mydb, sql3, &result, &row, &column, &errmsg);
    if(ret != SQLITE_OK)
    {
        fprintf(stderr, "LIKE error:%s\n", sqlite3_errmsg(mydb));
        exit(-1);
    }
    printf("row:%d,\t cloumn:%d\n", row, column);

    //排序
    printf("order:\n");
    sql2 = "select address from user order by id desc";
    ret = sqlite3_get_table(mydb, sql2, &result, &row, &column, &errmsg);
    if(ret != SQLITE_OK)
    {
        fprintf(stderr, "order error:%s\n", sqlite3_errmsg(mydb));
        exit(-1);
    }
    /* 查询数据库 : 每次读取一行 */
    for (int idx = 0; idx <= row; idx++)
    {
        for (int jdx = 0; jdx < column; jdx++)
        {
            printf("%s\t", result[idx * column + jdx]);
        }
        printf("\n");
    }

    #if 0
    /* 删除表：drop table 表名 */
    sql2 = "drop table user";
    ret = sqlite3_exec(mydb, sql2, NULL, NULL, &errmsg);
    if(ret != SQLITE_OK)
    {
        fprintf(stderr, "drop user error:%s\n", sqlite3_errmsg(mydb));
        exit(-1);
    }
    #endif

    /* 关闭数据库 */
    sqlite3_close(mydb);
    return 0;
}