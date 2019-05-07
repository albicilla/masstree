#pragma once


const int NUM_TESTS=6;


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

//キーの比較関数のテスト
bool myCompare_test(){
    string strn[]={"1","2","33","1111111111","111111112","111111113"};


    vector<string> ret;
    //vecはuint64_tの配列で著されたキー
    vector<uint64_t> vec;
    for(int i=0;i<6;i++){
        //key_stringをuint64_t配列に変換
        ret=eight_partition(strn[i]);

        for(int i=0;i<(int)ret.size();i++){
            uint64_t key = DumpLetterCode(join_offset(ret[i]));
            vec.push_back(key);
        }
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

    string strn[]={"1","2","33","1111111111","111111112","111111113","11111111223"};

    //レイヤー1に集合させるテスト
    string str2[]={"aa","bb","cc","dd","ee","ff","gg","h"};
    vector<string> strs=make_keys();

    DATA data[8] ={ DATA(1,5), DATA(2,4), DATA(3,3), DATA(4,2),DATA(11,5),DATA(11,6),DATA(12,6),DATA(13,6)};
    for(int i=0;i<NUM_TESTS;i++){
        //
        //key_stringをuint64_t配列に変換
        vector<string> ret=eight_partition(strn[i]);

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
            cout<<"挿入するキー="<<uint64toLetter(itr)<<endl;
            remind[i].push_back(itr);
        }



        masstree_insert(vec,&data[i],0,NULL);

        cout<<"finish insert"<<endl;
    }


    cout<<"finish input"<<endl;


    //サンプルインプットのチェック
    for(int i=0;i<NUM_TESTS;i++){


        cout<<"こいつ->";
        for(auto itr:remind[i]){
            cout<<bitset<64>(itr)<<" ";
        }
        cout<<"を見つけてくれ!"<<endl;
        if(!search_core(remind[i])){
            cout<<"見つからなかったよ.."<<endl;
            return false;
        }
    }


    cout<<"finish"<<endl;


}