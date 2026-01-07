// PB0: rings (Left + Right in parallel, mirrored, 45 LEDs total)
// PB4: numberplate (10 LEDs)
// Wiring Colour Codes
// Idle:  White (PWR)
// Stop:  Red
// Left:  Blue
// Right: Yellow
// GND:   Black (GND)

#include <Adafruit_NeoPixel.h>

// ---------------- CONFIG ----------------
#define RING_LEDS               45
#define PLATE_LEDS              10

#define RINGS_PIN               PB0
#define PLATE_PIN               PB4

#define PIN_STOP                PB1
#define PIN_LEFT                PB2
#define PIN_RIGHT               PB3

#define IDLE_MIN_BRIGHTNESS     5
#define IDLE_MAX_BRIGHTNESS     64
#define IDLE_SPAN               (IDLE_MAX_BRIGHTNESS - IDLE_MIN_BRIGHTNESS)
#define IDLE_PERIOD             2000  // breathing animation
#define IDLE_PERIOD_TICKS       (IDLE_PERIOD / 10)

#define STOP_BRIGHTNESS         200   // for R

#define PLATE_BRIGHTNESS        64    // for R, G, B
#define PLATE_TRAIL_LEN         2
//#define PLATE_STOP_TRAIL_LEN  2

#define BLINKER_TIMEOUT         2000
#define BLINKER_TIMEOUT_TICKS   (BLINKER_TIMEOUT / 10)
#define BLINKER_DEBOUNCE        20
#define BLINKER_DEBOUNCE_TICKS  ((BLINKER_DEBOUNCE + 49) / 50)  // 1

#define EMERG_LATCH_TICKS       50    // ~500 ms (confirmation)
#define EMERG_RELEASE_TICKS     150   // ~1.5 s (release)

#define LOOP_DELAY              10    // ms
#define RINGS_EMERG_DELAY       50    // ms
#define LOOP_DIVIDER            10
#define RINGS_EMERG_DIV         10
#define PLATE_STOP_DIV          10

#define BLINK_STEP_TICKS        2     // ticks per eyelid step
#define BLINK_INTERVAL_TICKS    500   // interval between blinks

// ---------------- MODE ----------------
enum Mode {
  MODE_IDLE,
  MODE_STOP,
  MODE_LEFT,
  MODE_RIGHT,
  MODE_EMERGENCY
};
static Mode currentMode = MODE_IDLE;

// -------------- EYEBLINK --------------
enum EyeBlinkState {
  BLINK_IDLE,
  BLINK_ACTIVE
};
static EyeBlinkState blinkState = BLINK_IDLE;

// blink sequence
static const int8_t blinkOrder[] = { +3, +2, +1, 0, -1, -2, -3 };
static const uint8_t BLINK_STEPS = sizeof(blinkOrder) / sizeof(blinkOrder[0]);

static bool blinkOpening = false;   // false = closing, true = opening
static int8_t blinkIndex = 0;
static uint8_t blinkTick = 0;

static const int8_t ledY[RING_LEDS] = {
  // ---- center ----
  /*  0 */  0,

  // ---- ring 1 (6) ----
  /*  1 */ +1,  // N
  /*  2 */ +1,
  /*  3 */ -1,
  /*  4 */ -1,  // S
  /*  5 */ -1,
  /*  6 */ +1,

  // ---- ring 2 (12) ----
  /*  7 */ +2,  /*  8 */ +2,      // N arc
  /*  9 */ +2,  /* 10 */  0,
  /* 11 */ -2,  /* 12 */ -2,
  /* 13 */ -2,  /* 14 */ -2,
  /* 15 */ -2,  /* 16 */  0,      // S arc
  /* 17 */ +2,  /* 18 */ +2,

  // ---- ring 3 (16) ----
  /* 19 */ +3,  /* 20 */ +3,  /* 21 */ +3,  // N arc
  /* 22 */ +2,  /* 23 */  0,
  /* 24 */ -2,  /* 25 */ -3,
  /* 26 */ -3,  /* 27 */ -3,
  /* 28 */ -3,  /* 29 */ -3,  /* 30 */ -2,  // S arc
  /* 31 */  0,  /* 32 */ +2,
  /* 33 */ +3,  /* 34 */ +3,

  // ---- left parenthesis (35–39) ----
  /* 35 */ +33,   // near 21
  /* 36 */ +33,
  /* 37 */  30,
  /* 38 */ -33,
  /* 39 */ -33,   // near 25

  // ---- right parenthesis (40–44) ----
  /* 40 */ -33,   // near 29
  /* 41 */ -33,
  /* 42 */  30,
  /* 43 */ +33,
  /* 44 */ +33    // near 33
};

// -------------- EMERGENCY RING BLINK --------------
enum RingEmergState {
  RING_EMERG_IDLE,
  RING_EMERG_ACTIVE
};
static RingEmergState ringEmergState = RING_EMERG_IDLE;

static const int8_t ringOrder[] = { 0, 1, 2, 3, 4 };
static const uint8_t RING_STEPS = sizeof(ringOrder) / sizeof(ringOrder[0]);

static int8_t ringEmergIndex = 0;
static bool ringOpening = false;
static uint8_t ringTick = 0;

static bool     emergencyLatched = false;
static uint16_t emergencyLatchTick = 0;
static uint16_t emergencyReleaseTick = 0;

static const int8_t ledRing[RING_LEDS] = {
  /*  0 */ 0,

  // ring 1
  /*  1 */ 1, /*  2 */ 1, /*  3 */ 1, /*  4 */ 1, /*  5 */ 1, /*  6 */ 1,

  // ring 2
  /*  7 */ 2, /*  8 */ 2, /*  9 */ 2, /* 10 */ 2,
  /* 11 */ 2, /* 12 */ 2, /* 13 */ 2, /* 14 */ 2,
  /* 15 */ 2, /* 16 */ 2,
  /* 17 */ 2, /* 18 */ 2,

  // ring 3
  /* 19 */ 3, /* 20 */ 3, /* 21 */ 3,
  /* 22 */ 3, /* 23 */ 3,
  /* 24 */ 3, /* 25 */ 3,
  /* 26 */ 3, /* 27 */ 3,
  /* 28 */ 3, /* 29 */ 3, /* 30 */ 3,
  /* 31 */ 3, /* 32 */ 3,
  /* 33 */ 3, /* 34 */ 3,

  // parentheses (outer accent ring)
  /* 35 */ 4, /* 36 */ 4, /* 37 */ 4, /* 38 */ 4, /* 39 */ 4,
  /* 40 */ 4, /* 41 */ 4, /* 42 */ 4, /* 43 */ 4, /* 44 */ 4
};


// ---------------- LED STRIPS ----------------
static Adafruit_NeoPixel rings(RING_LEDS, RINGS_PIN, NEO_GRB + NEO_KHZ800);
static Adafruit_NeoPixel plate(PLATE_LEDS, PLATE_PIN, NEO_GRB + NEO_KHZ800);

// timing
static uint16_t tick = 0;
static uint16_t animTick = 0;

// animation state
static bool ringsDirty = false;
static bool plateDirty = false;
static int8_t platePos = 0;
static int8_t  plateDir = 1;
//static int8_t plateDoubleIndex = 0;   // 0..4

// blinker pulse smoothing
static uint16_t lastLeftPulse  = 0;
static uint16_t lastRightPulse = 0;

// debounce
static bool lastLeftStateStable = LOW;
static bool lastRightStateStable = LOW;
static uint16_t lastLeftChangeTime = 0;
static uint16_t lastRightChangeTime = 0;

// ring direction (single ring only)
static const bool RING_CW = false;

struct PlateTrailProfile {
  int8_t  dir;
  bool    bounce;
  uint8_t r, g, b;
  uint8_t fill_r, fill_g, fill_b;
};

// ---------------- INDEX HELPER ----------------
static inline uint8_t ringIndex(uint8_t i, bool cw) {
  return cw ? i : (RING_LEDS - 1 - i);
}

// ---------------- COMMON HELPERS ----------------
/*
void fillRings(uint8_t r, uint8_t g, uint8_t b) {
  uint32_t colour = rings.Color(r, g, b);
  for (uint8_t i = 0; i < RING_LEDS; i++)
    rings.setPixelColor(i, colour);

  ringsDirty = true;
*/
void fillRings(uint8_t r, uint8_t g, uint8_t b, uint8_t start = 0, uint8_t end = RING_LEDS - 1) {
  if (start >= RING_LEDS) return;
  if (end   >= RING_LEDS) end = RING_LEDS - 1;
  if (start > end) return;

  uint32_t colour = rings.Color(r, g, b);

  for (uint8_t i = start; i <= end; i++) {
    rings.setPixelColor(i, colour);
  }

  ringsDirty = true;
}


void fillPlate(uint8_t r, uint8_t g, uint8_t b) {
  uint32_t colour = plate.Color(r, g, b);
  
  for (uint8_t i = 0; i < PLATE_LEDS; i++) {
    plate.setPixelColor(i, colour);
  }

  plateDirty = true;
}

static inline void fadePlateTo(uint8_t bg_r, uint8_t bg_g, uint8_t bg_b, uint8_t decay) {
  for (uint8_t i = 0; i < PLATE_LEDS; i++) {
    uint32_t c = plate.getPixelColor(i);

    uint8_t r = (uint8_t)(c >> 16);
    uint8_t g = (uint8_t)(c >> 8);
    uint8_t b = (uint8_t)c;

    // move each channel toward background by 'decay' steps
    if (r > bg_r) r = (r - decay > bg_r) ? r - decay : bg_r;
    else if (r < bg_r) r = (r + decay < bg_r) ? r + decay : bg_r;

    if (g > bg_g) g = (g - decay > bg_g) ? g - decay : bg_g;
    else if (g < bg_g) g = (g + decay < bg_g) ? g + decay : bg_g;

    if (b > bg_b) b = (b - decay > bg_b) ? b - decay : bg_b;
    else if (b < bg_b) b = (b + decay < bg_b) ? b + decay : bg_b;

    plate.setPixelColor(i, plate.Color(r, g, b));
  }

  plateDirty = true;
}

static inline void fadePlate(uint8_t decay) { // to black
  fadePlateTo(0, 0, 0, decay);
}

static inline void advancePlatePos(bool bounce) {
  platePos += plateDir;

  if (bounce) {
    if (platePos <= 0) {
      platePos = 0;
      plateDir = +1;
    } else if (platePos >= PLATE_LEDS - 1) {
      platePos = PLATE_LEDS - 1;
      plateDir = -1;
    }
  } else {
    if (platePos < 0)
      platePos = PLATE_LEDS - 1;
    else if (platePos >= PLATE_LEDS)
      platePos = 0;
  }
}

static inline void renderPlateTrailEx(const PlateTrailProfile& profile) {
  if ((animTick % PLATE_STOP_DIV) != 0) return;

  if (!profile.bounce)
    plateDir = (profile.dir == 0) ? +1 : profile.dir;

  fillPlate(profile.fill_r, profile.fill_g, profile.fill_b);

  advancePlatePos(profile.bounce);

  // -------- NORMAL SWEEP (dir != 0) --------
  if (profile.dir != 0) {
    for (uint8_t i = 0; i < PLATE_TRAIL_LEN; i++) {
      int16_t p = platePos - (plateDir * i);
      if (p < 0 || p >= PLATE_LEDS) break;

      uint8_t v = 255 - (i * (255 / PLATE_TRAIL_LEN));
      plate.setPixelColor(
        p,
        plate.Color(
          (uint16_t)profile.r * v / 255,
          (uint16_t)profile.g * v / 255,
          (uint16_t)profile.b * v / 255
        )
      );
    }
  }

  // -------- DOUBLE SWEEP (dir == 0) --------
  else {
    const uint8_t half = PLATE_LEDS / 2;

    // abstract position inside half-domain
    uint8_t pos = platePos % half;

    for (uint8_t i = 0; i < PLATE_TRAIL_LEN; i++) {
      int16_t p = pos - i;
      if (p < 0) break;

      uint8_t v = 255 - (i * (255 / PLATE_TRAIL_LEN));

      uint8_t left  = (half - 1) - p;
      uint8_t right = half + p;

      if (left < PLATE_LEDS)
        plate.setPixelColor(
          left,
          plate.Color(
            (uint16_t)profile.r * v / 255,
            (uint16_t)profile.g * v / 255,
            (uint16_t)profile.b * v / 255
          )
        );

      if (right < PLATE_LEDS)
        plate.setPixelColor(
          right,
          plate.Color(
            (uint16_t)profile.r * v / 255,
            (uint16_t)profile.g * v / 255,
            (uint16_t)profile.b * v / 255
          )
        );
    }
  }

  plateDirty = true;
}

//static void renderEyeBlink(int8_t activeLevel, bool turnOn) {
//  uint32_t colour = rings.Color(55, 0, 0);
//
//  for (uint8_t i = 0; i < RING_LEDS; i++) {
//    if (ledY[i] == activeLevel) {
//      rings.setPixelColor(i, turnOn ? colour : 0);
//    }
//  }
//
//  ringsDirty = true;
//}

static inline void updateEmergencyState(bool leftActive, bool rightActive) {
  if (leftActive && rightActive) {
    emergencyLatched = true;
    emergencyLatchTick = tick;
    emergencyReleaseTick = 0;
  } else if (emergencyLatched) {
    // count how long both are inactive
    if (!leftActive && !rightActive) {
      if (emergencyReleaseTick == 0)
        emergencyReleaseTick = tick;
      else if ((uint16_t)(tick - emergencyReleaseTick) >= EMERG_RELEASE_TICKS)
        emergencyLatched = false;
    } else {
      emergencyReleaseTick = 0;
    }
  }
}

// ---------------- RING ANIMATIONS ----------------
void animRingsIdle(uint16_t animTick) {
  uint16_t idle_t = animTick % IDLE_PERIOD_TICKS;

  uint8_t idle_wave =
    (idle_t < IDLE_PERIOD_TICKS / 2)
      ? (uint32_t)idle_t * (IDLE_SPAN * 2) / IDLE_PERIOD_TICKS
      : (uint32_t)(IDLE_PERIOD_TICKS - idle_t) * (IDLE_SPAN * 2) / IDLE_PERIOD_TICKS;

  uint8_t idle_val = IDLE_MIN_BRIGHTNESS + idle_wave;
  uint32_t colour = rings.Color(idle_val, (idle_val / 10) + 1, 0);

  for (uint8_t i = 0; i < RING_LEDS; i++)
    rings.setPixelColor(ringIndex(i, RING_CW), colour);
    //rings.setPixelColor(i, colour);

  fillRings(0, 0, IDLE_MAX_BRIGHTNESS, 35, 44);

  ringsDirty = true;
}

void animRingsStop() {
  /*
  uint32_t colour = rings.Color(STOP_BRIGHTNESS, 10, 0);
  for (uint8_t i = 0; i < RING_LEDS; i++)
    rings.setPixelColor(i, colour);
  */
  
  fillRings(STOP_BRIGHTNESS, 5, 0, 0, 34);
  fillRings(255, 5, 0, 35, 44);
  
  ringsDirty = true;
}

void animRingsTurn(uint16_t animTick) {
  rings.clear();

  uint8_t pos = animTick % RING_LEDS;

  rings.setPixelColor(ringIndex(pos, RING_CW), rings.Color(255, 80, 0));

  ringsDirty = true;
}

void animRingsEmergency(uint16_t animTick) {
  uint8_t phase = (animTick / RINGS_EMERG_DELAY) & 1;
  uint8_t val = phase ? 0 : 55;

  uint32_t colour = rings.Color(val, val / 4, 0);

  for (uint8_t i = 0; i < RING_LEDS; i++)
    rings.setPixelColor(i, colour);

  ringsDirty = true;
}

void animRingsEmergencyRings() {
  if (ringEmergState != RING_EMERG_ACTIVE)
    return;

  // ---- timing gate ----
  if (++ringTick < RINGS_EMERG_DIV)
    return;
  ringTick = 0;

  uint32_t colour = rings.Color(255, 80, 0);
  int8_t level = ringOrder[ringEmergIndex];

  for (uint8_t i = 0; i < RING_LEDS; i++) {
    if (ledRing[i] == level) {
      rings.setPixelColor(i, ringOpening ? colour : 0);
    }
  }

  ringsDirty = true;

  // ---- advance ----
  if (!ringOpening) {
    ringEmergIndex++;
    if (ringEmergIndex >= RING_STEPS) {
      ringEmergIndex = RING_STEPS - 1;
      ringOpening = true;
    }
  } else {
    ringEmergIndex--;
    if (ringEmergIndex < 0) {
      ringEmergIndex = 0;
      ringOpening = false;
    }
  }
}

void startEmergencyRingAnim() {
  ringEmergIndex = 0;
  ringOpening = false;
  ringTick = 0;
  ringEmergState = RING_EMERG_ACTIVE;
}

void stopEmergencyRingAnim() {
  ringEmergState = RING_EMERG_IDLE;
}

void animRingsEyeBlink() {
  if (blinkState != BLINK_ACTIVE)
    return;

  // ---- timing gate ----
  if (++blinkTick < BLINK_STEP_TICKS)
    return;
  blinkTick = 0;

  uint32_t colour = rings.Color(
        (currentMode == MODE_STOP) ? STOP_BRIGHTNESS : IDLE_MAX_BRIGHTNESS, 10, 0);

  int8_t level = blinkOrder[blinkIndex];

  for (uint8_t i = 0; i < RING_LEDS; i++) {
    if (ledY[i] == level) {
      rings.setPixelColor(i, blinkOpening ? colour : 0);
    }
  }

  ringsDirty = true;

  // ---- advance ----
  if (!blinkOpening) {
    if (blinkIndex < BLINK_STEPS - 1) {
      blinkIndex++;
    } else {
      // we have already rendered the last step
      blinkOpening = true;
    }
  } else {
    blinkIndex--;
    if (blinkIndex < 0) {
      blinkState = BLINK_IDLE;
    }
  }
}

void startEyeBlink() {
  /*
  uint32_t colour = rings.Color(STOP_BRIGHTNESS, 0, 0);

  // ensure fully open
  for (uint8_t i = 0; i < RING_LEDS; i++) {
    rings.setPixelColor(i, colour);
  }

  ringsDirty = true;
  */

  blinkIndex = 0;
  blinkOpening = false;
  blinkTick = 0;
  blinkState = BLINK_ACTIVE;
}

// ---------------- NUMBERPLATE ----------------
void animPlateIdle() {
  fillPlate(PLATE_BRIGHTNESS, PLATE_BRIGHTNESS, PLATE_BRIGHTNESS - 5);
}

static const PlateTrailProfile PLATE_IDLE  = { +1, true,  255, 10, 0, PLATE_BRIGHTNESS, PLATE_BRIGHTNESS, PLATE_BRIGHTNESS - 5 };
static const PlateTrailProfile PLATE_STOP  = { +1, true,  255, 80, 0, 0, 0, 0 };
static const PlateTrailProfile PLATE_LEFT  = { -1, false, 255, 80, 0, 0, 0, 0 };
static const PlateTrailProfile PLATE_RIGHT = { +1, false, 255, 80, 0, 0, 0, 0 };
static const PlateTrailProfile PLATE_EMERG = { +1, true,  255, 80, 0, 0, 0, 0 };
static const PlateTrailProfile PLATE_EMERG_DOUBLE = { 0, false, 255, 80, 0, 0, 0, 0 };

void animPlateIdleTrail()  { renderPlateTrailEx(PLATE_IDLE); }
void animPlateStopTrail()  { renderPlateTrailEx(PLATE_STOP); }
void animPlateLeftTrail()  { renderPlateTrailEx(PLATE_LEFT); }
void animPlateRightTrail() { renderPlateTrailEx(PLATE_RIGHT); }
void animPlateEmergTrail() { renderPlateTrailEx(PLATE_EMERG); }
void animPlateEmergDoubleTrail() { renderPlateTrailEx(PLATE_EMERG_DOUBLE); }

/*
void animPlateDoubleSweepTrail() {
  if ((animTick % PLATE_STOP_DIV) != 0)
    return;

  // background
  fillPlate(
    PLATE_EMERG_DOUBLE.fill_r,
    PLATE_EMERG_DOUBLE.fill_g,
    PLATE_EMERG_DOUBLE.fill_b
  );

  // ---- primary heads ----
  int8_t left  = 4 - plateDoubleIndex;
  int8_t right = 5 + plateDoubleIndex;

  uint32_t headColor = plate.Color(
    PLATE_EMERG_DOUBLE.r,
    PLATE_EMERG_DOUBLE.g,
    PLATE_EMERG_DOUBLE.b
  );

  if (left >= 0)
    plate.setPixelColor(left, headColor);

  if (right < PLATE_LEDS)
    plate.setPixelColor(right, headColor);

  // ---- trail ----
  for (uint8_t i = 1; i < PLATE_TRAIL_LEN; i++) {
    uint8_t v = 255 - (i * (255 / PLATE_TRAIL_LEN));

    uint32_t trailColor = plate.Color(
      (uint16_t)PLATE_EMERG_DOUBLE.r * v / 255,
      (uint16_t)PLATE_EMERG_DOUBLE.g * v / 255,
      (uint16_t)PLATE_EMERG_DOUBLE.b * v / 255
    );

    int8_t l = left  + i;
    int8_t r = right - i;

    if (l >= 0 && l < PLATE_LEDS)
      plate.setPixelColor(l, trailColor);

    if (r >= 0 && r < PLATE_LEDS)
      plate.setPixelColor(r, trailColor);
  }

  plateDirty = true;

  // ---- advance ----
  plateDoubleIndex++;
  if (plateDoubleIndex > 4)
    plateDoubleIndex = 0;
}
*/

/*
void animPlateStop() {
  static uint8_t div = 0;
  if (++div < PLATE_STOP_DIV) return;
  div = 0;

  fillPlate(PLATE_BRIGHTNESS, PLATE_BRIGHTNESS, PLATE_BRIGHTNESS);
  plate.setPixelColor(platePos, plate.Color(255, 0, 0));

  //plate.show();
  plateDirty = true;

  platePos += plateDir;
  if (platePos == 0 || platePos == PLATE_LEDS - 1)
    plateDir = -plateDir;
}

void animPlateLeft() {
  fillPlate(PLATE_BRIGHTNESS, PLATE_BRIGHTNESS, PLATE_BRIGHTNESS);
  plate.setPixelColor(platePos, plate.Color(255, 80, 0));

  //plate.show();
  plateDirty = true;

  platePos++;
  if (platePos >= PLATE_LEDS)
    platePos = 0;
}

void animPlateRight() {
  fillPlate(PLATE_BRIGHTNESS, PLATE_BRIGHTNESS, PLATE_BRIGHTNESS);
  plate.setPixelColor(platePos, plate.Color(255, 80, 0));

  //plate.show();
  plateDirty = true;

  if (platePos == 0)
    platePos = PLATE_LEDS - 1;
  else
    platePos--;
}
*/

// ---------------- MODE MGMT ----------------
void setMode(Mode m) {
  if (m != currentMode) {
    currentMode = m;

//    if (m == MODE_EMERGENCY) {
//      startEmergencyRingAnim();
//    } else {
//      stopEmergencyRingAnim();
//    }

    if (m == MODE_STOP) {
      platePos = 0;
      plateDir = 1;
    }
    else if (m == MODE_LEFT) {
      platePos = PLATE_LEDS - 1;
      plateDir = -1;
    }
    else if (m == MODE_RIGHT) {
      platePos = 0;
      plateDir = +1;
    }
  }
}

// ---------------- ANIMATION UPDATE ----------------
void updateAnimation() {
  animTick++;
  tick++;

  delay(LOOP_DELAY);

  // ---------- RINGS ----------
//  if (ringEmergState == RING_EMERG_ACTIVE) {
//    animRingsEmergencyRings();
//  }
//  else 
  if ((blinkState == BLINK_ACTIVE) && (currentMode == MODE_IDLE || currentMode == MODE_STOP)) {
      animRingsEyeBlink();
  } else {
    switch (currentMode) {

      case MODE_IDLE:
        animRingsIdle(animTick);
        break;

      case MODE_STOP:
        animRingsStop();
        break;

      case MODE_LEFT:
      case MODE_RIGHT:
        //animRingsTurn(animTick);
        animRingsIdle(animTick);
        break;

      case MODE_EMERGENCY:
        animRingsEmergency(animTick);
        break;
    }
  }

  // ---------- PLATE ----------
//  if (ringEmergState == RING_EMERG_ACTIVE) {
//    animPlateEmergTrail();   // or animPlateIdleOverlay(), etc.
//  }
//  else {
    switch (currentMode) {
  
      case MODE_IDLE:
        animPlateIdleTrail();
        break;
  
      case MODE_STOP:
        animPlateStopTrail();
        break;
  
      case MODE_LEFT:
        animPlateLeftTrail();
        break;
  
      case MODE_RIGHT:
        animPlateRightTrail();
        break;
  
      case MODE_EMERGENCY:
        //animPlateIdleTrail();
        animPlateEmergDoubleTrail();
        break;
    }
//  }

  // ---------- TRIGGER BLINK ----------
  static uint16_t blinkIntervalTick = 0;

  if (blinkState == BLINK_IDLE) {
    if (++blinkIntervalTick >= BLINK_INTERVAL_TICKS) {
      blinkIntervalTick = 0;
      startEyeBlink();
    }
  }

  // ---------- FLUSH ----------
  if (ringsDirty) {
    rings.show();
    ringsDirty = false;
  }

  if (plateDirty) {
    plate.show();
    plateDirty = false;
  }
}

// ---------------- SETUP ----------------
void setup() {
  pinMode(PIN_STOP, INPUT);
  pinMode(PIN_LEFT, INPUT);
  pinMode(PIN_RIGHT, INPUT);

  // Keep NeoPixel pins as INPUT during bootloader window
  //pinMode(RINGS_PIN, INPUT);
  //pinMode(PLATE_PIN, INPUT);

  rings.begin();
  plate.begin();
  //rings.setBrightness(32);
  //plate.setBrightness(32);
  /*
  rings.clear();
  plate.clear();
  rings.show();
  plate.show();
  */
}

// ---------------- MAIN LOOP ----------------
void loop() {
  static uint8_t loopDivider = 0;
  if (++loopDivider < LOOP_DIVIDER) return;
  loopDivider = 0;

  bool leftState  = digitalRead(PIN_LEFT);
  bool rightState = digitalRead(PIN_RIGHT);
  bool stopActive = digitalRead(PIN_STOP);

  // -------- LEFT DEBOUNCE --------
  if (leftState != lastLeftStateStable)
    lastLeftChangeTime = tick;

  if ((uint16_t)(tick - lastLeftChangeTime) >= BLINKER_DEBOUNCE_TICKS) {
    lastLeftStateStable = leftState;
  }

  // -------- RIGHT DEBOUNCE --------
  if (rightState != lastRightStateStable)
    lastRightChangeTime = tick;

  if ((uint16_t)(tick - lastRightChangeTime) >= BLINKER_DEBOUNCE_TICKS) {
    lastRightStateStable = rightState;
  }

  // -------- RAW PULSE CAPTURE --------
  if (leftState)
    lastLeftPulse = tick;

  if (rightState)
    lastRightPulse = tick;

  bool leftActive =
    ((uint16_t)(tick - lastLeftPulse) < BLINKER_TIMEOUT_TICKS);

  bool rightActive =
    ((uint16_t)(tick - lastRightPulse) < BLINKER_TIMEOUT_TICKS);

  // DEBUG OVERRIDE
  leftActive  = false;
  rightActive = false;
//  stopActive  = false;
  /////////////////

  // -------- EMERGENCY LATCH UPDATE --------
  updateEmergencyState(leftActive, rightActive);

  // -------- MODE PRIORITY --------
  if (stopActive)
    setMode(MODE_STOP);
  //else if (leftActive && rightActive)
  else if (emergencyLatched)
    setMode(MODE_EMERGENCY);
  else if (leftActive)
    setMode(MODE_LEFT);
  else if (rightActive)
    setMode(MODE_RIGHT);
  else
    setMode(MODE_IDLE);

  updateAnimation();
}
