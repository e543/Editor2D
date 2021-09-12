#pragma once
#include <queue>
#include <optional>

class Mouse
{
	friend class RenderWindow;
public:
	class Event
	{
	public:
		enum class Type
		{
			LPress,
			LRelease,
			RPress,
			RRelease,
			WheelUp,
			WheelDown,
			Move,
			Enter,
			Leave,
		};
	private:
		Type type;
		bool leftIsPressed;
		bool rightIsPressed;
		int x;
		int y;
	public:
		Event(Type type, const Mouse& parent)
			:
			type(type),
			leftIsPressed(parent.leftIsPressed),
			rightIsPressed(parent.rightIsPressed),
			x(parent.x),
			y(parent.y)
		{}
		Type GetType() const
		{
			return type;
		}
		std::pair<int, int> GetPos() const
		{
			return{ x,y };
		}
	};
private:
	int x, y;

	bool leftIsPressed = false;
	bool rightIsPressed = false;
	static constexpr unsigned int bufferSize = 16u;

	std::queue<Event> buffer;
	void OnMouseMove(int x, int y);
	void OnLeftPressed();
	void OnLeftReleased();
	void OnRightPressed();
	void OnRightReleased();
	void TrimBuffer();
public:
	std::optional<Mouse::Event> Read();
	bool LeftIsPressed() const;
	bool RightIsPressed() const;
};
