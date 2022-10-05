//将左侧12档旋转开关设定为咬合点设定旋钮，函数同rotaryAnalog2Mode()
void rotaryLeft(int analogPin, int switchNumber, int fieldPlacement, int pos1, int pos2, int pos3, int pos4, int pos5, int pos6, int pos7, int pos8, int pos9, int pos10, int pos11, int pos12, bool reverse)
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
                for (int i = 0; i < maxPos + 1; i++)
                {
                    Joystick.releaseButton(i - 1 + Number);
                }

                analogSwitchMode1[N] = !analogSwitchMode1[N];
            }

            analogTimer2[N] = globalClock;

            analogTempState[N] = result - analogLastCounter[N];

            analogLastCounter[N] = result;

            if (biteButtonBit1 || biteButtonBit2)
            {

                uint8_t biteRes = result + 1;

                if (biteRes > 10)
                {
                    biteRes = 0;
                }

                if (biteButtonBit1 && !biteButtonBit2)
                {
                    bitePoint = biteRes * 100;
                    first = biteRes;
                    if (bitePoint > 1000)
                    {
                        bitePoint = 1000;
                    }
                }
                else if (!biteButtonBit1 && biteButtonBit2)
                {
                    bitePoint = first * 100 + biteRes * 10;
                    second = biteRes;
                    if (bitePoint > 1000)
                    {
                        bitePoint = 1000;
                    }
                }
                else if (biteButtonBit1 && biteButtonBit2)
                {
                    bitePoint = first * 100 + second * 10 + biteRes;
                    if (bitePoint > 1000)
                    {
                        bitePoint = 1000;
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
        if (quickSwitchState)
        {
            value = leftQuickValue;
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

    //传递模式值给按钮位字段
    long push = 0;
    push = push | analogSwitchMode1[N];
    push = push << (FieldPlacement - 1);
    buttonField = buttonField | push;
}

