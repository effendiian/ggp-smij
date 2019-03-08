#include "InputManager.h"

//Initialize default values
void InputManager::Init(HWND hWnd)
{
	this->hWnd = hWnd;

	winRequireFocus = true;
	mb_L_Down = false;
	mb_R_Down = false;
	mb_M_Down = false;
	prev_MB_L_Down = false;
	prev_MB_R_Down = false;
	prev_MB_M_Down = false;

	GetCursorPos(&mousePos);
	prevMousePos = mousePos;
}

InputManager::~InputManager()
{ }

//Update the input manager (only call ONCE PER FRAME!)
void InputManager::UpdateStates()
{
	if (GetFocus() == hWnd)
	{
		windowFocused = true;
	}
	else windowFocused = false;
	
	//Previouse mouse button states
	prev_MB_L_Down = mb_L_Down;
	prev_MB_R_Down = mb_R_Down;
	prev_MB_M_Down = mb_M_Down;

	//Get new cursor position
	prevMousePos = mousePos;
	GetCursorPos(&mousePos);

	/*
	static float timer = 0;
	timer += deltaTime;

	if (timer > 1)
	{
		timer = 0;
		std::cout << "F\t: " << windowFocused << std::endl;
		std::cout << "MW\t: " << hWnd << std::endl;
		std::cout << "GAW\t: " << GetActiveWindow() << std::endl;
		std::cout << "GFW\t: " << GetForegroundWindow() << std::endl;
		std::cout << "GF\t: " << GetFocus() << "\n" << std::endl;
	}
	*/
}

//Update the mouse position (only call ONCE PER FRAME!)
void InputManager::UpdateMousePos()
{
	//Get new cursor position
	prevMousePos = mousePos;
	GetCursorPos(&mousePos);
}

// Change the window focus requirement
void InputManager::SetWindowFocusRequirement(bool windowMustBeFocused)
{
	winRequireFocus = windowMustBeFocused;
}

// --------------------------------------------------------
// Helper method for mouse clicking.  We get this information
// from the OS-level messages anyway, so these helpers have
// been created to provide basic mouse input if you want it.
// --------------------------------------------------------
void InputManager::OnMouseDown(WPARAM buttonState, int x, int y)
{
	// Save the previous mouse position, so we have it for the future
	//prevMousePos.x = x;
	//prevMousePos.y = y;

	//Find what button was pressed
	if (buttonState & 0x0001) { mb_L_Down = true; }
	else if (buttonState & 0x0002) { mb_R_Down = true; }
	else if (buttonState & 0x0010) { mb_M_Down = true; }

	// Caputure the mouse so we keep getting mouse move
	// events even if the mouse leaves the window.  we'll be
	// releasing the capture once a mouse button is released
	SetCapture(hWnd);
}

// --------------------------------------------------------
// Helper method for mouse release
// --------------------------------------------------------
void InputManager::OnMouseUp(WPARAM buttonState, int x, int y, int button)
{
	//Find what button was released
	if (button & 0x0001) { mb_L_Down = false; }
	else if (button & 0x0002) { mb_R_Down = false; }
	else if (button & 0x0010) { mb_M_Down = false; }

	// We don't care about the tracking the cursor outside
	// the window anymore (we're not dragging if the mouse is up)
	ReleaseCapture();
}

// --------------------------------------------------------
// Helper method for mouse movement.  We only get this message
// if the mouse is currently over the window, or if we're 
// currently capturing the mouse.
// --------------------------------------------------------
void InputManager::OnMouseMove(WPARAM buttonState, int x, int y)
{
	// Save the previous mouse position, so we have it for the future
	//prevMousePos.x = x;
	//prevMousePos.y = y;
}

// --------------------------------------------------------
// Helper method for mouse wheel scrolling.  
// WheelDelta may be positive or negative, depending 
// on the direction of the scroll
// --------------------------------------------------------
void InputManager::OnMouseWheel(float wheelDelta, int x, int y)
{
	//TODO: Implement wheel states
}

//Returns true while the inputted key is held down
bool InputManager::GetKey(char key)
{
	//Early return if we need focus and we don't have it
	if (winRequireFocus && !windowFocused)
		return false;

	return GetAsyncKeyState(key) & 0x8000;
}

//TODO: Implement keyboard states

/*
//Returns true during the frame the user pressed down the inputted key
bool InputManager::GetKeyDown(char key)
{
	return false;
}

//Returns true the first frame the user releases the inputted key
bool InputManager::GetKeyUp(char key)
{
	return false;
}
*/

// Returns true while the inputted mouse button is held down
bool InputManager::GetMouseButton(MouseButtons button)
{
	//Early return if we need focus and we don't have it
	if (winRequireFocus && !windowFocused)
		return false;

	//Find the correct case for the input
	switch (button)
	{
		case MouseButtons::L:
			return mb_L_Down && prev_MB_L_Down;
			break;

		case MouseButtons::R:
			return mb_R_Down && prev_MB_R_Down;
			break;

		case MouseButtons::M:
			return mb_M_Down && prev_MB_M_Down;
			break;

		default:
			return false;
			break;
	}
}

// Returns true during the frame the user pressed down the mouse button
bool InputManager::GetMouseButtonDown(MouseButtons button)
{
	//Early return if we need focus and we don't have it
	if (winRequireFocus && !windowFocused)
		return false;

	//Find the correct case for the input
	switch (button)
	{
		case MouseButtons::L:
			return mb_L_Down && !prev_MB_L_Down;
			break;

		case MouseButtons::R:
			return mb_R_Down && !prev_MB_R_Down;
			break;

		case MouseButtons::M:
			return mb_M_Down && !prev_MB_M_Down;
			break;

		default:
			return false;
			break;
	}
}

//Returns true the first frame the user releases the mouse button
bool InputManager::GetMouseButtonUp(MouseButtons button)
{
	//Early return if we need focus and we don't have it
	if (winRequireFocus && !windowFocused)
		return false;

	//Find the correct case for the input
	switch (button)
	{
		case MouseButtons::L:
			return !mb_L_Down && !prev_MB_L_Down;
			break;

		case MouseButtons::R:
			return !mb_R_Down && !prev_MB_R_Down;
			break;

		case MouseButtons::M:
			return !mb_M_Down && !prev_MB_M_Down;
			break;

		default:
			return false;
			break;
	}
}

// Get the current X mouse position
long InputManager::GetMouseX()
{
	return mousePos.x;
}

// Get the current Y mouse position
long InputManager::GetMouseY()
{
	return mousePos.y;
}

// Get the X coordinate of the window (left)
long InputManager::GetWindowX()
{
	RECT rect;
	GetWindowRect(hWnd, &rect);
	return rect.left;
}

// Get the Y coordinate of the window (top)
long InputManager::GetWindowY()
{
	RECT rect;
	GetWindowRect(hWnd, &rect);
	return rect.top;
}

// Get the center X of the window
long InputManager::GetWindowCenterX()
{
	RECT rect;
	GetWindowRect(hWnd, &rect);
	return rect.left + ((rect.right - rect.left) / 2);
}

// Get the center Y of the window
long InputManager::GetWindowCenterY()
{
	RECT rect;
	GetWindowRect(hWnd, &rect);
	return rect.top + ((rect.bottom - rect.top) / 2);
}

long InputManager::GetWindowWidth()
{
	RECT rect;
	GetWindowRect(hWnd, &rect);
	return (rect.right - rect.left) / 2;
}

long InputManager::GetWindowHeight()
{
	RECT rect;
	GetWindowRect(hWnd, &rect);
	return (rect.right - rect.left) / 2;
}
