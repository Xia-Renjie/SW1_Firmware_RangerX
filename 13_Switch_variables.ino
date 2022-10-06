//矩阵变量

int8_t rawState[rowCount][colCount];  //按钮原始状态
int8_t pushState[rowCount][colCount];  //按钮按下状态
bool latchLock[rowCount][colCount];  //自锁按钮锁
bool latchState[rowCount][colCount];  //自锁按钮状态
bool switchModeLock[rowCount][colCount];  //开关模式锁
long switchTimer[rowCount][colCount];  //开关计时器
long toggleTimer[rowCount][colCount];  //拨档开关计时器
bool switchMode[rowCount][colCount];  //开关模式
bool preset1[rowCount][colCount];  //预设1
bool preset2[rowCount][colCount];  //预设2
bool preset3[rowCount][colCount];  //预设3
bool preset4[rowCount][colCount];  //预设4
bool preset5[rowCount][colCount];  //预设5
bool preset6[rowCount][colCount];  //预设6
bool preset7[rowCount][colCount];  //预设7
bool preset8[rowCount][colCount];  //预设8

//模拟输入变量

int8_t analogLastCounter[10];  //模拟输入上次计数
int8_t analogTempState[10];  //模拟输入临时状态
bool analogLatchLock[10];  //模拟输入锁定状态
long analogTimer1[10];  //模拟输入计时器1
long analogTimer2[10];  //模拟输入计时器2
int8_t analogRotaryCount[10];  //模拟编码器计数
int8_t analogSwitchMode1[10];  //模拟开关模式1
int8_t analogSwitchMode2[10];  //模拟开关模式2
int8_t analogMode1Preset1[10];  //模拟模式1预设1
int8_t analogMode1Preset2[10];  //模拟模式1预设2
int8_t analogMode1Preset3[10];  //模拟模式1预设3
int8_t analogMode1Preset4[10];  //模拟模式1预设4
int8_t analogMode1Preset5[10];  //模拟模式1预设5
int8_t analogMode1Preset6[10];  //模拟模式1预设6
int8_t analogMode1Preset7[10];  //模拟模式1预设7
int8_t analogMode1Preset8[10];  //模拟模式1预设8
int8_t analogMode2Preset1[10];  //模拟模式2预设1
int8_t analogMode2Preset2[10];  //模拟模式2预设2
int8_t analogMode2Preset3[10];  //模拟模式2预设3
int8_t analogMode2Preset4[10];  //模拟模式2预设4
int8_t analogMode2Preset5[10];  //模拟模式2预设5
int8_t analogMode2Preset6[10];  //模拟模式2预设6
int8_t analogMode2Preset7[10];  //模拟模式2预设7
int8_t analogMode2Preset8[10];  //模拟模式2预设8


//模拟信号平滑处理

#define reads 10
int readings[10][reads];         //模拟输入读数（10行矩阵，读取10次）
int8_t readIndex[10];            //当前读数的索引号（行号）
int total[10];                  //读数总和
int average[10];                //度数平均值