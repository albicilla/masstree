//
// Created by albicilla on 2019/03/26.
//

#pragma once

#include "debug.h"
#include "utils.h"

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <assert.h>
#include <strings.h>
#include <math.h>
#include <iostream>
#include <vector>
#include <sys/time.h>
#include <random>
#include <algorithm>
#include <set>
#include <chrono>
#include <iomanip>
#include <bitset>
#define dump(x) cout<<#x<<"="<<x<<endl





#define N 4

using std::cout;
using std::endl;
using std::cerr;
using std::vector;
using std::mt19937_64;

typedef struct _DATA {
    _DATA(){};
    _DATA(uint64_t key,int val){this->key=key,this->val=val;};
    uint64_t key;
    int val;
    struct _DATA *next;
    struct _DATA *prev;
} DATA;

typedef struct _link_or_value{
    void* data;
    void* link;
}link_or_value;


typedef struct _NODE {
    bool isLeaf;
    bool isRoot=0;
    struct _NODE *chi[N];
    uint64_t key[N-1];
    int nkey;
    struct _NODE *parent;
    link_or_value lv[N];
} NODE;

typedef struct _TEMP {
    bool isLeaf;
    NODE *chi[N+1]; // for internal split (for leaf, only N is enough)
    uint64_t key[N]; // for leaf split
    int nkey;
    link_or_value lv[N+1];

} TEMP;


NODE *Root = NULL;
NODE *Dummy = NULL;

//個人的なnew node
NODE *
new_bptree_node()
{
    NODE *new_node;

    if(!(new_node=(NODE *)calloc(1,sizeof(NODE))))ERR;

    new_node->parent = NULL;
    /*new_node->next     = NULL;*/
    new_node->isLeaf = false;
    new_node->nkey = 0;

    new_node->isRoot = 0;


    return new_node;
}



//layerのrootノードのparentにこいつを指定してフラグにする
NODE *NewLayerRoot=new_bptree_node();



void masstree_insert(vector<uint64_t>& key_vec, DATA *data,int layer_now,NODE*& root);

void
print_tree_core(NODE *n)
{
    printf("[");
    for (int i = 0; i < n->nkey; i++) {
        if (!n->isLeaf) print_tree_core(n->chi[i]);
        cout<<uint64toLetter(n->key[i]);
        if (i != n->nkey-1 && n->isLeaf) putchar(' ');
    }
    if (!n->isLeaf) print_tree_core(n->chi[n->nkey]);
    printf("]");
}



void
print_tree(NODE *node)
{
    print_tree_core(node);
    printf("\n"); fflush(stdout);
}


//insertの時のようのノード生成
TEMP *
create_new_node(){
    TEMP *node;
    if (!(node = (TEMP *)calloc(1, sizeof(TEMP)))) ERR;
    node->isLeaf=true;
    node->nkey=0;
    return node;
}



TEMP *
new_bptree_temp()
{
    TEMP *new_node;

    if(!(new_node=(TEMP *)calloc(1,sizeof(TEMP))))ERR;
    /*new_node->next     = NULL;*/
    new_node->isLeaf = false;
    new_node->nkey = 0;


    return new_node;
}


NODE *
alloc_leaf(NODE *parent)
{
    NODE *node;
    if (!(node = (NODE *)calloc(1, sizeof(NODE)))) ERR;
    node->isLeaf = true;
    node->parent = parent;
    node->nkey = 0;

    return node;
}

NODE * alloc_newlayer_leaf(NODE *parent)
{
    NODE *node;
    if(!(node=(NODE *)calloc(1,sizeof(NODE))))ERR;
    node->isLeaf=true;
    node->isRoot=true;
    node->parent=parent;
    node->nkey=0;
    return node;
}

NODE * alloc_newlayer_leaf(TEMP *parent)
{
    NODE *node;
    if(!(node=(NODE *)calloc(1,sizeof(NODE))))ERR;
    node->isLeaf=true;
    node->isRoot=true;
    //node->parent=parent;
    node->nkey=0;
    return node;
}


NODE *
alloc_root(NODE *left, uint64_t rs_key, NODE *right,NODE *parent)
{
    NODE *node;

    if (!(node = (NODE *)calloc(1, sizeof(NODE)))) ERR;
    node->parent = parent;
    node->isLeaf = false;
    node->key[0] = rs_key;
    node->chi[0] = left;
    node->chi[1] = right;


    node->nkey = 1;

    left->parent=node;
    right->parent=node;

    //分割ノードを繋げるポインタ
    left->chi[N-1]=right;

    return node;
}

NODE *
find_leaf(NODE *node, uint64_t key)
{
    int kid;

    if (node->isLeaf) return node;

    //print_tree(node);

    for (kid = 0; kid < node->nkey; kid++) {
        //cout<<"find leaf key="<<uint64toLetter(node->key[kid])<<endl;

        if (key < node->key[kid]) break;
    }

    return find_leaf(node->chi[kid], key);
}

NODE *
insert_in_node(NODE *p,NODE *c,uint64_t key,int index){
    int i;
    for(i=p->nkey;i>index;--i){
        p->key[i]=p->key[i-1];
        p->chi[i+1]=p->chi[i];
    }
    p->key[index]=key;

    p->chi[index+1]=c;
    p->nkey+=1;
    c->parent=p;

    // cout<<"insert_in_node"<<endl;
    // for(int i=0;i<p->nkey;i++){
    // 	cout<<p->key[i]<<endl;
    // }

    return p;
}
TEMP *
insert_in_node_temp(TEMP *p,NODE *c,uint64_t key,int index){
    int i;
    for(i=p->nkey;i>index;--i){
        p->key[i]=p->key[i-1];
        p->chi[i+1]=p->chi[i];
    }
    p->key[index]=key;

    p->chi[index+1]=c;
    p->nkey+=1;
    c->parent=(NODE*)p;

    return p;
}

void
insert_in_parent(NODE *node, uint64_t key, NODE *node_dash, DATA *data,NODE*& r){

    if (node==Root){
        //cout<<"node=";print_tree(node);
        //cout<<"node_dash=";print_tree(node_dash);

        Root=alloc_root(node,key,node_dash,Root);

        //cout<<"r=";print_tree(r);

        //cout<<"r->key[0]"<<uint64toLetter(r->key[0])<<endl;

        if(r->chi[0]==NULL)ERR;
        if(r->chi[1]==NULL)ERR;
        return ;
    }

    //cout<<"ipt";
    //print_tree(r);
    //ローカルルート
    if(node->isRoot){

        
        //cout<<"local node";print_tree(node);
        //cout<<"local node_dash";print_tree(node_dash);

        r=alloc_root(node,key,node_dash,NewLayerRoot);
        //cout<<"local tree";print_tree(r);

        //cout<<"local r->key[0]"<<uint64toLetter(r->key[0])<<endl;

        if(r->chi[0]==NULL)ERR;
        if(r->chi[1]==NULL)ERR;


        return ;
    }




    NODE *parent=node->parent;

    int index=0,i;

    while(parent->key[index]<key && index<parent->nkey)index++;

    if(parent->nkey<N-1){
        //cout<<"call parent insert_in_node"<<endl;
        insert_in_node(parent,node_dash,key,index);
        return ;
    }else{
        //親がいっぱいなので再帰処理で親ブロックを追加
        // dump(parent->nkey);
        assert(parent->nkey==N-1);


        //copy p to t
        TEMP *t=new_bptree_temp();
        for(i=0;i<N-1;i++){
            t->key[i]=parent->key[i];
            t->chi[i]=parent->chi[i];
            t->nkey++;
        }
        assert(parent->nkey==t->nkey);
        t->chi[i]=parent->chi[i];


        //insert
        insert_in_node_temp(t,node_dash,key,index);
        assert((parent->nkey+1==t->nkey));

        int mid_pos=(N+1)/2;

        //erase
        for(i=0;i<N;i++){
            parent->key[i]=0;
            parent->chi[i]=NULL;
        }
        parent->chi[i]=NULL;
        parent->nkey=0;

        //create p'
        NODE *pp = new_bptree_node();

        //copy t into p  left
        for(i=0;i<mid_pos;i++){
            parent->key[i]=t->key[i];
            parent->chi[i]=t->chi[i];
            t->chi[i]->parent=parent;


            // //dump(t->key[i]);
            parent->nkey++;
        }
        assert(parent->nkey==mid_pos);
        parent->chi[i]=t->chi[i];
        t->chi[i]->parent=parent;

        uint64_t key_temp=t->key[mid_pos];
        //copy t into p' right
        for(i=mid_pos+1;i<t->nkey;i++){
            pp->chi[i-(mid_pos+1)]=t->chi[i];
            pp->key[i-(mid_pos+1)]=t->key[i];
            t->chi[i]->parent=pp;
            pp->nkey++;
        }
        pp->chi[i-(mid_pos+1)]=t->chi[i];
        t->chi[i]->parent=pp;

        return insert_in_parent(parent,key_temp,pp,data,r);

    }
}
//temp用の要素挿入関数
TEMP *
insert_in_leaf_temp(TEMP *leaf,vector<uint64_t>& key_vec,DATA *data,int layer_now){
    int i;


    uint64_t key=key_vec[layer_now];
    int layer_num=key_vec.size()-1;





    if(key<leaf->key[0]){



        for(i=leaf->nkey;i>0;i--){
            leaf->lv[i]=leaf->lv[i-1];
            leaf->key[i]=leaf->key[i-1];
        }
        leaf->key[0]=key;
        leaf->lv[0].link=NULL;
        //このレイヤーで終わりならデータをそうでないなら次のBptreeのRootへのポインタ
        if(layer_num==layer_now){
            leaf->lv[0].data = (void*)data;
        }
        else {
            //次のレイヤーのbptreeを作ってnext rootがそこを指すようにする。
            if(leaf->lv[0].link==NULL) {
                //cout<<"create new layer"<<endl;

                //leaf->lv[0].link = alloc_leaf(leaf);
                leaf->lv[0].link = alloc_newlayer_leaf(leaf);


            }else{
                // no commmon prefix
                ERR;
            }
            NODE* nd=(NODE*)leaf->lv[0].link;

            masstree_insert(key_vec,data,layer_now+1,nd);

        }
    }
    else{


        for(i = leaf->nkey-1;i>=0;--i){
            if(key > leaf->key[i]){
                break;
            }else{
                leaf->key[i+1]=leaf->key[i];
                leaf->lv[i+1]=leaf->lv[i];
            }
        }

        leaf->lv[i+1].link=NULL;

        leaf->key[i+1]=key;
        //leaf->lv[i+1].data=(void * )data;
        //このレイヤーで終わりならデータをそうでないなら次のBptreeのRootへのポインタ
        if(layer_num==layer_now){
            leaf->lv[i+1].data = (void*)data;
        }
        else {
            //次のレイヤーのbptreeを作ってnext rootがそこを指すようにする。
            if(leaf->lv[i+1].link==NULL) {
                //cout<<"create new layer"<<endl;

                //leaf->lv[i+1].link = alloc_leaf(leaf);
                leaf->lv[i+1].link = alloc_newlayer_leaf(leaf);

            }else{
                //no common prefix
                ERR;
            }
            NODE* nd=(NODE*)leaf->lv[i+1].link;

            masstree_insert(key_vec,data,layer_now+1,nd);

        }
    }
    leaf->nkey++;

    return leaf;

}

NODE *
insert_in_leaf(NODE *leaf, vector<uint64_t>& key_vec, DATA *data,int layer_now,bool dup)
{
    uint64_t key=key_vec[layer_now];

    int layer_num=key_vec.size()-1;



    for(int i=0;i<leaf->nkey;i++){
        //cout<<"bitset="<<bitset<64>(leaf->key[i])<<endl;
    }

    uint64_t i=0;
    if (key < leaf->key[0]) {
        for (i = leaf->nkey; i > 0; i--) {
            leaf->lv[i].data = leaf->lv[i-1].data ;
            leaf->lv[i].link = leaf->lv[i-1].link;
            leaf->key[i] = leaf->key[i-1] ;
        }
        leaf->key[0] = key;
        leaf->nkey++;

        leaf->lv[0].link=NULL;


        //cout<<"ここここ"<<endl;


        //このレイヤーで終わりならデータをそうでないなら次のBptreeのRootへのポインタ
        if(layer_num==layer_now){
            leaf->lv[0].data = (void*)data;
        }
        else {
            //次のレイヤーのbptreeを作ってnext rootがそこを指すようにする。
            if(leaf->lv[0].link==NULL) {
                //cout<<"create new layer"<<endl;

                leaf->lv[0].link = alloc_newlayer_leaf(leaf);
            }else {
                //no common prefix
                ERR;
            }
            NODE* nd =(NODE*)leaf->lv[0].link;
            masstree_insert(key_vec,data,layer_now+1,nd);
            leaf->lv[0].link=nd;

        }
    }
    else {

        if(!dup){
            for(i = leaf->nkey-1;i>=0;--i){
                if(key > leaf->key[i]){
                    break;
                }else{
                    leaf->key[i+1]=leaf->key[i];
                    leaf->lv[i+1].data=leaf->lv[i].data;
                    leaf->lv[i+1].link=leaf->lv[i].link;
                }
            }

            //cout<<"!dup====="<<i+1<<endl;

            leaf->lv[i+1].link=NULL;

            leaf->key[i+1]=key;
            leaf->nkey++;
        }else{
            for(i = leaf->nkey-1;i>=0;--i){
                if(key > leaf->key[i]){
                    break;
                }else{
//                    leaf->key[i+1]=leaf->key[i];
//                    leaf->lv[i+1].data=leaf->lv[i].data;
                }
            }

            cout<<"dup====="<<i<<endl;

            leaf->key[i+1]=key;
        }



        //dump(layer_num);dump(layer_now);
        //このレイヤーで終わりならデータをそうでないなら次のBptreeのRootへのポインタ
        if(layer_num==layer_now){
            leaf->lv[i+1].data = (void*)data;

            //DATA* ptr=(DATA*)(leaf->lv[i+1].data);
        }
        else
        {
            //次のレイヤーのbptreeを作ってnext rootがそこを指すようにする。
            if(leaf->lv[i+1].link==NULL){
                //cout<<"create new layer"<<endl;
                leaf->lv[i+1].link=alloc_newlayer_leaf(leaf);
            }else{
                //no common prefix
               // ERR;
            }
            NODE* nd =(NODE*)leaf->lv[i+1].link;

            //cout<<"before nd";
            //print_tree(nd);
            //cout<<endl;


            masstree_insert(key_vec,data,layer_now+1,nd);
            NODE* test=find_leaf((NODE*)leaf->lv[i+1].link,key_vec[layer_now]);
            //cout<<"チェエク====="<<test->nkey<<endl;

            //cout<<"after nd";
            //print_tree(nd);
            //cout<<"after lv[i+1]";
            //print_tree((NODE*)leaf->lv[i+1].link);
            //cout<<endl;


            /*
             * lvalue reference で(NODE*)leaf->lv[i+1].link=ndが無効になるので
             */
            leaf->lv[i+1].link = nd;
            //cout<<"after lv[i+1]";
            //print_tree((NODE*)leaf->lv[i+1].link);
            //cout<<endl;

        }


    }


    //cout<<"work correctly insert_in_leaf"<<endl;

    return leaf;
}

//小文字rootは次のrootノードへのポインタ
void
masstree_insert(vector<uint64_t>& key_vec, DATA *data,int layer_now,NODE*& root)
{

            //cout<<"key_vecのおおきさ"<<key_vec.size()<<endl;

            uint64_t key = key_vec[layer_now];
            NODE *leaf;

            //cout<<"processing letter="<<uint64toLetter(key)<<endl;



            if(root!=NULL){
                //cout<<"masstree_insertごとにroot"<<endl;
                //print_tree(root);


                //ここでroot = leafとなる
                leaf = find_leaf(root, key);


                //挿入しようとするleafノードに重複キーが含まれているか
                bool dup=false;
                for(int j=0;j<leaf->nkey;j++){
                    if(leaf->key[j]==key)dup=true;
                }
                //dump(leaf->nkey);
                if (leaf->nkey < (N - 1)||dup) {
                    //cout<<"call insert_in_leaf"<<endl;
                    insert_in_leaf(leaf, key_vec, data,layer_now,dup);
                } else {
                    TEMP *Temp = create_new_node();
                    for (int i = 0; i < N - 1; i++) {
                        Temp->key[i] = leaf->key[i];
                        Temp->lv[i] = leaf->lv[i];
                        Temp->nkey++;
                    }
                    insert_in_leaf_temp(Temp, key_vec, data,layer_now);

                    NODE *leaf_dash = alloc_leaf(NULL);

                    //clean leaf
                    for (int i = 0; i < N - 1; i++) {
                        leaf->lv[i].data = NULL;
                        leaf->lv[i].link = NULL;

                        leaf->key[i] = 0;
                    }
                    leaf->nkey = 0;


                    int mid_pos = (N - 1) / 2;
                    for (int i = 0; i <= mid_pos; i++) {
                        leaf->lv[i] = Temp->lv[i];
                        leaf->key[i] = Temp->key[i];
                        leaf->nkey++;
                    }
                    for (int i = mid_pos + 1; i < N; i++) {
                        leaf_dash->key[i - (mid_pos + 1)] = Temp->key[i];
                        leaf_dash->lv[i - (mid_pos + 1)] = Temp->lv[i];
                        leaf_dash->nkey++;
                    }

                    uint64_t key_dash = leaf_dash->key[0];




                    //cout<<"ready for insert_in_parent"<<endl;
                    //cout<<"root chk before"<<endl;
                    //print_tree(root);
                    insert_in_parent(leaf, key_dash, leaf_dash, data,root);
                    //cout<<"after"<<endl;
                    //print_tree(root);
                    //cout<<"fin";
                    //cout<<"work correctly insert_in_parent"<<endl;


                }
            }else{


                if (Root == NULL) {
                    // cout<<"呼び出し"<<endl;
                    leaf = alloc_leaf(NULL);
                    Root = leaf;
                } else {
                    leaf = find_leaf(Root, key);
                }



                //挿入しようとするleafノードに重複キーが含まれているか
                bool dup=false;
                for(int j=0;j<leaf->nkey;j++){
                    if(leaf->key[j]==key)dup=true;
                }

                //dump(leaf->key[0]);
                //dump(leaf->nkey);


                if (leaf->nkey < (N - 1)) {
                    //cout<<"call insert_in_leaf"<<endl;
                    insert_in_leaf(leaf, key_vec, data,layer_now,dup);
                } else {
                    TEMP *Temp = create_new_node();
                    for (int i = 0; i < N - 1; i++) {
                        Temp->key[i] = leaf->key[i];
                        Temp->lv[i] = leaf->lv[i];
                        Temp->nkey++;
                    }

                    //dump(Temp->nkey);
                    //ここで挿入してるんやなって
                    insert_in_leaf_temp(Temp, key_vec, data,layer_now);




                    NODE *leaf_dash = alloc_leaf(NULL);

                    //leaf_dash->lv[N - 1] = leaf->lv[N - 1];

                    //leaf->lv[N].link = leaf_dash;

                    //cout<<"Root";//print_tree(Root);


                    for (int i = 0; i < N - 1; i++) {
                        leaf->lv[i].data = NULL;
                        leaf->lv[i].link = NULL;
                        leaf->key[i] = 0;
                    }
                    leaf->nkey = 0;



                    int mid_pos = (N - 1) / 2;
                    for (int i = 0; i <= mid_pos; i++) {
                        leaf->lv[i] = Temp->lv[i];
                        leaf->key[i] = Temp->key[i];
                        leaf->nkey++;
                    }


                    for (int i = mid_pos + 1; i < N; i++) {
                        leaf_dash->key[i - (mid_pos + 1)] = Temp->key[i];
                        leaf_dash->lv[i - (mid_pos + 1)] = Temp->lv[i];
                        leaf_dash->nkey++;
                    }

                    uint64_t key_dash = leaf_dash->key[0];

                    //dleaf->parent =

                    //cout<<"Root split bf"<<endl;
                    //print_tree(Root);
                    //cout<<"Root split"<<endl;


                    //cout<<"leaf";
                    //print_tree(leaf);

                    //cout<<"leaf_dash";
                    //print_tree(leaf_dash);

                    //cout<<"key_dash="<<uint64toLetter(key_dash)<<endl;



                    insert_in_parent(leaf, key_dash, leaf_dash, data,Root);


                }
            }

}



void
masstree_init_root(void)
{
    Root = NULL;
    Dummy = NULL;
}

bool
search_core(const vector<uint64_t> keys)
{
    NODE *entry_point=Root;

    for(int lay=0;lay<keys.size();lay++){
        //dump(lay);
        uint64_t key=keys[lay];
        NODE *n = find_leaf(entry_point, key);

        //cout<<"entry_point";print_tree(entry_point);

        //dump(n->nkey+1);
        //cout<<"searching key="<<uint64toLetter(key)<<endl;
        //cout<<endl;
        for (int i = 0; i < n->nkey+1; i++) {
            //cout<<"          key="<<uint64toLetter(n->key[i])<<endl;

            if (n->key[i] == key ) {

                if(lay<(int)keys.size()-1){
                    if((NODE*)n->lv[i].link==NULL){
                        ERR;
                    }


                    entry_point = (NODE*)n->lv[i].link;


                    //cout<<"print_tree entry_point="<<endl;
                    //print_tree(entry_point);

                    //TODO entry_point の親がrootになってる
                    //entry_point=entry_point->parent;



                    //まだのこりのstringがある
                    //cout<<"go to next layer..."<<endl;

                    break;
                }
                DATA* ptr=(DATA*)(n->lv[i].data);
                if(ptr==NULL){
                    //cout<<"no data!!"<<endl;
                }
                //TODO
                //cout<<"find! sample_num="<<ptr->key<<" val="<<ptr->val<<endl;
                ////cout<<"find value from pointer="<< ptr->val <<endl;
                return 1;
            }
        }

    }

    cout<<"not found..."<<endl;
    ERR;

    return 0;
}




