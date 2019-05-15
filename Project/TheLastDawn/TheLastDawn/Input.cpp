// P4G - Semester 2 - Group Project 2019
// CS4G : Group F
// Charlie Batten - 27012619, Nico Caruana - 27022205, Alin Medianu - 27005327

#include <Windows.h>
#include <math.h>
#include <cassert>
#include <sstream>

#include "Input.h"
#include "D3DUtil.h"
#include "WindowUtils.h"

#include "AudioMgr.h"
#include "AudioMgrFMOD.h"

using namespace std;
using namespace DirectX;
using namespace DirectX::SimpleMath;

void Gamepad::Update()
{
  for (DWORD i = 0; i < XUSER_MAX_COUNT; i++)
  {
    State& s = pads_[i];
    s.port = -1;
    ZeroMemory(&s.state, sizeof(XINPUT_STATE));
    if (XInputGetState(i, &s.state) == ERROR_SUCCESS)
    {
      float normLX = fmaxf(-1, (float)s.state.Gamepad.sThumbLX / 32767);
      float normLY = fmaxf(-1, (float)s.state.Gamepad.sThumbLY / 32767);

      s.leftStickX = (abs(normLX) < s.deadzoneX ? 0 : (abs(normLX) - s.deadzoneX) * (normLX / abs(normLX)));
      s.leftStickY = (abs(normLY) < s.deadzoneY ? 0 : (abs(normLY) - s.deadzoneY) * (normLY / abs(normLY)));

      if (s.deadzoneX > 0)
        s.leftStickX *= 1 / (1 - s.deadzoneX);
      if (s.deadzoneY > 0)
        s.leftStickY *= 1 / (1 - s.deadzoneY);

      float normRX = fmaxf(-1, (float)s.state.Gamepad.sThumbRX / 32767);
      float normRY = fmaxf(-1, (float)s.state.Gamepad.sThumbRY / 32767);

      s.rightStickX = (abs(normRX) < s.deadzoneX ? 0 : (abs(normRX) - s.deadzoneX) * (normRX / abs(normRX)));
      s.rightStickY = (abs(normRY) < s.deadzoneY ? 0 : (abs(normRY) - s.deadzoneY) * (normRY / abs(normRY)));

      if (s.deadzoneX > 0)
        s.rightStickX *= 1 / (1 - s.deadzoneX);
      if (s.deadzoneY > 0)
        s.rightStickY *= 1 / (1 - s.deadzoneY);

      s.leftTrigger = (float)s.state.Gamepad.bLeftTrigger / 255;
      s.rightTrigger = (float)s.state.Gamepad.bRightTrigger / 255;

      s.port = i;
    }
  }

}

void Gamepad::Initialise()
{
  for (int i = 0; i < XUSER_MAX_COUNT; ++i)
    pads_[i].port = -1;
}


bool Gamepad::IsPressed(int idx, WORD button)
{
  assert(idx >= 0 && idx < XUSER_MAX_COUNT);
  assert(pads_[idx].port != -1);
  return (pads_[idx].state.Gamepad.wButtons & button) != 0;
}

void Gamepad::GetAnalogueNames(int idx, std::wstring & mssg)
{
  assert(idx >= 0 && idx < XUSER_MAX_COUNT);
  State & s = pads_[idx];
  mssg = L"";
  if (s.port == -1)
    return;

  wstringstream str;
  str.precision(2);
  str << L"Left stick(x=" << s.leftStickX << L",y=" << s.leftStickY << L")";
  str << L" Right stick(x=" << s.rightStickX << L",y=" << s.rightStickY << L")";
  str << L" Trigger Left=" << s.leftTrigger << L", right=" << s.rightTrigger;
  mssg = str.str();
}

void Gamepad::GetDigitalNames(int idx, std::wstring & mssg)
{
  assert(idx >= 0 && idx < XUSER_MAX_COUNT);
  State & s = pads_[idx];
  mssg = L"";
  if (s.port == -1)
    return;

  wstringstream str;
  if (IsPressed(idx, XINPUT_GAMEPAD_DPAD_UP))
    str << L"DPAD_UP ";
  if (IsPressed(idx, XINPUT_GAMEPAD_DPAD_DOWN))
    str << L"DPAD DOWN ";
  if (IsPressed(idx, XINPUT_GAMEPAD_DPAD_LEFT))
    str << L"DPAD LEFT ";
  if (IsPressed(idx, XINPUT_GAMEPAD_DPAD_RIGHT))
    str << L"DPAD RIGHT ";
  if (IsPressed(idx, XINPUT_GAMEPAD_START))
    str << L"START ";
  if (IsPressed(idx, XINPUT_GAMEPAD_BACK))
    str << L"BACK ";
  if (IsPressed(idx, XINPUT_GAMEPAD_LEFT_THUMB))
    str << L"LEFT THUMB ";
  if (IsPressed(idx, XINPUT_GAMEPAD_RIGHT_THUMB))
    str << L"RIGHT THUMB ";
  if (IsPressed(idx, XINPUT_GAMEPAD_LEFT_SHOULDER))
    str << L"LEFT SHOULDER ";
  if (IsPressed(idx, XINPUT_GAMEPAD_RIGHT_SHOULDER))
    str << L"RIGHT SHOULDER ";
  if (IsPressed(idx, XINPUT_GAMEPAD_A))
    str << L"A ";
  if (IsPressed(idx, XINPUT_GAMEPAD_B))
    str << L"B ";
  if (IsPressed(idx, XINPUT_GAMEPAD_X))
    str << L"X ";
  if (IsPressed(idx, XINPUT_GAMEPAD_Y))
    str << L"Y ";
  mssg = str.str();
}



//-------------------------------------------------------------
bool MouseAndKeys::IsPressed(unsigned short vkeyCode) const
{
  assert(vkeyCode < KEYBUFF_SIZE);
  bool pressed = keyBuffer_[vkeyCode] != 0;

  return pressed;
}

void MouseAndKeys::ProcessKeys(RAWINPUT * raw)
{
  //standard key code
  unsigned short flags = raw->data.keyboard.Flags;
  unsigned short vkey = raw->data.keyboard.VKey;
  //the scan code might be useful, but we won't use it here
  unsigned short scanCode = raw->data.keyboard.MakeCode;

  //ignore anything larger than this, escape codes, fake keys or weird keys
  if (vkey >= 255)
    return;

  if (vkey == VK_SHIFT)
  {
    // correct left-hand / right-hand SHIFT (VK_LSHIFT / VK_RSHIFT)
    vkey = MapVirtualKey(scanCode, MAPVK_VSC_TO_VK_EX);
  }
  else if (vkey == VK_NUMLOCK)
  {
    // correct PAUSE/BREAK and NUM LOCK silliness, and set the extended bit
    scanCode = (MapVirtualKey(vkey, MAPVK_VK_TO_VSC) | 0x100);
  }

  // e0 and e1 are escape sequences used for certain special keys, such as PRINT and PAUSE/BREAK.
  // see http://www.win.tue.nl/~aeb/linux/kbd/scancodes-1.html
  const bool isE0 = ((flags & RI_KEY_E0) != 0);
  const bool isE1 = ((flags & RI_KEY_E1) != 0);

  if (isE1)
  {
    // for escaped sequences, turn the virtual key into the correct scan code using MapVirtualKey.
    // however, MapVirtualKey is unable to map VK_PAUSE (this is a known bug), hence we map that by hand.
    if (vkey == VK_PAUSE)
      scanCode = 0x45;
    else
      scanCode = MapVirtualKey(vkey, MAPVK_VK_TO_VSC);
  }


  switch (vkey)
  {
    // right-hand CONTROL and ALT have their e0 bit set
  case VK_CONTROL:
    if (isE0)
      vkey = VK_RCONTROL; //right control
    else
      vkey = VK_LCONTROL;  //left control
    break;

  case VK_MENU:
    if (isE0)
      vkey = VK_RALT;  //right alt
    else
      vkey = VK_LALT;	//left alt
    break;

    // NUMPAD ENTER has its e0 bit set
  case VK_RETURN:
    if (isE0)
      vkey = VK_NUMPAD_ENTER; //numpad enter
    break;
  }
  //ignore anything larger than this, escape codes, fake keys or weird keys
  if (vkey >= 255)
    return;

  if (flags & RI_KEY_BREAK) //key up
    keyBuffer_[vkey] = 0;
  else
    keyBuffer_[vkey] = scanCode;
}

void MouseAndKeys::ProcessMouse(RAWINPUT * raw)
{
  unsigned short flags = raw->data.mouse.usButtonFlags;

  if (flags & RI_MOUSE_LEFT_BUTTON_DOWN)
    buttons_[LBUTTON] = true;
  if (flags & RI_MOUSE_LEFT_BUTTON_UP)
    buttons_[LBUTTON] = false;
  if (flags & RI_MOUSE_MIDDLE_BUTTON_DOWN)
    buttons_[MBUTTON] = true;
  if (flags & RI_MOUSE_MIDDLE_BUTTON_UP)
    buttons_[MBUTTON] = false;
  if (flags & RI_MOUSE_RIGHT_BUTTON_DOWN)
    buttons_[RBUTTON] = true;
  if (flags & RI_MOUSE_RIGHT_BUTTON_UP)
    buttons_[RBUTTON] = false;

  Vector2 last(mouseScreen_);
  GetMousePosAbsolute(mouseScreen_);
  mouseMove_ = mouseScreen_ - last;
}

void MouseAndKeys::GetMousePosAbsolute(Vector2 & pos)
{
  POINT mpos;
  if (GetCursorPos(&mpos))
  {
    if (ScreenToClient(hwnd_, &mpos))
    {
      pos = Vector2((float)mpos.x, (float)mpos.y);
    }
  }

}

void MouseAndKeys::MessageEvent(HRAWINPUT rawInput)
{
  UINT dwSize;

  UINT res = GetRawInputData(rawInput, RID_INPUT, NULL, &dwSize, sizeof(RAWINPUTHEADER));
  if (res != 0)
    return;//bad input so ignore it

  if (dwSize >= RAWBUFF_SIZE)
  {
    DBOUT("Buffer too small. Is " << RAWBUFF_SIZE << " wants " << dwSize);
    assert(false);
  }

  if (GetRawInputData(rawInput, RID_INPUT, inBuffer_, &dwSize, sizeof(RAWINPUTHEADER)) != dwSize)
  {
    DBOUT("GetRawInputData failed");
    assert(false);
  }

  RAWINPUT* raw = (RAWINPUT*)inBuffer_;

  if (raw->header.dwType == RIM_TYPEKEYBOARD)
  {
    ProcessKeys(raw);
  }
  else if (raw->header.dwType == RIM_TYPEMOUSE)
  {
    ProcessMouse(raw);
  }
}

void MouseAndKeys::GetPressedKeyNames(wstring & mssg)
{
  mssg = L"";
  for (int i = 0; i < KEYBUFF_SIZE; ++i)
  {
    if (keyBuffer_[i] == 0)
      continue;
    //numeric pad and arrow keys conflict, ise0 is set for arrow keys
    //here we just assume the codes refer to arrow keys
    switch (i)
    {
    case VK_RIGHT:
      mssg += L"RIGHT ARROW ";
      break;
    case VK_LEFT:
      mssg += L"LEFT ARROW ";
      break;
    case VK_UP:
      mssg += L"UP ARROW ";
      break;
    case VK_DOWN:
      mssg += L"DOWN ARROW ";
      break;
    default:
      wchar_t key[256];
      if (GetKeyNameTextW(keyBuffer_[i] << 16, key, sizeof(key)))
      {
        mssg += key;
        mssg += L" ";
      }
    }

  }
}



void MouseAndKeys::GetMouseNames(wstring & mssg)
{
  mssg = L"";
  static float oldX = 0, oldY = 0;
  if (!Equals(mouseMove_.x, 0))
    oldX = mouseMove_.x;
  if (!Equals(mouseMove_.y, 0))
    oldY = mouseMove_.y;
  wstringstream str;
  str.precision(0);
  str << L"x=" << mouseScreen_.x << L" y=" << mouseScreen_.y;
  str << L" xinc=" << oldX << L" yinc=" << oldY;
  if (buttons_[LBUTTON])
    str << L" LButton";
  if (buttons_[MBUTTON])
    str << L" MButton";
  if (buttons_[RBUTTON])
    str << L" RButton";

  mssg = str.str();
}

void MouseAndKeys::Initialise(HWND hwnd, bool showMouse, bool confineMouse)
{
  hwnd_ = hwnd;

  RAWINPUTDEVICE Rid[2];

  //these ids trigger mouse input
  Rid[0].usUsagePage = 0x01;
  Rid[0].usUsage = 0x02;
  Rid[0].dwFlags = RIDEV_INPUTSINK;// RIDEV_NOLEGACY;   // adds HID mouse and also ignores legacy mouse messages
  Rid[0].hwndTarget = GetMainWnd();

  //these ids trigger keyboard input
  Rid[1].usUsagePage = 0x01;
  Rid[1].usUsage = 0x06;
  Rid[1].dwFlags = RIDEV_INPUTSINK;// RIDEV_NOLEGACY;   // adds HID keyboard and also ignores legacy keyboard messages
  Rid[1].hwndTarget = GetMainWnd();

  // RIDEV_NOLEGACY will stop normal message pump WM_CHAR type messages
  // sometimes we might want that, for now we'll leave it, then we can
  // still use the normal message pump for things like player name entry

  if (RegisterRawInputDevices(Rid, 2, sizeof(Rid[0])) == FALSE) {
    MessageBox(0, "Cannot get keyboard and mouse input", 0, 0);
    assert(false);
  }

  ShowCursor(showMouse);
  confineMouse_ = confineMouse;
  GetClipCursor(&oldClip_);
  if (confineMouse_)
  {
    GetWindowRect(hwnd, &newClip_);
    ClipCursor(&newClip_);
  }
  GetMousePosAbsolute(mouseScreen_);
}

void MouseAndKeys::OnLost()
{
  Reset();
  ClipCursor(&oldClip_);
}

void MouseAndKeys::OnReset()
{
  Reset();
  ClipCursor(&newClip_);
}

void MouseAndKeys::PostProcess()
{
  mouseMove_.x = 0;
  mouseMove_.y = 0;
}


void MouseAndKeys::MousePosToWorldRay(const Matrix & view, const Matrix & proj, const Vector3 & camPos, Ray & ray)
{
  int w, h;
  GetClientExtents(w, h);

  float x = (2 * mouseScreen_.x / w) - 1;
  float y = ((2 * mouseScreen_.y / h) - 1) * -1;
  x = x / proj.m[0][0];
  y = y / proj.m[1][1];

  Matrix inv;
  view.Invert(inv);
  Vector3::TransformNormal(Vector3(x, y, 1), inv, ray.direction);

  ray.direction.Normalize();
  ray.position = camPos;
}


