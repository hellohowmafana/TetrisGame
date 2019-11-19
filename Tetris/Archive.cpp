#include "Archive.h"
#include <fstream>
#include "Utility.h"
#include "Controller.h"

bool Archive::Save(wchar_t* szArchive, Controller* pController)
{
	wfstream fs;
	try
	{
		wstring strFile = Configuration::singleton.pathArchives;
		strFile.append(L"\\").append(szArchive);
		fs.open(strFile.c_str(), wfstream::out);
		wchar_t* szStr;
		GameFrame* pGameFrame = pController->GetGameFrame();
		if (pGameFrame->Save((wchar_t*)Archive::labelFrame.c_str(), &szStr))
			fs.write(szStr, wcslen(szStr));
		if (pGameFrame->GetShape()->Save((wchar_t*)Archive::labelCurrent.c_str(), &szStr))
			fs.write(szStr, wcslen(szStr));
		if (pGameFrame->GetNextShape()->Save((wchar_t*)Archive::labelNext.c_str(), &szStr))
			fs.write(szStr, wcslen(szStr));
		if (pGameFrame->GetMass()->Save((wchar_t*)Archive::labelMass.c_str(), &szStr))
			fs.write(szStr, wcslen(szStr));
		if (pGameFrame->Save((wchar_t*)Archive::labelScore.c_str(), &szStr))
			fs.write(szStr, wcslen(szStr));
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

bool Archive::Load(wchar_t* szArchive, Controller* pController)
{
	wfstream fs;
	try {
		wstring strFile = Configuration::singleton.pathArchives;
		strFile.append(L"\\").append(szArchive);
		fs.open(strFile.c_str(), wfstream::in);
		wstring str;
		bool massData = false;
		wstring strMassData;
		while (!fs.eof())
		{
			getline(fs, str);
			if (0 == str.find(labelFrame + labelMark))
			{
				pController->GetGameFrame()->Load(labelFrame.c_str(),
					(wchar_t*)str.substr(str.find(labelMark) + 1).c_str());
			}
			else if (0 == str.find(labelNext + labelMark))
			{
				pController->GetGameFrame()->Load(labelNext.c_str(),
					(wchar_t*)str.substr(str.find(labelMark) + 1).c_str());
			}
			else if(0 == str.find(labelCurrent + labelMark))
			{
				pController->GetGameFrame()->Load(labelCurrent.c_str(),
					(wchar_t*)str.substr(str.find(labelMark) + 1).c_str());
			}
			else if (0 == str.find(labelScore + labelMark))
			{
				pController->GetGameFrame()->Load(labelScore.c_str(),
					(wchar_t*)str.substr(str.find(labelMark) + 1).c_str());
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
					pController->GetGameFrame()->Load(labelMass.c_str(),
						(wchar_t*)strMassData.c_str());
				}
			}
			else if (massData)
			{
				strMassData += str;
				strMassData += L"\n";
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

const wstring Archive::labelFrame = L"frame";
const wstring Archive::labelNext = L"next";
const wstring Archive::labelCurrent = L"current";
const wstring Archive::labelMass = L"mass";
const wstring Archive::labelScore = L"score";
const wstring Archive::labelMark = L":";
