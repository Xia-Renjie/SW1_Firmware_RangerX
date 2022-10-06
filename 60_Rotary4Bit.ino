//------------------------------
//---------4位旋转编码器---------
//------------------------------

void rotary4Modes(int row, int column, int fieldPlacement, int hybridPositions, bool reverse)  //参数为行、列号，编码器字段号，自定义档位数，是否反向旋转
{
    int Row = row - 1;
    int Column = column - 1;
    int Number = buttonNumber[Row][Column];
    int FieldPlacement = fieldPlacement;
    int HyPos = hybridPositions;
    int Reverse = reverse;

    int maxPos = max(16, HyPos);

    //找到编码器绝对位置

    bool Pin1 = rawState[Row][Column];
    bool Pin2 = rawState[Row][Column + 1];
    bool Pin3 = rawState[Row][Column + 2];
    bool Pin4 = rawState[Row][Column + 3];

    bool array[4] = { Pin1, Pin2, Pin3, Pin4 };  //读取4个针脚的状态

    int pos = 0;

    for (int i = 0; i < 4; i++)
    {
        if (array[i])
        {
            pos |= (1 << i);
        }
    }

    pos = pos ^ (pos >> 4);
    pos = pos ^ (pos >> 2);
    pos = pos ^ (pos >> 1);

    int result = pos;  //将4个针脚读数转换为编号0-15(0000-1111)的档位

    //旋转过程中去抖动

    if (pushState[Row][Column] != result)  //旋转开关功能相当于每一档都是一个脉冲模式开关
    {
        if (globalClock - switchTimer[Row][Column] > encoderPulse)
        {
            switchTimer[Row][Column] = globalClock;
        }
        else if (globalClock - switchTimer[Row][Column] > encoderWait)
        {
            //----------------------------------------------
            //-------------------模式切换-------------------
            //----------------------------------------------

            //因为占位符的存在，模式切换仅在编码器旋转时才会生效
            //如果你不想推送模式值给位字段，就把fieldPlacement设定为0

            if (pushState[modButtonRow - 1][modButtonCol - 1] == 1 && FieldPlacement != 0)
            {
                for (int i = 0; i < maxPos + 1; i++)  //清除模式1下的按钮状态
                {
                    Joystick.releaseButton(i - 1 + Number);
                }

                if (switchMode[Row][Column] && switchMode[Row][Column + 1]) //退出模式4
                {
                    switchMode[Row][Column + 1] = false;
                    switchMode[Row][Column] = false;
                }
                else //在模式1-3之间切换
                {
                    switchMode[Row][Column] = !switchMode[Row][Column];  //当前为模式1时切换到模式2
                    if (!switchMode[Row][Column]) //模式2时切换成模式3，将混合按钮锁定状态设置为0
                    {
                        switchMode[Row][Column + 1] = true;
                        latchState[hybridButtonRow - 1][hybridButtonCol - 1] = 0;
                    }
                    else
                    {
                        if (switchMode[Row][Column + 1])  //从模式3跳过模式4，切换到模式1
                        {
                            switchMode[Row][Column + 1] = false;
                            switchMode[Row][Column] = false;
                        }
                    }
                }
            }

            //激活编码器脉冲计时器
            switchTimer[Row][Column + 1] = globalClock;

            //将档位与初始的差值暂存到2号针脚
            pushState[Row][Column + 1] = result - pushState[Row][Column];

            //更新档位状态
            pushState[Row][Column] = result;

            //如果处于模式3，更新计数器
            if (!switchMode[Row][Column] && switchMode[Row][Column + 1])
            {
                if ((pushState[Row][Column + 1] > 0 && pushState[Row][Column + 1] < 8) || pushState[Row][Column + 1] < -8)  //顺时针旋转时计数器+1（reverse为1则为-1）
                {
                    pushState[Row][Column + 2] = pushState[Row][Column + 2] + 1 - (2 * Reverse);
                }
                else //逆时针旋转时计数器-1（reverse为1则为+1）
                {
                    pushState[Row][Column + 2] = pushState[Row][Column + 2] - 1 + (2 * Reverse);
                }
                if (pushState[Row][Column + 2] < 0)  //计数器小于0时，从最高档位重新开始计数
                {
                    pushState[Row][Column + 2] = HyPos - 1;
                }
            }

            //如果不在混合模式，将计数器清零
            else if (!switchMode[Row][Column + 1])
            {
                pushState[Row][Column + 2] = 0;
            }
        }
    }

    //处于混合模式时，允许切换开放式和封闭式
    if (switchMode[Row][Column + 1])
    {
        switchMode[Row][Column] = latchState[hybridButtonRow - 1][hybridButtonCol - 1];
    }

    //开关模式1：16档开关

    if (!switchMode[Row][Column] && !switchMode[Row][Column + 1])
    {
        pushState[Row][Column + 1] = 0; //清除暂存的差值

        for (int i = 0; i < 17; i++)
        {
            if (i == pushState[Row][Column])
            {
                Joystick.pressButton(i + Number);  //按下对应按钮并释放其它按钮
            }
            else
            {
                Joystick.releaseButton(i + Number);
            }
        }
    }

    //开关模式2和4：增量编码器和封闭式混合模式旋钮

    else if (switchMode[Row][Column])
    {
        Number = buttonNumber[Row][Column + 1];
        int difference = pushState[Row][Column + 1];
        if (difference != 0)  //暂存差值不为0时（即两个针脚状态有差别时）
        {
            if (globalClock - switchTimer[Row][Column + 1] < encoderPulse)
            {
                if ((difference > 0 && difference < 8) || difference < -8)  //顺时针旋转时触发+1（reverse为1则为-1）
                {
                    Joystick.setButton(Number + Reverse, 1);
                    Joystick.setButton(Number + 1 - Reverse, 0);
                }
                else  //逆时针旋转时触发-1（reverse为1则为+1）
                {
                    Joystick.setButton(Number + Reverse, 0);
                    Joystick.setButton(Number + 1 - Reverse, 1);
                }
            }
            else  //刷新时间到后，更新开关状态
            {
                pushState[Row][Column + 1] = 0;  //清空暂存的差值
                Joystick.setButton(Number, 0);
                Joystick.setButton(Number + 1, 0);
            }
        }
    }

    //开关模式3：开放式混合模式旋钮
    if (!switchMode[Row][Column] && switchMode[Row][Column + 1])
    {

        for (int i = 0; i < HyPos + 1; i++)  //按下对应档位的按钮，释放其它按钮
        {
            int e = pushState[Row][Column + 2] % HyPos;
            if (e == 0)
            {
                e = HyPos;
            }
            if (i == e)
            {
                Joystick.pressButton(i - 1 + Number);
            }
            else
            {
                Joystick.releaseButton(i - 1 + Number);
            }
        }
    }

    //传递模式值给编码器位字段
    long push = 0;
    push = push | switchMode[Row][Column];
    push = push | (switchMode[Row][Column + 1] << 1);
    push = push << (FieldPlacement - 1);
    rotaryField = rotaryField | push;
}

//一般4位增量编码器
void rotary4Inc(int row, int column, bool reverse)  //参数为行、列号，是否反向旋转
{
    int Row = row - 1;
    int Column = column - 1;
    int Number = buttonNumber[Row][Column];
    int Reverse = reverse;

    //找到编码器绝对位置

    bool Pin1 = rawState[Row][Column];
    bool Pin2 = rawState[Row][Column + 1];
    bool Pin3 = rawState[Row][Column + 2];
    bool Pin4 = rawState[Row][Column + 3];

    bool array[4] = { Pin1, Pin2, Pin3, Pin4 };

    int pos = 0;

    for (int i = 0; i < 4; i++)
    {
        if (array[i])
        {
            pos |= (1 << i);
        }
    }

    pos = pos ^ (pos >> 4);
    pos = pos ^ (pos >> 2);
    pos = pos ^ (pos >> 1);

    int result = pos;
    
    //旋转过程中去抖动

    if (pushState[Row][Column] != result && (globalClock - switchTimer[Row][Column] > encoderPulse))
    {
        switchTimer[Row][Column] = globalClock;
    }
    if (globalClock - switchTimer[Row][Column] > encoderWait)
    {
        if (globalClock - switchTimer[Row][Column] < encoderPulse)
        {
            int difference = result - pushState[Row][Column];

            if ((difference > 0 && difference < 6) || difference < -6)
            {
                Joystick.setButton(Number + Reverse, 1);
                Joystick.setButton(Number + 1 - Reverse, 0);
            }
            else
            {
                Joystick.setButton(Number + Reverse, 0);
                Joystick.setButton(Number + 1 - Reverse, 1);
            }
        }
        else
        {
            Joystick.setButton(Number, 0);
            Joystick.setButton(Number + 1, 0);
            pushState[Row][Column] = result;
        }
    }
}

//计数式多档位旋钮
void rotary4Multi(int row, int column, int positions, bool reverse)  //参数为行、列号，自定义档位数，是否反向旋转
{
    int Row = row - 1;
    int Column = column - 1;
    int Number = buttonNumber[Row][Column];
    int Pos = positions;
    int Reverse = reverse;

    //找到编码器绝对位置

    bool Pin1 = rawState[Row][Column];
    bool Pin2 = rawState[Row][Column + 1];
    bool Pin3 = rawState[Row][Column + 2];
    bool Pin4 = rawState[Row][Column + 3];

    bool array[4] = { Pin1, Pin2, Pin3, Pin4 };

    int pos = 0;

    for (int i = 0; i < 4; i++)
    {
        if (array[i])
        {
            pos |= (1 << i);
        }
    }

    pos = pos ^ (pos >> 4);
    pos = pos ^ (pos >> 2);
    pos = pos ^ (pos >> 1);

    int result = pos;

    //旋转过程中去抖动

    if (pushState[Row][Column] != result)
    {
        if (globalClock - switchTimer[Row][Column] > encoderPulse)
        {
            switchTimer[Row][Column] = globalClock;
        }
        else if (globalClock - switchTimer[Row][Column] > encoderWait)
        {
            switchTimer[Row][Column + 1] = globalClock;

            pushState[Row][Column + 1] = result - pushState[Row][Column];

            pushState[Row][Column] = result;

            //更新计数器
            if ((pushState[Row][Column + 1] > 0 && pushState[Row][Column + 1] < 6) || pushState[Row][Column + 1] < -6)
            {
                pushState[Row][Column + 2] = pushState[Row][Column + 2] + 1 - (2 * Reverse);
            }
            else
            {
                pushState[Row][Column + 2] = pushState[Row][Column + 2] - 1 + (2 * Reverse);
            }

            if (pushState[Row][Column + 2] < 0)
            {
                pushState[Row][Column + 2] = Pos - 1;
            }

            //触发对应按钮
            for (int i = 0; i < Pos + 1; i++)
            {
                int e = pushState[Row][Column + 2] % Pos;
                if (e == 0)
                {
                    e = Pos;
                }
                if (i == e)
                {
                    Joystick.pressButton(i - 1 + Number);
                }
                else
                {
                    Joystick.releaseButton(i - 1 + Number);
                }
            }
        }
    }
}

//4模式计数式多档位旋钮
void rotary4Multis(int row, int column, int fieldPlacement, int positions1, int positions2, int positions3, bool reverse)
{
    int Row = row - 1;
    int Column = column - 1;
    int Number = buttonNumber[Row][Column];
    int FieldPlacement = fieldPlacement;
    int Reverse = reverse;

    //找到编码器绝对位置

    bool Pin1 = rawState[Row][Column];
    bool Pin2 = rawState[Row][Column + 1];
    bool Pin3 = rawState[Row][Column + 2];
    bool Pin4 = rawState[Row][Column + 3];

    int maxPos = max(positions3, (max(positions2, max(16, positions1))));

    bool array[4] = { Pin1, Pin2, Pin3, Pin4 };

    int pos = 0;

    for (int i = 0; i < 4; i++)
    {
        if (array[i])
        {
            pos |= (1 << i);
        }
    }

    pos = pos ^ (pos >> 4);
    pos = pos ^ (pos >> 2);
    pos = pos ^ (pos >> 1);

    int result = pos;

    //旋转过程中去抖动

    if (pushState[Row][Column] != result)
    {
        if (globalClock - switchTimer[Row][Column] > encoderPulse)
        {
            switchTimer[Row][Column] = globalClock;
        }
        else if (globalClock - switchTimer[Row][Column] > encoderWait)
        {

            //----------------------------------------------
            //------------------模式切换--------------------
            //----------------------------------------------

            //因为占位符的存在，模式切换仅在编码器旋转时才会生效
            //如果你不想推送模式值给位字段，就把fieldPlacement设定为0

            if (pushState[modButtonRow - 1][modButtonCol - 1] == 1 && FieldPlacement != 0)
            {
                for (int i = 0; i < maxPos + 1; i++)  //清除模式1下的按钮状态
                {
                    Joystick.releaseButton(i - 1 + Number);
                }

                if (!switchMode[Row][Column] && !switchMode[Row][Column + 1])  //切换到模式2
                {
                    switchMode[Row][Column + 1] = false;
                    switchMode[Row][Column] = true;
                    pushState[Row][Column + 3] = positions1; //使用针脚4上的计数器来存储自定义档位数
                }
                else if (switchMode[Row][Column] && !switchMode[Row][Column + 1])  //切换到模式3
                {
                    switchMode[Row][Column + 1] = true;
                    switchMode[Row][Column] = false;
                    pushState[Row][Column + 3] = positions2;
                }
                else if (!switchMode[Row][Column] && switchMode[Row][Column + 1])  //切换到模式4
                {
                    switchMode[Row][Column + 1] = true;
                    switchMode[Row][Column] = true;
                    pushState[Row][Column + 3] = positions3;
                }
                else if (switchMode[Row][Column] && switchMode[Row][Column + 1]) //回到模式1
                {
                    switchMode[Row][Column + 1] = false;
                    switchMode[Row][Column] = false;
                }
            }

            switchTimer[Row][Column + 1] = globalClock;

            pushState[Row][Column + 1] = result - pushState[Row][Column];

            pushState[Row][Column] = result;

            //处于伪多档位模式（即3个自定义档位模式）时，更新计数器
            if (!(!switchMode[Row][Column] && !switchMode[Row][Column + 1]))
            {
                if ((pushState[Row][Column + 1] > 0 && pushState[Row][Column + 1] < 6) || pushState[Row][Column + 1] < -6)
                {
                    pushState[Row][Column + 2] = pushState[Row][Column + 2] + 1 - (2 * Reverse);
                }
                else
                {
                    pushState[Row][Column + 2] = pushState[Row][Column + 2] - 1 + (2 * Reverse);
                }
                if (pushState[Row][Column + 2] < 0)
                {
                    pushState[Row][Column + 2] = pushState[Row][Column + 3] - 1; //Col pin 4 push state holds info on how many positions on multiposition switch
                }
            }

            //如果不在多档位模式，重设计数器
            else
            {
                pushState[Row][Column + 2] = 0;
            }
        }

        //开关模式1：16档位开关（同rotary4Modes()的模式1）

        if (!switchMode[Row][Column] && !switchMode[Row][Column + 1])
        {
            pushState[Row][Column + 1] = 0;

            for (int i = 0; i < 17; i++)
            {
                if (i == pushState[Row][Column])
                {
                    Joystick.pressButton(i + Number);
                }
                else
                {
                    Joystick.releaseButton(i + Number);
                }
            }
        }
        else
        {
            //开关模式2-4：多档位开关（类似rotary4Modes()的模式3）

            for (int i = 0; i < pushState[Row][Column + 3] + 1; i++) //Col pin 4 push state holds info on how many positions on multiposition switch
            {
                int e = pushState[Row][Column + 2] % pushState[Row][Column + 3];
                if (e == 0)
                {
                    e = pushState[Row][Column + 3];
                }
                if (i == e)
                {
                    Joystick.pressButton(i - 1 + Number);
                }
                else
                {
                    Joystick.releaseButton(i - 1 + Number);
                }
            }
        }
    }

    //推送开关模式值给位字段
    long push = 0;
    push = push | switchMode[Row][Column];
    push = push | (switchMode[Row][Column + 1] << 1);
    push = push << (FieldPlacement - 1);
    rotaryField = rotaryField | push;
}

//DDS模式4位编码器
void DDS4bit(int row, int column, bool reverse)
{
    int Row = row - 1;
    int Column = column - 1;
    int Number = buttonNumber[Row][Column];
    int FieldPlacement = 8;
    int HyPos = 12;  //每一层12个档位
    int Reverse = reverse;

    if (latchState[ddButtonRow - 1][ddButtonCol - 1] && !switchMode[Row][Column + 1])  //两层之间跳转 
    {
        Number = Number + 12;
        if (!switchMode[Row][Column])
        {
            for (int i = 0; i < 12; i++) //处于高层时，清除低层12档位开关状态
            {
                Joystick.releaseButton(i + Number - 12);
            }
        }

    }


    //找到编码器绝对位置

    bool Pin1 = rawState[Row][Column];
    bool Pin2 = rawState[Row][Column + 1];
    bool Pin3 = rawState[Row][Column + 2];
    bool Pin4 = rawState[Row][Column + 3];

    bool array[4] = { Pin1, Pin2, Pin3, Pin4 };

    int pos = 0;

    for (int i = 0; i < 4; i++)
    {
        if (array[i])
        {
            pos |= (1 << i);
        }
    }

    pos = pos ^ (pos >> 4);
    pos = pos ^ (pos >> 2);
    pos = pos ^ (pos >> 1);

    int result = pos;

    //旋转过程中去抖动

    if (pushState[Row][Column] != result)
    {
        if (globalClock - switchTimer[Row][Column] > encoderPulse)
        {
            switchTimer[Row][Column] = globalClock;
        }
        else if (globalClock - switchTimer[Row][Column] > encoderWait)
        {


            //----------------------------------------------
            //------------------咬合点设定------------------
            //----------------------------------------------


            if (pushState[biteButtonRow - 1][biteButtonCol - 1] == 1)  //按下咬合点按钮时启用旋钮调整
            {
                if (!biteButtonBit1 && !biteButtonBit2)
                {
                    biteButtonBit1 = true;
                }
            }
            
            //----------------------------------------------
            //-------------------模式切换-------------------
            //----------------------------------------------
            //因为占位符的存在，模式切换仅在编码器旋转时才会生效
            //如果你不想推送模式值给位字段，就把fieldPlacement设定为0

            if (pushState[modButtonRow - 1][modButtonCol - 1] == 1)
            {
                for (int i = 0; i < 25; i++)  //清除模式3下的按钮状态
                {
                    Joystick.releaseButton(i - 1 + Number);
                }

                if (switchMode[Row][Column] && switchMode[Row][Column + 1])  //退出模式4，回到DDS模式
                {
                    switchMode[Row][Column + 1] = false;
                    switchMode[Row][Column] = false;
                    latchState[hybridButtonRow - 1][hybridButtonCol - 1] = 0; //进入混合模式，将混合按钮锁定状态设置为0
                }
                else if (!switchMode[Row][Column] && !switchMode[Row][Column + 1]) //从模式1（DDS）跳转到模式3
                {
                    switchMode[Row][Column + 1] = true;
                    switchMode[Row][Column] = false;
                }
                else if (!switchMode[Row][Column] && switchMode[Row][Column + 1]) //从模式3切换到模式4
                {
                    switchMode[Row][Column + 1] = true;
                    switchMode[Row][Column] = true;
                }
            }

            if (!biteButtonBit1 && !biteButtonBit2) //标准模式，未启用咬合点设定
            {
                //启用编码器脉冲计时器
                switchTimer[Row][Column + 1] = globalClock;

                //更新差值，暂存到针脚2的pushState中
                pushState[Row][Column + 1] = result - pushState[Row][Column];

                //给针脚1的pushState赋新值
                pushState[Row][Column] = result;

                //如果处于DDS混合模式，更新计数器
                if (!switchMode[Row][Column] && !switchMode[Row][Column + 1])
                {
                    if ((pushState[Row][Column + 1] > 0 && pushState[Row][Column + 1] < 8) || pushState[Row][Column + 1] < -8)
                    {
                        toggleTimer[Row][Column] = toggleTimer[Row][Column] + 1 - (2 * Reverse);
                    }
                    else
                    {
                        toggleTimer[Row][Column] = toggleTimer[Row][Column] - 1 + (2 * Reverse);
                    }
                    if (toggleTimer[Row][Column] < 0)
                    {
                        toggleTimer[Row][Column] = HyPos;
                    }
                }

                //如果不在混合模式，将计数器清零
                else if (switchMode[Row][Column + 1])
                {
                    toggleTimer[Row][Column] = 0;
                }
            }
            else  //启用咬合点设定
            {
                switchTimer[Row][Column + 1] = globalClock;

                pushState[Row][Column + 1] = result - pushState[Row][Column];

                pushState[Row][Column] = result;

                if ((((pushState[Row][Column + 1] > 0 && pushState[Row][Column + 1] < 2) || pushState[Row][Column + 1] < -2) && Reverse == 0) || (!((pushState[Row][Column + 1] > 0 && pushState[Row][Column + 1] < 2) || pushState[Row][Column + 1] < -2) && Reverse == 1)) //顺时针增加咬合点值
                {
                    if (biteButtonBit1 && !biteButtonBit2)  //初始状态10，单次量程100（即10%）
                    {
                        bitePoint = bitePoint + 100;
                        if (bitePoint > 1000)
                        {
                            bitePoint = 1000;  //咬合点值不能超过1000
                        }
                    }
                    else if (!biteButtonBit1 && biteButtonBit2)  //切换到状态01，单次量程10（即1%）
                    {
                        bitePoint = bitePoint + 10;
                        if (bitePoint > 1000)
                        {
                            bitePoint = 1000;
                        }
                    }
                    else if (biteButtonBit1 && biteButtonBit2)  //切换到状态11，单次量程1（即0.1%）
                    {
                        bitePoint = bitePoint + 1;
                        if (bitePoint > 1000)
                        {
                            bitePoint = 1000;
                        }
                    }
                }
                else //逆时针减少咬合点值
                {
                    if (biteButtonBit1 && !biteButtonBit2)
                    {
                        bitePoint = bitePoint - 100;
                        if (bitePoint < 0)
                        {
                            bitePoint = 0;  //咬合点值不能低于0
                        }
                    }
                    else if (!biteButtonBit1 && biteButtonBit2)
                    {
                        bitePoint = bitePoint - 10;
                        if (bitePoint < 0)
                        {
                            bitePoint = 0;
                        }
                    }
                    else if (biteButtonBit1 && biteButtonBit2)
                    {
                        bitePoint = bitePoint - 1;
                        if (bitePoint < 0)
                        {
                            bitePoint = 0;
                        }
                    }
                }
            }
        }
    }

    //处于DDS混合模式时，允许切换开放式和封闭式
    if (!switchMode[Row][Column + 1])
    {
        switchMode[Row][Column] = latchState[hybridButtonRow - 1][hybridButtonCol - 1];
    }

    //如果没有处于混合模式，重设DD按钮状态
    if (switchMode[Row][Column + 1])
    {
        latchLock[ddButtonRow - 1][ddButtonCol - 1] = false;
        latchState[ddButtonRow - 1][ddButtonCol - 1] = false;
    }

    if (!biteButtonBit1 && !biteButtonBit2) //没有处于咬合点设定时，可以切换开关模式
    {
        //开关模式3：16档开关

        if (!switchMode[Row][Column] && switchMode[Row][Column + 1])
        {
            pushState[Row][Column + 1] = 0; //清除暂存的差值

            for (int i = 0; i < 24; i++)
            {
                if (i == pushState[Row][Column])
                {
                    Joystick.pressButton(i + Number);  //按下对应按钮并释放其它按钮
                }
                else
                {
                    Joystick.releaseButton(i + Number);
                }
            }
        }

        //开关模式2和4：增量编码器和封闭式混合模式旋钮

        else if (switchMode[Row][Column])
        {
            Number = buttonNumber[Row][Column + 1];
            int difference = pushState[Row][Column + 1];
            if (difference != 0)  //暂存差值不为0时（即两个针脚状态有差别时）
            {
                if (globalClock - switchTimer[Row][Column + 1] < encoderPulse)
                {
                    if ((difference > 0 && difference < 6) || difference < -6)  //顺时针旋转时触发+1（reverse为1则为-1）
                    {
                        Joystick.setButton(Number + Reverse, 1);
                        Joystick.setButton(Number + 1 - Reverse, 0);
                    }
                    else  //逆时针旋转时触发-1（reverse为1则为+1）
                    {
                        Joystick.setButton(Number + Reverse, 0);
                        Joystick.setButton(Number + 1 - Reverse, 1);
                    }
                }
                else  //刷新时间到后，更新开关状态
                {
                    pushState[Row][Column + 1] = 0;
                    Joystick.setButton(Number, 0);
                    Joystick.setButton(Number + 1, 0);
                }
            }
        }

        //开关模式1：开放式混合模式旋钮
        if (!switchMode[Row][Column] && !switchMode[Row][Column + 1])
        {

            for (int i = 1; i < HyPos + 13; i++)  //按下对应档位的按钮，释放其它按钮
            {
                int e = toggleTimer[Row][Column] % HyPos;
                if (e == 0)
                {
                    e = HyPos;
                }
                if (i == e)
                {
                    Joystick.pressButton(i - 1 + Number);
                }
                else
                {
                    Joystick.releaseButton(i - 1 + Number);
                }
            }
        }

        //在模式切换时清除重设所有开关状态
        if (latchLock[ddButtonRow - 1][ddButtonCol - 1] && !(switchMode[Row][Column] && !switchMode[Row][Column + 1]))
        {
            for (int i = 0; i < 13; i++) //清除模式1下的按钮状态
            {
                Joystick.releaseButton(i - 1 + buttonNumber[Row][Column]);
            }
        }
    }



    //传递模式值给编码器位字段
    long push = 0;
    push = push | switchMode[Row][Column];
    push = push | (switchMode[Row][Column + 1] << 1);
    push = push << (2*(FieldPlacement - 1));  //注意此处位字段的位置与模式的位置不同，所有的DDS模式公用15 16位
    rotaryField = rotaryField | push;
}