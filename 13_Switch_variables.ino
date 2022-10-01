//矩阵变量

int8_t rawState[rowCount][colCount];  //按钮原始状态
int8_t pushState[rowCount][colCount];  //按钮按下状态
bool latchLock[rowCount][colCount];  //按钮锁定状态
bool latchState[rowCount][colCount];  //自锁按钮状态
bool switchModeLock[rowCount][colCount];  //开关模式锁定状态
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

int8_t analogLastCounter[4];  //模拟输入上次计数
int8_t analogTempState[4];  //模拟输入临时状态
bool analogLatchLock[4];  //模拟输入锁定状态
long analogTimer1[4];  //模拟输入计时器1
long analogTimer2[4];  //模拟输入计时器2
int8_t analogRotaryCount[4];  //模拟编码器计数
int8_t analogSwitchMode1[4];  //模拟开关模式1
int8_t analogSwitchMode2[4];  //模拟开关模式2
int8_t analogMode1Preset1[4];  //模拟模式1预设1
int8_t analogMode1Preset2[4];  //模拟模式1预设2
int8_t analogMode1Preset3[4];  //模拟模式1预设3
int8_t analogMode1Preset4[4];  //模拟模式1预设4
int8_t analogMode1Preset5[4];  //模拟模式1预设5
int8_t analogMode1Preset6[4];  //模拟模式1预设6
int8_t analogMode1Preset7[4];  //模拟模式1预设7
int8_t analogMode1Preset8[4];  //模拟模式1预设8
int8_t analogMode2Preset1[4];  //模拟模式2预设1
int8_t analogMode2Preset2[4];  //模拟模式2预设2
int8_t analogMode2Preset3[4];  //模拟模式2预设3
int8_t analogMode2Preset4[4];  //模拟模式2预设4
int8_t analogMode2Preset5[4];  //模拟模式2预设5
int8_t analogMode2Preset6[4];  //模拟模式2预设6
int8_t analogMode2Preset7[4];  //模拟模式2预设7
int8_t analogMode2Preset8[4];  //模拟模式2预设8

#define clutchTopDeadzone 10  //离合顶端死区10
#define clutchBottomDeadzone 10  //离合底部死区10

//模拟信号平滑处理

#define reads 4
int readings[4][reads];         //模拟输入读数（4行矩阵，读取4次）
int8_t readIndex[4];            //当前读数的索引号（行号）
int total[4];                  //读数总和
int average[4];                //度数平均值