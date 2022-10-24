//使用2位编码器和8个预设矩阵，这8个矩阵只能存储按钮和模拟输入的模式，无法存储其他预设值（如按钮函数定义、咬合点值等），在按下mod按钮的同时在8个预设中进行切换的函数
void presetSwitch(int row, int column, bool reverse)
{
    int Row = row - 1;
    int Column = column - 1;
    int Number = buttonNumber[Row][Column];
    int FieldPlacement = 11;
    int Reverse = reverse;

    //实现逻辑同Rotary2Bit，需要2位灰码编码器

    bool Pin1 = rawState[Row][Column];
    bool Pin2 = rawState[Row][Column + 1];

    bool array[2] = { Pin1, Pin2 };

    int pos = 0;

    for (int i = 0; i < 2; i++)
    {
        if (array[i])
        {
            pos |= (1 << i);
        }
    }

    pos = pos ^ (pos >> 1);

    int result = pos;


    if (pushState[Row][Column] != result)
    {
        if (globalClock - switchTimer[Row][Column] > (encoder2Wait + encoder2Pulse + encoderCooldown))
        {
            switchTimer[Row][Column] = globalClock;
            latchLock[Row][Column] = true;
        }
        else if ((globalClock - switchTimer[Row][Column] > encoder2Wait) && latchLock[Row][Column])
        {
            //----------------------------------------------
            //------------------预设切换--------------------
            //----------------------------------------------

            //因为占位符的存在，模式切换仅在编码器旋转时才会生效

            switchTimer[Row][Column + 1] = globalClock;

            pushState[Row][Column + 1] = result - pushState[Row][Column];

            int8_t difference = pushState[Row][Column + 1];

            if (pushState[modButtonRow - 1][modButtonCol - 1] == 1)  //按下mod按钮的同时生效
            {
                if ((difference > 0 && difference < 2) || difference < -2)
                {
                    switchPreset = switchPreset + 1 - (2 * Reverse);
                }
                else if ((difference < 0 && difference > -2) || difference > 2)
                {
                    switchPreset = switchPreset - 1 + (2 * Reverse);
                }
                if (switchPreset < 0)
                {
                    switchPreset = 7;
                }
                if (switchPreset > 7)  //到7后归0
                {
                    switchPreset = 0;
                }

                //切换预设后清除所有按钮的按下状态
                for (int i = 0; i < BUTTONCOUNT; i++)
                {
                    Joystick.releaseButton(i);
                }

                //加载预设矩阵

        //1
                if (switchPreset == 0)
                {
                    for (int e = 0; e < rowCount; e++)
                    {

                        for (int a = 0; a < colCount; a++)
                        {
                            switchMode[e][a] = preset1[e][a];
                        }
                    }
                    for (int o = 0; o < 10; o++)
                    {
                        analogSwitchMode1[o] = analogMode1Preset1[o];
                        analogSwitchMode2[o] = analogMode2Preset1[o];
                    }
                }
                //2
                else if (switchPreset == 1)
                {
                    for (int e = 0; e < rowCount; e++)
                    {

                        for (int a = 0; a < colCount; a++)
                        {
                            switchMode[e][a] = preset2[e][a];
                        }
                    }
                    for (int o = 0; o < 10; o++)
                    {
                        analogSwitchMode1[o] = analogMode1Preset2[o];
                        analogSwitchMode2[o] = analogMode2Preset2[o];
                    }
                }
                //3
                else if (switchPreset == 2)
                {
                    for (int e = 0; e < rowCount; e++)
                    {

                        for (int a = 0; a < colCount; a++)
                        {
                            switchMode[e][a] = preset3[e][a];
                        }
                    }
                    for (int o = 0; o < 10; o++)
                    {
                        analogSwitchMode1[o] = analogMode1Preset3[o];
                        analogSwitchMode2[o] = analogMode2Preset3[o];
                    }
                }
                //4
                else if (switchPreset == 3)
                {
                    for (int e = 0; e < rowCount; e++)
                    {

                        for (int a = 0; a < colCount; a++)
                        {
                            switchMode[e][a] = preset4[e][a];
                        }
                    }
                    for (int o = 0; o < 10; o++)
                    {
                        analogSwitchMode1[o] = analogMode1Preset4[o];
                        analogSwitchMode2[o] = analogMode2Preset4[o];
                    }
                }
                //5
                else if (switchPreset == 4)
                {
                    for (int e = 0; e < rowCount; e++)
                    {

                        for (int a = 0; a < colCount; a++)
                        {
                            switchMode[e][a] = preset5[e][a];
                        }
                    }
                    for (int o = 0; o < 10; o++)
                    {
                        analogSwitchMode1[o] = analogMode1Preset5[o];
                        analogSwitchMode2[o] = analogMode2Preset5[o];
                    }
                }
                //6
                else if (switchPreset == 5)
                {
                    for (int e = 0; e < rowCount; e++)
                    {

                        for (int a = 0; a < colCount; a++)
                        {
                            switchMode[e][a] = preset6[e][a];
                        }
                    }
                    for (int o = 0; o < 10; o++)
                    {
                        analogSwitchMode1[o] = analogMode1Preset6[o];
                        analogSwitchMode2[o] = analogMode2Preset6[o];
                    }
                }
                //7
                else if (switchPreset == 6)
                {
                    for (int e = 0; e < rowCount; e++)
                    {

                        for (int a = 0; a < colCount; a++)
                        {
                            switchMode[e][a] = preset7[e][a];
                        }
                    }
                    for (int o = 0; o < 10; o++)
                    {
                        analogSwitchMode1[o] = analogMode1Preset7[o];
                        analogSwitchMode2[o] = analogMode2Preset7[o];
                    }
                }
                //8
                else if (switchPreset == 7)
                {
                    for (int e = 0; e < rowCount; e++)
                    {

                        for (int a = 0; a < colCount; a++)
                        {
                            switchMode[e][a] = preset8[e][a];
                        }
                    }
                    for (int o = 0; o < 10; o++)
                    {
                        analogSwitchMode1[o] = analogMode1Preset8[o];
                        analogSwitchMode2[o] = analogMode2Preset8[o];
                    }
                }
            }

            pushState[Row][Column] = result;
            latchLock[Row][Column] = false;
        }
    }

    int8_t difference = pushState[Row][Column + 1];  //普通模式下就是一个增量编码器
    if (difference != 0)
    {
        if (globalClock - switchTimer[Row][Column + 1] < encoder2Pulse + encoder2Wait)
        {
            if ((difference > 0 && difference < 2) || difference < -2)
            {
                Joystick.setButton(Number + Reverse, 1);
                Joystick.setButton(Number + 1 - Reverse, 0);
            }
            else if ((difference < 0 && difference > -2) || difference > 2)
            {
                Joystick.setButton(Number + Reverse, 0);
                Joystick.setButton(Number + 1 - Reverse, 1);
            }
            else
            {
                pushState[Row][Column + 1] = 0;
            }
        }
        else
        {
            pushState[Row][Column + 1] = 0;
            pushState[Row][Column] = result;
            Joystick.setButton(Number, 0);
            Joystick.setButton(Number + 1, 0);
        }
    }


    //将预设值推送给按钮位字段

    long push = 0;
    push = push | (switchPreset << 11);
    buttonField = buttonField | push;


}
