//摇杆旋转编码器函数
void funkyRotary(int Arow, int Acol, int Bcol) 
//参数为编码器所在行号，A针脚列号，B针脚列号
{

    int Row = Arow - 1;
    int Column = Acol - 1;
    int Number = buttonNumber[Row][Column];

    int bCol = Bcol - 1;

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
        if ((latchLock[Row][bCol] && pushState[Row][Column] == 1) || (latchLock[Row][Column] && pushState[Row][Column] == 3))
        {
            switchTimer[Row][Column] = globalClock;  //读取到00时触发A针脚方向（顺时针）
        }

        else if ((latchLock[Row][bCol] && pushState[Row][Column] == 3) || (latchLock[Row][Column] && pushState[Row][Column] == 1))
        {
            switchTimer[Row][bCol] = globalClock;  //读取到11时触发B针脚方向（逆时针）
        }
    }

    else
    {
        latchLock[Row][bCol] = 0;
        latchLock[Row][Column] = 0;
    }

    Joystick.setButton(Number, (globalClock - switchTimer[Row][Column] < funkyPulse)); //A方向旋转
    Joystick.setButton(Number + 1, (globalClock - switchTimer[Row][bCol] < funkyPulse)); //B方向旋转

}

//DDS模式的摇杆编码器，实现方式同funkyRotary()，加入DDS后相当于可以在两个不同的编码器功能之间切换
void DDSfunky(int Arow, int Acol, int Bcol) {

    int Row = Arow - 1;
    int Column = Acol - 1;
    int Number = buttonNumber[Row][Column];
    if (latchState[ddButtonRow - 1][ddButtonCol - 1])  //DD按钮按下时切换成第二个编码器控制
    {
        Number = Number + 12;
    }
    int FieldPlacement = 8;

    int bCol = Bcol - 1;

    toggleTimer[Row][bCol] = switchMode[Row][bCol] << 1 | switchMode[Row][Column];

    if (!rawState[Row][Column] && !rawState[Row][bCol])
    {
        pushState[Row][Column] = 1;
    }
    else if (!rawState[Row][Column] && rawState[Row][bCol])
    {
        pushState[Row][Column] = 2;
        latchLock[Row][Column] = 1; 
    }
    else if (rawState[Row][Column] && rawState[Row][bCol])
    {
        pushState[Row][Column] = 3;
    }
    else if (rawState[Row][Column] && !rawState[Row][bCol])
    {
        pushState[Row][Column] = 4;
        latchLock[Row][bCol] = 1; 
    }

    if ((globalClock - switchTimer[Row][Column] > funkyCooldown) && (globalClock - switchTimer[Row][bCol] > funkyCooldown))
    {
        if ((latchLock[Row][bCol] && pushState[Row][Column] == 1) || (latchLock[Row][Column] && pushState[Row][Column] == 3)) //CLOCKWIZE TURN
        {
            switchTimer[Row][Column] = globalClock;

            if (pushState[biteButtonRow - 1][biteButtonCol - 1] && !biteButtonBit1 && !biteButtonBit2) //Engage in bite mode
            {
                biteButtonBit1 = true;
            }
            else if (!(!biteButtonBit1 && !biteButtonBit2)) 
            {
                if (biteButtonBit1 && !biteButtonBit2)
                {
                    bitePoint = bitePoint - 100;
                    if (bitePoint < 0)
                    {
                        bitePoint = 1000;
                    }
                }
                else if (!biteButtonBit1 && biteButtonBit2)
                {
                    bitePoint = bitePoint - 10;
                    if (bitePoint < 0)
                    {
                        bitePoint = 1000;
                    }
                }
                else if (biteButtonBit1 && biteButtonBit2)
                {
                    bitePoint = bitePoint - 1;
                    if (bitePoint < 0)
                    {
                        bitePoint = 1000;
                    }
                }
            }
            else
            {
                if (!toggleTimer[Row][bCol] == 1)
                {
                    toggleTimer[Row][Column] --;
                }
                switchModeLock[Row][bCol] = !switchModeLock[Row][bCol];
                if (pushState[modButtonRow - 1][modButtonCol - 1] == 1)
                {
                    for (int i = 0; i < 24; i++)
                    {
                        Joystick.releaseButton(i + buttonNumber[Row][Column]);
                    }
                    Joystick.releaseButton(buttonNumber[Row][bCol]);
                    Joystick.releaseButton(1 + buttonNumber[Row][bCol]);
                    latchLock[ddButtonRow - 1][ddButtonCol - 1] = false;
                    latchState[ddButtonRow - 1][ddButtonCol - 1] = false;
                    toggleTimer[Row][bCol] --; 
                    if (toggleTimer[Row][bCol] == 1) 
                    {
                        toggleTimer[Row][bCol] = 0;
                    }
                }
            }
        }

        else if ((latchLock[Row][bCol] && pushState[Row][Column] == 3) || (latchLock[Row][Column] && pushState[Row][Column] == 1)) //COUNTER CLOCKWIZE TURN
        {
            switchTimer[Row][bCol] = globalClock;

            if (pushState[biteButtonRow - 1][biteButtonCol - 1] && !biteButtonBit1 && !biteButtonBit2) //Engage in bite mode
            {
                biteButtonBit1 = true;
            }
            else if (!(!biteButtonBit1 && !biteButtonBit2)) 
            {
                if (biteButtonBit1 && !biteButtonBit2)
                {
                    bitePoint = bitePoint + 100;
                    if (bitePoint > 1000)
                    {
                        bitePoint = 1000;
                    }
                    else if (bitePoint == 1000)
                    {
                        bitePoint = 0;
                    }
                }
                else if (!biteButtonBit1 && biteButtonBit2)
                {
                    bitePoint = bitePoint + 10;
                    if (bitePoint > 1000)
                    {
                        bitePoint = 1000;
                    }
                    else if (bitePoint == 1000)
                    {
                        bitePoint = 0;
                    }
                }
                else if (biteButtonBit1 && biteButtonBit2)
                {
                    bitePoint = bitePoint + 1;
                    if (bitePoint > 1000)
                    {
                        bitePoint = 1000;
                    }
                    else if (bitePoint == 1000)
                    {
                        bitePoint = 0;
                    }
                }
            }
            else
            {
                if (!toggleTimer[Row][bCol] == 1)
                {
                    toggleTimer[Row][Column] ++;
                }
                switchModeLock[Row][Column] = !switchModeLock[Row][Column];
                if (pushState[modButtonRow - 1][modButtonCol - 1] == 1)
                {
                    for (int i = 0; i < 24; i++)
                    {
                        Joystick.releaseButton(i + buttonNumber[Row][Column]);
                    }
                    Joystick.releaseButton(buttonNumber[Row][bCol]);
                    Joystick.releaseButton(1 + buttonNumber[Row][bCol]);
                    latchLock[ddButtonRow - 1][ddButtonCol - 1] = false;
                    latchState[ddButtonRow - 1][ddButtonCol - 1] = false;
                    toggleTimer[Row][bCol] ++; 
                    if (toggleTimer[Row][bCol] == 1)
                    {
                        toggleTimer[Row][bCol] = 2;
                    }
                }
            }
        }
    }
    else
    {
        latchLock[Row][bCol] = 0;
        latchLock[Row][Column] = 0;
    }

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
        latchState[hybridButtonRow - 1][hybridButtonCol - 1] = 0;
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
            for (int i = 0; i < 12; i++)
            {
                Joystick.releaseButton(i + buttonNumber[Row][Column]);
            }
        }
        for (int i = 0; i < 24; i++)
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

    //模式2和3：增量编码器

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

    //推送开关状态给编码器位字段

    switchMode[Row][Column] = (toggleTimer[Row][bCol] & B00000001);
    switchMode[Row][bCol] = (toggleTimer[Row][bCol] & B00000010) >> 1;

    long push = 0;
    push = push | toggleTimer[Row][bCol];
    push = push << (2*(FieldPlacement - 1));  //注意此处位字段的位置与模式的位置不同，所有的DDS模式公用15 16位
    rotaryField = rotaryField | push;
}