#include "scanf.h"
#include <cstdarg>

static int __sformatter(int (*ReadProc)(void *, int, int), void * ReadProcArg,
                                            const char * format_str, va_list arg, int is_secure)
{

}

int __StringRead(void* pPtr, int ch, int act) {
    char ret;
    __InStrCtrl* Iscp = (__InStrCtrl*)pPtr;

    switch (act) {
    case __GetAChar:
        ret = *(Iscp->NextChar);

        if (ret == '\0') {
            Iscp->NullCharDetected = 1;
            return -1;
        } else {
            Iscp->NextChar++;
            return (unsigned char)ret;
        }

    case __UngetAChar:
        if (Iscp->NullCharDetected == 0) {
            Iscp->NextChar--;
        } else {
            Iscp->NullCharDetected = 0;
        }

        return ch;

    case __TestForError:
        return Iscp->NullCharDetected;
    }

    return 0;
}

inline int vsscanf(const char *s, const char *format, va_list arg) {
    __InStrCtrl isc;
    isc.NextChar = (char*)s;

    if ((s == 0) || (*isc.NextChar == '\0')) {
        return -1;
    }

    isc.NullCharDetected = 0;
    return __sformatter(&__StringRead, (void*)&isc, format, arg, 0);
}

int sscanf(const char* str, const char* format, ...) {
    va_list args;
    int done;

    va_start (args, format);
    return vsscanf(str, format, args);
}
