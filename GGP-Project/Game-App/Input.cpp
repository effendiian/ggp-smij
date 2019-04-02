#pragma once
#include "Input.h"

// Access the Input namespace.
using namespace Input;

///////////////////////////
// INTERFACES
///////////////////////////

#pragma region IEnableState

// --------------------------------------------------------
// interface IEnableState
//
// Helper functions for enable/disable state.
// --------------------------------------------------------

/// <summary>
/// Enable the instance by default.
/// </summary>
IEnableState::IEnableState() : IEnableState(true) {}

/// <summary>
/// Set enable state for the instance on creation.
/// </summary>
IEnableState::IEnableState(bool _state) 
{
	_enable = _state ? 1 : 0;
}

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

#pragma region Channel

// --------------------------------------------------------
// interface Channel
//
// Channel interface contains device type, device identity,
// and control code that a particular command will route
// to. Children must implement the 'Update' method.
// --------------------------------------------------------

/// <summary>
/// Finalizes an instance of the <see cref="Channel"/> class.
/// </summary>
Channel::Channel(InputIdentifier _inputDeviceId, InputDevice _inputDeviceType, InputCode _inputCode)
	: INPUT_DEVICE_ID( _inputDeviceId ), INPUT_DEVICE( _inputDeviceType ), INPUT_CODE( _inputCode )
{
	_value = 0; // Set channel to zero.
}

/// <summary>
/// Determines whether the specified device identifier is the stored identifier.
/// </summary>
/// <param name="deviceId">The device identifier.</param>
/// <param name="deviceType">Type of the device.</param>
/// <returns>
///   <c>true</c> if the specified device identifier is the same as the stored identifier; otherwise, <c>false</c>.
/// </returns>
bool Channel::IsDevice(InputIdentifier deviceId, InputDevice deviceType)
{
	if (deviceId == InputIdentifier::ALL) { return IsDeviceType(deviceType); }
	return (INPUT_DEVICE_ID == deviceId) && IsDeviceType(deviceType);
}

/// <summary>
/// Compare the channel's stored input device type.
/// </summary>
/// <param name="deviceType">Type of the device.</param>
/// <returns>
///   <c>true</c> if the stored device type is the same as the input device type; otherwise, <c>false</c>.
/// </returns>
bool Channel::IsDeviceType(InputDevice deviceType)
{
	return (INPUT_DEVICE == deviceType);
}

/// <summary>
/// Compare the channel's stored input code.
/// </summary>
/// <param name="code">The code to check for.</param>
/// <returns>
///   <c>true</c> if the stored code is the same as the input code; otherwise, <c>false</c>.
/// </returns>
bool Channel::IsCode(InputCode code)
{
	return (INPUT_CODE == code);
}

/// <summary>
/// Gets the stored _value.
/// </summary>
/// <returns>Value of the _value.</returns>
float Channel::GetValue() const
{
	return _value;
}

/// <summary>
/// Determines null value of device type or identifier.
/// </summary>
/// <returns>
///   <c>true</c> if the device/id is null; otherwise, <c>false</c>.
/// </returns>
bool Channel::IsNullDevice() const
{
	return (InputDevice::NULL == INPUT_DEVICE) || (InputIdentifier::NULL == INPUT_DEVICE_ID);		
}

/// <summary>
/// Determines whether [is null code].
/// </summary>
/// <returns>
///   <c>true</c> if [is null code]; otherwise, <c>false</c>.
/// </returns>
bool Channel::IsNullCode() const
{
	return (InputCode::NULL == INPUT_CODE);
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