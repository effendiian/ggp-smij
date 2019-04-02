#pragma once
#include "Input.h"

// Access the Input namespace.
using namespace Input;

///////////////////////////
// INTERFACES
///////////////////////////

#pragma region InputValue

//////////////////
// Constructors.

// Construct the input value object with known current and past values.
InputValue::InputValue(float currentValue, float previousValue) 
{
	this->SetCurrentValue(currentValue);
	this->SetPreviousValue(previousValue);
	this->CalculateDelta();
}

// Construct the input value object. Both current and previous values are set to the same value.
InputValue::InputValue(float currentValue) : InputValue(currentValue, currentValue) {}

//////////////////
// Interface Methods.

// Hard set the current value. No side effects.
void InputValue::SetCurrentValue(float value) 
{
	this->_currentValue = value;
}

// Hard set the previous value. No side effects.
void InputValue::SetPreviousValue(float value) 
{
	this->_previousValue = value;
}

// Hard set the delta value. No side effects.
void InputValue::SetDeltaValue(float value) 
{
	this->_deltaValue = value;
}

// Updates the current, previous, and delta values.
void InputValue::Update(float value)
{
	this->SetPreviousValue(this->Current);
	this->SetCurrentValue(value);
	this->CalculateDelta();
}

// Updates and sets the delta value based on the current values.
void InputValue::CalculateDelta() 
{
	float difference = this->Current - this->Previous;
	this->SetDeltaValue(difference);
}

#pragma endregion

#pragma region CommandValue

// Default, empty value.
CommandValue::CommandValue() : CommandValue(0.0f) {}

// Set current value to start with.
CommandValue::CommandValue(float currentValue) : CommandValue(currentValue, 0.0f) {}

// Set the current value and threshold to start with.
CommandValue::CommandValue(float currentValue, float threshold) : InputValue(currentValue), _threshold(threshold) {}

// Set the current and prevous value and threshold to start with.
CommandValue::CommandValue(float currentValue, float previousValue, float threshold) : InputValue(currentValue, previousValue), _threshold(threshold) {}

// Treat any value below threshold as 'zero'.
void CommandValue::Update(float value) 
{
	float adjustedValue = value;
	if (value < this->Threshold) {
		adjustedValue = 0.0f;
	}
	this->InputValue::Update(adjustedValue);
}

// Hard set the threshold value.
void CommandValue::SetThreshold(float threshold) 
{
	this->_threshold = threshold;
}

#pragma endregion

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
	this->Enable = _state ? 1 : 0;
}

/// <summary>
/// Enables this instance.
/// </summary>
void IEnableState::Enable()
{
	this->Enable = true;
}

/// <summary>
/// Disables this instance.
/// </summary>
void IEnableState::Disable() 
{
	this->Enable = false;
}

/// <summary>
/// Toggles the enabled state of the instance.
/// </summary>
void IEnableState::ToggleEnabledState()
{
	this->Enable = !Enable;
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
Channel::Channel(InputIdentifier inputDeviceId, InputDevice inputDeviceType, InputCode inputCode)
	: INPUT_DEVICE_ID( inputDeviceId ), INPUT_DEVICE( inputDeviceType ), INPUT_CODE( inputCode )
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
Channel::Channel(InputIdentifier inputDeviceId, InputDevice inputDeviceType, InputCode inputCode)
	: INPUT_DEVICE_ID( inputDeviceId ), INPUT_DEVICE( inputDeviceType ), INPUT_CODE( inputCode )
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

#pragma region Command

// --------------------------------------------------------
// interface Command
//
// Commands track current values for inputs based on
// device channels.
// --------------------------------------------------------

Command::Command(InputType inputType, float threshold, bool normalized) 
	: IEnableState(true), _threshold(threshold), Type(inputType), Normalize(normalized)
{
	_previousValue = 0;
	_currentValue = 0;
	_deltaValue = 0;
}

Command::Command() : Command(InputType::NULL, 0.0f, true) {

}

#pragma endregion

