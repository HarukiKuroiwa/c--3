#!/bin/sh
# 使い方: make submit ID=学籍番号
ID=$1
if [ -z "$ID" ]; then
  echo "使い方: make submit ID=学籍番号   （例: make submit ID=1234567890）"
  exit 1
fi
case "$ID" in
  *[!0-9A-Za-z]*) echo "学籍番号は半角英数字で指定してください: $ID"; exit 1 ;;
esac

SET=$(basename "$PWD" | sed 's/^debugging_exercises_//')
OUT="submit_${SET}_${ID}"

# レポートの記入漏れを確認する
if ! grep -Eq '^学籍番号: *[0-9A-Za-z]' REPORT.md; then
  echo "[注意] REPORT.md の「学籍番号:」が空欄です"
fi
if ! grep -Eq '^氏名: *[^ ]' REPORT.md; then
  echo "[注意] REPORT.md の「氏名:」が空欄です"
fi

rm -rf "$OUT" "$OUT.zip" "$OUT.tar.gz"
mkdir "$OUT"

echo "全問を判定しています..."
WORKLOG_EVENT=submit make -s --no-print-directory check > "$OUT/check_result.txt" 2>&1
tail -n 7 "$OUT/check_result.txt"

for p in */problem.c; do
  d=${p%/problem.c}
  [ "$d" = "$OUT" ] && continue
  mkdir -p "$OUT/$d"
  cp "$p" "$OUT/$d/problem.c"
done
cp REPORT.md "$OUT/REPORT.md"
[ -d .worklog ] && cp -R .worklog "$OUT/worklog"
{
  echo "id=$ID"
  echo "set=$SET"
  echo "date=$(date '+%Y-%m-%d %H:%M:%S')"
  echo "os=$(uname -sm)"
  echo "cc=$(${CC:-cc} --version 2>/dev/null | head -1)"
} > "$OUT/SUBMISSION_INFO.txt"

if command -v zip > /dev/null 2>&1; then
  zip -qr "$OUT.zip" "$OUT" && FILE="$OUT.zip"
else
  tar czf "$OUT.tar.gz" "$OUT" && FILE="$OUT.tar.gz"
fi
rm -rf "$OUT"
echo
echo "提出ファイルを作成しました: $FILE"
echo "このファイルを提出してください（REPORT.md・各問題の problem.c・判定結果・作業記録が入っています）"
