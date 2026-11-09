# デバッグ演習 (difficult) レポート

学籍番号:1116241035
氏名:黒岩春希

## AIの利用

<!-- AI（ChatGPT・Claude・Copilot など）を使った場合は、どの問題で何を聞いたか、
     AIの答えのどこを自分で確かめたか（実行して確かめた結果など）を書く。使っていなければ「なし」 -->


<!-- 各問題について、見出しは変えずに、その下に書いてください。
     見出しを消したり順番を変えたりすると採点できません。 -->

---

## set1_multidimensional_arrays

### 1. 症状（どんな条件で失敗し、どんな条件では失敗しなかったか）
3x3行列以外で回転した後の数値が違う

### 2. 原因（どの関数の何行目の、何が間違っていたか）
rotate関数の
     dst[j * cols + (rows - 1 - i)] = src[i * cols + j];

### 3. 修正内容（何をどう変えたか）
     dst[j*rows+(rows-1-i)] = src[i * cols + j];

### 4. なぜその修正で直るのか
i行j列の行列をj行rows-i-1列に変換する
i*cols+jで１次元に圧縮している、
回転したときの圧縮はcols行rows列なので、列ごとに並べないといけない。

### 5. 観察と調べ方

<!-- 自分の環境で実際に見た出力や値を貼る（make run の出力、gdb の bt や print の結果、printf で表示した値、サニタイザの報告など）。
     最初に疑った場所と、それが違う（または正しい）と分かった理由も書く -->
     [FAIL] 出力が期待値と違います（< 期待される出力 / > 実際の出力）
          (期待値の 14 行目付近)
        >   0  0
          (期待値の 16,17 行目付近)
        <   7  3
        <   8  4
        >   0  0
          (期待値の 25,26 行目付近)
        <   7  5  3  1
        <   8  6  4  2
        >   7  5  8  6
        >   4  2  0  0
[FAIL] サニタイザ (-fsanitize=address,undefined) が問題を検出しました
        ==57484==ERROR: AddressSanitizer: heap-buffer-overflow on address 0x503000000094 at pc 0x60ff0de49571 bp 0x7ffd84cfba20 sp 0x7ffd84cfba10
        SUMMARY: AddressSanitizer: heap-buffer-overflow /home/kuro/projects/clang3/5/debugging_exercises_difficult/set1_multidimensional_arrays/problem.c:11 in rotate
        詳しく見るには: cc -g -fsanitize=address,undefined problem.c -lpthread && ./a.out 
>>> FAIL

3x3だと行列が同じなのでcols,rowsどっちで並べても関係なくなる。

## AIの利用
なし
---

## set2_linked_lists

### 1. 症状（どんな条件で失敗し、どんな条件では失敗しなかったか）
削除するキーが連続しているときにエラー。
Segmentation fault (core dumped)
### 2. 原因（どの関数の何行目の、何が間違っていたか）
remove_all関数のキーを発見したときに
prevにcurの次のアドレスを入れている
次に連続でキーを発見したとき、curは消されるので
prevの次のアドレスに情報がなくなる
(説明難しい)
### 3. 修正内容（何をどう変えたか）
if(cur->data==key)を
while (cur->data == key && cur != NULL) {
            next=cur->next;
            if (prev == NULL) {
                *head = next;
            } else {
                prev->next = next;
            }
            free(cur);
            cur=next;
        }
に変えた。

### 4. なぜその修正で直るのか
prev->nextがさすアドレスをキーがなくなるまで次のアドレスにする。最後までキーがあったらcur!=NULLに先に引っ掛かりcur->dataでNULL参照しない。

### 5. 観察と調べ方

<!-- 自分の環境で実際に見た出力や値を貼る（make run の出力、gdb の bt や print の結果、printf で表示した値、サニタイザの報告など）。
     最初に疑った場所と、それが違う（または正しい）と分かった理由も書く -->
     
before: 1 -> 2 -> 3 -> 2 -> 4 -> NULL
remove 2: 1 -> 3 -> 4 -> NULL
before: 5 -> 7 -> 7 -> 8 -> NULL
ここで終了する。
ポインタを多用しているのでどこかでNULLを参照してそう。
キーを消すのがwhileの中で１回だけなのでprev->nextが次のcurを指しているがもしこのcurがキーだった場合NULLを参照してしまうことが分かったので直した。
## AIの利用
なし
---

## set3_multithreading

### 1. 症状（どんな条件で失敗し、どんな条件では失敗しなかったか）
sold  = 175666 (expected 100000)
seats = -1 (expected 0)


### 2. 原因（どの関数の何行目の、何が間違っていたか）
スレッドが各々mutexを持っていて互いに共有しているseatsのsell関数に同時にアクセスしてしまう

### 3. 修正内容（何をどう変えたか）
pthread_mutex_t lock=PTHREAD_MUTEX_INITIALIZER;
int sell() {
    pthread_mutex_lock(&lock);
    if (seats > 0) {
        seats--;
        pthread_mutex_unlock(&lock);
        return 1;
    }
    pthread_mutex_unlock(&lock);
    return 0;
}

### 4. なぜその修正で直るのか
別々のmutexを持っているのを全スレッドで共通のmutexを持ち排他処理を行った。


### 5. 観察と調べ方

<!-- 自分の環境で実際に見た出力や値を貼る（make run の出力、gdb の bt や print の結果、printf で表示した値、サニタイザの報告など）。
     最初に疑った場所と、それが違う（または正しい）と分かった理由も書く -->
     構造体にmutexの変数があるの違和感。mutexの使い方が違うかも.
     window関数はカウンタ―ごとに別々のCounterを使うため競合はない
## AIの利用
スレッドを作った時の第三引数の役割
pthread_create(&t[i], NULL, window, &c[i]);
引数の意味は次の通りです。

&t[i]：作成されたスレッドIDの保存先
NULL：スレッド属性（標準設定）
window：スレッドが開始すると実行する関数
&c[i]：window に渡す引数

#sym:pthread_mutex_lock の引数には何を指定するのか
pthread_mutex_lock の引数には、ロック対象の pthread_mutex_t のアドレスを指定します。

---

## set4_file_io

### 1. 症状（どんな条件で失敗し、どんな条件では失敗しなかったか）


### 2. 原因（どの関数の何行目の、何が間違っていたか）


### 3. 修正内容（何をどう変えたか）


### 4. なぜその修正で直るのか


### 5. 観察と調べ方

<!-- 自分の環境で実際に見た出力や値を貼る（make run の出力、gdb の bt や print の結果、printf で表示した値、サニタイザの報告など）。
     最初に疑った場所と、それが違う（または正しい）と分かった理由も書く -->



---

## set5_binary_search_tree

### 1. 症状（どんな条件で失敗し、どんな条件では失敗しなかったか）


### 2. 原因（どの関数の何行目の、何が間違っていたか）


### 3. 修正内容（何をどう変えたか）


### 4. なぜその修正で直るのか


### 5. 観察と調べ方

<!-- 自分の環境で実際に見た出力や値を貼る（make run の出力、gdb の bt や print の結果、printf で表示した値、サニタイザの報告など）。
     最初に疑った場所と、それが違う（または正しい）と分かった理由も書く -->


