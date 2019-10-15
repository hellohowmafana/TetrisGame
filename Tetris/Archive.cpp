#include "Archive.h"
#include "tcharstd.h"
#include <fstream>
#include "Utility.h"
#include "Controller.h"

bool Archive::Save(TCHAR* szFile, Controller* pController)
{
	return false;
}

bool Archive::Load(TCHAR* szFile, Controller* pController)
{
	tfstream fs;
	try {
		fs.open(szFile, tfstream::in);
		tstring str;
		bool massData = false;
		tstring strMassData;
		while (!fs.eof())
		{
			getline(fs, str);
			int pos;
			if (massData)
			{
				strMassData += str;
			}
			else if (tstring::npos != (pos = str.find(labelFrame)))
			{
				pController->GetGameFrame()->Load((TCHAR*)str.c_str());
			}
			else if (tstring::npos != (pos = str.find(labelNext)))
			{
				pController->GetNextTetrisShape()->Load((TCHAR*)str.c_str());
			}
			else if(tstring::npos != (pos = str.find(labelCurrent)))
			{
				pController->GetTetrisShape()->Load((TCHAR*)str.c_str());
			}
			else if (tstring::npos != (pos = str.find(labelScore)))
			{
				pController->GetGameFrame()->Load((TCHAR*)str.c_str());
			}
			else if (tstring::npos != (pos = str.find(labelMass)))
			{
				massData = true;
				strMassData += str;
			}
			else if (str.empty())
			{
				if (massData)
				{
					massData = false;
					pController->GetMass()->Load((TCHAR*)strMassData.c_str());
				}
			}
		}
	}
	catch (...)
	{
	}

	if (fs.is_open())
		fs.close();

	return true;
}

const tstring Archive::labelFrame = _T("Frame");
const tstring Archive::labelNext = _T("next");
const tstring Archive::labelCurrent = _T("current");
const tstring Archive::labelMass = _T("mass");
const tstring Archive::labelScore = _T("score");