//------------------------------
//---------初始化设置------------
//------------------------------

void setup()
{
    //设置控制器轴
    Joystick.setXAxisRange(0, 1000);
    Joystick.setThrottleRange(0, 1000);
    Joystick.setBrakeRange(0, 1000);


    //填充矩阵与数组
    for (int i = 0; i < rowCount; i++)
    {
        for (int a = 0; a < colCount; a++)
        {
            rawState[i][a] = 0;
            pushState[i][a] = 0;
            latchLock[i][a] = 0;
            latchState[i][a] = 0;
            switchModeLock[i][a] = 0;
            switchTimer[i][a] = 0;
            toggleTimer[i][a] = 0;
        }
    }

    for (int u = 0; u < 4; u++)
    {
        analogLastCounter[u] = 0;
        analogTempState[u] = 0;
        analogLatchLock[u] = 0;
        analogTimer1[u] = 0;
        analogTimer2[u] = 0;
        analogRotaryCount[u] = 0;
        readIndex[u] = 0;
        total[u] = 0;
        average[u] = 0;

        for (int i = 0; i < reads; i++)
        {
            readings[u][i] = 0;
        }
    }

    //初始化按钮矩阵

    for (int i = 0; i < colCount; i++)
    { //所有针脚默认为断开状态，除非另行说明
        pinMode(col[i], INPUT_PULLUP);
    }

    for (int i = 0; i < rowCount; i++)
    { //所有针脚默认为断开状态，除非另行说明
        pinMode(row[i], INPUT_PULLUP);
    }

    Joystick.begin(0); //启用Joystick库

    Joystick.setZAxisRange(-32768, 32767);  //设置轴为16位精度
    Joystick.setYAxisRange(-32768, 32767);

}
