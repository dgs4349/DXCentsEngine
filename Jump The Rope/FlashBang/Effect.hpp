#pragma once

#include "FlashBang.hpp"
using namespace FlashBang;

// todo expand
enum COMMON_EFFECTS { TIME };

class Effect
{
public:

	float ValueMin;
	float ValueMax;
	float (*ValueFunctionPointer)(float val) = nullptr;

	float ActiveTime = 0.0f;
	
	float Value() { return value_; }
	float Value(float v) { value_ = v; return value_; }

	struct Connection { float* PointerToControl; float ControlMin; float ControlMax; };
	
	Effect() = default;
	
	Effect(float valueMin, float valueMax, bool active = true)
	{
		ValueMin = valueMin;
		ValueMax = valueMax;
		ValueFunctionPointer = noop_;
		
		active_ = active;
	}
	
	Effect(float (*PointerToValueSetFunc)(float val), float valueMin, float valueMax, bool active = true)
	{
		ValueMin = valueMin;
		ValueMax = valueMax;
		ValueFunctionPointer = PointerToValueSetFunc;
		
		active_ = active;
	}

	~Effect() = default;

	void Update()
	{
		if(active_ && *connection_.PointerToControl != previousControlValue_)
		{
			value_ = (*ValueFunctionPointer)(GetMappedValue());
			previousControlValue_ = *connection_.PointerToControl;
		}
	}
	
	void Update(float dt) {

	}

	void Start() { active_ = true; }
	
	void Stop()
	{
		active_ = false;
		ActiveTime = 0.0f;
	}
	
	void Connect(Connection connection) { connection_ = connection; }
	void Connect(float* pointerToControllingValue, float controllingValueMin, float controllingValueMax)
	{
		connection_ = { pointerToControllingValue, controllingValueMin, controllingValueMax };
	}
	void Connect(COMMON_EFFECTS effect, float min, float max)
	{
		switch(effect)
		{
		case TIME:
			
			break;
		}
	}

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

	static float noop_(float val) { return val; }
	
	bool active_;

	Connection connection_ = { nullptr, 0.0f, 1.0f };
};