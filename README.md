# masstree
masstree(under construction)


masstree.cpp 本体

bptreeを元に

4/22
(v0.0.1)
concurrency controlなし。テストが不十分だが動くはず.... サンプルテストは正常動作

4/29
(v0.0.2)
bptreeのmasstree化に伴う修正をinsert_in_temp関数に追加。


5/6
masstreeのレイヤーを跨いだ先でinsert_in_parentが起こる時、0レイヤーのRootをそのレイヤーのrootとしてしまっている。

5/24
masstreeの修正。
border_split等をポインタの参照に
キー長9の10^6数のランダムキーに対応
