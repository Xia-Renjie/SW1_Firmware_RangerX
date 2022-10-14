/*

一般来说，换挡拨片和功能拨片可以对每一个开关使用pushButton类函数来分别实现开关功能
因为原作者开发了推拉一体式的拨片结构，所以索性将换挡拨片和功能拨片写到一个函数中去
注意：这个函数要求四个拨片的按钮编号buttonNumber必须是顺序相邻的四个编号
在函数中还定义了模式切换，可以实现单侧推拉拨片换挡（不建议用于分体式拨片）
通过同时按下左右换挡拨片1秒来实现手刹功能（开关式手刹，非线性手刹）
为方便说明，以下注释中默认将pull拨片视为换挡拨片，push拨片视为功能拨片

*/
void pushPull(int rightPullRow, int rightPullCol, int rightPushRow, int rightPushCol, int leftPullRow, int leftPullCol, int leftPushRow, int leftPushCol, int fieldPlacement)  //传递参数为右换挡、右功能、左换挡、左功能拨片的行列号，按钮位字段
{
    int8_t RightPullRow = rightPullRow - 1;
    int8_t RightPullCol = rightPullCol - 1;
    int8_t RightPushRow = rightPushRow - 1;
    int8_t RightPushCol = rightPushCol - 1;
    int8_t LeftPullRow = leftPullRow - 1;
    int8_t LeftPullCol = leftPullCol - 1;
    int8_t LeftPushRow = leftPushRow - 1;
    int8_t LeftPushCol = leftPushCol - 1;

    int8_t FieldPlacement = fieldPlacement;

    int8_t Number = buttonNumber[RightPullRow][RightPullCol];

    //全部开关的功能实现与pushButton()相同

    //右换挡
    if (pushState[RightPullRow][RightPullCol] != rawState[RightPullRow][RightPullCol] && (globalClock - switchTimer[RightPullRow][RightPullCol]) > buttonCooldown)
    {
        switchTimer[RightPullRow][RightPullCol] = globalClock;
        pushState[RightPullRow][RightPullCol] = rawState[RightPullRow][RightPullCol];
    }

    if ((globalClock - switchTimer[RightPullRow][RightPullCol]) > buttonCooldown)
    {
        pushState[RightPullRow][RightPullCol] = rawState[RightPullRow][RightPullCol];
    }

    //右功能
    if (pushState[RightPushRow][RightPushCol] != rawState[RightPushRow][RightPushCol] && (globalClock - switchTimer[RightPushRow][RightPushCol]) > buttonCooldown)
    {
        switchTimer[RightPushRow][RightPushCol] = globalClock;
        pushState[RightPushRow][RightPushCol] = rawState[RightPushRow][RightPushCol];
    }

    if ((globalClock - switchTimer[RightPushRow][RightPushCol]) > buttonCooldown)
    {
        pushState[RightPushRow][RightPushCol] = rawState[RightPushRow][RightPushCol];
    }

    //左换挡
    if (pushState[LeftPullRow][LeftPullCol] != rawState[LeftPullRow][LeftPullCol] && (globalClock - switchTimer[LeftPullRow][LeftPullCol]) > buttonCooldown)
    {
        switchTimer[LeftPullRow][LeftPullCol] = globalClock;
        pushState[LeftPullRow][LeftPullCol] = rawState[LeftPullRow][LeftPullCol];
    }

    if ((globalClock - switchTimer[LeftPullRow][LeftPullCol]) > buttonCooldown)
    {
        pushState[LeftPullRow][LeftPullCol] = rawState[LeftPullRow][LeftPullCol];
    }

    //左功能
    if (pushState[LeftPushRow][LeftPushCol] != rawState[LeftPushRow][LeftPushCol] && (globalClock - switchTimer[LeftPushRow][LeftPushCol]) > buttonCooldown)
    {
        switchTimer[LeftPushRow][LeftPushCol] = globalClock;
        pushState[LeftPushRow][LeftPushCol] = rawState[LeftPushRow][LeftPushCol];
    }

    if ((globalClock - switchTimer[LeftPushRow][LeftPushCol]) > buttonCooldown)
    {
        pushState[LeftPushRow][LeftPushCol] = rawState[LeftPushRow][LeftPushCol];
    }


    //改变开关模式，与pushButtonM()相同
    if (pushState[RightPullRow][RightPullCol] == 0)
    {
        switchModeLock[RightPullRow][RightPullCol] = false;
    }

    if (pushState[RightPullRow][RightPullCol] == 1 && pushState[modButtonRow - 1][modButtonCol - 1] == 1 && !switchModeLock[RightPullRow][RightPullCol])
    {
        switchModeLock[RightPullRow][RightPullCol] = true;
        switchMode[RightPullRow][RightPullCol] = !switchMode[RightPullRow][RightPullCol];
        latchLock[RightPullRow][RightPullCol] = false;
        latchState[RightPullRow][RightPullCol] = false;
    }

    //传递开关模式值给按钮位字段
    int32_t push = 0;
    push = push | switchMode[RightPullRow][RightPullCol];
    push = push << (FieldPlacement - 1);
    buttonField = buttonField | push;

    //开关模式1：复合模式，实现推拉式换挡
    //左侧推拨片绑定到右侧拉拨片（一般为升档），右侧为推拨片绑定到左侧拉拨片（一般为降档，游戏里也可以更改按键映射）
    //（一般只用于推拉一体式拨片结构）
    if (!switchMode[RightPullRow][RightPullCol])
    {
        Joystick.setButton(Number, pushState[RightPullRow][RightPullCol]);
        Joystick.setButton(Number + 1, pushState[LeftPullRow][LeftPullCol]);  //正常读取拉拨片状态

        if (pushState[RightPullRow][RightPullCol] == 0)
        {
            Joystick.setButton(Number, pushState[LeftPushRow][LeftPushCol]);  //右侧无操作时，左侧推拨片为升档
        }

        if (pushState[LeftPullRow][LeftPullCol] == 0)
        {
            Joystick.setButton(Number + 1, pushState[RightPushRow][RightPushCol]);  //左侧无操作时，右侧推拨片为降档
        }

        Joystick.setButton(Number + 2, 0);  //原推拨片功能失效
        Joystick.setButton(Number + 3, 0);

    }
    //开关模式2：独立模式，各自实现按键功能
    else if (switchMode[RightPullRow][RightPullCol])
    {
        Joystick.setButton(Number, pushState[RightPullRow][RightPullCol]);
        Joystick.setButton(Number + 2, pushState[RightPushRow][RightPushCol]);
        Joystick.setButton(Number + 1, pushState[LeftPullRow][LeftPullCol]);
        Joystick.setButton(Number + 3, pushState[LeftPushRow][LeftPushCol]);
    }

    //手刹，通过同时按下左右换挡拨片1秒来激活手刹，再同时按下1秒来释放手刹

    if (handbrake)
    {
        Joystick.setBrake(1000);  //激活手刹，将刹车值拉满
    }
    else
    {
        Joystick.setBrake(0);
    }

    if (pushState[RightPullRow][RightPullCol] == 0 || pushState[LeftPullRow][LeftPullCol] == 0)
    {
        handbrakeLock = false;  //有一侧换挡拨片未按下，手刹锁解锁
    }

    if (!handbrakeLock && pushState[RightPullRow][RightPullCol] == 1 && pushState[LeftPullRow][LeftPullCol] == 1)  //左右换挡拨片同时按下
    {
        handbrakeTimer = globalClock;  //更新手刹计时器
        handbrakeLock = true;  //锁定手刹锁
    }

    else if (handbrakeLock && globalClock - handbrakeTimer > 1000)  //拨片同时按下超过1秒后
    {
        handbrake = true;  //启用手刹
        handbrakeLock = false;  //解锁手刹锁
    }

    if (pushState[RightPushRow][RightPushCol] == 0 && pushState[LeftPushRow][LeftPushCol] == 0)
    {
        handbrakeRelease = false;  //解锁手刹释放锁
    }

    if (!handbrakeRelease && pushState[RightPushRow][RightPushCol] == 1 && pushState[LeftPushRow][LeftPushCol] == 1)  //手刹已激活时左右拨片同时按下
    {
        handbrakeTimer = globalClock;  //更新手刹计时器
        handbrakeRelease = true;  //锁定手刹释锁
    }

    else if (handbrakeRelease && globalClock - handbrakeTimer > 1000)  //拨片同时按下时间超过1秒后
    {
        handbrake = false;  //松开手刹
        handbrakeRelease = false;  //解锁手刹释放锁
    }

    //传递手刹状态给编码器位字段
    int32_t pesh = 0;
    pesh = pesh | handbrake;
    rotaryField = rotaryField | pesh;
}
