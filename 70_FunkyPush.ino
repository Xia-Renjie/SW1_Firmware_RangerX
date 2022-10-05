//摇杆点动开关函数，类似pushButton()，但加入了算法保证点动开关和指向开关一次只能触发一个
void funkyPush(int row, int column, int aCol, int bCol, int cCol, int dCol)
//参数为按钮行、列号，4指向开关所在列号（顺序无所谓）
{
    int Row = row - 1;
    int Column = column - 1;
    int Number = buttonNumber[Row][Column];

    int Acol = aCol - 1;
    int Bcol = bCol - 1;
    int Ccol = cCol - 1;
    int Dcol = dCol - 1;

    if (!pushState[Row][Acol] && !pushState[Row][Bcol] && !pushState[Row][Ccol] && !pushState[Row][Dcol])
    {
        if (pushState[Row][Column] != rawState[Row][Column] && (globalClock - switchTimer[Row][Column]) > buttonCooldown)
        {
            switchTimer[Row][Column] = globalClock;
            pushState[Row][Column] = rawState[Row][Column];
        }

        if ((globalClock - switchTimer[Row][Column]) > buttonCooldown)
        {
            pushState[Row][Column] = rawState[Row][Column];
        }
    }

    Joystick.setButton(Number, pushState[Row][Column]);

}

//2模式摇杆点动开关函数，类似pushButtonM()，但加入了算法保证点动开关和指向开关一次只能触发一个
void funkyPushM(int row, int column, int fieldPlacement, int aCol, int bCol, int cCol, int dCol)
{
    int Row = row - 1;
    int Column = column - 1;
    int Number = buttonNumber[Row][Column];
    int FieldPlacement = fieldPlacement;

    int Acol = aCol - 1;
    int Bcol = bCol - 1;
    int Ccol = cCol - 1;
    int Dcol = dCol - 1;

    if (!pushState[Row][Acol] && !pushState[Row][Bcol] && !pushState[Row][Ccol] && !pushState[Row][Dcol])
    {
        if (pushState[Row][Column] != rawState[Row][Column] && (globalClock - switchTimer[Row][Column]) > buttonCooldown)
        {
            switchTimer[Row][Column] = globalClock;
            pushState[Row][Column] = rawState[Row][Column];
        }

        if ((globalClock - switchTimer[Row][Column]) > buttonCooldown)
        {
            pushState[Row][Column] = rawState[Row][Column];
        }
    }

    //切换开关模式
    if (pushState[Row][Column] == 0)
    {
        switchModeLock[Row][Column] = false;
    }

    if (pushState[Row][Column] == 1 && pushState[modButtonRow - 1][modButtonCol - 1] == 1 && FieldPlacement != 0 && !switchModeLock[Row][Column])
    {
        switchModeLock[Row][Column] = true;
        switchMode[Row][Column] = !switchMode[Row][Column];
        latchLock[Row][Column] = false;
        latchState[Row][Column] = false;
    }

    //推送开关模式给按钮位字段
    long push = 0;
    push = push | switchMode[Row][Column];
    push = push << (FieldPlacement - 1);
    buttonField = buttonField | push;

    //开关模式1：自复位按钮
    if (!switchMode[Row][Column])
    {
        Joystick.setButton(Number, pushState[Row][Column]);
    }

    //开关模式2：自锁按钮
    else if (switchMode[Row][Column])
    {
        if (pushState[Row][Column] == 0)
        {
            latchLock[Row][Column] = false;
        }

        if (pushState[Row][Column] == 1 && !latchLock[Row][Column])
        {
            latchLock[Row][Column] = true;
            latchState[Row][Column] = !latchState[Row][Column];
        }

        Joystick.setButton(Number, latchState[Row][Column]);
    }
}

//自锁式摇杆点动开关函数，类似pushButtonL()，但加入了算法保证点动开关和指向开关一次只能触发一个
void funkyPushL(int row, int column, int aCol, int bCol, int cCol, int dCol)
{
    int Row = row - 1;
    int Column = column - 1;
    int Number = buttonNumber[Row][Column];

    int Acol = aCol - 1;
    int Bcol = bCol - 1;
    int Ccol = cCol - 1;
    int Dcol = dCol - 1;

    if (!pushState[Row][Acol] && !pushState[Row][Bcol] && !pushState[Row][Ccol] && !pushState[Row][Dcol])
    {
        if (pushState[Row][Column] != rawState[Row][Column] && (globalClock - switchTimer[Row][Column]) > buttonCooldown)
        {
            switchTimer[Row][Column] = globalClock;
            pushState[Row][Column] = rawState[Row][Column];
        }

        if ((globalClock - switchTimer[Row][Column]) > buttonCooldown)
        {
            pushState[Row][Column] = rawState[Row][Column];
        }
    }

    if (pushState[Row][Column] == 0)
    {
        latchLock[Row][Column] = false;
    }

    if (pushState[Row][Column] == 1 && !latchLock[Row][Column])
    {
        latchLock[Row][Column] = true;
        latchState[Row][Column] = !latchState[Row][Column];
    }

    Joystick.setButton(Number, latchState[Row][Column]);
}

//将摇杆点动开关作为混合按钮hybridButton()的函数，加入了算法保证点动开关和指向开关一次只能触发一个
void funkyPushHybrid(int row, int column, int aCol, int bCol, int cCol, int dCol)
{
    hybridButtonRow = row;
    hybridButtonCol = column;

    int Row = row - 1;
    int Column = column - 1;
    int Acol = aCol - 1;
    int Bcol = bCol - 1;
    int Ccol = cCol - 1;
    int Dcol = dCol - 1;

    if (!pushState[Row][Acol] && !pushState[Row][Bcol] && !pushState[Row][Ccol] && !pushState[Row][Dcol])
    {
        if (pushState[Row][Column] != rawState[Row][Column] && (globalClock - switchTimer[Row][Column]) > buttonCooldown)
        {
            switchTimer[Row][Column] = globalClock;
            pushState[Row][Column] = rawState[Row][Column];
        }

        if ((globalClock - switchTimer[Row][Column]) > buttonCooldown)
        {
            pushState[Row][Column] = rawState[Row][Column];
        }
    }

    if (pushState[Row][Column] == 0)
    {
        latchLock[Row][Column] = false;
    }

    if (pushState[Row][Column] == 1 && !latchLock[Row][Column])
    {
        latchLock[Row][Column] = true;
        latchState[Row][Column] = !latchState[Row][Column];
    }
}

//将摇杆点动开关作为混合按钮ddButton()的函数，加入了算法保证点动开关和指向开关一次只能触发一个
void funkyPushDDButton(int row, int column, int aCol, int bCol, int cCol, int dCol)
{
    ddButtonRow = row;
    ddButtonCol = column;

    int Row = row - 1;
    int Column = column - 1;
    int Acol = aCol - 1;
    int Bcol = bCol - 1;
    int Ccol = cCol - 1;
    int Dcol = dCol - 1;

    if (!pushState[Row][Acol] && !pushState[Row][Bcol] && !pushState[Row][Ccol] && !pushState[Row][Dcol])
    {
        if (pushState[Row][Column] != rawState[Row][Column] && (globalClock - switchTimer[Row][Column]) > buttonCooldown)
        {
            switchTimer[Row][Column] = globalClock;
            pushState[Row][Column] = rawState[Row][Column];
        }

        if ((globalClock - switchTimer[Row][Column]) > buttonCooldown)
        {
            pushState[Row][Column] = rawState[Row][Column];
        }
    }
    if (pushState[Row][Column] == 0)
    {
        latchLock[Row][Column] = false;
    }

    if (pushState[Row][Column] == 1 && !latchLock[Row][Column])
    {
        latchLock[Row][Column] = true;
        latchState[Row][Column] = !latchState[Row][Column];
    }
}