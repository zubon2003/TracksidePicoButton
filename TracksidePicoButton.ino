#include <Keyboard.h>

//----------------------------------------------------
// 定数・グローバル変数の定義
//----------------------------------------------------
#define START_BUTTON_DURATION 950          // 0.8秒（ミリ秒）
#define KEY_RELEASE 30                     // 押してから10ms後にキーをリリース
#define DEBOUNCE_TIME 200                  // 200msの間は同一ボタンの入力を受け付けない

enum ButtonType { LAP1, LAP2, LAP3, START, CANCEL1, CANCEL2, CANCEL3 };

const uint8_t ButtonPin[] = { 11, 15, 16, 20, 10, 14, 17 };
uint32_t keyDebounceMillis[sizeof(ButtonPin) / sizeof(ButtonPin[0])] = { 0 };
bool waitDebounce[sizeof(ButtonPin) / sizeof(ButtonPin[0])] = { false };

//----------------------------------------------------
// setup
//----------------------------------------------------
void setup() {
  for (uint8_t i = 0; i < sizeof(ButtonPin) / sizeof(ButtonPin[0]); i++) {
    pinMode(ButtonPin[i], INPUT_PULLUP);
  }
  Keyboard.begin();
}

//----------------------------------------------------
// loop
//----------------------------------------------------
void loop() {
  if (isButtonPushed(START)) {
    startSequence();
  }

  processButton(LAP1, KEY_LEFT_ALT, '1');
  processButton(LAP2, KEY_LEFT_ALT, '2');
  processButton(LAP3, KEY_LEFT_ALT, '3');
  processButton(CANCEL1, KEY_LEFT_CTRL, '1');
  processButton(CANCEL2, KEY_LEFT_CTRL, '2');
  processButton(CANCEL3, KEY_LEFT_CTRL, '3');
}

//----------------------------------------------------
// ボタン処理関数
//----------------------------------------------------
void processButton(ButtonType button, uint8_t modifierKey, char key) {
  if (!waitDebounce[button] && isButtonPushed(button)) {
    Keyboard.press(modifierKey);
    Keyboard.press(key);
    delay(KEY_RELEASE);
    Keyboard.release(modifierKey);
    Keyboard.release(key);

    waitDebounce[button] = true;
    keyDebounceMillis[button] = millis() + DEBOUNCE_TIME;
  }

  // デバウンス解除処理
  if (waitDebounce[button] && millis() > keyDebounceMillis[button]) {
    waitDebounce[button] = false;
  }
}

bool isButtonPushed(ButtonType button) {
  return digitalRead(ButtonPin[button]) == LOW;
}

//----------------------------------------------------
// シーケンス処理
//----------------------------------------------------
void startSequence() {
  Keyboard.press(KEY_LEFT_ALT);
  Keyboard.press(KEY_LEFT_CTRL);
  delay(100);
  Keyboard.press('1');
  delay(100);
  Keyboard.release(KEY_LEFT_ALT);
  Keyboard.release(KEY_LEFT_CTRL);
  Keyboard.release('1');

  delay(START_BUTTON_DURATION - 200);
  Keyboard.press(' ');
  delay(100);
  Keyboard.release(' ');
}