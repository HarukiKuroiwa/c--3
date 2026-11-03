#!/bin/sh
# 作業記録（自動）: make run / make check を実行するたびに、時刻・結果・problem.c の変更点を記録する。
# 記録は提出ファイルに含まれます。消したり書き換えたりしないでください。
EV=$1; ST=$2
P=$(basename "$PWD")
D=../.worklog
mkdir -p "$D" 2> /dev/null || exit 0
LOG="$D/$P.log"; LAST="$D/$P.last"
[ -f problem.c ] || exit 0
SUM=$(cksum < problem.c | cut -d' ' -f1)
printf '=== %s %s %s cksum=%s\n' "$(date '+%Y-%m-%d %H:%M:%S')" "$EV" "$ST" "$SUM" >> "$LOG"
if [ -f "$LAST" ] && ! cmp -s "$LAST" problem.c; then
  diff -u "$LAST" problem.c | tail -n +3 | head -n 300 >> "$LOG"
fi
cp problem.c "$LAST"
exit 0
