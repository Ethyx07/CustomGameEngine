#pragma once


namespace engine 
{
	class Application
	{
	public:
		virtual bool Init() = 0;
		//deltaTime is in seconds
		virtual void Update(float deltaTime) = 0;
		virtual void Destroy() = 0;

		void SetNeedsToClose(bool value);
		bool GetNeedsToBeClosed() const;

		bool bNeedsToBeClosed = false;
	};
}