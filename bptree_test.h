//
// Created by albicilla on 2019/04/21.
//

#pragma once

bool test_bptree_normal(){
    init_root();
    vector<int> vec;

    data_root=(DATA*)calloc(1,sizeof(DATA));
    data_root->next=data_root;
    data_root->prev=data_root;

    //挿入するデータの数
    const int snum=112;

    for(int i=1;i<snum;i++){
        datatable[i]=DATA(i,i*3);
        vec.push_back(i);
    }

    mt19937_64 get_rand_mt;
    vector<int> svec=vec;

    for(auto itr:vec){
        insert(itr,&datatable[itr]);
    }

    //print_tree(Root);


    //開始時刻
    auto start_time =std::chrono::system_clock::now();

    for(auto itr:vec){
        //cout<<"こいつ->"<<itr<<"を見つけてくれ!"<<endl;
        if(!search_core(itr)){
            cout<<itr<<"が見つからなかったよ.."<<endl;
            return false;
        }
    }
    auto curr_time = std::chrono::system_clock::now();
    auto elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(curr_time - start_time).count();

    cout<<"elapased time sort ="<<elapsed/(double)snum<<"ms"<<endl;

    cout<<"finish normal test..."<<endl;
    return true;

}

bool test_bptree_rev(){
    init_root();

    vector<int> vec;

    data_root=(DATA*)calloc(1,sizeof(DATA));
    data_root->next=data_root;
    data_root->prev=data_root;

    //挿入するデータの数
    const int snum=112;

    for(int i=1;i<snum;i++){
        datatable[i]=DATA(i,i*3);
        vec.push_back(i);
    }

    mt19937_64 get_rand_mt;
    vector<int> svec=vec;

    for(auto itr:vec){
        insert(itr,&datatable[itr]);
    }
    reverse(vec.begin(),vec.end());


    //開始時刻
    auto start_time = std::chrono::system_clock::now();

    for(auto itr:vec){
        //cout<<"こいつ->"<<itr<<"を見つけてくれ!"<<endl;
        if(!search_core(itr)){
            cout<<itr<<"が見つからなかったよ.."<<endl;
            return false;
        }
    }

    auto curr_time = std::chrono::system_clock::now();
    auto elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(curr_time - start_time).count();

    cout<<"elapased time reverse ="<<elapsed/(double)snum<<"ms"<<endl;
    cout<<"finish reverse test..."<<endl;
    return true;
}

bool test_bptree_shuffle(){

    init_root();

    vector<int> vec;

    data_root=(DATA*)calloc(1,sizeof(DATA));
    data_root->next=data_root;
    data_root->prev=data_root;

    //挿入するデータの数
    const int snum=112;

    for(int i=1;i<snum;i++){
        datatable[i]=DATA(i,i*3);
        vec.push_back(i);
    }

    mt19937_64 get_rand_mt;
    vector<int> svec=vec;


    shuffle(vec.begin(),vec.end(),get_rand_mt);

    for(auto itr:vec){
        insert(itr,&datatable[itr]);
    }

    //開始時刻
    auto start_time = std::chrono::system_clock::now();
    for(auto itr:vec){
        //cout<<"こいつ->"<<itr<<"を見つけてくれ!"<<endl;
        if(!search_core(itr)){
            cout<<itr<<"が見つからなかったよ.."<<endl;
            return false;
        }
    }
    auto curr_time = std::chrono::system_clock::now();
    auto elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(curr_time - start_time).count();
    cout<<"elapased time shuffle ="<<elapsed/(double)snum<<"ms"<<endl;
    cout<<"finish shuffle test..."<<endl;
    return true;
}


bool test_all(){
    if(test_bptree_normal()&&test_bptree_rev()&&test_bptree_shuffle()){
        cout<<"==== OK!  your bptree work correctly! ====="<<endl;
    }else{
        cout<<":("<<endl;
    }
}

