
// local includes
#include "appguiitem.h"
#include "appguiservice.h"
namespace nap
{
    //////////////////////////////////////////////////////////////////////////
    // AppGUIItem
    //////////////////////////////////////////////////////////////////////////

    void AppGUIItem::setAppGUIService(AppGUIService& service)
    {
        mService = &service;
    }


    AppGUIService& AppGUIItem::getAppGUIService() const
    {
        return *mService;
    }
}
