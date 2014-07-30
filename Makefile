study: study.c dialog.c dialog.h
	cc study.c dialog.c -o study -lncursesw -Wall -Werror -pedantic -std=gnu99 -g
