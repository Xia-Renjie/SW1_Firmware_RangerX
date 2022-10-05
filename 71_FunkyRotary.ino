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
