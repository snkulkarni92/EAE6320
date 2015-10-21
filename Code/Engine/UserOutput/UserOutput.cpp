// Header Files

#include "UserOutput.h"

#include "../Windows/Includes.h"
namespace eae6320
{
	namespace UserOutput
	{
		void Print(std::string i_pMsg)
		{
			OutputDebugString(i_pMsg.c_str());
			MessageBox(NULL, i_pMsg.c_str(), NULL, MB_OK);
		}
	}
}