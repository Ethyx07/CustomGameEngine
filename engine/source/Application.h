#pragma once


namespace eng
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