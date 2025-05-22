#ifndef _DOLPHIN_DBINTERFACE_H_
#define _DOLPHIN_DBINTERFACE_H_

#include <dolphin/os.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct DBInterface
{
    u32     bPresent;
    u32     exceptionMask;
    void    (*ExceptionDestination) ( void );
    void    *exceptionReturn;
} DBInterface;

extern DBInterface* __DBInterface;

void DBInit(void);
void DBInitComm(int* inputFlagPtr, int* mtrCallback);
int DBInitInterrupts(void);
int DBQueryData(void);
int DBRead(void* param1, u32 param2);
int DBWrite(const void* data, u32 size);
int DBOpen(void);
int DBClose(void);
void __DBExceptionDestination(void);
void __DBExceptionDestinationAux(void);
BOOL __DBIsExceptionMarked(__OSException exception);
void __DBMarkException(u8 exception, int value);
void __DBSetPresent(u32 value);

#ifdef __cplusplus
}
#endif

#endif
