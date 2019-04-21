#include <iostream>
#include <sstream>
//#include "bptree.h"
//#include "bptree_test.h"
#include "string_slice.h"
#include "masstree.h"
using namespace std;




signed main(){
    //test_all();

    test_string_slice();

    //初期化
    masstree_init_root();

    //挿入したキーを記憶するためのvector
    vector<uint64_t> remind;

    //とりあえずサンプルインプット
    string str[]={"a","b","ddd"};
    for(int i=0;i<3;i++){
        //
        //key_stringをuint64_t配列に変換
        vector<string> ret=eight_partition(str[i]);
        //vecはuint64_tの配列で著されたキー
        vector<uint64_t> vec;
        for(int i=0;i<(int)ret.size();i++){
            uint64_t key = DumpLetterCode(join_offset(ret[i]));
            vec.push_back(key);
        }
        //整数一つにつき一つのトライ木のレイヤーができる。
        int layer_num=vec.size();

        cout<<"vec[0]="<<vec[0]<<endl;

        //TODO とりあえずlayor一つ data適当
        uint64_t key=vec[0];
        remind.push_back(key);
        DATA data = DATA(114,514);
        masstree_insert(vec,&data);
    }


    //サンプルインプットのチェック
    for(int i=0;i<3;i++){

        cout<<"こいつ->"<<remind[i]<<"を見つけてくれ!"<<endl;
        if(!search_core(remind[i])){
            cout<<remind[i]<<"が見つからなかったよ.."<<endl;
            return false;
        }
    }


    cout<<"finish"<<endl;



}
