/*

快速切换开关函数，功能实现与pushButtonM()完全相同
模式1为普通点动按钮，可以任意绑定游戏内按键
模式2为快速切换按钮，激活后可以在12档旋转开关绑定的12个值之间切换

*/
void quickSwitch(int8_t row, int8_t column)
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
    long pesh = 0;
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

    //传递开关激活状态给按钮位字段
    long push = 0;
    push = push | quickSwitchState;
    push = push << 7;
    buttonField = buttonField | push;
}
