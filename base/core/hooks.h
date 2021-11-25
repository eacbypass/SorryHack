#pragma once
// used: winapi, directx, fmt includes
#include "../common.h"
// used: hook setup/destroy
#include "../utilities/detourhook.h"
// used: recvprop hook setup/destroy, recvproxydata
#include "netvar.h"
// used: baseclasses
#include "interfaces.h"
#include "../../Headers/netmessages.pb.h"

#ifndef FASTCALL
#define FASTCALL __fastcall
#endif

/*
* VTABLE INDEXES
* functions indexes in their virtual tables
*/
namespace VTABLE
{
	// work with namespace cuz if use enum class need additional convert to int
	enum
	{
		/* directx table */
		RESET = 16,
		ENDSCENE = 42,
		RESETEX = 132,

		/* client table */
		FRAMESTAGENOTIFY = 37,

		/* panel table */
		PAINTTRAVERSE = 41,

		/* clientmode table */
		OVERRIDEVIEW = 18,
		OVERRIDEMOUSEINPUT = 23,
		CREATEMOVE = 24,
		GETVIEWMODELFOV = 35,
		DOPOSTSCREENEFFECTS = 44,

		/* modelrender table */
		DRAWMODELEXECUTE = 21,

		/* studiorender table */
		DRAWMODEL = 29,

		/* viewrender table */
		RENDERSMOKEOVERLAY = 41,

		/* engine table */
		ISCONNECTED = 27,

		/* bsp query table */
		LISTLEAVESINBOX = 6,

		/* prediction table */
		RUNCOMMAND = 19,

		/* steamgamecoordinator table */
		SENDMESSAGE = 0,
		RETRIEVEMESSAGE = 2,

		/* sound table */
		EMITSOUND = 5,

		/* materialsystem table */
		OVERRIDECONFIG = 21,

		/* renderview table */
		SCENEEND = 9,

		/* surface table */
		LOCKCURSOR = 67,
		PLAYSOUND = 82,

		/* gameevent table */
		FIREEVENT = 9,

		/* convar table */
		GETBOOL = 13,
		GETINT = 13,

		/* netchannel table */
		SENDNETMSG = 40,
		SENDDATAGRAM = 46
	};
}

/*
 * DETOURS
 * detour hook managers
 */
namespace DTR
{
	inline CDetourHook Reset;
	inline CDetourHook EndScene;
	inline CDetourHook FrameStageNotify;
	inline CDetourHook OverrideView;
	inline CDetourHook OverrideMouseInput;
	inline CDetourHook CreateMove;
	inline CDetourHook NextThinkTime;

	inline CDetourHook SendNetMsg;
	inline CDetourHook ClientCmd;
	inline CDetourHook NET_SendLong;
	inline CDetourHook ClMove;
	inline CDetourHook TimingOut;
	inline CDetourHook ProcessPacketEntities;
	inline CDetourHook TimeSinceLastPacket;

	inline CDetourHook SendDatagram;
	inline CDetourHook GetViewModelFOV;
	inline CDetourHook DoPostScreenEffects;
	inline CDetourHook IsConnected;
	inline CDetourHook RenderSmokeOverlay;
	inline CDetourHook ListLeavesInBox;
	inline CDetourHook PaintTraverse;
	inline CDetourHook DrawModel;
	inline CDetourHook RunCommand;
	inline CDetourHook SendMessageGC;
	inline CDetourHook RetrieveMessage;
	inline CDetourHook LockCursor;
	inline CDetourHook PlaySoundSurface;
	inline CDetourHook SvCheatsGetBool;
	inline CDetourHook NetGraphGetInt;
}

/*
 * HOOKS
 * swap functions with given pointers
 */
namespace H
{
	// Get
	bool	Setup();
	void	Restore();

	// Handlers
	/* [type][call]		hk[name] (args...) */
	long	D3DAPI		hkReset(IDirect3DDevice9* pDevice, D3DPRESENT_PARAMETERS* pPresentationParameters);
	long	D3DAPI		hkEndScene(IDirect3DDevice9* pDevice);
	bool	FASTCALL	hkCreateMove(IClientModeShared* thisptr, int edx, float flInputSampleTime, CUserCmd* pCmd);
	void	FASTCALL	hkPaintTraverse(ISurface* thisptr, int edx, unsigned int uPanel, bool bForceRepaint, bool bForce);
	void	FASTCALL	hkPlaySound(ISurface* thisptr, int edx, const char* szFileName);
	void	FASTCALL	hkLockCursor(ISurface* thisptr, int edx);
	void __fastcall hkClientCmd(void*, void*, const char* szCmdString);
	void	FASTCALL	hkFrameStageNotify(IBaseClientDll* thisptr, int edx, EClientFrameStage stage);
	void	FASTCALL	hkDrawModel(IStudioRender* thisptr, int edx, DrawModelResults_t* pResults, const DrawModelInfo_t& info, matrix3x4_t* pBoneToWorld, float* flFlexWeights, float* flFlexDelayedWeights, const Vector& vecModelOrigin, int nFlags);
	void	FASTCALL	hkRenderSmokeOverlay(IViewRender* thisptr, int edx, bool bPreViewModel);
	int		FASTCALL	hkListLeavesInBox(void* thisptr, int edx, const Vector& vecMins, const Vector& vecMaxs, unsigned short* puList, int nListMax);
	bool	FASTCALL	hkIsConnected(IEngineClient* thisptr, int edx);
	bool __fastcall hkProcessPacketEntities(DWORD* ecx);
	//bool	FASTCALL	hkProcessPacketEntities(void* ecx, void* edx, CSVCMsg_PacketEntities& efx);
	bool	FASTCALL	hkSendNetMsg(INetChannel* thisptr, int edx, INetMessage* pMessage, bool bForceReliable, bool bVoice);
	unsigned int __stdcall CheckerThread(void* pParam);
	unsigned int __stdcall FreezerThread();
	int64_t __fastcall hk_SNP_GetNextThinkTime(void* ecx, void* edx, int64_t usecNow);
	int WriteLagToBuffer(bf_write sexbuf);
	void DoLag();
	unsigned int WINAPI LagThread(void* pParam);
	unsigned int __stdcall FreezerThread(void* pParam);
	int __fastcall hkSendLong(void* netchannel, DWORD* socket, int nBytesLeft, int nMaxRoutableSize);
	unsigned int __stdcall ServerLagger(void*);
	bool __fastcall hkTimingOut(void* ecx, void* edx);
	float __fastcall hkLastPacket(void* ecx, void* edx);
	int		FASTCALL	hkSendDatagram(INetChannel* thisptr, int edx, bf_write* pDatagram);
	void	hkClMove(void* ecx, float accumulated_extra_samples, bool bFinalTick);


	void	FASTCALL	hkOverrideView(IClientModeShared* thisptr, int edx, CViewSetup* pSetup);
	void	FASTCALL	hkOverrideMouseInput(IClientModeShared* thisptr, int edx, float* x, float* y);
	float	FASTCALL	hkGetViewModelFOV(IClientModeShared* thisptr, int edx);
	int		FASTCALL	hkDoPostScreenEffects(IClientModeShared* thisptr, int edx, CViewSetup* pSetup);
	void	FASTCALL	hkRunCommand(IPrediction* thisptr, int edx, CBaseEntity* pEntity, CUserCmd* pCmd, IMoveHelper* pMoveHelper);
	int		FASTCALL	hkSendMessage(ISteamGameCoordinator* thisptr, int edx, std::uint32_t uMsgType, const void* pData, std::uint32_t uData);
	int		FASTCALL	hkRetrieveMessage(ISteamGameCoordinator* thisptr, int edx, std::uint32_t* puMsgType, void* pDest, std::uint32_t uDest, std::uint32_t* puMsgSize);
	bool	FASTCALL	hkSvCheatsGetBool(CConVar* thisptr, int edx);
	int __fastcall hkNetGraph(CConVar* thisptr, int edx);
	long	CALLBACK	hkWndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
}

/*
 * RECV VAR PROXY MANAGERS
 * proxy property hook managers
 */
namespace RVP
{
	inline std::shared_ptr<CRecvPropHook> SmokeEffectTickBegin;
}

/*
 * PROXIES
 * networkable property proxy swap functions
 */
namespace P
{
	// Get
	bool	Setup();
	void	Restore();

	// Handlers
	void	SmokeEffectTickBegin(const CRecvProxyData* pData, void* pStruct, void* pOut);
}
