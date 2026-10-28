# set4 行数・単語数・バイト数を数える

## このプログラムがやりたいこと
引数で渡したファイルについて、行数・単語数・バイト数を表示する。**`wc -l -w -c` と同じ結果** を出すのが仕様。

- 行数 = 改行文字 `\n` の個数
- 単語数 = 空白文字（スペース、タブ、改行、`\r` など `isspace()` が真になる文字）で区切られた文字の並びの個数
- バイト数 = ファイルのバイト数（日本語の「文字数」ではない）
- ファイルを開けなければ `cannot open` と表示する

## テスト用のファイル（`tests/`）
| ファイル | 内容 |
|---|---|
| `normal.txt` | 普通の英文。連続したスペース、空行、タブを含む |
| `crlf.txt` | 改行が CRLF（Windows 形式）のファイル |
| `longline.txt` | 4000バイトを超える長い行を含むファイル |
| `missing.txt` | 存在しない（開けないファイルの扱いを確認する） |

## 期待される出力
```
tests/normal.txt     lines=4 words=13 bytes=73
tests/crlf.txt       lines=3 words=4 bytes=28
tests/longline.txt   lines=2 words=502 bytes=4012
tests/missing.txt    cannot open
```

## 症状
`normal.txt` は正しく数えられる。`crlf.txt` と `longline.txt` で、数の一部が `wc` と合わない。

## 課題
1. `make run` で、どのファイルの、どの数がずれているかを確認する
2. 各テストファイルを `wc -l -w -c tests/*.txt` で数えて比べる。中身は `od -c tests/crlf.txt` や `head -c 100 tests/longline.txt` で確かめる
3. ずれの原因をファイルごとに特定し、`problem.c` を修正する。`make check` が **PASS** になれば完了

## ルール
- **`BUF_SIZE` を大きくして解決してはいけない**。どんなに大きくしても、それより長い行のファイルでは同じ問題が起きる。
  行の長さに関係なく正しく数えられる方法を考えること

## ヒント
- `fgets` は1回の呼び出しで最大何バイト読むか。1行がそれより長いとき、何が起きるか
- `crlf.txt` の空行は、実際にはどんなバイトでできているか
- `fgets` と `strtok` を使わず、`fgetc` で1文字ずつ読む方法も考えてみよう
