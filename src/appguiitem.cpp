
// local includes
#include "appguiitem.h"
#include "appguiservice.h"

RTTI_BEGIN_CLASS_NO_DEFAULT_CONSTRUCTOR(nap::AppGUIItem)
RTTI_END_CLASS

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
