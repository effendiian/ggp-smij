#pragma once

// Input classes oversees Input objects.
namespace Input {
	
	// For command objects:
	// Action.Active returns boolean value.
	// Action.Value returns 0 or 1.
	// State.Active returns boolean value.
	// State.Value returns 0 or 1.
	// Axis.Active returns boolean value.
	// // if Threshold > 0, Axis.Value must be greater than the threshold to return true.
	// Axis.Value returns axis value.
	// // if Normalized = true, Axis.Value will be between 0 and 1.
	// // if Normalized = false, Axis.Value will be raw value. (useful for pointers).

	// Three Layers.

	// RawInput - Gather information from the Operating System.
	// Context - Chain of responsibility callbacks that dispatches updates.
	// Action - Action that collects the 'Context.Actions'

	// --------------------------------------------------------
	// enum InputType
	//
	// Definitions for possible input types. 
	// - Action - One-Time Triggers ('Key Press' / 'Key Release').
	// - State - Continuous Events ('Key Hold').
	// - Range - Value Axis (Pointer, GamePad Triggers, etc.).
	// - NULL - Ignore. Always return as no input.
	// --------------------------------------------------------
	enum InputType
	{
		ACTION,
		STATE,
		RANGE,
		NULL
	};

	// --------------------------------------------------------
	// enum InputCode
	//
	// Definitions for possible input codes. Does not map
	// directly to WPARAMs. That must be done when the input
	// manager is being setup.
	// --------------------------------------------------------
	enum InputCode 
	{

		///////////////////////////
		// KEYBOARD CODES
		///////////////////////////

		KEYBOARD_A,
		KEYBOARD_B,
		KEYBOARD_C,
		KEYBOARD_D,
		KEYBOARD_E,
		KEYBOARD_F,
		KEYBOARD_G,
		KEYBOARD_H,
		KEYBOARD_I,
		KEYBOARD_J,
		KEYBOARD_K,
		KEYBOARD_L,
		KEYBOARD_M,
		KEYBOARD_N,
		KEYBOARD_O,
		KEYBOARD_P,
		KEYBOARD_Q,
		KEYBOARD_R,
		KEYBOARD_S,
		KEYBOARD_T,
		KEYBOARD_U,
		KEYBOARD_V,
		KEYBOARD_W,
		KEYBOARD_X,
		KEYBOARD_Y,
		KEYBOARD_Z,

		KEYBOARD_0,
		KEYBOARD_1,
		KEYBOARD_2,
		KEYBOARD_3,
		KEYBOARD_4,
		KEYBOARD_5,
		KEYBOARD_6,
		KEYBOARD_7,
		KEYBOARD_8,
		KEYBOARD_9,

		KEYBOARD_UP,
		KEYBOARD_DOWN,
		KEYBOARD_LEFT,
		KEYBOARD_RIGHT,
		KEYBOARD_SPACE,
		KEYBOARD_LSHIFT,
		KEYBOARD_RSHIFT,
		KEYBOARD_LCTRL,
		KEYBOARD_RCTRL,
		KEYBOARD_LALT,
		KEYBOARD_RALT,

		///////////////////////////
		// POINTER CODES
		///////////////////////////

		POINTER_UP,
		POINTER_DOWN,
		POINTER_LEFT,
		POINTER_RIGHT,
		POINTER_0,
		POINTER_1,
		POINTER_3,

		///////////////////////////
		// GAMEPAD CODES
		///////////////////////////

		GAMEPAD_A,
		GAMEPAD_B,
		GAMEPAD_X,
		GAMEPAD_Y,
		GAMEPAD_Z,
		GAMEPAD_LB,
		GAMEPAD_RB,
		GAMEPAD_LT,
		GAMEPAD_RT,
		GAMEPAD_START,
		GAMEPAD_SELECT,
		GAMEPAD_L,
		GAMEPAD_L_UP,
		GAMEPAD_L_RIGHT,
		GAMEPAD_L_LEFT,
		GAMEPAD_L_RIGHT,
		GAMEPAD_R,
		GAMEPAD_R_UP,
		GAMEPAD_R_DOWN,
		GAMEPAD_R_LEFT,
		GAMEPAD_R_RIGHT,
		GAMEPAD_DPAD_UP,
		GAMEPAD_DPAD_DOWN,
		GAMEPAD_DPAD_LEFT,
		GAMEPAD_DPAD_RIGHT

	};
	
	// <TODO> - Add input code map to WPARAMs in InputManager.

	// --------------------------------------------------------
	// interface Pointer
	//
	// Pointer are device agnostic. Implementations need to be.
	// written for mouse and touch-based input.
	// --------------------------------------------------------
	class Pointer {
	private:

		///////////////////////////
		// DATA MEMBERS
		///////////////////////////

		bool _pointerMoved;
		bool _pointerPressed;

		float _previousX;
		float _previousY;
		float _currentX;
		float _currentY;
		float _deltaX;
		float _deltaY;

		unsigned int _identifier;
				
	public:

		///////////////////////////
		// CONFIGURATION SETTINGS
		/////////////////////////// 


		
	};

	// --------------------------------------------------------
	// interface Command
	//
	// <TODO>.
	// --------------------------------------------------------
	class Command
	{
	private:
				
		///////////////////////////
		// DATA MEMBERS
		///////////////////////////
		
		float _threshold; // Deadzone threshold for axes. Defaults to zero for other types.
		float _previousValue; // Contains last frame's command state value.
		float _currentValue; // Contains current frame's command state value.

	public:

		///////////////////////////
		// CONFIGURATION SETTINGS
		///////////////////////////
		
		Input::InputType Type; // Input type for the command.
		unsigned int Normalize; // Determines which axis value should be used. (0 or 1).
		
		// Command updates.
		virtual void Update();
		 
	};

























}

