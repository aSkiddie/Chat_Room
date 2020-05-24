#include <ncurses.h>
#include <signal.h>
#include <stdlib.h>
WINDOW* chatmenu;

static volatile sig_atomic_t signal_ = 0;

static void sig_handler(int sig)
{
  if (SIGWINCH == sig) {
	signal_ = true;
  }
} // sig_handler


void chat_Getline(WINDOW * chatbox, char* buff, int y, int x) {
	
	int i, temp, tempY, tempX, keytempY; 
	int j = 0;
	int c;
	tempY = (y - y);
	tempX = (x - x);
	keytempY = (y - y) + 1;
	for(i = 0;(c=wgetch(chatbox)) != '\n'; i++) {	
		switch(c) {
			case KEY_UP:
				echo();
				tempY--;
				if(tempY == 0)
					tempY++;
				wmove(chatbox, tempY, tempX);
				break;
			case KEY_DOWN:
				echo();
				tempY++;
				if(tempY == (y - 1))
					tempY--;
				wmove(chatbox, tempY, tempX);
				break;
			case KEY_LEFT:
				echo();
				tempX--;
				if(tempX == 0)
					tempX++;
				wmove(chatbox, tempY, tempX);
				break;
			case KEY_RIGHT:
				echo();
			      	tempX++;
				if(tempX == (x - 1))
					tempX--;
			      	wmove(chatbox, tempY, tempX);
			      	break;
			case KEY_BACKSPACE:
				echo();
				tempX--;
				wmove(chatbox, tempY, tempX);
				break;
			default:
				j++;
				buff[i] = c;
				temp = j;
				
				if(temp >= (x - 2)) {
					keytempY++;
					wmove(chatbox, keytempY, 1);
				
					if(keytempY >= (y-1)) {
						noecho();
						keytempY--;
				       		wmove(chatbox, keytempY, x-2);
					}
				       j = 0;
				}
			break;
		}
		
	}
	if(buff[i] == '\n') {
		flushinp();
		buff[i] = '\n';
	}
	buff[i] = '\0';
}
		
	
void titleMenu(WINDOW * menuwin) {	
	wattron(menuwin, A_BOLD);
	wattron(menuwin, COLOR_PAIR(1));
	mvwprintw(menuwin, 0, 0, "      DEAD");
	wattroff(menuwin, COLOR_PAIR(1));
	wattroff(menuwin, A_BOLD);
	refresh();
}

void borderMenu(WINDOW * chatmenu, int yMax, int xMax) {
	refresh();	
	chatmenu = newwin(LINES, COLS, yMax-yMax, xMax-xMax);
	if(!has_colors()) exit(0);
	start_color();
	init_pair(1, COLOR_RED, COLOR_BLACK);
	wattron(chatmenu, COLOR_PAIR(1));
	box(chatmenu, '*', '*');
	wattroff(chatmenu, COLOR_PAIR(1));
	wrefresh(chatmenu);
}




int main() {

	initscr(); //stdscr
	noecho();
	cbreak();

	int yMax, xMax;
	getmaxyx(stdscr, yMax, xMax);
	
	

	WINDOW* menuwin = newwin(5, 15, (yMax/2.5), (xMax/2.15));
	wrefresh(menuwin);
	keypad(menuwin, true);
	
	borderMenu(chatmenu, yMax, xMax);		
	titleMenu(menuwin);	

	const char* options[3] = {"Start Chatting", "Enter Username", "Exit         "};
	
	int choice;
	int highlight = 0;
	

	while(1) {
		

		wtimeout(menuwin, 100); // 0.1 seconds
		
 		choice = wgetch(menuwin);
		if(signal(SIGWINCH, sig_handler) == SIG_ERR) 
			printw("signal error");
	

				
  		if (signal_) {
			endwin();
			refresh();
			clear();
			getmaxyx(stdscr, yMax, xMax);
			menuwin = newwin(5, 15, (yMax/2.5), (xMax/2.15));
			titleMenu(menuwin);
			keypad(menuwin, true);
			borderMenu(chatmenu, yMax, xMax);
		}
		
		
	
		for(int i = 0; i < 3; i++) {
			if(i == highlight)
			wattron(menuwin, A_REVERSE);
			mvwprintw(menuwin, i+2, 1, options[i]);
			wattroff(menuwin, A_REVERSE);
		}
	//choice = wgetch(menuwin);

		switch (choice)
		{
			case KEY_UP:
				highlight--;
				if(highlight == -1)
					highlight = 0;
				break;
			case KEY_DOWN:
				highlight++;
				if(highlight == 3)
					highlight = 2;
				break;
			default:
				break;
		}
		
		if(choice == 10 && highlight == 0) {
			
			endwin();
			clear();
			werase(menuwin);
			werase(chatmenu);
			getmaxyx(stdscr, yMax, xMax);
			WINDOW* chatusers = newwin(LINES * .4, COLS * .126, yMax-yMax, xMax-xMax);
			WINDOW* chatroom = newwin(LINES * .7, COLS * .50, (yMax*.10) , xMax/4);
			WINDOW* chatbox = newwin(LINES * .125, COLS * .50, yMax * .80, xMax/4);
			keypad(stdscr, true);
			box(chatusers, 0, 0);
			box(chatroom, 0, 0);
			box(chatbox, 0, 0);
			
			refresh();
			wrefresh(chatusers);
			wrefresh(chatroom);
			wrefresh(chatbox);

			printw("LINES: %d, COLS %d",yMax, xMax);
		//	char key_stroke = wgetch(chatbox);
			echo();
			keypad(chatbox, true);
			char buff[4096];
			while(1) {
				wmove(chatbox, 1, 1);
				chat_Getline(chatbox, buff, (LINES * .125), (COLS * .50));	
			}
		}
		if(choice == 10 && highlight == 1) {
			printw("1");

		}
		if(choice == 10 && highlight == 2) {
			printw("2");

		}
	}



	
	wrefresh(menuwin);
	endwin();
	return 0;

}
