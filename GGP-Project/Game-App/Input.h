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
	// enum InputIdentifier
	//
	// Definitions for unique device ID's. Supports up to 8
	// unique ID's for each type of device. Support for up to
	// 8 gamepads, 8 keyboards, 8 pointers, etc.
	// --------------------------------------------------------
	enum InputIdentifier {
		DEVICE_1,
		DEVICE_2,
		DEVICE_3,
		DEVICE_4,
		DEVICE_5,
		DEVICE_6,
		DEVICE_7,
		DEVICE_8,
		ALL,
		NULL
	};

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
	// enum InputDevice
	//
	// Definitions for known devices.
	// --------------------------------------------------------
	enum InputDevice {
		KEYBOARD,
		POINTER,
		GAMEPAD,
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
		GAMEPAD_DPAD_RIGHT,

		///////////////////////////
		// NULL CODE
		///////////////////////////

		NULL

	};
	
	// <TODO> - Add input code map to WPARAMs in InputManager.
	
	// --------------------------------------------------------
	// interface IEnable
	//
	// Interface that allows children to be enabled/disabled.
	// --------------------------------------------------------
	class IEnableState {
	protected:

		// Protected constructor to enable inheritance construction only.
		IEnableState(bool _state);
		IEnableState();

	private:

		///////////////////////////
		// DATA MEMBERS
		///////////////////////////

		unsigned int _enable; // Internal flag to store enable/disable status.

	public:

		///////////////////////////
		// INTERFACE METHODS
		///////////////////////////

		virtual void Enable(); // Enable the input object.
		virtual void Disable(); // Disable the input object.
		virtual void ToggleEnabledState(); // Toggle the value of enabled.

		///////////////////////////
		// ACCESSORS METHODS
		///////////////////////////

		bool IsEnabled(); // Return respective flag value.
		bool IsDisabled(); // Return respective flag value.

	};


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
		
		float _pointerSpeed; 
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
	// interface Channel
	//
	// Channel interface contains device type, device identity,
	// and control code that a particular command will route
	// to. Children must implement the 'Update' method.
	// --------------------------------------------------------
	class Channel : IEnableState
	{
	private:

		///////////////////////////
		// DATA MEMBERS
		///////////////////////////

		float _value; // Internal store for updated value.

	public:

		///////////////////////////
		// CONFIGURATION SETTINGS
		///////////////////////////

		const Input::InputIdentifier INPUT_DEVICE_ID; // Unique identifier for the channel device. (eg. If there are multiple gamepads, this can be used to differentiate between them).
		const Input::InputDevice INPUT_DEVICE; // Device type the channel should read from.
		const Input::InputCode INPUT_CODE; // Specific key, button, or axes the channel should update value for.

		Channel(Input::InputIdentifier _inputDeviceId, Input::InputDevice _inputDeviceType, Input::InputCode _inputCode);
		
		///////////////////////////
		// INTERFACE METHODS
		///////////////////////////

		virtual void Update() = 0; // Abstract method for updating channel value based on channel description.
		virtual bool IsDevice(Input::InputIdentifier deviceId, Input::InputDevice deviceType) = 0;
		virtual bool IsDeviceType(Input::InputDevice deviceType) = 0;
		virtual bool IsCode(Input::InputCode code) = 0;
		
		///////////////////////////
		// SERVICE METHODS
		///////////////////////////

		// Note: This class does not store the value;
		// it is not responsible for maintaining state or processing
		// the raw value. It acts as a simple wrapper to
		// convert between the WPARAM and the Input::InputCode.

		float GetValue() const; // Route to get updated value.

	};


	// --------------------------------------------------------
	// interface Command
	//
	// Commands track current values for inputs based on
	// device channels.
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
		float _deltaValue; // Contains change in value.



	public:

		///////////////////////////
		// CONFIGURATION SETTINGS
		///////////////////////////
		
		Input::InputType Type; // Input type for the command.
		unsigned int Enabled; // Is the current command active? (0 or 1).
		unsigned int Normalize; // Determines which axis value should be used. (0 or 1).

		Command();
		~Command();

		///////////////////////////
		// INTERFACE METHODS
		///////////////////////////

		virtual void Update(); // Update previous and current values.
		virtual void AssignChannel(); // <TODO>.
		virtual void RemoveChannel(); // <TODO>.
		virtual void Reset(); // Removes all channels and resets the command.

		///////////////////////////
		// SERVICE METHODS
		///////////////////////////

		void Enable(); // Enable the command.
		void Disable(); // Disable the command.
		 
	};

	class AxisCommand : Command {
	public:




	};
























}

