#include "Application.h"


namespace eng
{
	void Application::SetNeedsToClose(bool value)
	{
		bNeedsToBeClosed = true;
	}

	bool Application::GetNeedsToBeClosed() const
	{
		return bNeedsToBeClosed;
	}
}