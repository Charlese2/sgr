void ReadAIDQueue()
{
    
}

/* 8019e988-8019ea48        .text fn_8019e988 */
bool fn_8019e988(unk * param_1, int param_2) 
{
    bool level;

    level = OSDisableInterrupts();
    if (param_2 == 2) {
        param_1->field311_0x140 = 0;
        param_1->field312_0x142 = 0;
    }
    else if (param_2 < 2) {
        if (param_2 == 0) {
            param_1->field311_0x140 = 2;
        }
        else if (-1 < param_2) {
            param_1->field311_0x140 = 1;
        }
    }
    else if (param_2 == 4) {
        param_1->field311_0x140 = 0;
        param_1->field312_0x142 = 2;
    }
    else if (param_2 < 4) {
        param_1->field311_0x140 = 0;
        param_1->field312_0x142 = 1;
    }
    param_1->SomeStatus = param_1->SomeStatus | 1;
    level = OSRestoreInterrupts(level);
    return level;
}
