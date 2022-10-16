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

    pushButton(6, 4);  //B28 设置按钮
    pushButton(3, 1);  //B12 TOW按钮
    pushButton(3, 2);  //B11 退出按钮

    pushButton(1, 5);  //B3 PIT按钮
    pushButton(1, 6);  //B4 雨刷按钮
    pushButton(1, 7);  //B5 右方向按钮
    pushButton(4, 6);  //B19 远光灯按钮
    pushButton(4, 7);  //B20 左方向按钮

    pushButton(6, 7);  //B30 无线电按钮（自锁开关）
    pushButton(7, 3);  //B15 双闪按钮

    pushPull(3, 3, 3, 4, 6, 6, 6, 5, 7);  //B14 B13 B27 B26 换挡拨片和功能拨片

    toggleM(3, 5, 5);  //T1 Launch发动机启动拨档
    toggleM(6, 3, 6);  //T2 Ignite车辆电源拨档
    brakeMagic(7, 2, 4);  //T3 魔法刹车拨档

    //特殊开关

    modButton(7, 1);  //模式调整按钮B29（背后按钮）
    neutralButton(4, 5);  //空挡按钮B18
    biteButton(4, 3);  //咬合点按钮B16
    presetButton(4, 4);  //预设按钮B17
    quickSwitch(1, 4);  //快速切换按钮B2
    throttleHoldM(1, 3, 7, 4, true);  //油门保持功能开关B1 编码器E7（背后旋钮）

    //编码器

    PEC11(1, 1, true);  //E1 右手柄旋钮，向右为增加
    PEC11(4, 1, false);  //E4 右侧旋钮，向上为增加

    PEC11(3, 6, true);  //E2 左手柄旋钮，向右为增加
    PEC11(6, 1, true);  //E5 左侧旋钮，向上为增加


    //摇杆开关

    funkyButtonDDButton(5, 4, 5, 1, 6, 7);  //B24 B方向（上） DD按钮
    funkyButtonHybrid(5, 6, 5, 1, 4, 7);  //B22 D_1方向（下） 混合按钮
    funkyButton(5, 1, 5, 4, 6, 7);  //B21 C方向（左）
    funkyButton(5, 7, 5, 1, 4, 6);  //B23 A方向（右）
    funkyPush(5, 5, 1, 4, 6, 7);  //B25 点按开关

    DDSfunky(5, 2, 3);  //E6 DDS模式旋钮

    funkyButton(2, 1, 5, 2, 4, 7);  //B7 D_1方向（下）
    funkyButton(2, 2, 5, 1, 4, 7);  //B8 C方向（左）
    funkyButton(2, 4, 5, 1, 2, 7);  //B6 A方向（右）
    funkyButton(2, 7, 5, 1, 2, 4);  //B9 B方向（上）
    funkyPush(2, 5, 1, 2, 4, 7);  //B10 点按开关
    funkyRotary(2, 3, 6);  //E3 编码器旋钮

    //模拟输入

    rotaryLeft(  //左旋转开关，咬合点调整和绑定一个参数进行调整
        A3,                                                           //模拟输入针脚
        1,                                                            //模拟输入编号1
        3,                                                            //编码器位字段
        16, 107, 200, 291, 383, 474, 566, 657, 749, 841, 932, 1023,   //12档读数值
        false);                                                        //旋转方向，false为顺时针

    rotaryRight(  //右旋转开关，预设调整和绑定一个参数进行调整
        A1,                                                           //模拟输入针脚
        2,                                                            //模拟输入编号2
        2,                                                            //编码器位字段
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
