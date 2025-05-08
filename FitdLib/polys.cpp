#include "common.h"

void hline(int x1, int x2, int y, unsigned char c);
void line(int x1, int y1, int x2, int y2, unsigned char c);
void pixel(int x, int y, unsigned char c);

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define MAX_PTS 10

#define put_dot(x,y) if ((y >= 0) && (y < _SCREEN_INTERNAL_HEIGHT)) dots[y][counters[y]++] = x;

void swapFunc(int* a, int* b);
#define swap(a,b) (swapFunc(&a,&b))

void fillpoly(s16* datas, int n, unsigned char c)
{
	static int dots[_SCREEN_INTERNAL_HEIGHT][MAX_PTS];
	static int counters[_SCREEN_INTERNAL_HEIGHT];
	s16 x1, y1, x2, y2;
	int i, j, k, dir = -2;
	double step, curx;

	if (n <= 2) {
		switch (n) {
			case 0:
				return;
			case 1:
				pixel(datas[0], datas[1], c);
				return;
			case 2:
				line(datas[0], datas[1], datas[2], datas[3], c);
				return;
		}
	}

	// Reinit array counters

	for (i = 0; i < _SCREEN_INTERNAL_HEIGHT; i++) {
		counters[i] = 0;
	}

	// Drawing lines

	x2 = datas[n * 2 - 2];
	y2 = datas[n * 2 - 1];

	for (i = 0; i < n; i++) {
		x1 = x2;
		y1 = y2;
		x2 = datas[i * 2];
		y2 = datas[i * 2 + 1];

		// line(x1, y1, x2, y2, c);
		// continue;

		if (y1 == y2) {
			// printf("Horizontal line. x1: %i, y1: %i, x2: %i, y2: %i\n", x1, y1, x2, y2);
			if (!dir) continue;
			put_dot(x1, y1);
			dir = 0;
			continue;
		}

		step = (double)(x2 - x1) / (y2 - y1);

		// printf("x1: %i, y1 = %i, x2 = %i, y2 = %i, step: %f\n", x1, y1, x2, y2, step);

		curx = x1;

		if (y1 < y2) {
			for (j = y1; j < y2; j++, curx += step) {
				// printf("j = %i, curx = %f\n", j, curx);
				put_dot((int)(curx + 0.5), j);
			}
			if (dir == -1) {
				// printf("Adding extra (%i, %i)\n", x1, y1);
				put_dot(x1, y1);
			}
			dir = 1;
		} else {
			for (j = y1; j > y2; j--, curx -= step) {
				// printf("j = %i, curx = %f\n", j, curx);
				put_dot((int)(curx + 0.5), j);
			}
			if (dir == 1) {
				// printf("Adding extra (%i, %i)\n", x1, y1);
				put_dot(x1, y1);
			}
			dir = -1;
		}
	}

	x1 = x2;
	y1 = y2;
	x2 = datas[0];
	y2 = datas[1];

	if (((y1 < y2) && (dir == -1)) || ((y1 > y2) && (dir == 1)) || ((y1 == y2) && (dir == 0))) {
		// printf("Adding final extra (%i, %i)\n", x1, y1);
		put_dot(x1, y1);
	}

	// NOTE: all counters should be even now. If not, this is a bad (C) thing :-P

	// Sorting datas

	for (i = 0; i < _SCREEN_INTERNAL_HEIGHT; i++) {
		// Very bad sorting... but arrays are very small (0, 2 or 4), so it's no quite use...
		for (j = 0; j < (counters[i] - 1); j++) {
			for (k = 0; k < (counters[i] - 1); k++) {
				if (dots[i][k] > dots[i][k + 1])
					swap(dots[i][k], dots[i][k + 1]);
			}
		}
	}

	// Drawing.

	for (i = 0; i < _SCREEN_INTERNAL_HEIGHT; i++) {
		if (counters[i]) {
			// printf("%i dots on line %i\n", counters[i], i);
			for (j = 0; j < counters[i] - 1; j += 2) {
				// printf("Drawing line (%i, %i)-%i\n", dots[i][j], dots[i][j + 1], i);
				hline(dots[i][j], dots[i][j + 1], i, c);
#ifdef DEBUGGING_POLYS
				if ((!dots[i][j]) || !(dots[i][j + 1])) {
					printf("fillpoly: BLARGH!\n");
					FITD_throwFatal(); // assert(0);
				}
#endif
			}
		}
	}
}

#undef MAX_PTS
#undef put_dot
#undef swap
