/*

原作者设计这个函数是用于Bourns品牌PEC11型编码器的，这款编码器国内不好买到，但它与常见的EC11、EC12、EC16型编码器电气性能是一样的，可以完美匹配

*/

void PEC11(int row, int col, bool reverse)  //参数为行、列号，是否反向旋转
{
    int Row = row - 1;
    int Column = col - 1;
    int Number = buttonNumber[Row][Column];

    int Reverse = reverse;

    switchTimer[Row][Column + 1] = (rawState[Row][Column] | rawState[Row][Column + 1] << 1); //暂存编码器2个针脚状态读数（0-3）

    if //开始旋转，但是还没有完成一个周期时
        (switchTimer[Row][Column + 1] > 0
            &&
            (globalClock - switchTimer[Row][Column] > PEC11Cooldown)
            &&
            pushState[Row][Column] == 0
            &&
            pushState[Row][Column + 1] == 0)
    {
        switchTimer[Row][Column] = globalClock; //更新计时器
        if (switchTimer[Row][Column + 1] == 2) //顺时针旋转
        {
            pushState[Row][Column] = 1;
        }
        else if (switchTimer[Row][Column + 1] == 1) //逆时针旋转
        {
            pushState[Row][Column + 1] = 1;
        }
    }

    //顺时针周期
    if (pushState[Row][Column] == 1 && rawState[Row][Column])
    {
        pushState[Row][Column] = 2;
    }
    if (pushState[Row][Column] == 2 && switchTimer[Row][Column + 1] == 0)
    {
        pushState[Row][Column] = 3;
    }

    //逆时针周期
    if (pushState[Row][Column + 1] == 1 && rawState[Row][Column + 1])
    {
        pushState[Row][Column + 1] = 2;
    }
    if (pushState[Row][Column + 1] == 2 && switchTimer[Row][Column + 1] == 0)
    {
        pushState[Row][Column + 1] = 3;
    }

    //推送成功完成的旋转周期数

    if (pushState[Row][Column] == 3)
    {
        toggleTimer[Row][Column] = globalClock;  //顺时针到3计算一个周期
    }
    else if (pushState[Row][Column + 1] == 3)
    {
        toggleTimer[Row][Column + 1] = globalClock;  //逆时针到3计算一个周期
    }

    if (switchTimer[Row][Column + 1] == 0)  //完成一个周期后回零
    {
        pushState[Row][Column + 1] = 0;
        pushState[Row][Column] = 0;
    }

    Joystick.setButton(Number + Reverse, (globalClock - toggleTimer[Row][Column] < PEC11Pulse));  //顺时针触发完成的周期数（reverse为1则为逆时针）
    Joystick.setButton(Number + 1 - Reverse, (globalClock - toggleTimer[Row][Column + 1] < PEC11Pulse));  //逆时针触发完成的周期数（reverse为1则为顺时针）
}

void PEC11bite(int row, int col, bool reverse)  //专门用于咬合点设定的PEC11编码器
{
    int Row = row - 1;
    int Column = col - 1;
    int Number = buttonNumber[Row][Column];

    int Reverse = reverse;

    switchTimer[Row][Column + 1] = (rawState[Row][Column] | rawState[Row][Column + 1] << 1);

    if
        (switchTimer[Row][Column + 1] > 0
            &&
            (globalClock - switchTimer[Row][Column] > PEC11Cooldown)
            &&
            pushState[Row][Column] == 0
            &&
            pushState[Row][Column + 1] == 0)
    {
        switchTimer[Row][Column] = globalClock;
        if (switchTimer[Row][Column + 1] == 2)
        {
            pushState[Row][Column] = 1;
        }
        else if (switchTimer[Row][Column + 1] == 1)
        {
            pushState[Row][Column + 1] = 1;
        }

        //----------------------------------------------
        //-----------------咬合点设定--------------------
        //----------------------------------------------

        if (pushState[biteButtonRow - 1][biteButtonCol - 1] == 1)  //按下咬合点按钮时启用旋钮调整
        {
            if (!biteButtonBit1 && !biteButtonBit2)
            {
                biteButtonBit1 = true;
            }
        }
    }

    if (pushState[Row][Column] == 1 && rawState[Row][Column])
    {
        pushState[Row][Column] = 2;
    }
    if (pushState[Row][Column] == 2 && switchTimer[Row][Column + 1] == 0)
    {
        pushState[Row][Column] = 3;
    }

    if (pushState[Row][Column + 1] == 1 && rawState[Row][Column + 1])
    {
        pushState[Row][Column + 1] = 2;
    }
    if (pushState[Row][Column + 1] == 2 && switchTimer[Row][Column + 1] == 0)
    {
        pushState[Row][Column + 1] = 3;
    }

    //推送成功完成的旋转周期数

    if (pushState[Row][Column] == 3)
    {
        toggleTimer[Row][Column] = globalClock;
        if (biteButtonBit1 || biteButtonBit2)
        {
            latchState[Row][Column] = true;
        }
    }
    else if (pushState[Row][Column + 1] == 3)
    {
        toggleTimer[Row][Column + 1] = globalClock;
        if (biteButtonBit1 || biteButtonBit2)
        {
            latchState[Row][Column + 1] = true;
        }
    }

    if (switchTimer[Row][Column + 1] == 0)
    {
        pushState[Row][Column + 1] = 0;
        pushState[Row][Column] = 0;
    }

    //改变咬合点值

    if (latchState[Row][Column + 1]) //顺时针增加咬合点值
    {
        if (biteButtonBit1 && !biteButtonBit2)  //初始状态10，单次量程100（即10%）
        {
            bitePoint = bitePoint + 100;
            if (bitePoint > 1000)
            {
                bitePoint = 1000;  //咬合点值不能超过1000
            }
        }
        else if (!biteButtonBit1 && biteButtonBit2)  //切换到状态01，单次量程10（即1%）
        {
            bitePoint = bitePoint + 10;
            if (bitePoint > 1000)
            {
                bitePoint = 1000;
            }
        }
        else if (biteButtonBit1 && biteButtonBit2)  //切换到状态11，单次量程1（即0.1%）
        {
            bitePoint = bitePoint + 1;
            if (bitePoint > 1000)
            {
                bitePoint = 1000;
            }
        }
        latchState[Row][Column + 1] = false;
    }
    else if (latchState[Row][Column]) //逆时针减少咬合点值
    {
        if (biteButtonBit1 && !biteButtonBit2)
        {
            bitePoint = bitePoint - 100;
            if (bitePoint < 0)
            {
                bitePoint = 0;  //咬合点值不能低于0
            }
        }
        else if (!biteButtonBit1 && biteButtonBit2)
        {
            bitePoint = bitePoint - 10;
            if (bitePoint < 0)
            {
                bitePoint = 0;
            }
        }
        else if (biteButtonBit1 && biteButtonBit2)
        {
            bitePoint = bitePoint - 1;
            if (bitePoint < 0)
            {
                bitePoint = 0;
            }
        }
        latchState[Row][Column] = false;
    }
    Joystick.setButton(Number + Reverse, (globalClock - toggleTimer[Row][Column] < PEC11Pulse));  //未按下咬合点按钮时的默认编码器功能：顺时针触发完成的周期数（reverse为1则为逆时针）
    Joystick.setButton(Number + 1 - Reverse, (globalClock - toggleTimer[Row][Column + 1] < PEC11Pulse));  //未按下咬合点按钮时的默认编码器功能：逆时针触发完成的周期数（reverse为1则为顺时针）
}