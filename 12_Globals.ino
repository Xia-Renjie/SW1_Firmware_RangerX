//------------------------------
//--------OTHER VARIABLES-------
//------------------------------

//Handbrake
bool handbrake = false;
bool handbrakeLock = false;
bool handbrakeRelease = false;
long handbrakeTimer = 0;

bool brakeMagicOn;
float brakeMagicValue; 
float throttleHoldValue; 
float bitePoint;
uint8_t leftQuickValue;
uint8_t rightQuickValue;
bool quickSwitchState;

//Function buttons
int8_t hybridButtonRow;
int8_t hybridButtonCol;
int8_t modButtonRow;
int8_t modButtonCol;
int8_t ddButtonRow;
int8_t ddButtonCol;
int8_t neutralButtonRow;
int8_t neutralButtonCol;
int8_t biteButtonRow;
int8_t biteButtonCol;
int8_t presetButtonRow;
int8_t presetButtonCol;

bool biteButtonBit1 = false;
bool biteButtonBit2 = false;
uint8_t first = 0;
uint8_t second = 0;

int8_t switchPreset = 0;
bool bootPreset = true;

//Clock and counter
unsigned long globalClock;

//Debouncing
#define buttonCooldown 25
#define analogWait 50
#define analogPulse 100
#define encoderWait 5
#define encoderPulse 50
#define encoder2Wait 2
#define encoder2Pulse 50
#define encoderCooldown 0
#define togglePulse 50
#define funkyPulse 60
#define funkyCooldown 70



