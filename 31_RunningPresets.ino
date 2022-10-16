void runningPresets()
{
    /*
    这个函数将在主loop中循环运行，也就是说下面设定的预设将会一直被调用。presets()函数下的12个预设只有切换预设时才会被调用一次。
    这非常适合切换那些静态值，例如开关模式、咬合点值等。但是，如果你在预设中设定了一个咬合点值，那你就无法用旋钮来改变它的值了，
    因为这个函数会一直刷新你设定的咬合点值。（不想这样的话，建议使用67_PresetRotary.ino定义一个12档旋钮用于切换预设，或者
    99_PresetSwitch.ino中的preset2Bit()函数定义一个编码器来实现调整预设旋钮时才会加载预设里的设定）。

    这块区域是为了改变按钮编号、开关的函数定义等等。如果你想12档旋转开关在ACC中作为常规12档模式，而在另一个游戏预设中使用
    multiComplex24()函数（它可以实现更多的按钮数），你就可以在这里设置。你可能在一个预设中定义了混合按钮，这个按钮并不会产生按
    钮编号，那到了其它不支持这种按钮的游戏中它就无法使用了。你可以在一些预设中设定它为hybridButton(1,2)，而在F1 2022等游戏预设
    中设定为pushButton(1,2)。

    以上只是举例，你可以发挥想象力编写自己的预设。多种预设能让你的控制器变得更加灵活多变。   
    */

    switch (switchPreset)
    {
    case 0: //PRESET 1 

        break;
    case 1: //PRESET 2 

        break;
    case 2: //PRESET 3 

        break;
    case 3: //PRESET 4 

        break;
    case 4: //PRESET 5 

        break;
    case 5: //PRESET 6 

        break;
    case 6: //PRESET 7 

        break;
    case 7: //PRESET 8 

        break;
    case 8: //PRESET 9 

        break;
    case 9: //PRESET 10

        break;
    case 10: //PRESET 11 

        break;
    case 11: //PRESET 12 

        break;
    }
}

