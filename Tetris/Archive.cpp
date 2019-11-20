#include "Archive.h"
#include <fstream>
#include "Utility.h"
#include "Controller.h"

bool Archive::Exist(wstring archive)
{
	wstring strFile = Configuration::singleton.pathArchives;
	strFile.append(L"\\").append(archive); 
	return (bool)wifstream(strFile);
}

bool Archive::Save(wstring archive, Controller* pController)
{
	wfstream fs;
	try
	{
		wstring strFile = Configuration::singleton.pathArchives;
		strFile.append(L"\\").append(archive);
		fs.open(strFile.c_str(), wfstream::out);
		wstring str;
		GameFrame* pGameFrame = pController->GetGameFrame();
		if (pGameFrame->Save(labelFrame, str))
		{
			str = labelFrame + labelMark + str + L"\n";
			fs.write(str.c_str(), str.length());
		}
		if (pGameFrame->Save(labelNext, str))
		{
			str = labelNext + labelMark + str + L"\n";
			fs.write(str.c_str(), str.length());
		}
		if (pGameFrame->Save(labelCurrent, str))
		{
			str = labelCurrent + labelMark + str + L"\n";
			fs.write(str.c_str(), str.length());
		}
		if (pGameFrame->Save(labelMass, str))
		{
			str = labelMass + labelMark + str + L"\n";
			fs.write(str.c_str(), str.length());
		}
		if (pGameFrame->Save(labelScore, str))
		{
			str = labelScore + labelMark + str + L"\n";
			fs.write(str.c_str(), str.length());
		}
		if (pGameFrame->Save(labelStartLevel, str))
		{
			str = labelStartLevel + labelMark + str + L"\n";
			fs.write(str.c_str(), str.length());
		}
		if (pGameFrame->Save(labelStartLine, str))
		{
			str = labelStartLine + labelMark + str + L"\n";
			fs.write(str.c_str(), str.length());
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

bool Archive::Load(wstring archive, Controller* pController)
{
	wfstream fs;
	try {
		wstring strFile = Configuration::singleton.pathArchives;
		strFile.append(L"\\").append(archive);
		fs.open(strFile.c_str(), wfstream::in);
		wstring str;
		bool massData = false;
		wstring strMassData;
		while (!fs.eof())
		{
			getline(fs, str);
			if (0 == str.find(labelFrame + labelMark))
			{
				pController->GetGameFrame()->Load(labelFrame,
					str.substr(str.find(labelMark) + 1));
			}
			else if (0 == str.find(labelNext + labelMark))
			{
				pController->GetGameFrame()->Load(labelNext,
					str.substr(str.find(labelMark) + 1));
			}
			else if(0 == str.find(labelCurrent + labelMark))
			{
				pController->GetGameFrame()->Load(labelCurrent,
					str.substr(str.find(labelMark) + 1));
			}
			else if (0 == str.find(labelScore + labelMark))
			{
				pController->GetGameFrame()->Load(labelScore,
					str.substr(str.find(labelMark) + 1));
			}
			else if (0 == str.find(labelStartLevel + labelMark))
			{
				pController->GetGameFrame()->Load(labelStartLevel,
					str.substr(str.find(labelMark) + 1));
			}
			else if (0 == str.find(labelStartLine + labelMark))
			{
				pController->GetGameFrame()->Load(labelStartLine,
					str.substr(str.find(labelMark) + 1));
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
					pController->GetGameFrame()->Load(labelMass,
						strMassData);
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
const wstring Archive::labelStartLevel = L"start level";
const wstring Archive::labelStartLine = L"start line";
const wstring Archive::labelMark = L":";
