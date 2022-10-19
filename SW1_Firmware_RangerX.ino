#include <Joystick.h>

#define DEVICE_NAME "SW1 RangerX"  //自定义设备名称
#define BUTTONCOUNT 97  //定义按钮编号总数97

//定义Joystick函数参数
Joystick_ Joystick(JOYSTICK_DEFAULT_REPORT_ID, JOYSTICK_TYPE_JOYSTICK,
    BUTTONCOUNT,    //按钮总数
    0,              //苦力帽数量
    true,          //X轴，用于离合
    true,          //Y轴，用于按钮模拟
    true,          //Z轴，用于编码器模拟
    false,         //Rx轴，未启用 
    true,          //Ry轴，用于设定咬合点
    false,         //Rz轴，未启用 
    false,         //脚舵，未启用
    true,          //油门轴，用于双离合模式4
    false,         //加速器轴，未启用
    true,          //刹车轴，用于双离合模式
    false);        //转向轴，未启用
