#pragma once
#include "Input.h"

// Access the Input namespace.
using namespace Input;

///////////////////////////
// INTERFACES
///////////////////////////

#pragma region IEnableState

/// <summary>
/// Enables this instance.
/// </summary>
void IEnableState::Enable()
{
	this->_enable = true;
}

/// <summary>
/// Disables this instance.
/// </summary>
void IEnableState::Disable() 
{
	this->_enable = false;
}

/// <summary>
/// Toggles the enabled state of the instance.
/// </summary>
void IEnableState::ToggleEnabledState()
{
	this->_enable = !_enable;
}

/// <summary>
/// Determines whether this instance is enabled.
/// </summary>
bool IEnableState::IsEnabled()
{
	return (_enable);
}

/// <summary>
/// Determines whether this instance is disabled.
/// </summary>
bool IEnableState::IsDisabled()
{
	return !IsEnabled();
}

#pragma endregion

// --------------------------------------------------------
// interface Command
//
// Commands track current values for inputs based on
// device channels.
// --------------------------------------------------------

///////////////////////////
// Constructors.
///////////////////////////

Command::Command() {
	
}

Command::~Command() {
	
}