# 02 文字列にタグを付ける

## このプログラムがやりたいこと
文字列 `"hello world"` の末尾に `" [checked]"` を付けて表示する。

## 期待される出力
```
before: hello world (11)
after : hello world [checked] (21)
```

## 症状
`make run` で実行すると、出力の後に `stack smashing detected` や `Abort trap` などと表示されて異常終了する
（`make check` ではサニタイザでも検出される）。

## 課題
1. `make run` で症状を確認する
2. 原因を特定して `problem.c` を修正する
3. `make check` が **PASS** になれば完了

## ヒント
- `text` は何バイト確保されている？ 連結後の文字列は何バイト必要？（終端の `'\0'` を忘れずに）
- 配列を大きくするだけでなく、`addTag` が「書き込み先の大きさ」を知る方法も考えよう
- `addTag` の中で `sizeof(str)` を使っても配列の大きさは分からない。なぜか？
