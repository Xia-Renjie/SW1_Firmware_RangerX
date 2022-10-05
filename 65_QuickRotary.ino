//----------------------------------
//使用电阻式旋转开关实现4种快速切换功能
//----------------------------------

//快速切换功能1：带咬合点值切换功能
void quickRotary1bite(int analogPin, int switchNumber, int fieldPlacement, int pos1, int pos2, int pos3, int pos4, int pos5, int pos6, int pos7, int pos8, int pos9, int pos10, int pos11, int pos12, bool reverse)
//参数为模拟输入针脚，按钮编号，编码器位字段，1-12档位读数，是否反向旋转
//功能实现与rotaryAnalog2Mode()相同
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

    int maxPos = 12;


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
                analogTimer2[N] = globalClock;

                analogTempState[N] = result - analogLastCounter[N];

                analogLastCounter[N] = result;
            }

            else //启用咬合点设定
            {
                analogTimer2[N] = globalClock;

                analogTempState[N] = result - analogLastCounter[N];

                analogLastCounter[N] = result;

                //增加或减小咬合点值
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

    //开关模式1：12档开关

    if (!analogSwitchMode1[N] && !biteButtonBit1 && !biteButtonBit2)
    {
        analogTempState[N] = 0;

        uint8_t value = analogLastCounter[N];

        if (quickSwitchState || quickSwitch12State)
        {
            value = quickValue1;
        }

        for (int i = 0; i < 12; i++)
        {
            if (i == value)
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

    //传递模式值给编码器位字段
    long push = 0;
    push = push | analogSwitchMode1[N];
    push = push << (FieldPlacement - 1);
    rotaryField = rotaryField | push;
}

//快速切换功能2：不带咬合点值切换功能
void quickRotary2(int analogPin, int switchNumber, int fieldPlacement, int pos1, int pos2, int pos3, int pos4, int pos5, int pos6, int pos7, int pos8, int pos9, int pos10, int pos11, int pos12, bool reverse)
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

    int maxPos = 12;


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
            //-------------------模式切换-------------------
            //----------------------------------------------
            //因为占位符的存在，模式切换仅在编码器旋转时才会生效
            //如果你不想推送模式值给位字段，就把fieldPlacement设定为0

            if (pushState[modButtonRow - 1][modButtonCol - 1] == 1 && FieldPlacement != 0)
            {
                for (int i = 0; i < maxPos + 1; i++)
                {
                    Joystick.releaseButton(i - 1 + Number);
                }

                analogSwitchMode1[N] = !analogSwitchMode1[N];
            }

 
            analogTimer2[N] = globalClock;

            analogTempState[N] = result - analogLastCounter[N];

            analogLastCounter[N] = result;
        }
    }

    //开关模式1：12档开关

    if (!analogSwitchMode1[N])
    {
        analogTempState[N] = 0;

        uint8_t value = analogLastCounter[N];

        if (quickSwitchState || quickSwitch12State)
        {
            value = quickValue2;
        }

        for (int i = 0; i < 12; i++)
        {
            if (i == value)
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

    //传递模式值给编码器位字段
    long push = 0;
    push = push | analogSwitchMode1[N];
    push = push << (FieldPlacement - 1);
    rotaryField = rotaryField | push;
}

//快速切换功能3：与2相同
void quickRotary3(int analogPin, int switchNumber, int fieldPlacement, int pos1, int pos2, int pos3, int pos4, int pos5, int pos6, int pos7, int pos8, int pos9, int pos10, int pos11, int pos12, bool reverse)
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

    int maxPos = 12;


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



            if (pushState[modButtonRow - 1][modButtonCol - 1] == 1 && FieldPlacement != 0)
            {
                for (int i = 0; i < maxPos + 1; i++) 
                {
                    Joystick.releaseButton(i - 1 + Number);
                }

                analogSwitchMode1[N] = !analogSwitchMode1[N]; 
            }


            analogTimer2[N] = globalClock;

            analogTempState[N] = result - analogLastCounter[N];

            analogLastCounter[N] = result;
        }
    }

    //开关模式1：12档开关

    if (!analogSwitchMode1[N])
    {
        analogTempState[N] = 0;

        uint8_t value = analogLastCounter[N];

        if (quickSwitchState || quickSwitch34State)
        {
            value = quickValue3;
        }

        for (int i = 0; i < 12; i++)
        {
            if (i == value)
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

    //传递模式值给编码器位字段
    long push = 0;
    push = push | analogSwitchMode1[N];
    push = push << (FieldPlacement - 1);
    rotaryField = rotaryField | push;
}

//快速切换功能4：与2相同
void quickRotary4(int analogPin, int switchNumber, int fieldPlacement, int pos1, int pos2, int pos3, int pos4, int pos5, int pos6, int pos7, int pos8, int pos9, int pos10, int pos11, int pos12, bool reverse)
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

    int maxPos = 12;


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



            if (pushState[modButtonRow - 1][modButtonCol - 1] == 1 && FieldPlacement != 0)
            {
                for (int i = 0; i < maxPos + 1; i++) 
                {
                    Joystick.releaseButton(i - 1 + Number);
                }

                analogSwitchMode1[N] = !analogSwitchMode1[N];
            }


            analogTimer2[N] = globalClock;

            analogTempState[N] = result - analogLastCounter[N];

            analogLastCounter[N] = result;
        }
    }

    //开关模式1：12档开关

    if (!analogSwitchMode1[N])
    {
        analogTempState[N] = 0;

        uint8_t value = analogLastCounter[N];

        if (quickSwitchState || quickSwitch34State)
        {
            value = quickValue4;
        }

        for (int i = 0; i < 12; i++)
        {
            if (i == value)
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

    //传递模式值给编码器位字段
    long push = 0;
    push = push | analogSwitchMode1[N];
    push = push << (FieldPlacement - 1);
    rotaryField = rotaryField | push;
}