//魔法刹车功能开关函数，通过拨档开关将刹车锁定在某个值
void brakeMagic(int row, int column, int fieldPlacement)  //参数为开关行、列号，按钮位字段号
{
    int Row = row - 1;
    int Column = column - 1;
    int Number = buttonNumber[Row][Column];
    int FieldPlacement = 4;

    //开关状态刷新
    if (pushState[Row][Column] != rawState[Row][Column] && (globalClock - switchTimer[Row][Column]) > buttonCooldown)
    {
        switchTimer[Row][Column] = globalClock;
        pushState[Row][Column] = rawState[Row][Column];
    }

    if ((globalClock - switchTimer[Row][Column]) > buttonCooldown)
    {
        pushState[Row][Column] = rawState[Row][Column];
    }

    //切换开关模式
    if (pushState[Row][Column] == 0)
    {
        switchModeLock[Row][Column] = false;
    }

    if (((globalClock - switchTimer[Row][Column]) < buttonCooldown) && pushState[modButtonRow - 1][modButtonCol - 1] == 1 && !switchModeLock[Row][Column])
    {
        switchModeLock[Row][Column] = true;
        switchMode[Row][Column] = !switchMode[Row][Column];
        latchLock[Row][Column] = false;
        latchState[Row][Column] = false;
        Joystick.setButton(Number, 0);
    }

    //传递开关模式值给按钮位字段
    int32_t push = 0;
    push = push | switchMode[Row][Column];
    push = push << (FieldPlacement - 1);
    buttonField = buttonField | push;

    brakeMagicOn = false;

    //开关模式1：魔法刹车
    if (!switchMode[Row][Column])
    {
        if (pushState[Row][Column] == 1)
        {
            brakeMagicOn = true;
        }
        else
        {
            brakeMagicOn = false;
        }

        if (pushState[Row][Column] == 1 && !latchState[Row][Column])
        {
            latchLock[Row][Column] = true;
        }

        if (pushState[Row][Column] == 1 && latchLock[Row][Column])
        {
            Joystick.setBrake(brakeMagicValue);  //开关打开，将魔法刹车值赋予刹车轴
            latchState[Row][Column] = true;

            int32_t push = 1;
            push = push << 9;
            buttonField = buttonField | push;  //传递开关模式值给按钮位字段，默认在第10位
        }

        if (pushState[Row][Column] == 0 && latchState[Row][Column])
        {
            Joystick.setBrake(0);  //开关关闭，将刹车轴归0
            latchState[Row][Column] = false;
            latchLock[Row][Column] = false;

            int32_t push = 0;
            push = push << 9;
            buttonField = buttonField | push;
        }
    }

    //开关模式2：脉冲模式，同toggleP()
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
            Joystick.setButton(Number, 0);
        }
    }
}
