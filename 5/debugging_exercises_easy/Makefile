DIRS = $(sort $(dir $(wildcard */problem.c)))

.PHONY: all check check-solution clean submit

all:
	@for d in $(DIRS); do $(MAKE) -s -C $$d problem; done

# 全問の判定結果を一覧表示する
check:
	@pass=0; total=0; summary=""; \
	for d in $(DIRS); do \
		total=$$((total+1)); \
		if $(MAKE) -s --no-print-directory -C $$d check 2>/dev/null; then pass=$$((pass+1)); r="PASS"; else r="FAIL"; fi; \
		summary="$$summary  $$r  $${d%/}\n"; echo; \
	done; \
	printf "===== 結果: %d / %d 問 PASS =====\n$$summary" $$pass $$total

check-solution:
	@for d in $(DIRS); do $(MAKE) -s -C $$d check-solution || true; echo; done

clean:
	@for d in $(DIRS); do $(MAKE) -s -C $$d clean; done

# 提出ファイルを作る: make submit ID=学籍番号
submit:
	@sh tools/submit.sh "$(ID)"
