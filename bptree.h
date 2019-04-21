//
// Created by albicilla on 2019/03/26.
//

#pragma once

#include "debug.h"

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


//
//following codes are b+tree code for masstree
//

#define N 15

using std::cout;
using std::endl;
using std::cerr;
using std::vector;
using std::mt19937_64;

typedef struct _DATA {
    _DATA(){};
    _DATA(int key,int val){this->key=key,this->val=val;};
    int key;
    int val;
    struct _DATA *next;
    struct _DATA *prev;
} DATA;

typedef struct _NODE {
    bool isLeaf;
    struct _NODE *chi[N];
    //int key[N-1];
    int key[N-1];
    int nkey;
    struct _NODE *parent;
    void* lv[N];
} NODE;

typedef struct _TEMP {
    bool isLeaf;
    NODE *chi[N+1]; // for internal split (for leaf, only N is enough)
    int key[N]; // for leaf split
    int nkey;
    void* lv[N+1];

} TEMP;

//data table の頭ポインタ
DATA *data_root;

//datatable
DATA datatable[1123456];


NODE *Root = NULL;

void
print_tree_core(NODE *n)
{
    printf("[");
    for (int i = 0; i < n->nkey; i++) {
        if (!n->isLeaf) print_tree_core(n->chi[i]);
        printf("%d", n->key[i]);
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


    return new_node;
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
alloc_leaf_dash(NODE *parent){
    NODE *node;
    if (!(node = (NODE *)calloc(1, sizeof(NODE)))) ERR;
    node->isLeaf = true;
    node->parent = parent;
    node->nkey = 0;

    return node;

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

NODE *
alloc_internal(NODE *parent)
{
    NODE *node;
    if (!(node = (NODE *)calloc(1, sizeof(NODE)))) ERR;
    node->isLeaf = false;
    node->parent = parent;
    node->nkey = 0;

    return node;
}

NODE *
alloc_root(NODE *left, int rs_key, NODE *right)
{
    NODE *node;

    if (!(node = (NODE *)calloc(1, sizeof(NODE)))) ERR;
    node->parent = NULL;
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
find_leaf(NODE *node, int key)
{
    int kid;

    if (node->isLeaf) return node;
    for (kid = 0; kid < node->nkey; kid++) {
        if (key < node->key[kid]) break;
    }

    return find_leaf(node->chi[kid], key);
}

NODE *
insert_in_node(NODE *p,NODE *c,int key,int index){
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
insert_in_node_temp(TEMP *p,NODE *c,int key,int index){
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

NODE *
insert_in_parent(NODE *node, int key, NODE *node_dash, DATA *data){
    if (node==Root){
        return Root=alloc_root(node,key,node_dash);
    }


    NODE *parent=node->parent;

    int index=0,i;

    while(parent->key[index]<key && index<parent->nkey)index++;

    if(parent->nkey<N-1){
        insert_in_node(parent,node_dash,key,index);
        return Root;
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
            // print_tree(t->chi[i]);
            t->chi[i]->parent=parent;


            // dump(t->key[i]);

            parent->nkey++;
        }
        assert(parent->nkey==mid_pos);
        parent->chi[i]=t->chi[i];
        t->chi[i]->parent=parent;

        // print_tree(t->chi[i]);
        //let k'' = t[mid_pos]
        int key_temp=t->key[mid_pos];
        // cout<<"key_tempの値"<<endl;
        // cout<<key_temp<<endl;
        //copy t into p' right
        for(i=mid_pos+1;i<t->nkey;i++){
            pp->chi[i-(mid_pos+1)]=t->chi[i];
            pp->key[i-(mid_pos+1)]=t->key[i];
            t->chi[i]->parent=pp;
            pp->nkey++;
        }
        pp->chi[i-(mid_pos+1)]=t->chi[i];
        t->chi[i]->parent=pp;

        return insert_in_parent(parent,key_temp,pp,data);

    }
}
//temp用の要素挿入関数
TEMP *
insert_in_leaf_temp(TEMP *leaf,int key,DATA *data){
    int i;

    if(key<leaf->key[0]){
        for(int i=leaf->nkey;i>0;i--){
            leaf->lv[i]=leaf->lv[i-1];
            leaf->key[i]=leaf->key[i-1];
        }
        leaf->key[0]=key;
        leaf->lv[0]=(void *)data;
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

        leaf->key[i+1]=key;
        leaf->lv[i+1]=(void * )data;
    }
    leaf->nkey++;

    return leaf;

}

NODE *
insert_in_leaf(NODE *leaf, int key, DATA *data)
{
    int i;
    //cout<<"key="<<data->key<<": val="<<data->val<<endl;
    if (key < leaf->key[0]) {
        for (i = leaf->nkey; i > 0; i--) {
            leaf->lv[i] = leaf->lv[i-1] ;
            leaf->key[i] = leaf->key[i-1] ;
        }
        leaf->key[0] = key;
        leaf->lv[0] = (void *)data;
    }
    else {
        for(i = leaf->nkey-1;i>=0;--i){
            if(key > (int)leaf->key[i]){
                break;
            }else{
                leaf->key[i+1]=leaf->key[i];
                leaf->lv[i+1]=leaf->lv[i];
            }
        }

        leaf->key[i+1]=key;
        leaf->lv[i+1]=(void * )data;

    }
    leaf->nkey++;

    return leaf;
}


void
insert(int key, DATA *data)
{
    NODE *leaf;

    if (Root == NULL) {
        // cout<<"呼び出し"<<endl;
        leaf = alloc_leaf(NULL);
        Root = leaf;
    }
    else {
        leaf = find_leaf(Root, key);
    }

    if (leaf->nkey < (N-1)) {
        insert_in_leaf(leaf, key, data);
    }
    else {
        TEMP *Temp=create_new_node();
        for(int i=0;i<N-1;i++){
            Temp->key[i]=leaf->key[i];
            Temp->lv[i]=leaf->lv[i];
            Temp->nkey++;
        }
        insert_in_leaf_temp(Temp,key,data);


        NODE *leaf_dash=alloc_leaf_dash(NULL);

        leaf_dash->lv[N-1]=leaf->lv[N-1];

        leaf->lv[N-1]=leaf_dash;

        for(int i=0;i<N-1;i++){
            leaf->lv[i]=NULL;
            leaf->key[i]=0;
        }
        leaf->nkey=0;

        int mid_pos=(N-1)/2;
        for(int i=0;i<=mid_pos;i++){
            leaf->lv[i]=Temp->lv[i];
            leaf->key[i]=Temp->key[i];
            leaf->nkey++;
        }
        for(int i=mid_pos+1;i<N;i++){
            leaf_dash->key[i-(mid_pos+1)]=Temp->key[i];
            leaf_dash->lv[i-(mid_pos+1)]=Temp->lv[i];
            leaf_dash->nkey++;
        }

        int key_dash=leaf_dash->key[0];

        //dleaf->parent =

        insert_in_parent(leaf,key_dash,leaf_dash,data);


    }
}

void adjust_root(NODE* root){
//    dump(root->nkey);
    if(root->nkey>0)return;
    if(!root->isLeaf){
        cerr<<"rootノードで子があと一個しかなかったら"<<endl;
        Root=Root->chi[0];
        Root->parent=NULL;
    }else{
        root=NULL;
    }
}




void delete_entry(NODE *nd,int k)
{
    //リーフノードから値がkのものを削除する。 delete(K,P) from N
    cerr<<"nd->nkey"<<nd->nkey<<endl;
    int i;

    if (nd->isLeaf){
        free(nd->chi[k]);  // destroy the record
        for (i = k; i < nd->nkey - 1; i++){
            nd->key[i] = nd->key[i + 1];
            nd->chi[i] = nd->chi[i + 1];
        }
        nd->key[i] = 0;
        nd->chi[i] = NULL;
    }
    else{
        int index_k = k - 1;  // index_p == index
        for (i = index_k; i < nd->nkey - 1; i++){
            nd->key[i] = nd->key[i + 1];
            nd->chi[i + 1] = nd->chi[i + 2];
        }
        nd->key[i] = 0;
        nd->chi[i + 1] = NULL;
    }
    //キーを減らす
    nd->nkey--;

//    dump(nd->nkey);
    for(int i=0;i<nd->nkey;i++){
//        dump(nd->key[i]);
    }

    //N has toofew values/pointers
    //葉の時とそうでない時で基準が違う
    //葉だったらN/2 そうでないなら(N-1)/2
    int min_keys = nd->isLeaf ? N / 2 :(N-1)/2;

    if(nd==Root){
        adjust_root(nd);
    }
    else if(nd->nkey<min_keys){
//        dump("a");
        //Let N' be the previous or next child of parent(N)
        //Let K' be the value between pointers N and N' in parent(N)
        int idx;
        NODE *parent=nd->parent;
//        dump(parent->nkey);



        for(idx=0;idx<parent->nkey && parent->chi[idx]!=nd;idx++);
        NODE *neighbor;

//        dump(idx);
//        dump(nd->nkey);
        //親から見て左端の子かそうでないか
        if(idx==0){
            neighbor=nd->parent->chi[1]; //right neighbor
        }else{
            neighbor=nd->parent->chi[idx-1]; //left neighbor
        }

        int cap=nd->isLeaf ? N-1:N-2;

        if(neighbor->nkey+nd->nkey<=cap){
            //coalesece node

            //if N is predecessor of N' swap N N'
            if(idx==0){
                NODE *tmp=nd;
                nd=neighbor;
                neighbor=tmp;
                idx=1;
            }

            int i,j;
            int start = neighbor->nkey;
            if(!nd->isLeaf){
                //append all(Ki,Pi)pairs in N to N'; set N'.Pn = N.Pn
                //neighbor->key[start]=(int)malloc(15);
                neighbor->key[neighbor->nkey]=nd->parent->key[idx-1];

                for(i=start,j=0;j<nd->nkey;i++,j++){
                    neighbor->key[i]=nd->key[j];
                    neighbor->chi[i]=nd->chi[j];
                }
                neighbor->chi[i]=nd->chi[j];
                neighbor->nkey+=nd->nkey+1;

                for(i=0;i<=neighbor->nkey;i++){
                    NODE *tmp=(NODE*)neighbor->chi[i];
                    tmp->parent=neighbor;
                }

            }else{
                //append K' amd all pointers and values in N to N'
                for(i=start,j=0;j<nd->nkey;i++,j++){
                    neighbor->key[i]=nd->key[j];
                    neighbor->chi[i]=nd->chi[j];
                    nd->key[j]=0;
                    nd->chi[j]=NULL;
                }
                neighbor->nkey+=nd->nkey;
                neighbor->chi[N-1]=nd->chi[N-1];

                for(int i=0;i<neighbor->nkey;i++){
//                    dump(neighbor->key[i]);
                }
            }

            free(nd);
//            dump(idx);
            return delete_entry(parent,idx);
        }else{
            //distribute node

            int i;
            NODE *tmp;
            if(idx!=0){
                if(!nd->isLeaf){nd->chi[nd->nkey+1]=nd->chi[nd->nkey];}

                for(i=nd->nkey;i>0;i--){
                    nd->key[i]=nd->key[i-1];
                    nd->chi[i]=nd->chi[i-1];
                }

                if(!nd->isLeaf){
                    nd->key[0]=nd->parent->key[idx-1];

                    nd->chi[0]=neighbor->chi[neighbor->nkey];
                    tmp=(NODE*)nd->chi[0];
                    tmp->parent=nd;
                    neighbor->chi[neighbor->nkey]=NULL;

                    nd->parent->key[idx-1]=neighbor->key[neighbor->nkey-1];
                    neighbor->key[neighbor->nkey-1]=0;
                }else{
                    nd->key[0]=neighbor->key[neighbor->nkey-1];
                    neighbor->key[neighbor->nkey-1]=0;

                    nd->chi[0]=neighbor->chi[neighbor->nkey-1];
                    neighbor->chi[neighbor->nkey-1]=NULL;
                    nd->parent->key[idx - 1]=nd->key[0];
                }
            }else{
                if (!nd->isLeaf){
                    nd->key[nd->nkey] = nd->parent->key[0];
                    nd->chi[nd->nkey + 1] = neighbor->chi[0];
                    tmp = (NODE *)nd->chi[nd->nkey + 1];
                    tmp->parent = nd;
                    nd->parent->key[0] = neighbor->key[0];  //
                }
                else {
                    nd->key[nd->nkey] = neighbor->key[0];
                    nd->chi[nd->nkey] = neighbor->chi[0];
                    nd->parent->key[0]=neighbor->key[1];
                }
                for (i = 0; i < neighbor->nkey - 1; i++){
                    neighbor->key[i] = neighbor->key[i + 1];
                    neighbor->chi[i] = neighbor->chi[i + 1];
                }
                neighbor->key[i] = 0;
                if (!nd->isLeaf)
                    neighbor->chi[i] = neighbor->chi[i + 1];
                else
                    neighbor->chi[i] = NULL;
            }

            neighbor->nkey--;
            nd->nkey++;

        }//end of distribute node


    }//end of if(nd->nkey<min_keys)
}



void
deleteKey(int k,DATA *data)
{
    NODE *leaf;
    leaf=find_leaf(Root,k);

    if(leaf == NULL){
        return;
    }
    int i;
    for(i=0;i<leaf->nkey&&leaf->key[i]!=k;i++);


    if(i==leaf->nkey){
        cerr<<"no such key"<<endl;
        return ;
    }

    int tnum=leaf->key[i];
//    dump(tnum);
//    dump(i);

    delete_entry(leaf,i);
}



bool find(int key){
    NODE *leaf;
    Root=leaf;
    if(leaf==NULL){
        cerr<<"要素が挿入されてない"<<endl;
        return 0;
    }

    cerr<<"ここ"<<endl;


    leaf=find_leaf(Root,key);

    //cerr<<"ここ"<<endl;


    for(int i=0;i<leaf->nkey+1;i++){

        cout<<"key="<<leaf->key[i]<<endl;

        if(leaf->key[i]==key){
            cout<<"find "<<key<<"!"<<endl;
            return 1;
        }
    }
    cout<<"not found..."<<endl;


    return 0;


}

void
init_root(void)
{
    Root = NULL;
}

bool
search_core(const int key)
{
    NODE *n = find_leaf(Root, key);
    for (int i = 0; i < n->nkey+1; i++) {
        if (n->key[i] == key) {
            DATA* ptr=(DATA*)(n->lv[i]);
            //TODO
            cout<<"find! key="<<ptr->key<<" val="<<ptr->val<<endl;
            //cout<<"find value from pointer="<< ptr->val <<endl;
            return 1;
        }
    }
    cout << "Key not found: " << key << endl;
    ERR;
    return 0;
}



int
interactive()
{
    int key;

    std::cout << "Key: ";
    std::cin >> key;

    return key;
}


//100万件のデータランダムに挿入して動作を確認する。

bool test_bptree(){

}

//not work
void insert_dataset_data(int data){
    DATA *new_node;
    new_node=(DATA*)calloc(1,sizeof(DATA));


    new_node->next=data_root->next;//new_nodeの次の要素はrootの次の要素
    data_root->next->prev=new_node;//rootの次の要素の前の要素はnew_node
    data_root->next=new_node;//rootの次の要素はnew_node
    new_node->prev=data_root;//new_nodeの前の要素はroot
    new_node->key=data;
}


// end of b+tree implemention


