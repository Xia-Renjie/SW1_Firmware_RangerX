//在自定义12个预设中进行切换的功能函数
void preset2Bit(int row, int column, bool reverse)
{
    int Row = row - 1;
    int Column = column - 1;
    int Number = buttonNumber[Row][Column];
    int Reverse = reverse;

    //功能实现同2位增量编码器

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
            //-------------------切换预设-------------------
            //----------------------------------------------

            //因为占位符的存在，模式切换仅在编码器旋转时才会生效

            switchTimer[Row][Column + 1] = globalClock;

            pushState[Row][Column + 1] = result - pushState[Row][Column];

            int8_t difference = pushState[Row][Column + 1];

            if (pushState[modButtonRow - 1][modButtonCol - 1] == 1)  //按下模式调整按钮时才可以切换预设
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
                    switchPreset = 11;
                }
                if (switchPreset > 11)
                {
                    switchPreset = 0;
                }

                presets(switchPreset);  //调用函数启用预设
            }

            pushState[Row][Column] = result;
            latchLock[Row][Column] = false;
        }
    }

    int8_t difference = pushState[Row][Column + 1];
    //未按下模式调整按钮时，功能同普通2位增量编码器
    if (pushState[modButtonRow - 1][modButtonCol - 1] == 0)
    {
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
    }

    //推送预设值给按钮位字段

    long push = 0;
    push = push | (switchPreset << 10);
    buttonField = buttonField | push;
}
