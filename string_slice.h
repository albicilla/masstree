#pragma once

#include <iostream>
#include <sstream>
#include <vector>

using namespace std;



inline uint64_t toInt(string s) { uint64_t v; istringstream sin(s); sin >> v; return v; }


//TODO 10は適当
const int MAX_LAYER_NUM=10;

uint64_t DumpLetterCode(string str){

    vector<string> strs;
    for(auto itr:str){
        strs.push_back(to_string((unsigned char)itr));
    }


    uint64_t ret=0;
    for(int i=strs.size()-1;i>=0;i--){
        //cout<<"strs="<<strs[i]<<endl;
        ret+=(1LL<<(i*8))*toInt(strs[strs.size()-1-i]); //8byte区切り
    }
    return (uint64_t)ret;
}

string uint64toLetter(uint64_t a){
    vector<string> vec;
    for(int i=0;i<8;i++){

        //cout<<"a&0xff="<<((a>>(i*8))&(0xff))<<endl;
        char asciiChar=(char)((a>>(i*8))&0xff);
        //cout<<"asc="<<asciiChar<<endl;
        string temp={asciiChar};
        vec.push_back(temp);
    }

    string ret;
    for(auto itr:vec){
        ret+=itr;
    }
    return ret;
}


//test用strings
string strs[]={"a","b","c","bb","bbbbb","abcdefghijk"};


//string をうけとってオフセット付きで返す関数
string join_offset(string s){
    string ret="";
    int a=s.size();
    for(int i=0;i<(8-(a%8))%8;i++){
        ret+='-';
    }
    ret+=s;
    return ret;
}

//ながさが8文字以上の文字列を8文字ずつにする。
vector<string> eight_partition(string s){
    vector<string> ret;
    string temp="";
    for(int i=0;i<(int)s.size();i++){
        temp+=s[i];
        if((i!=0&&(i+1)%8==0)||i==(int)s.size()-1){
            ret.push_back(temp);
            temp="";

        }
    }

    return ret;
}

bool test_string_slice(){
    //keysliceは char 1文字で1byte = 8bit = 2^8 = 256通り トライの階層ごとに8文字分
    // 'a' には 0x61=97　が割り当てられてる
    //64bit整数=8byte
    //1layor 64bit
    //オフセットは'-'


    for(auto itr:strs){
        cout<<"itr="<<itr<<endl;

        vector<string> ret=eight_partition(itr);

        //for(auto ii:ret)cout<<"ii="<<ii<<endl;
        vector<uint64_t > vec;
        for(int i=0;i<(int)ret.size();i++){
            uint64_t key = DumpLetterCode(join_offset(ret[i]));
            vec.push_back(key);
        }
        //cout<<key<<endl;
       // printf("%#X ", (unsigned char)key);

        for(int i=0;i<(int)vec.size();i++){
            string ret=uint64toLetter(vec[i]);
            cout<<ret<<endl;
        }
    }
    return true;


}