#pragma once


const int NUM_TESTS=5;

bool sample_test(){

    //初期化
    masstree_init_root();

    //挿入したキーを記憶するためのvector
    vector<vector<uint64_t>> remind(NUM_TESTS);

    //とりあえずサンプルインプット
    string str[]={"a","b","ddd","aaaaaaaaa","aaaaaaaab"};

    DATA data[5] ={ DATA(1,5), DATA(2,4), DATA(3,3), DATA(4,2),DATA(11,5)};
    for(int i=0;i<NUM_TESTS;i++){
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


        //TODO サンプルテストは　とりあえずlayer一つ data適当
        uint64_t key=vec[0];
        for(auto itr:vec)remind[i].push_back(itr);

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