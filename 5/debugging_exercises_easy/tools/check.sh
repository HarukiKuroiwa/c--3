#!/bin/sh
# 使い方: sh ../tools/check.sh <source.c>
# Makefile から OPT / SAN / REPEAT / LDLIBS を受け取る
SRC=$1
CC=${CC:-cc}
OPT=${OPT:--O1}
SAN=${SAN:-address,undefined}
REPEAT=${REPEAT:-1}

if [ -t 1 ]; then
  G='\033[32m'; R='\033[31m'; Y='\033[33m'; B='\033[1m'; D='\033[2m'; N='\033[0m'
else
  G=''; R=''; Y=''; B=''; D=''; N=''
fi
say() { printf "%b\n" "$*"; }
wl() {   # 作業記録（problem.c の判定のときだけ）
  if [ "$SRC" = problem.c ] && [ "${WORKLOG:-on}" = on ] && [ -f ../tools/worklog.sh ]; then
    sh ../tools/worklog.sh "${WORKLOG_EVENT:-check}" "$1"
  fi
}

TMP=$(mktemp -d 2>/dev/null || mktemp -d -t dbg)
trap 'rm -rf "$TMP"' EXIT
fail=0

say "${B}=== $(basename "$PWD") / $SRC ===${N}"

# --- [1] コンパイル -------------------------------------------------
if ! $CC -Wall -Wextra -g $OPT -o "$TMP/prog" "$SRC" $LDLIBS 2> "$TMP/build.log"; then
  cat "$TMP/build.log"
  say "${R}[FAIL] コンパイルできません${N}"
  wl COMPILE_ERROR
  exit 1
fi
if grep -q "warning" "$TMP/build.log"; then
  say "${Y}[警告] コンパイラが警告を出しています（バグの手がかりかもしれません）${N}"
  grep "warning" "$TMP/build.log" | sed 's/^/        /'
else
  say "${G}[ OK ] コンパイル（警告なし）${N}"
fi

# --- [2] 出力が期待値と一致するか -------------------------------------
i=1
while [ "$i" -le "$REPEAT" ]; do
  "$TMP/prog" > "$TMP/out.txt" 2> "$TMP/err.txt"
  st=$?
  if [ "$st" -ge 128 ]; then
    say "${R}[FAIL] プログラムが異常終了しました（終了コード $st = シグナル $((st - 128))）${N}"
    [ -s "$TMP/err.txt" ] && sed 's/^/        /' "$TMP/err.txt" | head -5
    fail=1; break
  fi
  if ! diff -q expected.txt "$TMP/out.txt" > /dev/null 2>&1; then
    if [ "$REPEAT" -gt 1 ]; then
      say "${R}[FAIL] ${REPEAT}回実行のうち ${i}回目で出力が期待値と違いました${N}"
    else
      say "${R}[FAIL] 出力が期待値と違います${N}"
    fi
    say "${D}        --- 期待される出力 (expected.txt)${N}"
    sed 's/^/        /' expected.txt
    say "${D}        --- 実際の出力${N}"
    sed 's/^/        /' "$TMP/out.txt"
    fail=1; break
  fi
  i=$((i + 1))
done
if [ "$fail" -eq 0 ]; then
  if [ "$REPEAT" -gt 1 ]; then
    say "${G}[ OK ] 出力が期待値と一致（${REPEAT}回とも）${N}"
  else
    say "${G}[ OK ] 出力が期待値と一致${N}"
  fi
fi

# --- [3] サニタイザ（見た目は正しくても隠れたバグがないか） -----------
if $CC -g -O0 -fno-omit-frame-pointer -fsanitize=$SAN -o "$TMP/san" "$SRC" $LDLIBS 2> /dev/null; then
  ASAN_OPTIONS=detect_leaks=0 TSAN_OPTIONS=halt_on_error=0 \
    "$TMP/san" > /dev/null 2> "$TMP/san.txt"
  if grep -Eq "ERROR: AddressSanitizer|WARNING: ThreadSanitizer|runtime error" "$TMP/san.txt"; then
    say "${R}[FAIL] サニタイザ (-fsanitize=$SAN) が問題を検出しました${N}"
    grep -E "ERROR: AddressSanitizer|WARNING: ThreadSanitizer|runtime error|SUMMARY" "$TMP/san.txt" \
      | sort -u | head -5 | sed 's/^/        /'
    fail=1
  else
    say "${G}[ OK ] サニタイザ (-fsanitize=$SAN) で問題なし${N}"
  fi
else
  say "${Y}[SKIP] この環境では -fsanitize=$SAN が使えないため省略${N}"
fi

if [ "$fail" -eq 0 ]; then wl PASS; else wl FAIL; fi
if [ "$fail" -eq 0 ]; then
  say "${G}${B}>>> PASS${N}"
  exit 0
else
  say "${R}${B}>>> FAIL${N}"
  exit 1
fi
