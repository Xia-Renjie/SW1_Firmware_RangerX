//摇杆指向开关函数，类似pushButton()，但加入了算法保证点动开关和指向开关一次只能触发一个
void funkyButton(int row, int column, int pCol, int Col1, int Col2, int Col3)
//参数为按钮行、列号，点动开关列号，剩余3个指向开关所在列号（顺序无所谓）
{
    int Row = row - 1;
    int Column = column - 1;
    int Number = buttonNumber[Row][Column];

    int pcol = pCol - 1;
    int col1 = Col1 - 1;
    int col2 = Col2 - 1;
    int col3 = Col3 - 1;

    if (!pushState[Row][pcol] && !pushState[Row][col1] && !pushState[Row][col2] && !pushState[Row][col3])
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

//2模式摇杆指向开关函数，类似pushButtonM()，但加入了算法保证点动开关和指向开关一次只能触发一个
void funkyButtonM(int row, int column, int fieldPlacement, int pCol, int Col1, int Col2, int Col3)
{
    int Row = row - 1;
    int Column = column - 1;
    int Number = buttonNumber[Row][Column];
    int FieldPlacement = fieldPlacement;

    int pcol = pCol - 1;
    int col1 = Col1 - 1;
    int col2 = Col2 - 1;
    int col3 = Col3 - 1;

    if (!pushState[Row][pcol] && !pushState[Row][col1] && !pushState[Row][col2] && !pushState[Row][col3])
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

//自锁式摇杆指向开关函数，类似pushButtonL()，但加入了算法保证点动开关和指向开关一次只能触发一个
void funkyButtonL(int row, int column, int pCol, int Col1, int Col2, int Col3)
{
    int Row = row - 1;
    int Column = column - 1;
    int Number = buttonNumber[Row][Column];

    int pcol = pCol - 1;
    int col1 = Col1 - 1;
    int col2 = Col2 - 1;
    int col3 = Col3 - 1;

    if (!pushState[Row][pcol] && !pushState[Row][col1] && !pushState[Row][col2] && !pushState[Row][col3])
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

//将摇杆指向开关作为混合按钮hybridButton()的函数，加入了算法保证点动开关和指向开关一次只能触发一个
void funkyButtonHybrid(int row, int column, int pCol, int Col1, int Col2, int Col3)
{
    hybridButtonRow = row;
    hybridButtonCol = column;

    int Row = row - 1;
    int Column = column - 1;
    int pcol = pCol - 1;
    int col1 = Col1 - 1;
    int col2 = Col2 - 1;
    int col3 = Col3 - 1;

    if (!pushState[Row][pcol] && !pushState[Row][col1] && !pushState[Row][col2] && !pushState[Row][col3])
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

//将摇杆指向开关作为混合按钮ddButton()的函数，加入了算法保证点动开关和指向开关一次只能触发一个
void funkyButtonDDButton(int row, int column, int pCol, int Col1, int Col2, int Col3)
{
    ddButtonRow = row;
    ddButtonCol = column;

    int Row = row - 1;
    int Column = column - 1;
    int pcol = pCol - 1;
    int col1 = Col1 - 1;
    int col2 = Col2 - 1;
    int col3 = Col3 - 1;

    if (!pushState[Row][pcol] && !pushState[Row][col1] && !pushState[Row][col2] && !pushState[Row][col3])
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
