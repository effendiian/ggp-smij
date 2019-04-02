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
	// interface Pointer
	//
	// Pointer are device agnostic. Implementations need to be.
	// written for mouse and touch-based input.
	// --------------------------------------------------------
	class Pointer {
	private:

	public:
		
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

