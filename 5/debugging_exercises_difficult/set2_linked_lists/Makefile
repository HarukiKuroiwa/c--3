# ---- この問題の設定（変更しないこと） ----
OPT    = -O1
SAN    = address,undefined
REPEAT = 1
ARGS   = 
LDLIBS = -lpthread
CFLAGS = -Wall -Wextra -g $(OPT)
export CC OPT SAN REPEAT ARGS LDLIBS

.PHONY: all run check check-solution clean

all: problem

problem: problem.c
	$(CC) $(CFLAGS) -o problem problem.c $(LDLIBS)

solution: solution.c
	$(CC) $(CFLAGS) -o solution solution.c $(LDLIBS)

# 実行して症状を確認する
run: problem
	@./problem $(ARGS); sh ../tools/worklog.sh run "exit=$$?"

# 直ったかどうかを判定する（出力の比較＋サニタイザ）
check:
	@sh ../tools/check.sh problem.c

check-solution:
	@sh ../tools/check.sh solution.c

clean:
	rm -rf problem solution a.out *.dSYM
