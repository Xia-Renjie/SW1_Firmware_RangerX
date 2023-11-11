//双离合函数
void dualClutch(int masterPin, int masterSwitchNumber, int masterReleasedValue, int masterFullyPressedValue, int slavePin, int slaveSwitchNumber, int slaveReleasedValue, int slaveFullyPressedValue, bool throttleMaster)
//参数为主拨片模拟针脚、按钮编号、释放时的值、完全按下时的值，从拨片模拟针脚、按钮编号、释放时的值、完全按下时的值，模式4下是否将主拨片模拟为油门
{

    //--------------------------------
    //---------主拨片读数计算----------
    //--------------------------------

    int MasterPin = masterPin;
    int masterValue = analogRead(MasterPin);
    int M = masterSwitchNumber - 1;
    float masterNormalized = 0;
    int FieldPlacement = 7;
    bool ThrottleMaster = throttleMaster;


    if (masterFullyPressedValue > masterReleasedValue)
    {
        masterFullyPressedValue = masterFullyPressedValue - clutchTopDeadzone;  //应用离合器死区
        masterReleasedValue = masterReleasedValue + clutchBottomDeadzone;
        float gap = masterFullyPressedValue - masterReleasedValue;
        float normFactor = 1000 / gap;
        masterNormalized = normFactor * (masterValue - masterReleasedValue);  //将拨片模拟读书转化为离合器值（0-1000）
        if (masterNormalized < 0)
        {
            masterNormalized = 0;  //值不能低于0
        }
    }

    else if (masterFullyPressedValue < masterReleasedValue)  //传感器读数方向相反时
    {
        masterFullyPressedValue = masterFullyPressedValue + clutchTopDeadzone;
        masterReleasedValue = masterReleasedValue - clutchBottomDeadzone;
        float gap = masterReleasedValue - masterFullyPressedValue;
        float normFactor = 1000 / gap;
        masterNormalized = normFactor * (masterReleasedValue - masterValue);
        if (masterNormalized < 0)
        {
            masterNormalized = 0;
        }
    }

    //读取10次取平均值来平滑读数
    total[M] = total[M] - readings[M][readIndex[M]];
    readings[M][readIndex[M]] = masterNormalized;
    total[M] = total[M] + readings[M][readIndex[M]];

    readIndex[M]++;

    if (readIndex[M] > (reads-1))
    {
        readIndex[M] = 0;
    }

    average[M] = total[M] / reads;
    if (average[M] > 1000)
    {
        average[M] = 1000;
    }

    //Serial.println(average[M]);  //本句用于调试时输出当前读数

    //--------------------------------
    //---------从拨片读数计算----------
    //--------------------------------

    int SlavePin = slavePin;
    int slaveValue = analogRead(SlavePin);
    int S = slaveSwitchNumber - 1;
    float slaveNormalized = 0;

    if (slaveFullyPressedValue > slaveReleasedValue)
    {
        slaveFullyPressedValue = slaveFullyPressedValue - clutchTopDeadzone;
        slaveReleasedValue = slaveReleasedValue + clutchBottomDeadzone;
        float gap = slaveFullyPressedValue - slaveReleasedValue;
        float normFactor = 1000 / gap;
        slaveNormalized = normFactor * (slaveValue - slaveReleasedValue);
        if (slaveNormalized < 0)
        {
            slaveNormalized = 0;
        }
    }

    else if (slaveFullyPressedValue < slaveReleasedValue)
    {
        slaveFullyPressedValue = slaveFullyPressedValue + clutchTopDeadzone;
        slaveReleasedValue = slaveReleasedValue - clutchBottomDeadzone;
        float gap = slaveReleasedValue - slaveFullyPressedValue;
        float normFactor = 1000 / gap;
        slaveNormalized = normFactor * (slaveReleasedValue - slaveValue);
        if (slaveNormalized < 0)
        {
            slaveNormalized = 0;
        }
    }


    total[S] = total[S] - readings[S][readIndex[S]];
    readings[S][readIndex[S]] = slaveNormalized;
    total[S] = total[S] + readings[S][readIndex[S]];

    readIndex[S]++;

    if (readIndex[S] > (reads-1))
    {
        readIndex[S] = 0;
    }

    average[S] = total[S] / reads;
    if (average[S] > 1000)
    {
        average[S] = 1000;
    }
    
    //Serial.println(slaveNormalized);  //本句用于调试时输出当前读数

    //------------------------
    //--------模式切换--------
    //------------------------
    int maxValue = max(average[S], average[M]);

    if (maxValue == 0)
    {
        analogLatchLock[M] = true;  //两个离合读数全部为0时不允许切换
    }

    if (maxValue == 1000 && analogLatchLock[M])
    {
        analogLatchLock[M] = false;  //完全按下其中一个离合后允许切换
        if (pushState[modButtonRow - 1][modButtonCol - 1] == 1) //使用模式切换按钮来切换
        {
            if (!analogSwitchMode1[M] && !analogSwitchMode2[M])
            {
                analogSwitchMode1[M] = true;
                Joystick.setXAxis(0);
                Joystick.setThrottle(0);
                Joystick.setBrake(0);
            }
            else if (analogSwitchMode1[M] && !analogSwitchMode2[M])
            {
                analogSwitchMode1[M] = false;
                analogSwitchMode2[M] = true;
                Joystick.setXAxis(0);
                Joystick.setThrottle(0);
                Joystick.setBrake(0);
            }
            else if (!analogSwitchMode1[M] && analogSwitchMode2[M])
            {
                analogSwitchMode1[M] = true;
                Joystick.setXAxis(0);
                Joystick.setThrottle(0);
                Joystick.setBrake(0);
            }
            else if (analogSwitchMode1[M] && analogSwitchMode2[M])
            {
                analogSwitchMode1[M] = false;
                analogSwitchMode2[M] = false;
                Joystick.setXAxis(0);
                Joystick.setThrottle(0);
                Joystick.setBrake(0);
            }
        }
    }

    //------------------------
    //--------离合模式--------
    //------------------------

    //模式1：静态模式，从拨片最大值为咬合点
    if (!analogSwitchMode1[M] && !analogSwitchMode2[M])
    {
        average[S] = average[S] * bitePoint / 1000;
        Joystick.setXAxis(max(average[S], average[M]));
    }
    //模式2：动态低模式，单个拨片只能达到咬合点，一起按下后才能达到100%
    else if (analogSwitchMode1[M] && !analogSwitchMode2[M])
    {

        if ((average[S] == 0 || average[M] == 0 || average[S] > bitePoint && average[M] < bitePoint) || (average[M] > bitePoint && average[S] < bitePoint))  //只按下一个拨片（或一大一小）时，最高只能达到咬合点
        {
            average[S] = average[S] * bitePoint / 1000;
            average[M] = average[M] * bitePoint / 1000;
            Joystick.setXAxis(max(average[S], average[M]));
        }
        else if (average[M] > bitePoint && average[S] > bitePoint)  //同时按下且大于咬合点值时，离合值为两拨片较高值
        {
            Joystick.setXAxis(max(average[S], average[M]));
        }
        else if ((average[S] == 1000 && average[M] < 1000 && average[M] > bitePoint) || (average[M] == 1000 && average[S] < 1000 && average[S] > bitePoint))  //一侧完全按下，释放另一侧，值范围为100&到咬合点
        {
            Joystick.setXAxis(min(average[S], average[M]));
        }
    }
    //模式3：动态高模式，两拨片同时按下后激活，只释放一侧拨片降低到咬合点，然后释放另一个拨片来再次降低离合值；未激活时相当于单拨片离合
    else if (!analogSwitchMode1[M] && analogSwitchMode2[M])
    {
        if (average[S] == 1000 && average[M] == 1000)
        {
            analogLatchLock[S] = true;
        }
        if (average[S] == 0 && average[M] == 0)
        {
            analogLatchLock[S] = false;
        }

        if (analogLatchLock[S])
        {
            if (average[S] == 1000 && average[M] > bitePoint)
            {
                Joystick.setXAxis(average[M]);
            }
            else if (average[M] == 1000 && average[S] > bitePoint)
            {
                Joystick.setXAxis(average[S]);
            }
            else
            {
                Joystick.setXAxis(max(average[S], average[M]) * bitePoint / 1000);
            }
        }
        else
        {
            Joystick.setXAxis(max(average[S], average[M]));
        }
    }
    //模式4：刹车和油门
    else
    {
        Joystick.setXAxis(0);  //取消离合控制
        if (ThrottleMaster)  //参数为1时，主拨片为油门，从拨片为刹车
        {
            Joystick.setThrottle(average[M]);
            Joystick.setBrake(average[S]);
        }
        else  //参数为0时，相反
        {
            Joystick.setThrottle(average[S]);
            Joystick.setBrake(average[M]);
        }
    }

    if (latchState[neutralButtonRow - 1][neutralButtonCol - 1])  //空挡按钮按下时，离合器拉满
    {
        Joystick.setXAxis(1000);
    }

    //传递模式值给编码器字段
    int32_t push = 0;
    push = push | analogSwitchMode1[M];
    push = push | (analogSwitchMode2[M] << 1);
    push = push << (2 * (FieldPlacement - 1));
    rotaryField = rotaryField | push;

}

//带滤波的双离合

void filteredDualClutch(int masterPin, int masterSwitchNumber, int masterReleasedValue, int masterFullyPressedValue, int masterCurvePush, float masterExpFactor, int slavePin, int slaveSwitchNumber, int slaveReleasedValue, int slaveFullyPressedValue, int slaveCurvePush, float slaveExpFactor, bool throttleMaster)
{

    int FieldPlacement = 7;

    //--------------------------------
    //---------主离合滤波计算----------
    //--------------------------------
    int inputPin = masterPin;
    int masterRaw = analogRead(inputPin);
    Serial.println((String)"masterRaw : " + masterRaw); //显示主离合原始值
    Serial.println("");
    int M = masterSwitchNumber - 1;
    float masterNormalized = 0;
    float MasterFullyPressedValue = curveFilter(masterFullyPressedValue, masterReleasedValue, masterFullyPressedValue, masterCurvePush, masterExpFactor);
    float MasterReleasedValue = curveFilter(masterReleasedValue, masterReleasedValue, masterFullyPressedValue, masterCurvePush, masterExpFactor);
    float MasterValue = curveFilter(masterRaw, masterReleasedValue, masterFullyPressedValue, masterCurvePush, masterExpFactor);

    if (MasterFullyPressedValue > MasterReleasedValue)
    {
        float gap = MasterFullyPressedValue - MasterReleasedValue;
        float normFactor = 1000 / gap;
        masterNormalized = normFactor * (MasterValue - MasterReleasedValue);
        if (masterNormalized < 0)
        {
            masterNormalized = 0;
        }
    }
    else if (MasterFullyPressedValue < MasterReleasedValue)
    {
        MasterValue = MasterReleasedValue - MasterValue;

        float gap = MasterReleasedValue - MasterFullyPressedValue;
        float normFactor = 1000 / gap;
        masterNormalized = normFactor * MasterValue;
        if (masterNormalized < 0)
        {
            masterNormalized = 0;
        }
    }

    total[M] = total[M] - readings[M][readIndex[M]];
    readings[M][readIndex[M]] = masterNormalized;
    total[M] = total[M] + readings[M][readIndex[M]];

    readIndex[M]++;

    if (readIndex[M] > (reads - 1))
    {
        readIndex[M] = 0;
    }

    average[M] = total[M] / reads;
    if (average[M] > 1000)
    {
        average[M] = 1000;
    }
    Serial.println((String)"master_average : " + average[M]); //显示主离合滤波值
    Serial.println("");

    //--------------------------------
    //---------从离合滤波计算----------
    //--------------------------------

    int SlavePin = slavePin;
    int slaveRaw = analogRead(SlavePin);
    Serial.println((String)"slaveRaw : " + slaveRaw); //显示主离合原始值
    Serial.println("");
    int S = slaveSwitchNumber - 1;
    float slaveNormalized = 0;

    float SlaveFullyPressedValue = curveFilter(slaveFullyPressedValue, slaveReleasedValue, slaveFullyPressedValue, slaveCurvePush, slaveExpFactor);
    float SlaveReleasedValue = curveFilter(slaveReleasedValue, slaveReleasedValue, slaveFullyPressedValue, slaveCurvePush, slaveExpFactor);
    float SlaveValue = curveFilter(slaveRaw, slaveReleasedValue, slaveFullyPressedValue, slaveCurvePush, slaveExpFactor);

    if (SlaveFullyPressedValue > SlaveReleasedValue)
    {
        float gap = SlaveFullyPressedValue - SlaveReleasedValue;
        float normFactor = 1000 / gap;
        slaveNormalized = normFactor * (SlaveValue - SlaveReleasedValue);
        if (slaveNormalized < 0)
        {
            slaveNormalized = 0;
        }
    }
    else if (SlaveFullyPressedValue < SlaveReleasedValue)
    {
        SlaveValue = SlaveReleasedValue - SlaveValue;

        float gap = SlaveReleasedValue - SlaveFullyPressedValue;
        float normFactor = 1000 / gap;
        slaveNormalized = normFactor * SlaveValue;
        if (slaveNormalized < 0)
        {
            slaveNormalized = 0;
        }
    }

    total[S] = total[S] - readings[S][readIndex[S]];
    readings[S][readIndex[S]] = slaveNormalized;
    total[S] = total[S] + readings[S][readIndex[S]];

    readIndex[S]++;

    if (readIndex[S] > (reads - 1))
    {
        readIndex[S] = 0;
    }

    average[S] = total[S] / reads;
    if (average[S] > 1000)
    {
        average[S] = 1000;
    }
    Serial.println((String)"slave_average : " + average[S]); //显示从离合滤波值
    Serial.println("");

    //------------------------------------------
    //-------通过咬合点按钮设置咬合点离合值--------
    //------------------------------------------

    if (max(average[S], average[M]) > 0 && pushState[biteButtonRow - 1][biteButtonCol - 1] == 1)
    {
        bitePoint = max(average[S], average[M]);
    }


    //------------------------
    //--------切换模式--------
    //------------------------
    int maxValue = max(average[S], average[M]);

    if (maxValue == 0)
    {
        analogLatchLock[M] = true;
    }

    if (maxValue == 1000 && analogLatchLock[M])
    {
        analogLatchLock[M] = false;
        if (pushState[modButtonRow - 1][modButtonCol - 1] == 1) 
        {
            if (!analogSwitchMode1[M] && !analogSwitchMode2[M])
            {
                analogSwitchMode1[M] = true;
                Joystick.setXAxis(0);
                Joystick.setThrottle(0);
                Joystick.setBrake(0);
            }
            else if (analogSwitchMode1[M] && !analogSwitchMode2[M])
            {
                analogSwitchMode1[M] = false;
                analogSwitchMode2[M] = true;
                Joystick.setXAxis(0);
                Joystick.setThrottle(0);
                Joystick.setBrake(0);
            }
            else if (!analogSwitchMode1[M] && analogSwitchMode2[M])
            {
                analogSwitchMode1[M] = true;
                Joystick.setXAxis(0);
                Joystick.setThrottle(0);
                Joystick.setBrake(0);
            }
            else if (analogSwitchMode1[M] && analogSwitchMode2[M])
            {
                analogSwitchMode1[M] = false;
                analogSwitchMode2[M] = false;
                Joystick.setXAxis(0);
                Joystick.setThrottle(0);
                Joystick.setBrake(0);
            }
        }
    }

    //起步按钮逻辑，按下起步按钮，双离合最大值为咬合点值
    if (launchButtonLatch)
    {
        average[M] = average[M] * bitePoint / 1000;
        average[S] = average[S] * bitePoint / 1000;
    }

    //------------------------
    //--------离合模式--------
    //------------------------

    //模式1：静态模式，从拨片最大值为咬合点
    if (!analogSwitchMode1[M] && !analogSwitchMode2[M])
    {
        average[S] = average[S] * bitePoint / 1000;
        Joystick.setXAxis(max(average[S], average[M]));
    }
    //模式2：动态低模式，单个拨片只能达到咬合点，一起按下后才能达到100%
    else if (analogSwitchMode1[M] && !analogSwitchMode2[M])
    {

        if ((average[S] == 0 || average[M] == 0 || average[S] > bitePoint && average[M] < bitePoint) || (average[M] > bitePoint && average[S] < bitePoint))
        {
            average[S] = average[S] * bitePoint / 1000;
            average[M] = average[M] * bitePoint / 1000;
            Joystick.setXAxis(max(average[S], average[M]));
        }
        else if (average[M] > bitePoint && average[S] > bitePoint)
        {
            Joystick.setXAxis(max(average[S], average[M]));
        }
        else if ((average[S] == 1000 && average[M] < 1000 && average[M] > bitePoint) || (average[M] == 1000 && average[S] < 1000 && average[S] > bitePoint))
        {
            Joystick.setXAxis(min(average[S], average[M]));
        }
    }
    //模式3：动态高模式，两拨片同时按下后激活，只释放一侧拨片降低到咬合点，然后释放另一个拨片来再次降低离合值；未激活时相当于单拨片离合
    else if (!analogSwitchMode1[M] && analogSwitchMode2[M])
    {
        if (average[S] == 1000 && average[M] == 1000)
        {
            analogLatchLock[S] = true;
        }
        if (average[S] == 0 && average[M] == 0)
        {
            analogLatchLock[S] = false;
        }

        if (analogLatchLock[S])
        {
            if (average[S] == 1000 && average[M] > bitePoint)
            {
                Joystick.setXAxis(average[M]);
            }
            else if (average[M] == 1000 && average[S] > bitePoint)
            {
                Joystick.setXAxis(average[S]);
            }
            else
            {
                Joystick.setXAxis(max(average[S], average[M]) * bitePoint / 1000);
            }
        }
        else
        {
            Joystick.setXAxis(max(average[S], average[M]));
        }
    }
    //模式4：刹车和油门
    else
    {
        Joystick.setXAxis(0);
        if (throttleMaster)
        {
            Joystick.setThrottle(average[M]);
            Joystick.setBrake(average[S]);
        }
        else
        {
            Joystick.setThrottle(average[S]);
            Joystick.setBrake(average[M]);
        }
    }

    if (latchState[neutralButtonRow - 1][neutralButtonCol - 1])
    {
        Joystick.setXAxis(1000);
    }


    int32_t push = 0;
    push = push | analogSwitchMode1[M];
    push = push | (analogSwitchMode2[M] << 1);
    push = push << (2 * (FieldPlacement - 1));
    rotaryField = rotaryField | push;

}