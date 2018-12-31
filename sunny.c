#include <ncurses.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <time.h>
#include <unistd.h>

#define LINES 52
#define COLUMNS 187

#define MENUS_HEIGHT 8
#define SUN_HEIGHT (LINES - MENUS_HEIGHT)
#define SUN_WIDTH (COLUMNS)

char *choices[] = {
			"Choice 1",
			"Choice 2",
			"Choice 3",
			"Choice 4",
			"Exit",
		  };

int n_choices = sizeof(choices) / sizeof(char *);
struct tm* get_time();
void print_menu(WINDOW *menu_win, int highlight, char **choices);
void sunrise(WINDOW *sun_win);
void sunset(WINDOW *sun_win);

void sunrise(WINDOW *sun_win)
{
	init_pair(1, COLOR_BLACK, COLOR_WHITE);
	bkgd(COLOR_PAIR(1));
}

void sunset(WINDOW *sun_win)
{
	init_pair(2, COLOR_WHITE, COLOR_BLACK);
	bkgd(COLOR_PAIR(2));
}

void print_menu(WINDOW *menu_win, int highlight, char **choices)
{
	int x, y, i;

	x = 2;
	y = 2;
	box(menu_win, 0, 0);
	for(i = 0; i < n_choices; ++i)
	{	if(highlight == i + 1) /* Highlight the present choice */
		{	wattron(menu_win, A_BOLD);
			mvwprintw(menu_win, y, x, "%s", choices[i]);
			wattroff(menu_win, A_BOLD);
		}
		else
			mvwprintw(menu_win, y, x, "%s", choices[i]);
		++y;
	}
	wrefresh(menu_win);
}

struct tm* get_time()
{
	time_t t;
	time(&t);
	struct tm *timeinfo = localtime(&t);
	timeinfo->tm_mon += 1;
	return timeinfo;
}

int main()
{
	WINDOW *sun_win;

	// setup
	initscr();
	clear();
	noecho();
	curs_set(0);
	cbreak();
	if(!has_colors())
		goto STOP;
	start_color();

	int startx = 0;
	int starty = MENUS_HEIGHT;
	sun_win = newwin(SUN_HEIGHT, SUN_WIDTH, starty, startx);
	mvprintw(SUN_HEIGHT/2, SUN_WIDTH/2, "WAKE UP NEIL");

	sunrise(sun_win);
	pid_t pid = fork();
	if(pid == 0) {
		printf("???");
		execl("/bin/bash", "-c", "/usr/bin/xdg-open",
		      "./honey.mp3", (char *)NULL);
		exit(0);
	}
	if(pid > 0) {
		waitpid(-1, NULL, 0);
	}
	int c = getch();
	sunset(sun_win);

STOP:
	endwin();
	refresh();
	return 0;
}

/*
int main()
{	WINDOW *menu_win;
	int highlight = 1;
	int choice = 0;
	int c;

	initscr();
	clear();
	noecho();
	curs_set(0);
	cbreak();
	startx = (COLUMNS - WIDTH) / 2;
	starty = (LINES - HEIGHT) / 2;

	if(!has_colors()) 
		goto STOP;

	menu_win = newwin(HEIGHT, WIDTH, starty, startx);
	keypad(menu_win, TRUE);
	mvprintw(0, 0, "Use arrow keys to go up and down, Press enter to select a choice");
	refresh();
	print_menu(menu_win, highlight);
	while(1)
	{	
		//c = wgetch(menu_win);
		//nodelay(menu_win, TRUE);
		wtimeout(menu_win, 1);
		c = wgetch(menu_win);
		struct tm *time = get_time();
		mvprintw(0, 0, "Time: %d %d %d",
			 time->tm_hour, time->tm_min, time->tm_sec);
		refresh();
		switch(c)
		{	case KEY_UP:
				if(highlight == 1)
					highlight = n_choices;
				else
					--highlight;
				break;
			case KEY_DOWN:
				if(highlight == n_choices)
					highlight = 1;
				else
					++highlight;
				break;
			case 10:
				choice = highlight;
				break;
			default:
				if(c != -1) {
					mvprintw(24, 0, "Charcter pressed is = %3d Hopefully it can be printed as '%c'", c, c);
					refresh();
				}
				break;
		}
		print_menu(menu_win, highlight);
		if(choice != 0)
			break;
	}
	mvprintw(23, 0, "You chose choice %d with choice string %s\n", choice, choices[choice - 1]);
	start_color();
	init_pair(1, COLOR_BLACK, COLOR_WHITE);
	bkgd(COLOR_PAIR(1));
	refresh();

	while(wgetch(menu_win) != 10) {
	}

	clrtoeol();
	refresh();

STOP:
	endwin();

	pid_t pid = fork();
	if(pid == 0) {
		printf("???");
		execl("/bin/bash", "-c", "/usr/bin/xdg-open",
		      "./honey.mp3", (char *)NULL);
		exit(0);
	}
	if(pid > 0) {
		waitpid(-1, NULL, 0);
	}
	return 0;
}
*/
/*
int main()
{	WINDOW *menu_win;
	int highlight = 1;
	int choice = 0;
	int c;

	initscr();
	clear();
	noecho();
	curs_set(0);
	cbreak();
	startx = (COLUMNS - WIDTH) / 2;
	starty = (LINES - HEIGHT) / 2;

	if(!has_colors()) 
		goto STOP;

	menu_win = newwin(HEIGHT, WIDTH, starty, startx);
	keypad(menu_win, TRUE);
	mvprintw(0, 0, "Use arrow keys to go up and down, Press enter to select a choice");
	refresh();
	print_menu(menu_win, highlight);
	while(1)
	{	
		//c = wgetch(menu_win);
		//nodelay(menu_win, TRUE);
		wtimeout(menu_win, 1);
		c = wgetch(menu_win);
		struct tm *time = get_time();
		mvprintw(0, 0, "Time: %d %d %d",
			 time->tm_hour, time->tm_min, time->tm_sec);
		refresh();
		switch(c)
		{	case KEY_UP:
				if(highlight == 1)
					highlight = n_choices;
				else
					--highlight;
				break;
			case KEY_DOWN:
				if(highlight == n_choices)
					highlight = 1;
				else
					++highlight;
				break;
			case 10:
				choice = highlight;
				break;
			default:
				if(c != -1) {
					mvprintw(24, 0, "Charcter pressed is = %3d Hopefully it can be printed as '%c'", c, c);
					refresh();
				}
				break;
		}
		print_menu(menu_win, highlight);
		if(choice != 0)
			break;
	}
	mvprintw(23, 0, "You chose choice %d with choice string %s\n", choice, choices[choice - 1]);
	start_color();
	init_pair(1, COLOR_BLACK, COLOR_WHITE);
	bkgd(COLOR_PAIR(1));
	refresh();

	while(wgetch(menu_win) != 10) {
	}

	clrtoeol();
	refresh();

STOP:
	endwin();

	pid_t pid = fork();
	if(pid == 0) {
		printf("???");
		execl("/bin/bash", "-c", "/usr/bin/xdg-open",
		      "./honey.mp3", (char *)NULL);
		exit(0);
	}
	if(pid > 0) {
		waitpid(-1, NULL, 0);
	}
	return 0;
}
*/
