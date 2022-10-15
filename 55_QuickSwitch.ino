/*

快速切换开关函数，功能实现与pushButtonM()完全相同
模式1为普通点动按钮，可以任意绑定游戏内按键
模式2为快速切换按钮，激活后可以快速切换到12档旋转开关中设定为quickvalue的值

*/
void quickSwitch(int8_t row, int8_t column)  //搭配两个旋钮使用，同时切换两个旋钮的快速预设值（rotaryLeft()和rotaryRight()）
{
    int8_t Row = row - 1;
    int8_t Column = column - 1;
    int8_t Number = buttonNumber[Row][Column];

    if (pushState[Row][Column] != rawState[Row][Column] && (globalClock - switchTimer[Row][Column]) > buttonCooldown)
    {
        switchTimer[Row][Column] = globalClock;
        pushState[Row][Column] = rawState[Row][Column];
    }

    if ((globalClock - switchTimer[Row][Column]) > buttonCooldown)
    {
        pushState[Row][Column] = rawState[Row][Column];
    }

    //改变开关模式
    if (pushState[Row][Column] == 0)
    {
        switchModeLock[Row][Column] = false;
    }

    if (pushState[Row][Column] == 1 && pushState[modButtonRow - 1][modButtonCol - 1] == 1 && !switchModeLock[Row][Column])
    {
        switchModeLock[Row][Column] = true;
        switchMode[Row][Column] = !switchMode[Row][Column];
        latchLock[Row][Column] = false;
        latchState[Row][Column] = false;
        quickSwitchState = false;
    }

    //推送开关模式值给编码器位字段
    int32_t pesh = 0;
    pesh = pesh | switchMode[Row][Column];
    pesh = pesh << 1;
    rotaryField = rotaryField | pesh;

    //开关模式2：自复位按钮
    if (switchMode[Row][Column])
    {
        Joystick.setButton(Number, pushState[Row][Column]);
    }

    //开关模式1：快速切换按钮（自锁式）
    else if (!switchMode[Row][Column])
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

        quickSwitchState = latchState[Row][Column];
    }

    //传递开关激活状态给编码器位字段
    int32_t push = 0;
    push = push | quickSwitchState;
    push = push << 14;
    rotaryField = rotaryField | push;
}

//以下两个函数一起使用，搭配4个12档旋钮（或12档开关模式的编码器）实现一次切换其中2个的快速预设值
void quickSwitch12(int8_t row, int8_t column, int8_t fieldPlacement)  //搭配quickRotary1、2旋钮使用，同时切换两个旋钮的快速预设值
{
    int8_t Row = row - 1;
    int8_t Column = column - 1;
    int8_t Number = buttonNumber[Row][Column];
    int8_t FieldPlacement = fieldPlacement;

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
        switchModeLock[Row][Column] = false;
    }

    if (pushState[Row][Column] == 1 && pushState[modButtonRow - 1][modButtonCol - 1] == 1 && !switchModeLock[Row][Column])
    {
        switchModeLock[Row][Column] = true;
        switchMode[Row][Column] = !switchMode[Row][Column];
        latchLock[Row][Column] = false;
        latchState[Row][Column] = false;
        quickSwitch12State = false;
    }

    int32_t pesh = 0;
    pesh = pesh | switchMode[Row][Column];
    pesh = pesh << (FieldPlacement-1);
    buttonField = buttonField | pesh;

    if (switchMode[Row][Column])
    {
        Joystick.setButton(Number, pushState[Row][Column]);
    }

    else if (!switchMode[Row][Column])
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

        quickSwitch12State = latchState[Row][Column];
    }

    int32_t push = 0;
    push = push | quickSwitch12State;
    push = push << (FieldPlacement);
    buttonField = buttonField | push;
}

void quickSwitch34(int8_t row, int8_t column, int8_t fieldPlacement)  //搭配quickRotary3、4旋钮使用，同时切换两个旋钮的快速预设值
{
    int8_t Row = row - 1;
    int8_t Column = column - 1;
    int8_t Number = buttonNumber[Row][Column];
    int8_t FieldPlacement = fieldPlacement;

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
        switchModeLock[Row][Column] = false;
    }

    if (pushState[Row][Column] == 1 && pushState[modButtonRow - 1][modButtonCol - 1] == 1 && !switchModeLock[Row][Column])
    {
        switchModeLock[Row][Column] = true;
        switchMode[Row][Column] = !switchMode[Row][Column];
        latchLock[Row][Column] = false;
        latchState[Row][Column] = false;
        quickSwitch34State = false;
    }

    int32_t pesh = 0;
    pesh = pesh | switchMode[Row][Column];
    pesh = pesh << (FieldPlacement - 1);
    buttonField = buttonField | pesh;

    if (switchMode[Row][Column])
    {
        Joystick.setButton(Number, pushState[Row][Column]);
    }

    else if (!switchMode[Row][Column])
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

        quickSwitch34State = latchState[Row][Column];
    }

    int32_t push = 0;
    push = push | quickSwitch34State;
    push = push << (FieldPlacement);
    buttonField = buttonField | push;
}