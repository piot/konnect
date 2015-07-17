/*
The MIT License (MIT)

Copyright (c) 2015 Peter Bjorklund

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.
*/
#include <stdarg.h>
#include <stdio.h>

static void internal_log(const char *format, va_list ap)
{
	char	buf[1024];

	if (
#ifdef KONNECT_OS_WINDOWS
	_vsnprintf(buf, sizeof(buf), format, ap)
#else
	vsnprintf(buf, sizeof(buf), format, ap)
#endif
	< 0) {
		buf[sizeof(buf) - 1] = '\0';
	}

#ifdef KONNECT_OS_WINDOWS
	OutputDebugString(buf);
	OutputDebugString("\n");
#else
	fprintf(stdout, "%s\n", buf);
	fflush(stdout);
#endif
}

void konnect_log(const char *format, ...)
{
	va_list argument_list;

	va_start(argument_list, format);
	internal_log(format, argument_list);
	va_end(argument_list);
}
