#include "Curl/CurlHttp.h"
#include "Core.h"

#include <Windows.h>

bool FCurlHttpRequest::ProcessRequest()
{
    std::wstring URL(*GetURL());
    size_t PathIndex = URL.find(L"ol.epicgames.com");
    size_t PathIndexTwo = URL.find(L"ol.epicgames.net");

    if (PathIndex != std::wstring::npos || 
        PathIndexTwo != std::wstring::npos)
    {
        std::wstring Path;

        if (PathIndex == std::wstring::npos) 
        {
            Path = URL.substr(PathIndexTwo + 16);
        }
        else 
        {
            Path = URL.substr(PathIndex + 16);
        }

        auto NewURL = API_URL + Path;

        SetURL(NewURL.c_str());
    }

    return true;
}