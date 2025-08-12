#pragma once

#include <bitset>
#include <optional>
#include <queue>
#include <vector>

namespace DXD
{

	class UKeyboard
	{
	public:
		friend class UWindow;

		class UEvent
		{
		public:
			enum class EEventType
			{
				PRESS,
				RELEASE
			};

			UEvent(EEventType EventType, unsigned char KeyCode) : EventType(EventType), KeyCode(KeyCode) {}

			bool IsPress() const
			{
				return EventType == EEventType::PRESS;
			}
			bool IsRelease() const
			{
				return EventType == EEventType::RELEASE;
			}
			unsigned char GetKeyCode() const
			{
				return KeyCode;
			}

		private:
			EEventType EventType;
			unsigned char KeyCode;
		};

		//----------------------------------------------------------------------------------------//
		~UKeyboard() = default;

		UKeyboard() = default;

		UKeyboard(const UKeyboard&)			  = delete;
		UKeyboard operator=(const UKeyboard&) = delete;

		UKeyboard(UKeyboard&&)			 = delete;
		UKeyboard operator=(UKeyboard&&) = delete;

		void Flush();

		void FlushKey();
		bool IsKeyPressed(unsigned char KeyCode) const;
		bool IsKeyEmpty() const;

		std::optional<UEvent> ReadKey();
		std::optional<char> ReadChar();
		void FlushChar();
		bool IsCharEmpty() const;

		void EnableAutoRepeat();
		void DisableAutoRepeat();
		bool IsAutoRepeatEnabled() const;
			
	private:
		template<typename TElement>
		static void TrimBuffer(std::queue<TElement>& Buffer);

		static constexpr unsigned int NUM_KEYS	  = 256u;
		static constexpr unsigned int BUFFER_SIZE = 16u;

		void OnKeyPressed(unsigned char KeyCode);
		void OnKeyReleased(unsigned char KeyCode);
		void OnChar(char Character);
		void ClearState();

		bool bIsAutoRepeatEnabled;

		std::bitset<NUM_KEYS> KeyStates;
		std::queue<UEvent> KeyBuffer;
		std::queue<char> CharBuffer;
	};

	template<typename TElement>
	inline void UKeyboard::TrimBuffer(std::queue<TElement>& Buffer)
	{
		while (Buffer.size() > BUFFER_SIZE)
		{
			Buffer.pop();
		}
	}

} // namespace dxd
