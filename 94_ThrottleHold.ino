//油门保持功能按钮函数，通过拨动开关将油门锁定在某个值
void throttleHoldT(int8_t buttonRow, int8_t buttonCol, int8_t rotaryRow, int8_t rotaryCol, bool reverse)  //参数为按钮行、列号，用于调整参数的编码器行、列号，是否反向旋转调整参数
{
    int8_t ButtonRow = buttonRow-1;
    int8_t ButtonCol = buttonCol-1;
    int8_t Row = rotaryRow - 1;
    int8_t Column = rotaryCol - 1;
    int8_t Number = buttonNumber[Row][Column];
    int8_t Reverse = reverse;

    //按钮状态刷新逻辑
    if (pushState[ButtonRow][ButtonCol] != rawState[ButtonRow][ButtonCol] && (globalClock - switchTimer[ButtonRow][ButtonCol]) > buttonCooldown)
    {
        switchTimer[ButtonRow][ButtonCol] = globalClock;
        pushState[ButtonRow][ButtonCol] = rawState[ButtonRow][ButtonCol];
    }

    if ((globalClock - switchTimer[ButtonRow][ButtonCol]) > buttonCooldown)
    {
        pushState[ButtonRow][ButtonCol] = rawState[ButtonRow][ButtonCol];
    }

    //编码器调整参数逻辑，同rotary2Inc()

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
            switchTimer[Row][Column + 1] = globalClock;

            pushState[Row][Column + 1] = result - pushState[Row][Column];

            pushState[Row][Column] = result;

            latchLock[Row][Column] = false;

            int8_t difference = pushState[Row][Column + 1];
            if ((difference > 0 && difference < 2) || difference < -2)
            {
                if (brakeMagicOn)  //如果打开了魔法刹车开关，则编码器用于增加魔法刹车值（量程为10，即1%）
                {
                    brakeMagicValue = brakeMagicValue + 10 - (20 * Reverse);
                }
                else if (pushState[ButtonRow][ButtonCol]==1)  //如果按下了油门保持按钮，则编码器增加调整油门保持值（量程为10，即1%）
                {
                    throttleHoldValue = throttleHoldValue + 10 - (20 * Reverse);
                }
            }
            else if ((difference < 0 && difference > -2) || difference > 2)  //反向旋转为减少
            {
                if (brakeMagicOn)
                {
                    brakeMagicValue = brakeMagicValue - 10 + (20 * Reverse);
                }
                else if (pushState[ButtonRow][ButtonCol]==1)
                {
                    throttleHoldValue = throttleHoldValue - 10 + (20 * Reverse);
                }
            }
        }
    }

    int8_t difference = pushState[Row][Column + 1];
    if (difference != 0 && !brakeMagicOn && pushState[ButtonRow][ButtonCol] == 0)  //魔法刹车和油门保持都没打开时，编码器为一般增减调整功能
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

    if (pushState[ButtonRow][ButtonCol] == 1)
    {
        Joystick.setThrottle(throttleHoldValue);  //功能打开时，将油门保持值赋予油门轴
    }
    else
    {
        Joystick.setThrottle(0);  //功能关闭时，将油门轴归0
    }

    //传递油门保持模式激活状态给按钮位字段
    int32_t push = pushState[ButtonRow][ButtonCol];
    push = push << 8;
    buttonField = buttonField | push;
}

//油门保持功能按钮函数，通过点动开关将油门锁定在某个值
void throttleHoldM(int8_t buttonRow, int8_t buttonCol, int8_t rotaryRow, int8_t rotaryCol, bool reverse)
{
    int8_t ButtonRow = buttonRow - 1;
    int8_t ButtonCol = buttonCol - 1;
    int8_t Row = rotaryRow - 1;
    int8_t Column = rotaryCol - 1;
    int8_t Number = buttonNumber[Row][Column];
    int8_t Reverse = reverse;

    if (pushState[ButtonRow][ButtonCol] != rawState[ButtonRow][ButtonCol] && (globalClock - switchTimer[ButtonRow][ButtonCol]) > buttonCooldown)
    {
        switchTimer[ButtonRow][ButtonCol] = globalClock;
        pushState[ButtonRow][ButtonCol] = rawState[ButtonRow][ButtonCol];
    }

    if ((globalClock - switchTimer[ButtonRow][ButtonCol]) > buttonCooldown)
    {
        pushState[ButtonRow][ButtonCol] = rawState[ButtonRow][ButtonCol];
    }

    if (pushState[ButtonRow][ButtonCol] == 0)
    {
        latchLock[ButtonRow][ButtonCol] = false;
    }

    if (pushState[ButtonRow][ButtonCol] == 1 && !latchLock[ButtonRow][ButtonCol])
    {
        latchLock[ButtonRow][ButtonCol] = true;
        latchState[ButtonRow][ButtonCol] = !latchState[ButtonRow][ButtonCol];
    }


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
            switchTimer[Row][Column + 1] = globalClock;

            pushState[Row][Column + 1] = result - pushState[Row][Column];

            pushState[Row][Column] = result;

            latchLock[Row][Column] = false;

            int8_t difference = pushState[Row][Column + 1];
            if ((difference > 0 && difference < 2) || difference < -2)
            {
                if (brakeMagicOn)
                {
                    brakeMagicValue = brakeMagicValue + 10 - (20 * Reverse);
                }
                else if (latchState[ButtonRow][ButtonCol])
                {
                    throttleHoldValue = throttleHoldValue + 10 - (20 * Reverse);
                }
            }
            else if ((difference < 0 && difference > -2) || difference > 2)
            {
                if (brakeMagicOn)
                {
                    brakeMagicValue = brakeMagicValue - 10 + (20 * Reverse);
                }
                else if (latchState[ButtonRow][ButtonCol])
                {
                    throttleHoldValue = throttleHoldValue - 10 + (20 * Reverse);
                }
            }
        }
    }

    int8_t difference = pushState[Row][Column + 1];
    if (difference != 0 && !brakeMagicOn && !latchState[ButtonRow][ButtonCol])
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

    if (latchState[ButtonRow][ButtonCol])
    {
        Joystick.setThrottle(throttleHoldValue);
    }
    else
    {
        Joystick.setThrottle(0);
    }

    int32_t push = latchState[ButtonRow][ButtonCol];
    push = push << 8;
    buttonField = buttonField | push;
}

void throttleHoldSW1(int8_t buttonRow, int8_t buttonCol, int8_t rotaryRow, int8_t rotaryColA, int rotaryColB, bool reverse)
{
    int8_t ButtonRow = buttonRow - 1;
    int8_t ButtonCol = buttonCol - 1;
    int Row = rotaryRow - 1;
    int Column = rotaryColA - 1;
    int Number = buttonNumber[rotaryRow][rotaryColA];

    int bCol = rotaryColB - 1;
    
    int8_t Reverse = reverse;

    //按钮逻辑
    if (pushState[ButtonRow][ButtonCol] != rawState[ButtonRow][ButtonCol] && (globalClock - switchTimer[ButtonRow][ButtonCol]) > buttonCooldown)
    {
        switchTimer[ButtonRow][ButtonCol] = globalClock;
        pushState[ButtonRow][ButtonCol] = rawState[ButtonRow][ButtonCol];
    }

    if ((globalClock - switchTimer[ButtonRow][ButtonCol]) > buttonCooldown)
    {
        pushState[ButtonRow][ButtonCol] = rawState[ButtonRow][ButtonCol];
    }


    //编码器逻辑
    if (!rawState[Row][Column] && !rawState[Row][bCol])
    {
        pushState[Row][Column] = 1;
    }
    else if (!rawState[Row][Column] && rawState[Row][bCol])
    {
        pushState[Row][Column] = 2;
        latchLock[Row][Column] = 1; //Fetching 01
    }
    else if (rawState[Row][Column] && rawState[Row][bCol])
    {
        pushState[Row][Column] = 3;
    }
    else if (rawState[Row][Column] && !rawState[Row][bCol])
    {
        pushState[Row][Column] = 4;
        latchLock[Row][bCol] = 1; //Fetching 10
    }

    if ((globalClock - switchTimer[Row][Column] > funkyCooldown) && (globalClock - switchTimer[Row][bCol] > funkyCooldown))
    {
        if ((latchLock[Row][bCol] && pushState[Row][Column] == 1) || (latchLock[Row][Column] && pushState[Row][Column] == 3))
        {
            switchTimer[Row][Column] = globalClock;

            if (brakeMagicOn)
            {
                brakeMagicValue = brakeMagicValue + 10 - (20 * Reverse);
            }
            else if (pushState[ButtonRow][ButtonCol] == 1)
            {
                throttleHoldValue = throttleHoldValue + 10 - (20 * Reverse);
            }
        }

        else if ((latchLock[Row][bCol] && pushState[Row][Column] == 3) || (latchLock[Row][Column] && pushState[Row][Column] == 1))
        {
            switchTimer[Row][bCol] = globalClock;
            if (brakeMagicOn)
            {
                brakeMagicValue = brakeMagicValue - 10 + (20 * Reverse);
            }
            else if (pushState[ButtonRow][ButtonCol] == 1)
            {
                throttleHoldValue = throttleHoldValue - 10 + (20 * Reverse);
            }
            
        }
    }

    else
    {
        latchLock[Row][bCol] = 0;
        latchLock[Row][Column] = 0;
    }
    if (!brakeMagicOn && pushState[ButtonRow][ButtonCol] == 0) //魔法刹车和油门保持都没打开时，编码器为一般增减调整功能
    {
      Joystick.setButton(Number + Reverse, (globalClock - switchTimer[Row][Column] < funkyPulse));
      Joystick.setButton(Number + 1 - Reverse, (globalClock - switchTimer[Row][bCol] < funkyPulse));
    }

    //数值调整 
    if (brakeMagicValue < 0)
    {
      brakeMagicValue = 0;
    }
    else if (brakeMagicValue > 1000)
    {
      brakeMagicValue = 1000;
    }

    if (throttleHoldValue < 0)
    {
      throttleHoldValue = 0;
    }
    else if (throttleHoldValue > 1000)
    {
      throttleHoldValue = 1000;
    }

    
    if (pushState[ButtonRow][ButtonCol] == 1)
    {
        Joystick.setThrottle(throttleHoldValue);
    }
    else
    {
        Joystick.setThrottle(0);
    }

    long push = pushState[ButtonRow][ButtonCol];
    push = push << 5;
    buttonField = buttonField | push;
}