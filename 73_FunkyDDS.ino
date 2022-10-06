//DDS模式（2层24档位）的摇杆旋转编码器函数
void DDSfunky(int Arow, int Acol, int Bcol)
//参数为编码器所在行号，A针脚列号，B针脚列号
{

    int Row = Arow - 1;
    int Column = Acol - 1;
    int Number = buttonNumber[Row][Column];
    if (latchState[ddButtonRow - 1][ddButtonCol - 1])
    {
        Number = Number + 12;
    }
    int FieldPlacement = 8;

    int bCol = Bcol - 1;

    //读取开关模式
    toggleTimer[Row][bCol] = switchMode[Row][bCol] << 1 | switchMode[Row][Column];

    //标准模式
    if (!rawState[Row][Column] && !rawState[Row][bCol])
    {
        pushState[Row][Column] = 1; //读取到 00
    }
    else if (!rawState[Row][Column] && rawState[Row][bCol])
    {
        pushState[Row][Column] = 2;
        latchLock[Row][Column] = 1; //读取到 01
    }
    else if (rawState[Row][Column] && rawState[Row][bCol])
    {
        pushState[Row][Column] = 3; //读取到 11
    }
    else if (rawState[Row][Column] && !rawState[Row][bCol])
    {
        pushState[Row][Column] = 4;
        latchLock[Row][bCol] = 1; //读取到 10
    }

    if ((globalClock - switchTimer[Row][Column] > funkyCooldown) && (globalClock - switchTimer[Row][bCol] > funkyCooldown))
    {
        if ((latchLock[Row][bCol] && pushState[Row][Column] == 1) || (latchLock[Row][Column] && pushState[Row][Column] == 3)) //顺时针旋转
        {
            switchTimer[Row][Column] = globalClock;

            if (!toggleTimer[Row][bCol] == 1)
            {
                toggleTimer[Row][Column] --; //计算旋转档位数
            }
            switchModeLock[Row][bCol] = !switchModeLock[Row][bCol]; //切换到模式4
            if (pushState[modButtonRow - 1][modButtonCol - 1] == 1) //切换模式状态
            {
                for (int i = 0; i < 24; i++) //重设所有按钮状态
                {
                    Joystick.releaseButton(i + buttonNumber[Row][Column]);
                }
                Joystick.releaseButton(buttonNumber[Row][bCol]);
                Joystick.releaseButton(1 + buttonNumber[Row][bCol]);
                latchLock[ddButtonRow - 1][ddButtonCol - 1] = false; //重设DD按钮
                latchState[ddButtonRow - 1][ddButtonCol - 1] = false;
                toggleTimer[Row][bCol] --; //模式计数器
                if (toggleTimer[Row][bCol] == 1) //跳过封闭式DDS模式
                {
                    toggleTimer[Row][bCol] = 0;
                }
            }
        }

        else if ((latchLock[Row][bCol] && pushState[Row][Column] == 3) || (latchLock[Row][Column] && pushState[Row][Column] == 1)) //逆时针旋转
        {
            switchTimer[Row][bCol] = globalClock;

            if (!toggleTimer[Row][bCol] == 1)
            {
                toggleTimer[Row][Column] ++;
            }
            switchModeLock[Row][Column] = !switchModeLock[Row][Column]; //切换到模式4
            if (pushState[modButtonRow - 1][modButtonCol - 1] == 1) //切换模式状态
            {
                for (int i = 0; i < 24; i++) //重设所有按钮状态
                {
                    Joystick.releaseButton(i + buttonNumber[Row][Column]);
                }
                Joystick.releaseButton(buttonNumber[Row][bCol]);
                Joystick.releaseButton(1 + buttonNumber[Row][bCol]);
                latchLock[ddButtonRow - 1][ddButtonCol - 1] = false; //重设DD按钮
                latchState[ddButtonRow - 1][ddButtonCol - 1] = false;
                toggleTimer[Row][bCol] ++; //模式计数器
                if (toggleTimer[Row][bCol] == 1) //跳过封闭式DDS模式
                {
                    toggleTimer[Row][bCol] = 2;
                }
            }
        }
    }
    else
    {
        latchLock[Row][bCol] = 0;
        latchLock[Row][Column] = 0;
    }

    //保证计数器在限定范围内计数
    if (toggleTimer[Row][Column] > 11)
    {
        toggleTimer[Row][Column] = 0;
    }
    else if (toggleTimer[Row][Column] < 0)
    {
        toggleTimer[Row][Column] = 11;
    }
    if (toggleTimer[Row][bCol] > 3)
    {
        toggleTimer[Row][bCol] = 0;
        latchState[hybridButtonRow - 1][hybridButtonCol - 1] = 0; //进入这个模式1后重设混合按钮
    }
    else if (toggleTimer[Row][bCol] < 0)
    {
        toggleTimer[Row][bCol] = 3;
    }
    if (toggleTimer[Row][bCol] == 0 && latchState[hybridButtonRow - 1][hybridButtonCol - 1])
    {
        toggleTimer[Row][bCol] = 1;
    }
    else if (toggleTimer[Row][bCol] == 1 && !latchState[hybridButtonRow - 1][hybridButtonCol - 1])
    {
        toggleTimer[Row][bCol] = 0;
    }


    //模式1：开放式DDS模式
    if (toggleTimer[Row][bCol] == 0)
    {

        if (latchState[ddButtonRow - 1][ddButtonCol - 1])
        {
            for (int i = 0; i < 12; i++) //清楚模式1下的所有按钮状态
            {
                Joystick.releaseButton(i + buttonNumber[Row][Column]);
            }
        }
        for (int i = 0; i < 24; i++) //按下对应档位的按钮
        {
            if (i == toggleTimer[Row][Column])
            {
                Joystick.pressButton(i + Number);
            }
            else
            {
                Joystick.releaseButton(i + Number);
            }
        }
    }

    //模式2、3：增量编码器

    if ((toggleTimer[Row][bCol] == 1 || toggleTimer[Row][bCol] == 2) && !biteButtonBit1 && !biteButtonBit2)
    {
        Number = buttonNumber[Row][bCol];
        Joystick.setButton(Number, (globalClock - switchTimer[Row][Column] < funkyPulse));
        Joystick.setButton(Number + 1, (globalClock - switchTimer[Row][bCol] < funkyPulse));
    }

    //模式4：切换开关

    if (toggleTimer[Row][bCol] == 3)
    {
        Number = buttonNumber[Row][bCol];
        Joystick.setButton(Number, switchModeLock[Row][bCol]);
        Joystick.setButton(Number + 1, switchModeLock[Row][Column]);;
    }

    //传递模式值给编码器位字段

    switchMode[Row][Column] = (toggleTimer[Row][bCol] & B00000001);
    switchMode[Row][bCol] = (toggleTimer[Row][bCol] & B00000010) >> 1;

    long push = 0;
    push = push | toggleTimer[Row][bCol];
    push = push << (2 * (FieldPlacement - 1));
    rotaryField = rotaryField | push;
}
