#include "Application.h"


namespace engine
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