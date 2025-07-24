#include "Patches/CurlHttpRequestPatch.h"
#include "KismetDetoursLibrary.h"

static bool (*_ProcessRequest)(FCurlHttpRequest*);

UDetour* ProcessRequestDetour = NULL;

bool FCurlHttpRequestPatch::ProcessRequestHook(FCurlHttpRequest* Request)
{
	return Request->ProcessRequest() && _ProcessRequest(Request);
}

void FCurlHttpRequestPatch::Init()
{
	_ProcessRequest = UKismetMemoryLibrary::Get<decltype(_ProcessRequest)>(L"FCurlHttpRequest::ProcessRequest");

	ProcessRequestDetour = new UDetour();
	ProcessRequestDetour->Init(_ProcessRequest, ProcessRequestHook, EDetourType::VFSwap);
	ProcessRequestDetour->Commit();
}