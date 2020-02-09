#pragma once

#include "FlashBang.hpp"
using namespace FlashBang;

class Effect
{
public:

	/*
		TODO:
			- Custom typing, for int and vector support
			- LFO and other non-pointer connected types
			- Custom external mapping functions
				- Curved/Sloped values for vertical mixing with indeces
	*/


	struct Connection {

		float* PointerToControl; float ControlMin; float ControlMax;

		Connection(float* pointerToControl, float controlMin, float controlMax) {
			PointerToControl = pointerToControl; ControlMin = controlMin; ControlMax = controlMax;
		}
	};

	float ValueMin;
	float ValueMax;
	float (*ValueFunctionPointer)(float val) = nullptr;

	float ActiveTime = 0.0f;
	
	float Value() { return value_; }
	float Value(float v) { value_ = v; return value_; }

	
	Effect() = default;
	
	Effect(float valueMin, float valueMax)
	{
		ValueMin = valueMin;
		ValueMax = valueMax;
		ValueFunctionPointer = noop_;
	}
	
	Effect(float (*PointerToValueSetFunc)(float val), float valueMin, float valueMax)
	{
		ValueMin = valueMin;
		ValueMax = valueMax;
		ValueFunctionPointer = PointerToValueSetFunc;
	}

	Effect(float (*PointerToValueSetFunc)(float val), float valueMin, float valueMax, Connection connection)
	{
		ValueMin = valueMin;
		ValueMax = valueMax;
		ValueFunctionPointer = PointerToValueSetFunc;

		Connect(connection);
	}

	~Effect() = default;

	Effect* Connect(Connection connection, bool start = true)
	{
		connection_ = connection;

		active_ = start;
		return this;
	}

	Effect* Connect(float* pointerToControllingValue, float controllingValueMin, 
		float controllingValueMax, bool start = true)
	{
		connection_ = { pointerToControllingValue, controllingValueMin, controllingValueMax };
		active_ = start;
		return this;
	}

	Effect* Connect(COMMON_EFFECTS effect, float from, float to, bool start = true)
	{
		switch (effect)
		{
		case COMMON_EFFECTS::TIME:
			Connect({ &ActiveTime, from, to });
			break;
		}
		active_ = start;
		return this;
	}

	void Update(float dt) {
		ActiveTime += dt;

		if (active_ && *connection_.PointerToControl != previousControlValue_)
		{
			value_ = (*ValueFunctionPointer)(GetMappedValue());
			previousControlValue_ = *connection_.PointerToControl;
		}
	}
	
	bool Active() { return active_; }
	bool Active(bool val) { active_ = val; return active_; }
	
	void Start() { active_ = true; }
	void Stop() { active_ = false; ActiveTime = 0.0f;}

	/*
	 * forces value within it's original range, then converts to a new range
	 */
	static float MapRange(float value, float valueMin, float valueMax, float toRangeMin, float toRangeMax)
	{
		if (value < valueMin) value = valueMin;
		if (value > valueMax) value = valueMax;
		return toRangeMin + ((toRangeMax - toRangeMin) / (valueMax - valueMin) * (value - valueMin));
	}

	float GetMappedValue()
	{
		// map connection params to this value params
		return MapRange(
			*connection_.PointerToControl,
			connection_.ControlMin,
			connection_.ControlMax,
			ValueMin,
			ValueMax
		);
	}

	// future plans, will be like a connection with a custom function, or pre-
	//   declared functions to adjust like Time, but oscillating
	/*void LFO();
	enum LFO_TYPES{};*/

private:

	float value_ = 0.0f;
	float previousControlValue_ = 0.0f;
	
	// default is inactive until a connection is made
	bool active_ = false;

	Connection connection_ = { nullptr, 0.0f, 1.0f };

	static float noop_(float val) { return val; }
};