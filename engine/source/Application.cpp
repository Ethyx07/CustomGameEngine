#include "Application.h"


namespace eng
{
	void Application::RegisterTypes()
	{
	}

	void Application::SetNeedsToClose(bool value)
	{
		bNeedsToBeClosed = true;
	}

	bool Application::GetNeedsToBeClosed() const
	{
		return bNeedsToBeClosed;
	}
}