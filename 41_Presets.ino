void presets(int8_t presetNumber)
{
    int8_t PresetNumber = presetNumber;

    //清除所有按钮状态
    for (int i = 0; i < BUTTONCOUNT; i++)
    {
        Joystick.setButton(i, 0);
    }

    //清除所有按钮模式
    for (int i = 0; i < rowCount; i++)
    {
        for (int a = 0; a < colCount; a++)
        {
            switchMode[i][a] = 0;
        }
    }
    for (int i = 0; i < 4; i++)
    {
        analogSwitchMode1[i] = 0;
        analogSwitchMode2[i] = 0;
    }

    //设定默认值

    bitePoint = 300;
    brakeMagicValue = 50;
    throttleHoldValue = 1000;
    leftQuickValue = 11; //左12档开关快速切换到12
    rightQuickValue = 11; //右12档开关快速切换到12
    switchMode[3][3] = 1; //默认拨片为独立模式
    analogSwitchMode2[2] = 1; //默认离合模式3，分离单离合，同时按下后激活双离合

    //----------------------------------------
    //------------在下方输入预设值-------------
    //----------------------------------------

    switch (presetNumber)
    {
        case 0: //预设 1 - iRacing PORSCHE 911 CUP CAR
            switchMode[5][2] = 1;
            bitePoint = 265;
            brakeMagicValue = 200;
            break;
        case 1: //预设 2 - iRacing PORSCHE 911 GT3
            switchMode[5][2] = 1;
            bitePoint = 300;
            break;
        case 2: //预设 3 - iRacing FERRARI 488 GT3
            switchMode[5][2] = 1;
            bitePoint = 500;
            break;
        case 3: //预设 4 - iRacing MERCEDES AMG GT3
            switchMode[5][2] = 1;
            bitePoint = 200;
            switchMode[4][2] = 1;
            break;
        case 4: //预设 5 - iRacing F3
            switchMode[5][2] = 1;
            break;
        case 5: //预设 6 - iRacing F4
            switchMode[5][2] = 1;
            break;
        case 6: //预设 7 - iRacing MERCEDES W12
            switchMode[5][2] = 1;
            break;
        case 7: //预设 8 - iRacing RADICAL SR10
            switchMode[5][2] = 1;
            break;
        case 8: //预设 9 - ACC MERCEDES AMG GT3
            break;
        case 9: //预设 10 - ACC LAMBORGHINI HURACAN
            break;
        case 10: //预设 11 - ACC FERRARI 488 GT3
            break;
        case 11: //预设 12 - F1 2022
            break;
    }
}
