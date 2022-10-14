//---------------------------
//---------按钮函数----------
//---------------------------

void pushButton(int8_t row, int8_t column)  //一般点动按钮（自复位按钮就是自复位功能，自锁按钮就是自锁功能），参数为行、列号
{
    int8_t Row = row - 1;
    int8_t Column = column - 1;
    int8_t Number = buttonNumber[Row][Column];

    if (pushState[Row][Column] != rawState[Row][Column] && (globalClock - switchTimer[Row][Column]) > buttonCooldown)  //当按钮状态变化，且间隔时间超过冷却时间
    {
        switchTimer[Row][Column] = globalClock;  //重设按钮计时器
        pushState[Row][Column] = rawState[Row][Column];  //更新按钮状态矩阵
    }

    if ((globalClock - switchTimer[Row][Column]) > buttonCooldown)  //间隔时间超过冷却时间
    {
        pushState[Row][Column] = rawState[Row][Column];  //更新按钮状态但不更新按钮计时器
    }

    Joystick.setButton(Number, pushState[Row][Column]);  //将按钮状态传递给MCU
}

void pushButtonM(int8_t row, int8_t column, int8_t fieldPlacement)  //多模式自复位按钮，参数为行、列号和占位参数，这个占位参数表示按钮在位字段中的编号
{
    int8_t Row = row - 1;
    int8_t Column = column - 1;
    int8_t Number = buttonNumber[Row][Column];
    int8_t FieldPlacement = fieldPlacement;

    if (pushState[Row][Column] != rawState[Row][Column] && (globalClock - switchTimer[Row][Column]) > buttonCooldown)  //一般模式下与pushButton()功能相同
    {
        switchTimer[Row][Column] = globalClock;
        pushState[Row][Column] = rawState[Row][Column];
    }

    if ((globalClock - switchTimer[Row][Column]) > buttonCooldown)
    {
        pushState[Row][Column] = rawState[Row][Column];
    }

    //改变按钮开关模式
    if (pushState[Row][Column] == 0)
    {
        switchModeLock[Row][Column] = false;  //按钮未按下时，解锁开关模式锁
    }

    if (pushState[Row][Column] == 1 && pushState[modButtonRow - 1][modButtonCol - 1] == 1 && FieldPlacement != 0 && !switchModeLock[Row][Column])  //当按钮按下，且模式调整按钮按下，且占位符不为0，且开关模式锁已解锁
    {
        switchModeLock[Row][Column] = true;  //锁定开关模式锁
        switchMode[Row][Column] = !switchMode[Row][Column];  //开关模式切换
        latchLock[Row][Column] = false;  //禁用自锁按钮锁
        latchState[Row][Column] = false;  //禁用自锁状态
    }

    //推送按钮模式的值给按钮位字段
    int32_t push = 0;
    push = push | switchMode[Row][Column];
    push = push << (FieldPlacement - 1);
    buttonField = buttonField | push;

    //开关模式1：自复位按钮
    if (!switchMode[Row][Column])  //模式值为0时为模式1
    {
        Joystick.setButton(Number, pushState[Row][Column]);
    }

    //开关模式2：自锁按钮
    else if (switchMode[Row][Column])  //模式值为1时为模式2
    {
        if (pushState[Row][Column] == 0)
        {
            latchLock[Row][Column] = false;  //按钮未按下时，解锁自锁模式锁
        }

        if (pushState[Row][Column] == 1 && !latchLock[Row][Column])  //按钮按下，且自锁模式已解锁
        {
            latchLock[Row][Column] = true;  //锁定自锁模式锁
            latchState[Row][Column] = !latchState[Row][Column];  //自锁状态切换
        }

        Joystick.setButton(Number, latchState[Row][Column]);  
    }
}


void pushButtonL(int8_t row, int8_t column)  //用自复位按钮实现自锁功能，参数为行、列号和占位参数
{
    int8_t Row = row - 1;
    int8_t Column = column - 1;
    int8_t Number = buttonNumber[Row][Column];

    if (pushState[Row][Column] != rawState[Row][Column] && (globalClock - switchTimer[Row][Column]) > buttonCooldown)  //当按钮状态变化，且间隔时间超过冷却时间
    {
        switchTimer[Row][Column] = globalClock;  //重设开关计时器
        pushState[Row][Column] = rawState[Row][Column];  //更新按钮状态
    }

    if ((globalClock - switchTimer[Row][Column]) > buttonCooldown)  //间隔时间超过冷却时间
    {
        pushState[Row][Column] = rawState[Row][Column];  //更新按钮状态但不更新按钮计时器
    }

    if (pushState[Row][Column] == 0)
    {
        latchLock[Row][Column] = false;
    }

    if (pushState[Row][Column] == 1 && !latchLock[Row][Column])
    {
        latchLock[Row][Column] = true;
        latchState[Row][Column] = !latchState[Row][Column];  //按钮按下后切换自锁状态
    }

    Joystick.setButton(Number, latchState[Row][Column]);
}