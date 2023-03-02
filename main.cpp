#include <QCoreApplication>
#include <iostream>
#include <fstream>
#include <string.h>
#include <direct.h>

#include <stdlib.h>
#include <stdio.h>
#include <iomanip>

#include <windows.h>
#include <math.h>
//#include <libloaderapi.h>

using namespace std;


static const char *ponit[]={"\x20", "\xA8\x87", "\xA8\x86", "\xA8\x84", "\xA8\x83", "\xA8\x80"};////  ▏▎▍▊█

//int per 百分比范围（0 ~ 100）

void progress_bar(int per,int whole)
{
    int i=0;
    int num0=0;

    printf("\r  [");
    //空格占多少
    num0  = (whole-per)/(whole/20);

    while(per>(whole/20)) //完整单元格
    {
        printf("%s", ponit[5]);
        per -= whole/20;
    }

    if((per)&&(per/(whole/20/5)))  //不完整的单元格
    {
        printf("%s", ponit[i]);
    }

    for(i=0;i<num0;i++) //填充空格
    {
        printf("%s", ponit[0]);
    }

    printf("] %2d%% %d %d", per/whole,per,whole);

    if(per == whole)
    {
        printf("\n");
    }

}

void DoProgress(int t, int n) {
    putchar('[');
    for (int i = 0; i < n; i++) {
        putchar(i < t ? '>' : ' '); // 输出> 或者 ' '
    }
    putchar(']');
    printf("%3d%%",(int)((double(t)/n) *100));

    // 光标回退，实现刷新
    for (int i = 0; i != n + 6; i++) {
        putchar('\b');
    }

}


char convertHexChart(char ch)
{
    if((ch >= '0') && (ch <= '9'))
                return ch-0x30;  // 0x30 对应 ‘0’
    else if((ch >= 'A') && (ch <= 'F'))
                return ch-'A'+10;
    else if((ch >= 'a') && (ch <= 'f'))
                return ch-'a'+10;
    else return ch-ch;//不在0-f范围内的会发送成0

}

char* getcurpath(){
    char* path;
    path = getcwd(NULL,0);
     //   cout<<"path is "<<path<<endl;
    if(path == NULL){
        cout<<"get path error!"<<endl;
    }
    else{

    }
        return path;
}





string getExepath(){
    char szFilePath[MAX_PATH+1]={0};
    GetModuleFileNameA(NULL,szFilePath,MAX_PATH);
    (strrchr(szFilePath,'\\'))[0]=0;
    string path = szFilePath;
    return path;
}


int main1(int argc,char *argv[]){

    if(argc!=3){
        cout<<"usage:gen_binfile.exe data(DEC) size(MB)"<<endl;
        return 0;
    }

//    char* cur_path=getcurpath();
//    cout<<cur_path<<endl;
    string ExePath=getExepath()+"\\a.out";
  //  cout<<"exe_path "<<ExePath<<endl;
    const char* fname = ExePath.data();

   // cout<<fname<<endl;
    ofstream fout(fname,ios::binary);

   // char data=atoi(argv[1]);
   // char data=convertHexChart(argv[1][0])*16+convertHexChart(argv[1][1]);
    uint64_t  data=stoull(argv[1]);
    //string data=argv[1];
      cout<<"data is "<<data<<endl;
    printf("data is 0x%0llx\n",data);

    long double size = stold(argv[2])*1024*1024; //size MByte to Byte
    //vector<char> chr1;
    printf("file size is %0.6fGB(%0.2fMB)\n",stoi(argv[2])/1024.0,stof(argv[2])/1.0);

    cout << "begin to write data" << endl;
    cout<<"output file path is "<<fname<<endl;
    cout<<"Please wait a minute ... "<<endl;
    long long i=0;
    while(i<size){
        if((data<=255)){ //uint8_t
              if(data>0){
              fout.write((char *)&data,sizeof(uint8_t));//地址为char类型。
              i++;
            //  cout<<data<<endl;
              }
        }
        else if((data>=256)&&(data<=65535)){//uint16_t
            for(int j=1;j>=0;j--){//uint16_t
              uint8_t u8_data=data>>(j*8);
              if(u8_data>0){
              fout.write((char *)&u8_data,sizeof(uint8_t));//地址为char类型。
              i++;
              }
            }
        }
        else if(data<=4294967295){ //uint32_t
            for(int j=3;j>=0;j--){
              uint8_t u8_data=data>>(j*8);
              if(u8_data>0){
              fout.write((char *)&u8_data,sizeof(uint8_t));
              i++;
              }
            }
        }
        else if(data<pow(2,64)){
            for(int j=7;j>=0;j--){   //uint64_t
              uint8_t u8_data=data>>(j*8);
            //  printf("data is 0x%x\n",u8_data);
              if(u8_data>0){
              fout.write((char *)&u8_data,sizeof(uint8_t));
              i++;
              }
            }
        }
    }

    cout<<"write data down "<<endl;

    fout.close();

    return 0;
}

int main(int argc,char *argv[]){  //char *argv[]指针数组

    // cout<<argc<<endl;
    uint64_t str;
    sscanf(*(argv+1),"%0llx",&str);
   // cout<<strlen(argv[1])<<endl;
     int byte_num=(strlen(argv[1])+1)/2;
    printf("data is 0x%0llx\n",str);
  //  uint64_t data=stoll(*(argv+1));
    if(argc!=3){
         cout<<"usage:gen_binfile.exe data(DEC) size(MB)"<<endl;
         return 0;
    }
    string ExePath=getExepath()+"\\a.out";
    const char* fname = ExePath.data();

    ofstream fout(fname,ios::binary);

    long double size = stold(argv[2])*1024*1024; //size MByte to Byte
    printf("file size is %0.6fGB(%0.2fMB)\n",stoi(argv[2])/1024.0,stof(argv[2])/1.0);

    cout << "begin to write data" << endl;
    cout<<"output file path is "<<fname<<endl;
    cout<<"Please wait a minute ... "<<endl;

    long long i=0;
    while(i<size){
         for(int j=byte_num-1;j>=0;j--){
            uint8_t u8_data=str>>(j*8);
            fout.write((char *)&u8_data,sizeof(uint8_t));
            i++;
         }
      }

    cout<<"write data down "<<endl;

    fout.close();

     return 0;

}

