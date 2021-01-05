#include <iostream>
using namespace std;

#pragma pack(1)

struct test_t {

    /* 长度4 > 1 按1 对齐；起始offset=0 0%1=0；存放位置区间[0,3] */   
    int a;

    /* 长度1 = 1 按1 对齐；起始offset=4 4%1=0；存放位置区间[4] */
    char b;

    /* 长度2 > 1 按1 对齐；起始offset=5 5%1=0；存放位置区间[5,6] */
    short c;

    /* 长度1 = 1 按1 对齐；起始offset=7 7%1=0；存放位置区间[7] */
    char d;
};

#pragma pack(2)

struct test_t2 {

    /* 长度4 > 2 按2 对齐；起始offset=0 0%2=0；存放位置区间[0,3] */
    int a;

     /* 长度1 < 2 按2 对齐；起始offset=4 4%1=0；存放位置区间[4] */
    char b;

    /* 长度2 = 2 按2 对齐；起始offset=6 6%2=0；存放位置区间[6,7] */
    short c;

    /* 长度1 < 2 按2 对齐；起始offset=8 8%1=0；存放位置区间[8,9] */
    char d;
};

#pragma pack(4)

struct test_t4 {

    /* 长度4 = 4 按4 对齐；起始offset=0 0%4=0；存放位置区间[0,3] */
    int a;

    /* 长度1 < 4 按1 对齐；起始offset=4 4%1=0；存放位置区间[4] */
    char b;

    /* 长度2 < 4 按2 对齐；起始offset=6 6%2=0；存放位置区间[6,7] */
    short c;

    /* 长度1 < 4 按1 对齐；起始offset=8 8%1=0；存放位置区间[8] */
    char d;
};

#pragma pack(8)

struct test_t8 {

    /* 长度4 < 8 按4 对齐；起始offset=0 0%4=0；存放位置区间[0,3] */
    int a;

    /* 长度1 < 8 按1 对齐；起始offset=4 4%1=0；存放位置区间[4] */
    char b;

    /* 长度2 < 8 按2 对齐；起始offset=6 6%2=0；存放位置区间[6,7] */
    short c; 

    /* 长度1 < 8 按1 对齐；起始offset=8 8%1=0；存放位置区间[8] */
    char d;
};

#pragma pack(16)

struct test_t16 {

    /* 长度4 < 16 按4 对齐；起始offset=0 0%4=0；存放位置区间[0,3] */
    int a;

    /* 长度1 < 16 按1 对齐；起始offset=4 4%1=0；存放位置区间[4] */
    char b;

    /* 长度2 < 16 按2 对齐；起始offset=6 6%2=0；存放位置区间[6,7] */
    short c;

    /* 长度1 < 16 按1 对齐；起始offset=8 8%1=0；存放位置区间[8] */
    char d;
};

// 8 字节和16 字节对齐试验证明了“规则”的第3 点：“当#pragma pack 的n 值等于或超过所有数据成员长度的时候，这个n 值的大小将不产生任何效果”。
int main1()
{
    cout<<"size(char):"<<sizeof(char)<<endl;
    cout<<"size(short):"<<sizeof(short)<<endl;
    cout<<"size(int):"<<sizeof(int)<<endl;
    cout<<"size(long):"<<sizeof(long)<<endl;
    cout<<"size(long long):"<<sizeof(long long)<<endl;
    cout<<"size(double):"<<sizeof(double)<<endl;
    cout<<"size(float):"<<sizeof(float)<<endl;
    cout<<endl;
    
    cout<<"size(1):"<<sizeof(struct test_t)<<endl;
    cout<<"size(2):"<<sizeof(struct test_t2)<<endl;
    cout<<"size(4):"<<sizeof(struct test_t4)<<endl;
    cout<<"size(8):"<<sizeof(struct test_t8)<<endl;
    cout<<"size(16):"<<sizeof(struct test_t16)<<endl;
    return 0;
}

// #pragma pack(4)
struct st1 
{
    char a ;
    int  b ;
    short c ;
};
 
struct st2
{
    short c ;
    char  a ;
    int   b ;
};
 
 int main()
 {
     cout<<"sizeof(st1) is "<<sizeof(st1)<<endl;
     cout<<"sizeof(st2) is "<<sizeof(st2)<<endl;
     return 0 ;
 }