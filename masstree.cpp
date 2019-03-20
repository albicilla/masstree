//
// Created by 拓真 on 2018/11/12.
//

#include "masstree.h"









int
main(int argc, char *argv[])
{
    init_root();



    vector<int> vec;

    data_root=(DATA*)calloc(1,sizeof(DATA));
    data_root->next=data_root;
    data_root->prev=data_root;

    //挿入するデータの数
    const int snum=112;

    //TODO
    for(int i=1;i<snum;i++){
        datatable[i]=DATA(i,i*3);
        //insert_dataset_data(i);
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
        }
    }
    auto curr_time = std::chrono::system_clock::now();
    auto elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(curr_time - start_time).count();

    cout<<"elapased time sort ="<<elapsed/(double)snum<<"ms"<<endl;


    reverse(vec.begin(),vec.end());

    init_root();

    for(auto itr:vec){
        insert(itr,NULL);
    }

    //開始時刻
    start_time = std::chrono::system_clock::now();

    for(auto itr:vec){
        //cout<<"こいつ->"<<itr<<"を見つけてくれ!"<<endl;
        if(!search_core(itr)){
            cout<<itr<<"が見つからなかったよ.."<<endl;
        }
    }

    curr_time = std::chrono::system_clock::now();
    elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(curr_time - start_time).count();

    cout<<"elapased time reverse ="<<elapsed/(double)snum<<"ms"<<endl;



    shuffle(vec.begin(),vec.end(),get_rand_mt);

    for(auto itr:vec){
        insert(itr,NULL);
    }

    //開始時刻
    start_time = std::chrono::system_clock::now();
    for(auto itr:vec){
        //cout<<"こいつ->"<<itr<<"を見つけてくれ!"<<endl;
        if(!search_core(itr)){
            cout<<itr<<"が見つからなかったよ.."<<endl;
        }
    }
    curr_time = std::chrono::system_clock::now();
    elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(curr_time - start_time).count();
    cout<<"elapased time shuffle ="<<elapsed/(double)snum<<"ms"<<endl;








    return 0;
}
