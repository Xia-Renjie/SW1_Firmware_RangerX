//Matrix variables

int8_t rawState[rowCount][colCount];
int8_t pushState[rowCount][colCount];
bool latchLock[rowCount][colCount];
bool latchState[rowCount][colCount];
bool switchModeLock[rowCount][colCount];
long switchTimer[rowCount][colCount];
long toggleTimer[rowCount][colCount];
bool switchMode[rowCount][colCount];
bool preset1[rowCount][colCount];
bool preset2[rowCount][colCount];
bool preset3[rowCount][colCount];
bool preset4[rowCount][colCount];
bool preset5[rowCount][colCount];
bool preset6[rowCount][colCount];
bool preset7[rowCount][colCount];
bool preset8[rowCount][colCount];

//Analog variables

int8_t analogLastCounter[4];
int8_t analogTempState[4];
bool analogLatchLock[4];
long analogTimer1[4];
long analogTimer2[4];
int8_t analogRotaryCount[4];
int8_t analogSwitchMode1[4];
int8_t analogSwitchMode2[4];
int8_t analogMode1Preset1[4];
int8_t analogMode1Preset2[4];
int8_t analogMode1Preset3[4];
int8_t analogMode1Preset4[4];
int8_t analogMode1Preset5[4];
int8_t analogMode1Preset6[4];
int8_t analogMode1Preset7[4];
int8_t analogMode1Preset8[4];
int8_t analogMode2Preset1[4];
int8_t analogMode2Preset2[4];
int8_t analogMode2Preset3[4];
int8_t analogMode2Preset4[4];
int8_t analogMode2Preset5[4];
int8_t analogMode2Preset6[4];
int8_t analogMode2Preset7[4];
int8_t analogMode2Preset8[4];

#define clutchTopDeadzone 10
#define clutchBottomDeadzone 10

//Smoothing of analog signal

#define reads 4
int readings[4][reads];         // the readings from the analog input
int8_t readIndex[4];              // the index of the current reading
int total[4];                  // the running total
int average[4];