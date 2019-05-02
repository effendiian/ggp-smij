#pragma once

#include <exception>

// --------------------------------------------------------
// 
// --------------------------------------------------------

// ----------------------------
// Input handler acts as an interface
// that wraps the commands passed in
// from the game interface.
// ----------------------------

namespace Input 
{

	// --------------------------------------------------------
	// class
	//
	// Exception class for handling input code with no implementation.
	// --------------------------------------------------------
	class InputException : public std::exception 
	{
		// Returns message when called on the exception object.
		const char* what() const throw () {
			return "Requested input implementation does not exist.";
		}
	};
		
	// --------------------------------------------------------
	// enum
	//
	// Representation of keyboard and mouse codes.
	// --------------------------------------------------------
	enum InputCode 
	{

		// Basic keys.

		A,
		B,
		C,
		D,
		E,
		F,
		G,
		H,
		I,
		J,
		K,
		L,
		M,
		N,
		O,
		P,
		Q,
		R,
		S,
		T,
		U,
		V,
		W,
		X,
		Y,
		Z,

		// Punctuation.

		PERIOD,
		COMMA,
		SLASH,
		BACKSLASH,
		SEMI_COLON,
		SINGLE_QUOTE,
		BACKTICK,
		DASH,
		EQUALS,
		LBRACKET,
		RBRACKET,

		// Arrow keys.

		LEFT,
		RIGHT,
		UP,
		DOWN,

		// Numbers.

		NUM1,
		NUM2,
		NUM3,
		NUM4,
		NUM5,
		NUM6,
		NUM7,
		NUM8,
		NUM9,
		NUM0,

		NUMPAD1,
		NUMPAD2,
		NUMPAD3,
		NUMPAD4,
		NUMPAD5,
		NUMPAD6,
		NUMPAD7,
		NUMPAD8,
		NUMPAD9,
		NUMPAD0,
		
		// Special keys.

		TAB,
		CAPSLOCK,
		SPACE,
		ESCAPE,
		ENTER,
		RETURN,
		BACKSPACE,
		DELETE,
		PAUSE,
		INSERT,
		HOME,
		END,

		// Special modifier keys.

		COMMAND,
		LSHIFT,
		RSHIFT,
		LCTRL,
		RCTRL,
		LALT,
		RALT,

		// Mouse codes.

		LMB,
		RMB,
		MMB,
		MOUSE_X,
		MOUSE_Y,
		MOUSE_WHEEL

	};

	// --------------------------------------------------------
	// enum
	//
	// Input type of the control.
	// --------------------------------------------------------
	enum InputType 
	{
		// Provides values relative to previous state. Useful for Mouse movement.
		DELTA, 

		// Provides raw axis value of current state. Useful for mouse movement.
		RAW_AXIS,

		// Provides absolute value for a particular axis. [0...1].
		AXIS,
		
		// Provides 0 or 1 values.
		BUTTON,

		// Always returns a 0 value.
		DISABLED
	};

	// --------------------------------------------------------
	// class
	//
	// Control class.
	// --------------------------------------------------------
	class Control 
	{
	private:

		// The type of value being stored in the control.
		InputType _type;

		// Values below this threshold are clamped to zero.
		float _threshold; 

		// [0, 1], inclusive value holding the input data.
		float _value;

	public:

		// -----------------
		// Constructor(s).
		// -----------------
		Control(InputType type);


		// -----------------
		// Helper methods.
		// -----------------



	};




};