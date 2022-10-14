/*

注意：一个自复位拨档开关应当被视为两个自复位按钮开关（上下各一个）
因此，以下函数仅适用于自锁拨档开关

*/

//------------------------------
//---------拨档开关函数----------
//------------------------------

void toggle(int row, int column)  //一般自锁拨档开关
{
    int Row = row - 1;
    int Column = column - 1;
    int Number = buttonNumber[Row][Column];

    if (pushState[Row][Column] != rawState[Row][Column] && (globalClock - switchTimer[Row][Column]) > buttonCooldown)
    {
        switchTimer[Row][Column] = globalClock;
        pushState[Row][Column] = rawState[Row][Column];  //开关状态变化且刷新时间超过冷却时间，更新开关计时器并更新开关状态
    }

    if ((globalClock - switchTimer[Row][Column]) > buttonCooldown)
    {
        pushState[Row][Column] = rawState[Row][Column];  //刷新时间超过冷却时间但开关状态未变化，只更新开关状态
    }

    Joystick.setButton(Number, pushState[Row][Column]);  //将开关状态传递给MCU
}

void toggleP(int row, int column)  //脉冲模式自锁拨档开关
{
    int Row = row - 1;
    int Column = column - 1;
    int Number = buttonNumber[Row][Column];

    if (pushState[Row][Column] != rawState[Row][Column] && (globalClock - switchTimer[Row][Column]) > buttonCooldown)
    {
        switchTimer[Row][Column] = globalClock;
        pushState[Row][Column] = rawState[Row][Column];  //开关状态变化且刷新时间超过冷却时间，更新开关计时器并更新开关状态
    }

    if ((globalClock - switchTimer[Row][Column]) > buttonCooldown)
    {
        pushState[Row][Column] = rawState[Row][Column];  //刷新时间超过冷却时间但开关状态未变化，只更新开关状态
    }

    if ((globalClock - switchTimer[Row][Column]) < buttonCooldown)
    {
        toggleTimer[Row][Column] = globalClock;  //刷新时间还未超过冷却时间（在开关状态变化时触发），更新拨档开关计时器
    }

    if ((globalClock - toggleTimer[Row][Column]) < togglePulse)
    {
        Joystick.setButton(Number, 1);  //当拨档开关状态刷新时间还未超过一次脉冲时间时，设置开关状态一直为按下
    }
    else
    {
        Joystick.setButton(Number, 0);  //当拨档开关状态刷新时间达到一次脉冲时间后，设置开关状态为松开
    }
}

void toggleM(int row, int column, int fieldPlacement)  //多模式自锁拨档开关，参数为行、列号和占位参数，这个占位参数表示开关在按钮位字段中的编号
{
    int Row = row - 1;
    int Column = column - 1;
    int Number = buttonNumber[Row][Column];
    int FieldPlacement = fieldPlacement;

    if (pushState[Row][Column] != rawState[Row][Column] && (globalClock - switchTimer[Row][Column]) > buttonCooldown)  //一般模式下与toggle()功能相同
    {
        switchTimer[Row][Column] = globalClock;
        pushState[Row][Column] = rawState[Row][Column];
    }

    if ((globalClock - switchTimer[Row][Column]) > buttonCooldown)
    {
        pushState[Row][Column] = rawState[Row][Column];
    }

    //改变拨档开关模式，与pushButtonM()方式相同
    if (pushState[Row][Column] == 0)
    {
        switchModeLock[Row][Column] = false;
    }

    if (((globalClock - switchTimer[Row][Column]) < buttonCooldown) && pushState[modButtonRow - 1][modButtonCol - 1] == 1 && FieldPlacement != 0 && !switchModeLock[Row][Column])
    {
        switchModeLock[Row][Column] = true;
        switchMode[Row][Column] = !switchMode[Row][Column];
        latchLock[Row][Column] = false;
        latchState[Row][Column] = false;
    }

    //推送开关模式的值给按钮位字段
    int32_t push = 0;
    push = push | switchMode[Row][Column];
    push = push << (FieldPlacement - 1);
    buttonField = buttonField | push;

    //开关模式1：拨档模式
    if (!switchMode[Row][Column])
    {
        Joystick.setButton(Number, pushState[Row][Column]);  //直接将开关状态传递给MCU
    }

    //开关模式2：脉冲模式
    if (switchMode[Row][Column])
    {
        if ((globalClock - switchTimer[Row][Column]) < buttonCooldown)
        {
            toggleTimer[Row][Column] = globalClock;
        }

        if ((globalClock - toggleTimer[Row][Column]) < togglePulse)
        {
            Joystick.setButton(Number, 1);
        }
        else
        {
            Joystick.setButton(Number, 0);  //传递时长为一次脉冲的按下状态给MCU
        }
    }
}
