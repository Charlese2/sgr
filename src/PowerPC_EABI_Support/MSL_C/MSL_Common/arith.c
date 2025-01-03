#include "arith.h"

int abs(int n) {
    if (n < 0)
        return (-n);
    else
        return (n);
}

long labs(long x) {
	if (x < 0)
		return -x;
	else
		return x;
}

div_t div(int numerator, int denominator) {
    div_t ret;
    int i = 1;
    int j = 1;

    if (numerator < 0 ) {
        numerator = -numerator;
        i = -1;
    }

    if (denominator < 0) {
        denominator = -denominator;
        j = -1;
    }

    ret.quot = (numerator / denominator) * (i * j);
    ret.rem = numerator * i - j * (ret.quot * denominator);
    return ret;
}
