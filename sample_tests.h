#pragma once
#include "utils.h"
#include <fstream>   // ifstream, ofstream
#include <sstream>   // istringstream

const int NUM_TESTS=1000000;


vector<string> make_keys(){
    string str="";
    vector<string>ret;
    for(int i=0;i<10;i++){
        str="aaaaaaaa";
        str+='a'+i;
        ret.push_back(str);
    }
    return ret;
}

bool test_DumpLetterCode(){

    vector<string> strs;
    strs.push_back("30");
    cout<<(DumpLetterCode("30"))<<endl;

    cout<<uint64toLetter(13104)<<endl;
    for(int i=0;i<strs.size();i++){
        if(strs[i]!=uint64toLetter(DumpLetterCode(strs[i]))){
            cerr<<uint64toLetter(DumpLetterCode(strs[i]))<<endl;
            ERR;
        };
    }
    return true;

}

//キーの比較関数のテスト
bool myCompare_test(){
    string strn[]={"1","2","30","1111111111","111111112","111111113"};


    vector<string> ret;
    //vecはuint64_tの配列で著されたキー
    vector<uint64_t> vec;
    for(int i=0;i<6;i++){
        //key_stringをuint64_t配列に変換
        ret=eight_partition(strn[i]);


        for(int i=0;i<(int)ret.size();i++){
            //cout<<"ret[i]="<<i<<" "<<ret[i]<<endl;

            uint64_t key = DumpLetterCode(join_offset(ret[i]));

            vec.push_back(key);

        }
        reverse(vec.begin(),vec.end());
    }

    for(auto itr: vec){
        cout<<uint64toLetter(itr)<<" ";
    }
    cout<<endl;

    sort(vec.begin(),vec.end(),myCompare());


    for(auto itr: vec){
        cout<<uint64toLetter(itr)<<" ";
    }
    cout<<endl;

    for(auto itr: vec){
        cout<<uint64toLetter(itr)<<" ";
        cout<<bitset<64>(itr)<<endl;
    }

    return true;
}

bool sample_test(){

    //初期化
    masstree_init_root();



    //挿入したキーを記憶するためのvector
    vector<vector<uint64_t>> remind(NUM_TESTS);

    //とりあえずサンプルインプット
    string str[]={"a","b","ddd","aaaaaaaaaa","aaaaaaaab","aaaaaaaac","aaaaaaaabbc","aaaaaaabc"};

    //string strn[]={"1","2","33","1111111111","111111112","111111113","11111111223"};

    string s;
    vector<string> str_vec;

    ifstream ifs("/Users/albicilla/CLionProjects/masstree/out.txt");

    string reading_line_buffer;

    cout<<"reading..."<<endl;

    if(ifs.fail()){
        ERR;
    }

    while(getline(ifs,reading_line_buffer))
    {


        //cout<<reading_line_buffer<<endl;
        str_vec.push_back(reading_line_buffer);

    }
    cout<<"fin read"<<endl;



    //レイヤー1に集合させるテスト
    string str2[]={"aa","bb","cc","dd","ee","ff","gg","h"};
    vector<string> strs=make_keys();

    DATA data[8] ={ DATA(1,5), DATA(2,4), DATA(3,3), DATA(4,2),DATA(11,5),DATA(11,6),DATA(12,6),DATA(13,6)};
    for(int i=0;i<NUM_TESTS;i++){
        //
        //key_stringをuint64_t配列に変換
        vector<string> ret=eight_partition(str_vec[i]);

        //vecはuint64_tの配列で著されたキー
        vector<uint64_t> vec;
        for(int i=0;i<(int)ret.size();i++){
            uint64_t key = DumpLetterCode(join_offset(ret[i]));
            vec.push_back(key);
        }
        //レイヤーが1
        //assert(vec.size()==1);



        //TODO サンプルテストは　data適当
        //uint64_t key=vec[0];
        for(auto itr:vec){
            //cout<<"挿入するキー="<<uint64toLetter(itr)<<endl;
            remind[i].push_back(itr);
        }



        masstree_insert(vec,&data[0],0,Dummy);
        //cout<<"after insert=";print_tree(Root);

        if(i%100000==0)cout<<"finish insert "<<i<<endl;
    }


    cout<<"finish input"<<endl;


    //サンプルインプットのチェック
    for(int i=0;i<NUM_TESTS;i++){


        //cout<<"こいつ->";
        for(auto itr:remind[i]){
            //cout<<bitset<64>(itr)<<" ";
        }
        //cout<<"を見つけてくれ!"<<endl;
        if(!search_core(remind[i])){
            //cout<<"見つからなかったよ.."<<endl;
            return false;
        }else{
            cout<<"find! "<<i<<endl;
        }
    }


    cout<<"finish"<<endl;


}

/*
111111111
222222222
333333333
444444444
555555555
666666666
777777777
0

333333333
444444444
555555555
666666666
777777777
111111111
222222222
0

dkQirBk33E
U44pUDiIL0
8rvwHdWQ7Y
6vpFvhQvz1
XAwA9Djdxh
4ZT4QLuFR9
rIzHo3vC2b
0

AAAAAAAAAA
BBBBBBBBBB
CCCCCCCCCC
DDDDDDDDDD
EEEEEEEEEE
FFFFFFFFFF
GGGGGGGGGG
0

AAAaAAAAA0
BBBbBBBBB0
CCCcCCCCCC
DDDdDDDDDD
EEEeEEEEEE
FFFfFFFFFF
GGGgGGGGGG
0

555555555
333333333
222222222
111111111
444444444
000000000
666666666
0

5555555
3333333
2222222
1111111
4444444
0000000
6666666
0


dkQirBk33E
U44pUDiIL0
8rvwHdWQ7Y
6vpFvhQvz1
XAwA9Djdxh
4ZT4QLuFR9
rIzHo3vC2b
OUcQIyKiYm
FzgmDCLZpZ
0O3Qx7lob2
GpBXHPOBIX
F5VFvf0eMd
CTZgu8HC1t
7kWtMTWmyr
0

dkQirBk33E
U44pUDiIL0
8rvwHdWQ7Y
6vpFvhQvz1
XAwA9Djdxh
4ZT4QLuFR9
rIzHo3vC2b
0


dkQirBk33E
U44pUDiIL0
8rvwHdWQ7Y
6vpFvhQvz1
XAwA9Djdxh
4ZT4QLuFR9
rIzHo3vC2b
OUcQIyKiYm
FzgmDCLZpZ
0O3Qx7lob2
GpBXHPOBIX
F5VFvf0eMd
CTZgu8HC1t
7kWtMTWmyr
6N1azIQwYJ
DaE3nGYCZH
WN6uzXLWQJ
okOrYsUZD0
0w5kH8FIt2
KFi934ivya
jcr4EbOmTS
u0AwFP91H3
BsHj6Z7E2g
ePZ0M1Waou
PBWL7hou6T
WCACewgWoa
ghexGoSkkn
IkBaseqk32
R95q84He2y
0

1
2
3
4
5
6
7
8
9
10
11
12
13
14
15
16
17
18
19
20
21
22
23
24
25
0

 */