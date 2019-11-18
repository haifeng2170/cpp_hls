#pragma once
#include <map>
#include <queue>
#include "RtspClient/RtpUnpacket.h"
#include "TsPacker.h"

class RtspClient;
class M3U8Client;

class StreamDistribution
{
public:
	StreamDistribution(const std::string& id, std::string& url);
	~StreamDistribution();

	int Run();

	void AddClient(const std::string& id, M3U8Client* cli);
	M3U8Client* GetClient(const std::string& id);
	size_t GetClientCount();
private:
	static unsigned rawcb(FrameInfo& f, void* arg);
	unsigned wrapRawCB(FrameInfo& f);

	static unsigned __stdcall StaticPacket(void* arg);
	unsigned WrapPacket();

	static unsigned tscb(TsFileInfo& f, void *arg);
	unsigned wrapTSCB(TsFileInfo& f);

	static unsigned __stdcall StaticCheck(void* arg);
	unsigned WrapCheck();
private:
	RtspClient* pstream;
	std::string m_streamID;
	std::string m_uri;
private:
	TsPacker* pPacker;
	std::wstring m_dir;
private:
	HANDLE m_checkThr;
	HANDLE m_checkEvent;
private:
	CRITICAL_SECTION m_clientLock;
	std::map<std::string, M3U8Client*> m_clients;
	HANDLE m_streamSourceThr;

	CRITICAL_SECTION m_frameLock;
	CONDITION_VARIABLE m_frameCondition;
	std::queue<FrameInfo> m_frames;

	std::queue<std::wstring> m_tsFiles;

	char items[3][64];
	unsigned int m_index;
	std::string m_header;
};
