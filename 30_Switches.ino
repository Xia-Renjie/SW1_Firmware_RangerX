//------------------------------
//----------运行函数------------
//------------------------------

void loop()
{

    //-------------------------
    //-------必要定义----------
    //-------------------------

    globalClock = millis();  //全局时钟为系统运行时间，单位毫秒

    rotaryField = 0;
    buttonField = 0;
    matrix();
    //runningPresets();  //使用31的预设更新方式

    //-------------------------
    //-------开关函数定义-------
    //-------------------------

    //方向盘按钮

    pushButton(1, 3);  //B1

    pushButton(3, 1);  //B12
    pushButton(3, 2);  //B11

    pushButton(4, 3);  //B16
    pushButton(4, 4);  //B17
    pushButton(4, 6);  //B19
    pushButton(4, 7);  //B20

    pushButton(6, 4);  //B28
    pushButton(6, 7);  //B30

    pushButton(7, 1);  //B15
    pushButton(7, 3);  //B29

    pushPull(3, 3, 3, 4, 6, 6, 6, 5, 7);  //B14 B13 B27 B26 换挡拨片和功能拨片

    toggle(3,5);  //T1
    toggleM(6, 3, 6);  //T2
    brakeMagic(7, 2, 4);  //T3

    //特殊开关

    modButton(1, 7);  //模式调整按钮B5
    neutralButton(4, 5);  //空挡按钮B18
    biteButton(1, 5);  //咬合点按钮B3
    presetButton(1, 6);  //预设按钮B4
    quickSwitch(1, 4);  //快速切换按钮B2
    throttleHoldM(1, 3, 7, 4, true);  //油门保持功能开关B1 编码器E7

    //编码器

    rotary2Inc(1, 1, false);  //E1
    rotary2Inc(4, 1, false);  //E4

    rotary2Inc(3, 6, true);  //E2
    rotary2Inc(6, 1, false);  //E5


    //摇杆开关

    funkyButtonDDButton(5, 4, 5, 1, 6, 7);  //B24 B方向 DD按钮模式
    funkyButtonHybrid(5, 6, 5, 1, 4, 7);  //B22 D_1方向 混合按钮模式
    funkyButton(5, 1, 5, 4, 6, 7);  //B21 C方向
    funkyButton(5, 7, 5, 1, 4, 6);  //B23 A方向
    funkyPush(5, 5, 1, 4, 6, 7);  //B25 点按开关

    DDSfunky(5, 2, 3);  //E6 DDS模式

    funkyButton(2, 1, 5, 2, 4, 7);  //B7 D_1方向
    funkyButton(2, 2, 5, 1, 4, 7);  //B8 C方向
    funkyButton(2, 4, 5, 1, 2, 7);  //B6 A方向
    funkyButton(2, 7, 5, 1, 2, 4);  //B9 B方向
    funkyPush(2, 5, 1, 2, 4, 7);  //B10 点按开关
    funkyRotary(2, 3, 6);  //E3

    //模拟输入

    rotaryLeft(  //左旋转开关
        A3,                                                           //模拟输入针脚
        1,                                                            //模拟输入编号1
        3,                                                            //Field placement
        16, 107, 200, 291, 383, 474, 566, 657, 749, 841, 932, 1023,   //12档读数值
        false);                                                        //旋转方向，false为顺时针

    rotaryRight(  //右旋转开关
        A1,                                                           //模拟输入针脚
        2,                                                            //模拟输入编号2
        2,                                                            //Field placement
        16, 107, 200, 291, 383, 474, 566, 657, 749, 841, 932, 1023,   //12档读数值
        false);                                                        //旋转方向，false为顺时针

    dualClutch(
        A2,                                                           //模拟输入针脚（主离合）
        3,                                                            //模拟输入编号3（主离合）
        571,                                                          //释放数值（主离合）
        169,                                                          //完全按下数值（主离合）
        A5,                                                           //模拟输入针脚（从离合）
        4,                                                            //模拟输入编号4（从离合）
        527,                                                          //释放数值（从离合）
        882,                                                          //完全按下数值（从离合）
        true);                                                        //True = 模式4时主离合为油门，从离合为刹车，false则反之

   

    Joystick.setZAxis(rotaryField - 32767);  //设置轴初始值
    Joystick.setYAxis(buttonField - 32767);

    Joystick.sendState();  //发送按钮状态
}
