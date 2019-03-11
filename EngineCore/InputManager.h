#pragma once
#include <d3d11.h>
#include <Windows.h>
#include "Singleton.h"


class InputManager : public Singleton<InputManager>
{
	friend class Singleton<InputManager>;
public:

	static LONG_PTR WindowProc(HWND hWnd, unsigned int uMsg, WPARAM wParam, LPARAM lParam);
	LONG_PTR ProcessInput(HWND hWnd, unsigned int uMsg, WPARAM wParam, LPARAM lParam);

	virtual void OnMouseDown(WPARAM buttonState, int x, int y) {}
	virtual void OnMouseUp(WPARAM buttonState, int x, int y) {}
	virtual void OnMouseMove(WPARAM buttonState, int x, int y) {}
	virtual void OnMouseWheel(float wheelDelta, int x, int y) {}

private:
	InputManager();
	~InputManager();
};

