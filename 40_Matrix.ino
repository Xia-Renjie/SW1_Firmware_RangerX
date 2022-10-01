void matrix()
{
    //开始矩阵逻辑，检查每个针脚的状态

    //因为要监测传递到行针脚的电流，所以将行针脚接地
    for (uint8_t i = 0; i < rowCount; i++)
    { //依次设定每一个行针脚

        pinMode(row[i], OUTPUT);  //将行针脚定义为输出针脚
        digitalWrite(row[i], LOW);  //低电位即视为接地

        for (uint8_t u = 0; u < colCount; u++) { //检查该行上的每一个列针脚
            pinMode(col[u], INPUT_PULLUP);  //将上拉电阻接入列针脚
            if (digitalRead(col[u]) == 0) {
                rawState[i][u] = 1;  //接入上拉电阻后，低电平0时表示开关接通
            }
            else {
                rawState[i][u] = 0;
            }
            pinMode(col[u], INPUT);  //将列针脚重新定义为输入针脚
        }

        pinMode(row[i], INPUT_PULLUP); //断开这一个行针脚
    }
}
