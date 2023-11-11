//---------------------------
//---------其它变量----------
//---------------------------

//手刹
bool handbrake = false;  //手刹状态，默认未启用
bool handbrakeLock = false;  //手刹锁，默认未启用
bool handbrakeRelease = false;  //手刹释放锁，默认未启用
long handbrakeTimer = 0;  //手刹计时器

bool brakeMagicOn;  //魔法刹车状态
float brakeMagicValue;  //魔法刹车值
float throttleHoldValue;  //油门保持值
float bitePoint;  //咬合点值
uint8_t leftQuickValue;  //左快速切换预设值
uint8_t rightQuickValue;  //右快速切换预设值
bool quickSwitchState;  //快速切换状态
uint8_t quickValue1;  //QuickRotary中使用的变量
uint8_t quickValue2;
uint8_t quickValue3;
uint8_t quickValue4;
bool quickSwitch12State;
bool quickSwitch34State;

//功能按钮
int8_t hybridButtonRow;  //混合按钮行编号
int8_t hybridButtonCol;  //混合按钮列编号
int8_t modButtonRow;  //模式调整按钮行编号
int8_t modButtonCol;  //模式调整按钮列编号
int8_t ddButtonRow;  //DD按钮行编号
int8_t ddButtonCol;  //DD按钮列编号
int8_t neutralButtonRow;  //空挡按钮行编号
int8_t neutralButtonCol;  //空挡按钮列编号
int8_t biteButtonRow;  //咬合点按钮行编号
int8_t biteButtonCol;  //咬合点按钮列编号
int8_t presetButtonRow;  //预设按钮行编号
int8_t presetButtonCol;  //预设按钮列编号

bool biteButtonBit1 = false;  //咬合点字段1
bool biteButtonBit2 = false;  //咬合点字段2
bool launchButtonLatch = false;  //起步按钮字段
uint8_t first = 0;
uint8_t second = 0;

int8_t switchPreset = 0;  //开关预设，默认为0
bool bootPreset = false;  //启动预设，不启用

//时钟与计数器
unsigned long globalClock;  //全局时钟

//去抖动
#define buttonCooldown 25  //按钮冷却时间25ms
#define analogWait 50  //模拟量等待间隔50ms
#define analogPulse 100  //模拟脉冲时长100ms
#define encoderWait 5  //编码器等待间隔5ms
#define encoderPulse 50  //编码器脉冲时长50ms
#define encoder2Wait 2  //编码器2等待间隔2ms
#define encoder2Pulse 50  //编码器2脉冲时长50ms
#define encoderCooldown 0  //编码器冷却时间0ms
#define togglePulse 50  //拨档开关脉冲时长50ms
#define funkyPulse 60  //摇杆开关脉冲时长60ms
#define funkyCooldown 70  //摇杆开关冷却时间70ms

#define PEC11Pulse 50  //PEC11编码器脉冲时长50ms
#define PEC11Cooldown 70  //PEC11编码器冷却时间70ms
#define pulseEncCD 20
#define pulseEncDuration 50

#define clutchTopDeadzone 10  //离合顶端死区10
#define clutchBottomDeadzone 10  //离合底部死区10
