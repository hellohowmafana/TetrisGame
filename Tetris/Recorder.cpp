#include "Recorder.hpp"
#include "BinarySerializer.h"
#include "Controller.hpp"

ofstream& Recorder::getofstream()
{
	return *dynamic_cast<ofstream*>(pstream);
}

ifstream& Recorder::getifstream()
{
	return *dynamic_cast<ifstream*>(pstream);
}

Recorder Recorder::singleton;

bool Recorder::StartRecord(wstring file, Configuration* pConfiguration, Controller* pController)
{
	try
	{
		output = true;
		file = pConfiguration->pathRecords + L"\\" + file;
		pstream = new ofstream(file.c_str(), ios::binary | ios::trunc, ios::_Default_open_prot);
		ofstream& ofs = getofstream();
	
		ofs.write(head, strlen(head));
		ofs.write((char*)&version, sizeof(version));

		IBinarySerializable* binarySerializables[] =
		{
			pConfiguration,
			pController->GetGameFrame(),
		};

		for (size_t i = 0; i < 2; i++)
		{
			IBinarySerializable* pBinarySerialiable = binarySerializables[i];

			char* pData = nullptr;
			unsigned int size = 0;
			pBinarySerialiable->Save(pData, size);
			ofs.write((char*)&size, sizeof(size));

			pData = new char[size];
			size = 0;
			pBinarySerialiable->Save(pData, size);
			ofs.write(pData - size, size);
			delete[] (pData - size);
		}

		posTrackSize = ofs.tellp();
		unsigned int trackSize = 0;
		ofs.write((char*)&trackSize, sizeof(unsigned int));

		times = 1;
		lastTrackedState = TrackState::End;
	}
	catch (const std::exception&)
	{
		return false;
	}

	return true;
}

bool Recorder::EndRecord()
{
	if (!output)
		return false;
	try
	{
		ofstream& ofs = getofstream();
		ofs.write(tail, strlen(tail));
		ofs.close();
		
		delete pstream;
		pstream = nullptr;

		return true;
	}
	catch (const std::exception&)
	{
		return false;
	}
}

bool Recorder::Load(wstring file, Configuration* pConfiguration, Controller* pController)
{
	try
	{
		bool output = false;
		file = pConfiguration->pathRecords + L"\\" + file;
		pstream = new ifstream(file.c_str(), ios::binary, ios::_Default_open_prot);
		ifstream& ifs = getifstream();

		char* data;
		
		// head
		data = new char[strlen(head)];
		ifs.read(data, strlen(head));
		if (strncmp(head, data, strlen(head)) != 0)
			return false;
		delete[] data;

		// version
		unsigned short version;
		ifs.read((char*)&version, sizeof(unsigned short));

		IBinarySerializable* binarySerializables[] =
		{
			pConfiguration,
			pController->GetGameFrame(),
		};

		for (size_t i = 0; i < 2; i++)
		{
			IBinarySerializable* pBinarySerialiable = binarySerializables[i];

			unsigned int size;
			ifs.read((char*)&size, sizeof(size));
			
			char* pData = new char[size];
			ifs.read(pData, size);
			pBinarySerialiable->Load(pData);
			delete[] (pData - size);
		}
	}
	catch (const std::exception&)
	{
		return false;
	}
	return true;
}

bool Recorder::Unload()
{
	if (output)
		return false;
	try
	{
		ifstream& ifs = getifstream();
		ifs.close();

		delete pstream;
		pstream = nullptr;

		return true;
	}
	catch (const std::exception&)
	{
		return false;
	}
}

bool Recorder::Track(TrackState state)
{
	bool record = false;
	if (state == lastTrackedState)
	{
		if (times == 0xFF)
			record = true;
		else
			times++;
	}
	else
	{
		record = true;
	}

	if (record)
	{
		try
		{
			//ofstream& ofs = getofstream();
			//if (times == 1)
			//	ofs.write((char*)state, sizeof(state));
			//else
			//	ofs.write((char*))
		}
		catch (const std::exception&)
		{
			return false;
		}
	}

	lastTrackedState = state;
	return true;
}

bool Recorder::EndTrack()
{
	try
	{
		ofstream& ofs = getofstream();
		size_t size = ofs.tellp() - posTrackSize - sizeof(size_t);
		ofs.seekp(posTrackSize);
		ofs.write((char*)size, sizeof(size_t));
		return true;
	}
	catch (const std::exception&)
	{
		return false;
	}
}

