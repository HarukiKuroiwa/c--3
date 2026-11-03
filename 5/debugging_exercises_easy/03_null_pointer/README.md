# 03 名簿の検索

## このプログラムがやりたいこと
名簿 `members` から `query` の名前を順に検索し、年齢を表示する。名簿にない名前は `not found` と表示する。

## 期待される出力
```
Suzuki: 22
Kato: not found
Tanaka: 20
```

## 症状
`make run` で実行すると、`Segmentation fault` で異常終了する（1行目が表示されないこともある）。

## 課題
1. `make run` で症状を確認する
2. **どの行で** 落ちているかを `gdb` で特定する
   ```
   gdb ./problem      （Mac の場合は lldb ./problem）
   (gdb) run
   (gdb) bt           ← 落ちた場所の呼び出し履歴
   (gdb) print p
   ```
3. `problem.c` を修正し、`make check` が **PASS** になれば完了

## ヒント
- 1行目が表示されないことがあるのは `printf` の出力がバッファにたまっているため。落ちた場所は出力だけでは判断できない
- 関数が「失敗したときに何を返すか」は呼び出し側が必ず確認する
