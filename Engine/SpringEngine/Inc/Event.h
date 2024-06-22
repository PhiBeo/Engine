#pragma once

namespace SpringEngine
{
	enum class EventType
	{
		None,
		AnimEvent,
		SpacePressed
	};

	class Event
	{
	public:
		Event() = default;
		Event(EventType et) : eventType(et) {}
		virtual ~Event() = default;

		EventType GetType() const { return eventType; }

	private:
		EventType eventType = EventType::None;
	};

	class AnimationEvent : public Event
	{
	public :
		AnimationEvent() : Event(EventType::AnimEvent) {}
		~AnimationEvent() = default;

		Graphics::ModelId modelId = 0;
		std::string eventName = "";
	};

	class SpacePressedEvent : public Event
	{
	public:
		SpacePressedEvent() : Event(EventType::SpacePressed) {}
		~SpacePressedEvent() = default;
	};
}