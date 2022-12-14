
// local includes
#include "appguiitem.h"
#include "appguiservice.h"
#include "appguicomponent.h"

RTTI_BEGIN_CLASS_NO_DEFAULT_CONSTRUCTOR(nap::AppGUIItem)
RTTI_END_CLASS

namespace nap
{
    //////////////////////////////////////////////////////////////////////////
    // AppGUIItem
    //////////////////////////////////////////////////////////////////////////

    bool AppGUIItem::setup(AppGUIService& service, AppGUIComponentInstance& appGuiComponent, utility::ErrorState& errorState)
    {
        return true;
    }

}
