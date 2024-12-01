extern void memset(void *Dst, int Val, unsigned int Size);
extern void Log(char * buffer);
typedef struct short_array
{
    short array[8];
} short_array;

extern short_array* lbl_80228F80;


void MainLoop() 
{
    do {

    } while (true);
}

/* 80014180-80014344       .text main */
void main()
{
    memset(lbl_80228F80, 0xff, 18);
    //array[8] = (short)0;
    if (&lbl_80228F80[8] == (short)0)
    {
        Log("Not a valid demo.  Setting default parameters.\n");
    }
    else {
        Log("Using SCE demo parameters.\n");
    }

    MainLoop();
}
