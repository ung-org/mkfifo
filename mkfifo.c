/*
 * UNG's Not GNU
 *
 * Copyright (c) 2019, Jakob Kaivo <jkk@ung.org>
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

#define _POSIX_C_SOURCE 2
#include <errno.h>
#include <locale.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

static mode_t translate_mode(const char *s)
{
	char *end = NULL;
	mode_t mode = (mode_t)strtol(s, &end, 8);

	if (*end == '\0') {
		return mode;
	}

	return 0;
}

static int mk_fifo(char *path, mode_t mode)
{
	if (mkfifo(path, mode) != 0) {
		fprintf(stderr, "mkfifo: %s: %s\n", path, strerror(errno));
		return 1;
	}

	return 0;
}

int main(int argc, char *argv[])
{
	setlocale(LC_ALL, "");

	mode_t mode = S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH | S_IWOTH;

	int c;
	while ((c = getopt(argc, argv, "m:p")) != -1) {
		switch (c) {
		case 'm':
			mode = translate_mode(optarg);
			break;

		default:
			return 1;
		}
	}

	if (optind >= argc) {
		fprintf(stderr, "mkfifo: missing operand\n");
		return 1;
	}

	int ret = 0;
	do {
		ret |= mk_fifo(argv[optind], mode);
	} while (++optind < argc);

	return ret;
}
