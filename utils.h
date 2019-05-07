//
// Created by albicilla on 2019/05/07.
//
#pragma once


#include <bitset>
//uint64で表現されたkeyの大小比較をする bが真に大きい時にtrue

class myCompare{
public:
    bool operator()(const uint64_t &a,const uint64_t &b){
        bitset<64> bit_a(a);
        string bit_a_str=bit_a.to_string();

        bitset<64> bit_b(b);
        string bit_b_str=bit_b.to_string();

        for(int i=0;i<64;i++){
            if(bit_a_str[i]-'0'<bit_b_str[i]-'0')return true;
            else if(bit_a_str[i]-'0'>bit_b_str[i]-'0')return false;
        }
        return false;
    }
};
