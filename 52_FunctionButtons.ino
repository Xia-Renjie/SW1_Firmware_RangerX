//-------------------------------
//----------特殊功能按钮----------
//-------------------------------

//模式调整按钮

void modButton(int row, int column)
{
    modButtonRow = row;
    modButtonCol = column;

    int Row = row - 1;
    int Column = column - 1;
    int Number = buttonNumber[Row][Column];

    //状态更新函数与pushButton()相同
    if (pushState[Row][Column] != rawState[Row][Column] && (globalClock - switchTimer[Row][Column]) > buttonCooldown)
    {
        switchTimer[Row][Column] = globalClock;
        pushState[Row][Column] = rawState[Row][Column];
    }

    if ((globalClock - switchTimer[Row][Column]) > buttonCooldown)
    {
        pushState[Row][Column] = rawState[Row][Column];
    }

    Joystick.setButton(Number, pushState[Row][Column]);
}

//预设按钮

void presetButton(int row, int column)
{
    presetButtonRow = row;
    presetButtonCol = column;

    int Row = row - 1;
    int Column = column - 1;
    int Number = buttonNumber[Row][Column];

    //状态更新函数与pushButton()相同
    if (pushState[Row][Column] != rawState[Row][Column] && (globalClock - switchTimer[Row][Column]) > buttonCooldown)
    {
        switchTimer[Row][Column] = globalClock;
        pushState[Row][Column] = rawState[Row][Column];
    }

    if ((globalClock - switchTimer[Row][Column]) > buttonCooldown)
    {
        pushState[Row][Column] = rawState[Row][Column];
    }

    Joystick.setButton(Number, pushState[Row][Column]);
}

//混合按钮和DD按钮

void hybridButton(int row, int column)
{
    hybridButtonRow = row;
    hybridButtonCol = column;

    int Row = row - 1;
    int Column = column - 1;

    //状态更新函数与pushButtonL()相同，但不需要按钮编号
    if (pushState[Row][Column] != rawState[Row][Column] && (globalClock - switchTimer[Row][Column]) > buttonCooldown)
    {
        switchTimer[Row][Column] = globalClock;
        pushState[Row][Column] = rawState[Row][Column];
    }

    if ((globalClock - switchTimer[Row][Column]) > buttonCooldown)
    {
        pushState[Row][Column] = rawState[Row][Column];
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

void DDButton(int row, int column)
{
    ddButtonRow = row;
    ddButtonCol = column;

    int Row = row - 1;
    int Column = column - 1;

    //状态更新函数与pushButtonL()相同，但不需要按钮编号
    if (pushState[Row][Column] != rawState[Row][Column] && (globalClock - switchTimer[Row][Column]) > buttonCooldown)
    {
        switchTimer[Row][Column] = globalClock;
        pushState[Row][Column] = rawState[Row][Column];
    }

    if ((globalClock - switchTimer[Row][Column]) > buttonCooldown)
    {
        pushState[Row][Column] = rawState[Row][Column];
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

//空挡按钮

void neutralButton(int row, int column)
{
    neutralButtonRow = row;
    neutralButtonCol = column;

    int Row = row - 1;
    int Column = column - 1;
    int Number = buttonNumber[Row][Column];
    int FieldPlacement = 15;
    int ActivePlacement = 16;

    //一般模式下状态更新函数与pushButton()功能相同
    if (pushState[Row][Column] != rawState[Row][Column] && (globalClock - switchTimer[Row][Column]) > buttonCooldown)
    {
        switchTimer[Row][Column] = globalClock;
        pushState[Row][Column] = rawState[Row][Column];
    }

    if ((globalClock - switchTimer[Row][Column]) > buttonCooldown)
    {
        pushState[Row][Column] = rawState[Row][Column];
    }

    //改变开关模式，与pushButtonM()相同
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

    //推送按钮模式的值给按钮位字段，与pushButtonM()相同
    long push = 0;
    push = push | switchMode[Row][Column];
    push = push << (FieldPlacement - 1);
    buttonField = buttonField | push;

    //开关模式2：自复位按钮
    if (switchMode[Row][Column])
    {
        Joystick.setButton(Number, pushState[Row][Column]);
    }

    //开关模式1：自锁空挡按钮（默认状态）
    else if (!switchMode[Row][Column])
    {
        if (pushState[Row][Column] == 0)
        {
            latchLock[Row][Column] = false;
        }

        if (pushState[Row][Column] == 1 && !latchLock[Row][Column] && !(pushState[modButtonRow - 1][modButtonCol - 1] == 1))
        {
            latchLock[Row][Column] = true;
            latchState[Row][Column] = !latchState[Row][Column];
        }

        //推送空挡激活状态的值给按钮位字段
        long push = 0;
        push = push | latchState[Row][Column];
        push = push << (ActivePlacement - 1);
        buttonField = buttonField | push;
    }
}

//咬合点按钮

void biteButton(int row, int column)
{
    Joystick.setRyAxisRange(0, 1000);  //先将离合设定为0-100%

    biteButtonRow = row;
    biteButtonCol = column;

    int Row = row - 1;
    int Column = column - 1;
    int Number = buttonNumber[Row][Column];
    int FieldPlacement = 6;

    //一般模式下状态更新函数与pushButton()功能相同
    if (pushState[Row][Column] != rawState[Row][Column] && (globalClock - switchTimer[Row][Column]) > buttonCooldown)
    {
        switchTimer[Row][Column] = globalClock;
        pushState[Row][Column] = rawState[Row][Column];
    }

    if ((globalClock - switchTimer[Row][Column]) > buttonCooldown)
    {
        pushState[Row][Column] = rawState[Row][Column];
    }

    //改变两个咬合点设定位字段的状态
    if (!pushState[Row][Column])
    {
        latchLock[Row][Column] = true;  //未按下时，保持开关状态锁定
    }

    if (pushState[Row][Column] && latchLock[Row][Column])  //按钮按下时，改变两个咬合点设定位字段的单次量程
    {
        latchLock[Row][Column] = false;
        if (biteButtonBit1 && !biteButtonBit2)  //第一次切换
        {
            biteButtonBit2 = true;
            biteButtonBit1 = false;
        }
        else if (!biteButtonBit1 && biteButtonBit2)  //第二次切换
        {
            biteButtonBit1 = true;
        }
        else if (biteButtonBit1 && biteButtonBit2)  //第三次切换
        {
            biteButtonBit2 = false;
            biteButtonBit1 = false;  //切换到状态00，锁定咬合点值
        }
    }
    //推送咬合点设定位字段值(字段用法详见咬合点编码器函数)
    long push = 0;
    push = push | biteButtonBit1;
    push = push | (biteButtonBit2 << 1);
    push = push << (2 * (FieldPlacement - 1));
    encoderField = encoderField | push;

    Joystick.setRyAxis(bitePoint);  //将咬合点设定编码器的值传递给咬合点轴
    Joystick.setButton(Number, pushState[Row][Column]);  //传递按钮按下状态
}