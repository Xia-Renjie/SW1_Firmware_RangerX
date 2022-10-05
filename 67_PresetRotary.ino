//将左侧12档旋转开关设定为预设切换旋钮，函数同rotaryAnalog2Mode()
void rotaryRight(int analogPin, int switchNumber, int fieldPlacement, int pos1, int pos2, int pos3, int pos4, int pos5, int pos6, int pos7, int pos8, int pos9, int pos10, int pos11, int pos12, bool reverse)
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

    if (bootPreset) //开机后加载启动预设
    {
        switchPreset = result;
        presets(switchPreset);
        bootPreset = false;
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

            if (pushState[modButtonRow - 1][modButtonCol - 1] == 1)
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

            if (pushState[presetButtonRow - 1][presetButtonCol - 1] == 1) //标准模式
            {
                //确定选择的预设
                switchPreset = result;

                //启用新预设
                presets(switchPreset);
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
            value = rightQuickValue;
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

    //开关模式2：增量编码器

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

    //传递预设编号给按钮位字段
    long pesh = 0;
    pesh = pesh | (switchPreset << 10);
    buttonField = buttonField | pesh;


}
