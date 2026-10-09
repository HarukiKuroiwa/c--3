#include <sys/types.h>
#include <curses.h>
#include<signal.h>
#include <stdlib.h>
#include <unistd.h>

bool ok=false;

void signal_handler(int signal){
    ok=true;
}


int main(void)
{
    int k = 0, row, col, pid;   // k を初期化
    char msg[128];              // バッファを確保
    int cnt = -1;
    int result[] = {-1, -1, -1, -1, -1};
    int v[5] = {0};

    struct sigaction sa;
    sigemptyset(&sa.sa_mask);
    sa.sa_handler=signal_handler;
    sa.sa_flags=0;
    sigaction(SIGUSR1,&sa,NULL);

    initscr();
    getmaxyx(stdscr,row,col);

    if (has_colors()) {
        start_color();
        init_pair(1,COLOR_RED,COLOR_WHITE);
        init_pair(2,COLOR_GREEN,COLOR_BLACK);
        init_pair(3,COLOR_WHITE,COLOR_BLUE);
    }
    curs_set(0);
    noecho();
    nodelay(stdscr, TRUE);

    if (has_colors()) bkgd(COLOR_PAIR(3));
    if (has_colors()) attron(COLOR_PAIR(1));

    if (row >= 3) {
        for (int i = 0; i < col; i++) {
            mvaddstr(0, i,  " ");
            mvaddstr(row-2, i,  " ");
            mvaddstr(row-1, i,  " ");
        }
    }
    pid = getpid();
    snprintf(msg, sizeof(msg), " Curses C Dynamic Text Example (pid : %d)", pid);
    mvaddstr(0, 0, msg);
    mvaddstr(row-2, 0,  " Key Commands: space - fix a parameter");
    mvaddstr(row-1, 0,  " Key Commands: q     - to quit");
    attroff(COLOR_PAIR(1));   
    mvaddstr(2, 5,"NOBUNAGA NO YABOU ZENKOKU BAN" );
    refresh();

    // Cycle with new values until a q key (133) is entered    
    while (k != 113)
    {
        attroff(COLOR_PAIR(2));
        for (int i = 0; i < 5; i++) {
	    switch(i) {
		case 0:
                    mvprintw((4+i), 5, " KENKOU  %d : ", i);
		    break;
		case 1:
                    mvprintw((4+i), 5, " YASHIN  %d : ", i);
		    break;
		case 2:
                    mvprintw((4+i), 5, " UN      %d : ", i);
		    break;
		case 3:
                    mvprintw((4+i), 5, " MIRYOKU %d : ", i);
		    break;
		case 4:
                    mvprintw((4+i), 5, " IQ      %d : ", i);
		    break;
		default:
            	    mvprintw((4+i), 5, " Sensor %d : ", i);
		}
        }
        attron(COLOR_PAIR(2));

        for (int i=0; i<5; i++) {
	    if (result[i] != -1) {
                mvprintw((4+i), 20, "%2d", result[i]);
	    } else {
                v[i] = rand() % 100;
                mvprintw((4+i), 20, "%2d", v[i]);
	    }
        }
	// 能力値の決定（スペースキーで入力）
        k = getch();
	if (ok) {
	    cnt++;
	    if (cnt < 5) {
	        result[cnt] = v[cnt];
	    }
        }
        ok=false;
	// 合計値の表示
	if (cnt == 4) {
	    int s = 0;
	    for(int i=0; i<5; i++) {
		s += result[i];
	    }
            mvprintw(11, 20, "GOUKEI %d", s);
            //mvprintw(12, 20, "result[i] %2d, %2d, %2d, %2d, %2d", v[0], v[1], v[2], v[3], v[4]);
            //mvprintw(13, 20, "v[i]      %2d, %2d, %2d, %2d, %2d", result[0], result[1], result[2], result[3], result[4]);
	}
	// 値の初期化
	if (cnt == 5) {
	   for(int i=0; i<5; i++) {
		result[i] = -1;
	   }
	   cnt = -1;
	}
    }
    endwin();
    exit(0);
}
