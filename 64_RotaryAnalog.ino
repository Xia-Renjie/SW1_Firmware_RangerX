//------------------------------
//--------电阻式旋转开关---------
//------------------------------

//4模式模拟旋转编码器
void rotaryAnalog(int analogPin, int switchNumber, int fieldPlacement, int hybridPositions, int pos1, int pos2, int pos3, int pos4, int pos5, int pos6, int pos7, int pos8, int pos9, int pos10, int pos11, int pos12, bool reverse)
//参数为模拟输入针脚，模拟输入编号，编码器位字段，自定义档位数，1-12档位读数，是否反向旋转
{
    int Pin = analogPin;
    int Pos1 = pos1;
    int Pos2 = pos2;
    int Pos3 = pos3;
    int Pos4 = pos4;
    int Pos5 = pos5;
    int Pos6 = pos6;
    int Pos7 = pos7;
    int Pos8 = pos8;
    int Pos9 = pos9;
    int Pos10 = pos10;
    int Pos11 = pos11;
    int Pos12 = pos12;

    int N = switchNumber - 1;
    bool Reverse = reverse;

    int Number = analogButtonNumber[N];
    int FieldPlacement = fieldPlacement;
    int HyPos = hybridPositions;

    int maxPos = max(12, HyPos);


    int value = analogRead(Pin);  //读取模拟针脚的读数

    int positions[12] = { Pos1, Pos2, Pos3, Pos4, Pos5, Pos6, Pos7, Pos8, Pos9, Pos10, Pos11, Pos12 };

    int differ = 0;
    int result = 0;
    for (int i = 0; i < 12; i++)  //根据读数确定值最接近的档位为当前档位
    {
        if (i == 0 || abs(positions[i] - value) < differ)
        {
            result++;
            differ = abs(positions[i] - value);
        }
    }

    result--;

    if (Reverse)  //定义为反向旋转时确定当前档位
    {
        result = 11 - result;
    }

    //旋转过程中去抖动

    if (analogLastCounter[N] != result)
    {
        if (globalClock - analogTimer1[N] > analogPulse)
        {
            analogTimer1[N] = globalClock;
        }
        else if (globalClock - analogTimer1[N] > analogWait)
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

                if (analogSwitchMode1[N] && analogSwitchMode2[N])  //退出模式4
                {
                    analogSwitchMode2[N] = false;
                    analogSwitchMode1[N] = false;
                }
                else  //在模式1-3之间切换
                {
                    analogSwitchMode1[N] = !analogSwitchMode1[N];  //当前为模式1时切换到模式2
                    if (!analogSwitchMode1[N]) //模式2时切换成模式3，将混合按钮锁定状态设置为0
                    {
                        analogSwitchMode2[N] = true;
                        latchState[hybridButtonRow - 1][hybridButtonCol - 1] = 0;
                    }
                    else
                    {
                        if (analogSwitchMode2[N])  //从模式3跳过模式4，切换到模式1
                        {
                            analogSwitchMode2[N] = false;
                            analogSwitchMode1[N] = false;
                        }
                    }
                }
            }

            //激活编码器脉冲计时器
            analogTimer2[N] = globalClock;

            //将档位与初始的差值暂存到2号针脚
            analogTempState[N] = result - analogLastCounter[N];

            //更新档位状态
            analogLastCounter[N] = result;

            //如果处于模式3，更新计数器
            if (!analogSwitchMode1[N] && analogSwitchMode2[N])
            {
                if ((analogTempState[N] > 0 && analogTempState[N] < 6) || analogTempState[N] < -6)  //向增大方向旋转时计数器+1
                {
                    analogRotaryCount[N] ++;
                }
                else //向减小方向旋转时计数器-1
                {
                    analogRotaryCount[N] --;
                }
                if (analogRotaryCount[N] < 0)  //计数器小于0时，从最高档位重新开始计数
                {
                    analogRotaryCount[N] = HyPos - 1;
                }
            }

            //如果不在混合模式，将计数器清零
            else if (!analogSwitchMode2[N])
            {
                analogRotaryCount[N] = 0;
            }
        }
    }

    //处于混合模式时，允许切换开放式和封闭式
    if (analogSwitchMode2[N])
    {
        analogSwitchMode1[N] = latchState[hybridButtonRow - 1][hybridButtonCol - 1];
    }

    //开关模式1：12档开关

    if (!analogSwitchMode1[N] && !analogSwitchMode2[N])
    {
        analogTempState[N] = 0; //清除暂存的差值

        for (int i = 0; i < 12; i++) //按下对应按钮并释放其它按钮
        {
            if (i == analogLastCounter[N])
            {
                Joystick.pressButton(i + Number);
            }
            else
            {
                Joystick.releaseButton(i + Number);
            }
        }
    }

    //开关模式2和4：增量编码器和封闭式混合模式旋钮

    else if (analogSwitchMode1[N])
    {
        Number = analogButtonNumberIncMode[N];
        int difference = analogTempState[N];
        if (difference != 0)
        {
            if (globalClock - analogTimer2[N] < encoderPulse)
            {
                if ((difference > 0 && difference < 6) || difference < -6)
                {
                    Joystick.setButton(Number, 1);
                    Joystick.setButton(Number + 1, 0);
                }
                else
                {
                    Joystick.setButton(Number, 0);
                    Joystick.setButton(Number + 1, 1);
                }
            }
            else
            {
                analogTempState[N] = 0;
                Joystick.setButton(Number, 0);
                Joystick.setButton(Number + 1, 0);
            }
        }
    }

    //开关模式3：开放式混合模式旋钮
    if (!analogSwitchMode1[N] && analogSwitchMode2[N])
    {

        for (int i = 1; i < HyPos + 1; i++)
        {
            int e = analogRotaryCount[N] % HyPos;
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
    int32_t push = 0;
    push = push | analogSwitchMode1[N];
    push = push | (analogSwitchMode2[N] << 1);
    push = push << (FieldPlacement - 1);
    rotaryField = rotaryField | push;
}

//双功能4模式模拟旋转编码器
void rotaryAnalog2Mode(int analogPin, int switchNumber, int fieldPlacement, int pos1, int pos2, int pos3, int pos4, int pos5, int pos6, int pos7, int pos8, int pos9, int pos10, int pos11, int pos12, bool reverse)
//参数为模拟输入针脚，模拟输入编号，编码器位字段，1-12档位读数，是否反向旋转
{
    int Pin = analogPin;
    int Pos1 = pos1;
    int Pos2 = pos2;
    int Pos3 = pos3;
    int Pos4 = pos4;
    int Pos5 = pos5;
    int Pos6 = pos6;
    int Pos7 = pos7;
    int Pos8 = pos8;
    int Pos9 = pos9;
    int Pos10 = pos10;
    int Pos11 = pos11;
    int Pos12 = pos12;

    bool Reverse = reverse;

    int N = switchNumber - 1;

    int Number = analogButtonNumber[N];
    int FieldPlacement = fieldPlacement;

    int maxPos = 12;  //档位数固定为12


    int value = analogRead(Pin);

    int positions[12] = { Pos1, Pos2, Pos3, Pos4, Pos5, Pos6, Pos7, Pos8, Pos9, Pos10, Pos11, Pos12 };

    int differ = 0;
    int result = 0;
    for (int i = 0; i < 12; i++)
    {
        if (i == 0 || abs(positions[i] - value) < differ)
        {
            result++;
            differ = abs(positions[i] - value);
        }
    }

    result--;

    if (Reverse)
    {
        result = 11 - result;
    }


    if (analogLastCounter[N] != result)
    {
        if (globalClock - analogTimer1[N] > analogPulse)
        {
            analogTimer1[N] = globalClock;
        }
        else if (globalClock - analogTimer1[N] > analogWait)
        {

            //----------------------------------------------
            //------------------咬合点设定------------------
            //----------------------------------------------


            if (pushState[biteButtonRow - 1][biteButtonCol - 1] == 1)
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

            else if (pushState[modButtonRow - 1][modButtonCol - 1] == 1 && FieldPlacement != 0)
            {
                for (int i = 0; i < maxPos + 1; i++) //清除模式1下的按钮状态
                {
                    Joystick.releaseButton(i - 1 + Number);
                }

                analogSwitchMode1[N] = !analogSwitchMode1[N]; //切换模式
            }

            if (!biteButtonBit1 && !biteButtonBit2) //标准模式，未启用咬合点设定
            {
                //启用编码器脉冲计时器
                analogTimer2[N] = globalClock;

                //更新差值，暂存到针脚2的pushState中
                analogTempState[N] = result - analogLastCounter[N];

                //给计数器赋新值
                analogLastCounter[N] = result;
            }

            else //启用咬合点设定
            {
                analogTimer2[N] = globalClock;

                analogTempState[N] = result - analogLastCounter[N];

                analogLastCounter[N] = result;

                if ((analogTempState[N] > 0 && analogTempState[N] < 6) || analogTempState[N] < -6)  //向增加方向旋转时提高咬合点值
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
                else
                {
                    if (biteButtonBit1 && !biteButtonBit2)  //向减小方向旋转时降低咬合点值
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

    //开关模式1：12档开关

    if (!analogSwitchMode1[N] && !biteButtonBit1 && !biteButtonBit2)
    {
        analogTempState[N] = 0; //清除暂存的差值

        for (int i = 0; i < 12; i++)
        {
            if (i == analogLastCounter[N])
            {
                Joystick.pressButton(i + Number);
            }
            else
            {
                Joystick.releaseButton(i + Number);
            }
        }
    }

    //开关模式2和4：增量编码器和封闭式混合模式旋钮

    else if (analogSwitchMode1[N] && !biteButtonBit1 && !biteButtonBit2)
    {
        Number = analogButtonNumberIncMode[N];
        int difference = analogTempState[N];
        if (difference != 0)
        {
            if (globalClock - analogTimer2[N] < encoderPulse)
            {
                if ((difference > 0 && difference < 6) || difference < -6)
                {
                    Joystick.setButton(Number, 1);
                    Joystick.setButton(Number + 1, 0);
                }
                else
                {
                    Joystick.setButton(Number, 0);
                    Joystick.setButton(Number + 1, 1);
                }
            }
            else
            {
                analogTempState[N] = 0;
                Joystick.setButton(Number, 0);
                Joystick.setButton(Number + 1, 0);
            }
        }
    }

    //传递模式值给编码器位字段
    int32_t push = 0;
    push = push | analogSwitchMode1[N];
    push = push << (FieldPlacement - 1);
    rotaryField = rotaryField | push;
}

//DDS模式模拟编码器
void DDSanalog(int analogPin, int switchNumber, int pos1, int pos2, int pos3, int pos4, int pos5, int pos6, int pos7, int pos8, int pos9, int pos10, int pos11, int pos12, bool reverse)
//参数为模拟输入针脚，模拟输入编号，1-12档位读数，是否反向旋转
{
    int Pin = analogPin;
    int Pos1 = pos1;
    int Pos2 = pos2;
    int Pos3 = pos3;
    int Pos4 = pos4;
    int Pos5 = pos5;
    int Pos6 = pos6;
    int Pos7 = pos7;
    int Pos8 = pos8;
    int Pos9 = pos9;
    int Pos10 = pos10;
    int Pos11 = pos11;
    int Pos12 = pos12;


    int N = switchNumber - 1;
    int HyPos = 12;  //每一层12个档位

    int Number = analogButtonNumber[N];
    int FieldPlacement = 5;
    bool Reverse = reverse;

    if (latchState[ddButtonRow - 1][ddButtonCol - 1] && !analogSwitchMode2[N])  //两层之间跳转 
    {
        Number = Number + 12;
    }

    int value = analogRead(Pin);

    int positions[12] = { Pos1, Pos2, Pos3, Pos4, Pos5, Pos6, Pos7, Pos8, Pos9, Pos10, Pos11, Pos12 };

    int differ = 0;
    int result = 0;
    for (int i = 0; i < 12; i++) //处于高层时，清除低层12档位开关状态
    {
        if (i == 0 || abs(positions[i] - value) < differ)
        {
            result++;
            differ = abs(positions[i] - value);
        }
    }
    result--;

    if (Reverse)
    {
        result = 11 - result;
    }

    //旋转过程中去抖动

    if (analogLastCounter[N] != result)
    {
        if (globalClock - analogTimer1[N] > analogPulse)
        {
            analogTimer1[N] = globalClock;
        }
        else if (globalClock - analogTimer1[N] > analogWait)
        {

            //----------------------------------------------
            //------------------咬合点设定------------------
            //----------------------------------------------


            if (pushState[biteButtonRow - 1][biteButtonCol - 1] == 1) //按下咬合点按钮时启用旋钮调整
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

            else if (pushState[modButtonRow - 1][modButtonCol - 1] == 1)
            {
                for (int i = 0; i < 24; i++) //清除模式3下的按钮状态
                {
                    Joystick.releaseButton(i + Number);
                }

                if (analogSwitchMode1[N] && analogSwitchMode2[N]) //退出模式4，回到DDS模式
                {
                    analogSwitchMode2[N] = false;
                    analogSwitchMode1[N] = false;
                    latchState[hybridButtonRow - 1][hybridButtonCol - 1] = 0; //进入混合模式，将混合按钮锁定状态设置为0
                }
                else if (!analogSwitchMode1[N] && !analogSwitchMode2[N]) //从模式1（DDS）跳转到模式3
                {
                    analogSwitchMode2[N] = true;
                    analogSwitchMode1[N] = false;
                }
                else if (!analogSwitchMode1[N] && analogSwitchMode2[N]) //从模式3切换到模式4
                {
                    analogSwitchMode2[N] = true;
                    analogSwitchMode1[N] = true;
                }
            }

            if (!biteButtonBit1 && !biteButtonBit2) //标准模式，未启用咬合点设定
            {
                //启用编码器脉冲计时器
                analogTimer2[N] = globalClock;

                //更新差值，暂存到针脚2的pushState中
                analogTempState[N] = result - analogLastCounter[N];

                //给计数器赋新值
                analogLastCounter[N] = result;

                //如果处于DDS混合模式，更新计数器
                if (!analogSwitchMode1[N] && !analogSwitchMode2[N])
                {
                    if ((analogTempState[N] > 0 && analogTempState[N] < 5) || analogTempState[N] < -5)
                    {
                        analogRotaryCount[N] ++;
                    }
                    else
                    {
                        analogRotaryCount[N] --;
                    }
                    if (analogRotaryCount[N] < 0)
                    {
                        analogRotaryCount[N] = HyPos;
                    }
                }

                //如果不在混合模式，将计数器清零
                else if (analogSwitchMode2[N])
                {
                    analogRotaryCount[N] = 0;
                }
            }
            else //启用咬合点设定
            {
                analogTimer2[N] = globalClock;

                analogTempState[N] = result - analogLastCounter[N];

                analogLastCounter[N] = result;

                //调整咬合点值增减
                if ((analogTempState[N] > 0 && analogTempState[N] < 5) || analogTempState[N] < -5)
                {
                    if (biteButtonBit1 && !biteButtonBit2)
                    {
                        bitePoint = bitePoint + 100;
                        if (bitePoint > 1000)
                        {
                            bitePoint = 1000;
                        }
                    }
                    else if (!biteButtonBit1 && biteButtonBit2)
                    {
                        bitePoint = bitePoint + 10;
                        if (bitePoint > 1000)
                        {
                            bitePoint = 1000;
                        }
                    }
                    else if (biteButtonBit1 && biteButtonBit2)
                    {
                        bitePoint = bitePoint + 1;
                        if (bitePoint > 1000)
                        {
                            bitePoint = 1000;
                        }
                    }
                }
                else
                {
                    if (biteButtonBit1 && !biteButtonBit2)
                    {
                        bitePoint = bitePoint - 100;
                        if (bitePoint < 0)
                        {
                            bitePoint = 0;
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
    if (!analogSwitchMode2[N])
    {
        analogSwitchMode1[N] = latchState[hybridButtonRow - 1][hybridButtonCol - 1];
    }
    //如果没有处于混合模式，重设DD按钮状态
    if (analogSwitchMode2[N])
    {
        latchLock[ddButtonRow - 1][ddButtonCol - 1] = false;
        latchState[ddButtonRow - 1][ddButtonCol - 1] = false;
    }

    if (!biteButtonBit1 && !biteButtonBit2) //没有处于咬合点设定时，可以切换开关模式
    {
        //开关模式3：12档开关

        if (!analogSwitchMode1[N] && analogSwitchMode2[N])
        {
            analogTempState[N] = 0; //清除暂存的差值

            for (int i = 0; i < 24; i++)
            {
                if (i == analogLastCounter[N])
                {
                    Joystick.pressButton(i + Number);
                }
                else
                {
                    Joystick.releaseButton(i + Number);
                }
            }
        }

        //开关模式2和4：增量编码器和封闭式混合模式旋钮

        else if (analogSwitchMode1[N])
        {

            Number = analogButtonNumberIncMode[N];
            int difference = analogTempState[N];
            if (difference != 0)
            {
                if (globalClock - analogTimer2[N] < analogPulse)
                {
                    if ((difference > 0 && difference < 5) || difference < -5)
                    {
                        Joystick.setButton(Number, 1);
                        Joystick.setButton(Number + 1, 0);
                    }
                    else
                    {
                        Joystick.setButton(Number, 0);
                        Joystick.setButton(Number + 1, 1);
                    }
                }
                else
                {
                    analogTempState[N] = 0;
                    Joystick.setButton(Number, 0);
                    Joystick.setButton(Number + 1, 0);
                }
            }
        }

        //开关模式1：开放式混合模式旋钮
        if (!analogSwitchMode1[N] && !analogSwitchMode2[N])
        {

            for (int i = 1; i < HyPos + 13; i++)
            {
                int e = (analogRotaryCount[N] % HyPos);
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
        if (latchState[ddButtonRow - 1][ddButtonCol - 1] && !(analogSwitchMode1[N] && !analogSwitchMode2[N]))
        {

            for (int i = 0; i < 12; i++) //清除模式1下的按钮状态
            {
                Joystick.releaseButton(i + analogButtonNumber[N]);
            }
        }
    }


    //传递模式值给编码器位字段
    int32_t push = 0;
    push = push | analogSwitchMode1[N];
    push = push | (analogSwitchMode2[N] << 1);
    push = push << (2*(FieldPlacement - 1));
    rotaryField = rotaryField | push;
}