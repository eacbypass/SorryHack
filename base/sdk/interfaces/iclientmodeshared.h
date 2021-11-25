#pragma once
// used: angle
#include "../datatypes/qangle.h"

#pragma region clientmode_definitions
#define SIGNONSTATE_NONE		0	// no state yet, about to connect
#define SIGNONSTATE_CHALLENGE	1	// client challenging server, all OOB packets
#define SIGNONSTATE_CONNECTED	2	// client is connected to server, netchans ready
#define SIGNONSTATE_NEW			3	// just got serverinfo and string tables
#define SIGNONSTATE_PRESPAWN	4	// received signon buffers
#define SIGNONSTATE_SPAWN		5	// ready to receive entity packets
#define SIGNONSTATE_FULL		6	// we are fully connected, first non-delta packet received (in-game check)
#define SIGNONSTATE_CHANGELEVEL	7	// server is changing level, please wait
#pragma endregion

class CViewSetup
{
public:
	int			iX;
	int			iUnscaledX;
	int			iY;
	int			iUnscaledY;
	int			iWidth;
	int			iUnscaledWidth;
	int			iHeight;
	int			iUnscaledHeight;
	bool		bOrtho;
	float		flOrthoLeft;
	float		flOrthoTop;
	float		flOrthoRight;
	float		flOrthoBottom;
	std::byte	pad0[0x7C];
	float		flFOV;
	float		flViewModelFOV;
	Vector		vecOrigin;
	QAngle		angView;
	float		flNearZ;
	float		flFarZ;
	float		flNearViewmodelZ;
	float		flFarViewmodelZ;
	float		flAspectRatio;
	float		flNearBlurDepth;
	float		flNearFocusDepth;
	float		flFarFocusDepth;
	float		flFarBlurDepth;
	float		flNearBlurRadius;
	float		flFarBlurRadius;
	float		flDoFQuality;
	int			nMotionBlurMode;
	float		flShutterTime;
	Vector		vecShutterOpenPosition;
	QAngle		vecShutterOpenAngles;
	Vector		vecShutterClosePosition;
	QAngle		vecShutterCloseAngles;
	float		flOffCenterTop;
	float		flOffCenterBottom;
	float		flOffCenterLeft;
	float		flOffCenterRight;
	bool		bOffCenter : 1;
	bool		bRenderToSubrectOfLargerScreen : 1;
	bool		bDoBloomAndToneMapping : 1;
	bool		bDoDepthOfField : 1;
	bool		bHDRTarget : 1;
	bool		bDrawWorldNormal : 1;
	bool		bCullFontFaces : 1;
	bool		bCacheFullSceneState : 1;
	bool		bCSMView : 1;
};

class HudChat {
public:

	template <typename... Args>
	void printf(int filter, const char* fmt, Args... args) noexcept
	{
		(*reinterpret_cast<void(__cdecl***)(void*, int, const char*, ...)>(this))[26](this, filter, fmt, args...);
	}
};


class IHudChat;
class IClientModeShared
{
public:
	std::byte	pad0[0x1B];
	void*		pViewport;
	IHudChat*	pChatElement;
	HCursor		hCursorNone;
	void*		pWeaponSelection;
	int			nRootSize[2];

	auto getHudChat() noexcept
	{
		return *reinterpret_cast<HudChat**>(std::uintptr_t(this) + 28);
	}
};

class IAppSystem
{
private:
	virtual void function0() = 0;
	virtual void function1() = 0;
	virtual void function2() = 0;
	virtual void function3() = 0;
	virtual void function4() = 0;
	virtual void function5() = 0;
	virtual void function6() = 0;
	virtual void function7() = 0;
	virtual void function8() = 0;
};

class CEventInfo
{
public:
	short				sClassID;		// 0x00 // 0 implies not in use
	float				flFireDelay;	// 0x02 // if non-zero, the delay time when the event should be fired ( fixed up on the client )
	const void*			pSendTable;		// 0x06
	const CBaseClient*	pClientClass;	// 0x0A // clienclass pointer
	void*				pData;			// 0x0E // raw event data
	std::intptr_t		iPackedBits;	// 0x12
	int					iFlags;			// 0x16
	std::byte			pad0[0x16];		// 0x1A
}; // Size: 0x30

class INetChannel;
class IClientState
{
public:
	std::byte		pad0[0x9C];				// 0x0000
	INetChannel*	pNetChannel;			// 0x009C
	int				iChallengeNr;			// 0x00A0
	std::byte		pad1[0x64];				// 0x00A4
	int				iSignonState;			// 0x0108
	std::byte		pad2[0x8];				// 0x010C
	float			flNextCmdTime;			// 0x0114
	uint32_t		nServerCount;			// 0x0118
	int				iCurrentSequence;		// 0x011C
	std::byte		pad3[0x54];				// 0x0120
	int				iDeltaTick;				// 0x0174
	bool			bPaused;				// 0x0178
	std::byte		pad4[0x7];				// 0x0179
	int				iViewEntity;			// 0x0180
	int				iPlayerSlot;			// 0x0184
	char			szLevelName[MAX_PATH];	// 0x0188
	char			szLevelNameShort[80];	// 0x028C
	char			szMapGroupName[80];		// 0x02DC
	char			szLastLevelNameShort[80]; // 0x032C
	std::byte		pad5[0xC];				// 0x037C
	int				nMaxClients;			// 0x0388 
	std::byte		pad6[0x498C];			// 0x038C
	float			flLastServerTickTime;	// 0x4D18
	bool			bInSimulation;			// 0x4D1C
	std::byte		pad7[0x3];				// 0x4D1D
	int				iOldTickcount;			// 0x4D20
	float			flTickRemainder;		// 0x4D24
	float			flFrameTime;			// 0x4D28
	int				iLastOutgoingCommand;	// 0x4D2C
	int				nChokedCommands;		// 0x4D30
	int				iLastCommandAck;		// 0x4D34
	int				iCommandAck;			// 0x4D38
	int				iSoundSequence;			// 0x4D3C
	std::byte		pad8[0x50];				// 0x4D40
	QAngle			angViewPoint;			// 0x4D90
	std::byte		pad9[0xD0];				// 0x4D9C
	CEventInfo*		pEvents;				// 0x4E6C
}; // Size: 0x4E70
