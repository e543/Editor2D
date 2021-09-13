#pragma once
#include <queue>
#include <optional>

class Keyboard
{
	friend class RenderWindow;
public:
	class Event
	{
	public:
		enum class Type
		{
			Press,
			Release,
		};
	private:
		Type type;
		unsigned char code;
	public:
		Event(Type type, unsigned char code)
			:
			type(type),
			code(code)
		{}
		bool IsPress() const
		{
			return type == Type::Press;
		}
		bool IsRelease() const
		{
			return type == Type::Release;
		}
		unsigned char GetCode() const
		{
			return code;
		}
	};
public:
	Keyboard() = default;
	std::optional<Event> ReadKey();
	std::optional<char> ReadChar();
private:
	void OnKeyPressed(unsigned char keycode);
	void OnKeyReleased(unsigned char keycode);
	void OnChar(char character);
	template<typename T>
	static void TrimBuffer(std::queue<T>& buffer);
	static constexpr unsigned int nKeys = 256u;
	static constexpr unsigned int bufferSize = 16u;
	std::queue<Event> keybuffer;
	std::queue<char> charbuffer;
};