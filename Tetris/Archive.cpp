#include "Archive.h"
#include "tcharstd.h"
#include <fstream>
#include "Utility.h"
#include "Controller.h"

bool Archive::Save(TCHAR* szArchive, Controller* pController)
{
	tfstream fs;
	try
	{
		tstring strFile = Configuration::singleton.pathArchives;
		strFile.append(_T("\\")).append(szArchive);
		fs.open(strFile.c_str(), tfstream::out);
		TCHAR* szStr;
		GameFrame* pGameFrame = pController->GetGameFrame();
		if (pGameFrame->Save((TCHAR*)Archive::labelFrame.c_str(), &szStr))
			fs.write(szStr, _tcslen(szStr));
		if (pGameFrame->GetShape()->Save((TCHAR*)Archive::labelCurrent.c_str(), &szStr))
			fs.write(szStr, _tcslen(szStr));
		if (pGameFrame->GetNextShape()->Save((TCHAR*)Archive::labelNext.c_str(), &szStr))
			fs.write(szStr, _tcslen(szStr));
		if (pGameFrame->GetMass()->Save((TCHAR*)Archive::labelMass.c_str(), &szStr))
			fs.write(szStr, _tcslen(szStr));
		if (pGameFrame->Save((TCHAR*)Archive::labelScore.c_str(), &szStr))
			fs.write(szStr, _tcslen(szStr));
	}
	catch (const std::exception&)
	{
		if (fs.is_open())
			fs.close();
		return false;
	}

	if (fs.is_open())
		fs.close();
	return true;
}

bool Archive::Load(TCHAR* szArchive, Controller* pController)
{
	tfstream fs;
	try {
		tstring strFile = Configuration::singleton.pathArchives;
		strFile.append(_T("\\")).append(szArchive);
		fs.open(strFile.c_str(), tfstream::in);
		tstring str;
		bool massData = false;
		tstring strMassData;
		while (!fs.eof())
		{
			getline(fs, str);
			if (0 == str.find(labelFrame + labelMark))
			{
				pController->GetGameFrame()->Load(labelFrame.c_str(),
					(TCHAR*)str.substr(str.find(labelMark) + 1).c_str());
			}
			else if (0 == str.find(labelNext + labelMark))
			{
				pController->GetNextTetrisShape()->Load(labelNext.c_str(),
					(TCHAR*)str.substr(str.find(labelMark) + 1).c_str());
			}
			else if(0 == str.find(labelCurrent + labelMark))
			{
				pController->GetTetrisShape()->Load(labelCurrent.c_str(),
					(TCHAR*)str.substr(str.find(labelMark) + 1).c_str());
			}
			else if (0 == str.find(labelScore + labelMark))
			{
				pController->GetGameFrame()->Load(labelScore.c_str(),
					(TCHAR*)str.substr(str.find(labelMark) + 1).c_str());
			}
			else if (0 == str.find(labelMass + labelMark))
			{
				massData = true;
			}
			else if (str.empty())
			{
				if (massData)
				{
					massData = false;
					strMassData.erase(strMassData.end() - 1);
					pController->GetMass()->Load(labelMass.c_str(),
						(TCHAR*)strMassData.c_str());
				}
			}
			else if (massData)
			{
				strMassData += str;
				strMassData += _T("\n");
			}
		}
	}
	catch (const std::exception&)
	{
		if (fs.is_open())
			fs.close();
		return false;
	}

	if (fs.is_open())
		fs.close();
	return true;
}

const tstring Archive::labelFrame = _T("frame");
const tstring Archive::labelNext = _T("next");
const tstring Archive::labelCurrent = _T("current");
const tstring Archive::labelMass = _T("mass");
const tstring Archive::labelScore = _T("score");
const tstring Archive::labelMark = _T(":");