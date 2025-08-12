#include "dxd/keyboard.h"

namespace DXD
{
	void UKeyboard::Flush()
	{
		FlushKey();
		FlushChar();
	}

	void UKeyboard::FlushKey()
	{
		KeyBuffer = std::queue<UEvent>();
	}

	bool UKeyboard::IsKeyPressed(unsigned char KeyCode) const
	{
		return KeyStates[KeyCode];
	}

	bool UKeyboard::IsKeyEmpty() const
	{
		return KeyBuffer.empty();
	}

	std::optional<UKeyboard::UEvent> UKeyboard::ReadKey()
	{
		std::optional<UEvent> Event;
		if (!IsKeyEmpty())
		{
			Event = KeyBuffer.front();
			KeyBuffer.pop();
		}
		return Event;
	}

	std::optional<char> UKeyboard::ReadChar()
	{
		std::optional<char> Character;
		if (!IsCharEmpty())
		{
			Character = CharBuffer.front();
			CharBuffer.pop();
		}
		return Character;
	}

	void UKeyboard::FlushChar()
	{
		CharBuffer = std::queue<char>();
	}

	bool UKeyboard::IsCharEmpty() const
	{
		return CharBuffer.empty();
	}

	void UKeyboard::EnableAutoRepeat()
	{
		bIsAutoRepeatEnabled = true;
	}

	void UKeyboard::DisableAutoRepeat()
	{
		bIsAutoRepeatEnabled = false;
	}

	bool UKeyboard::IsAutoRepeatEnabled() const
	{
		return bIsAutoRepeatEnabled;
	}

	void UKeyboard::OnKeyPressed(unsigned char KeyCode)
	{
		KeyStates[KeyCode] = true;
		KeyBuffer.push(UEvent(UEvent::EEventType::PRESS, KeyCode));
		TrimBuffer(KeyBuffer);
	}

	void UKeyboard::OnKeyReleased(unsigned char KeyCode)
	{
		KeyStates[KeyCode] = false;
		KeyBuffer.push(UEvent(UEvent::EEventType::RELEASE, KeyCode));
		TrimBuffer(KeyBuffer);	
	}

	void UKeyboard::OnChar(char Character)
	{
		CharBuffer.push(Character);
		TrimBuffer(CharBuffer);
	}

	void UKeyboard::ClearState()
	{
		KeyStates.reset();
	}


} // namespace dxd