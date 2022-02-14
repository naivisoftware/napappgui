
// local includes
#include "appguiitem.h"

namespace nap
{
    AppGUIItem::AppGUIItem(AppGUIService& service) : mAppGUIService(service){}

    AppGUIMenuItem::AppGUIMenuItem(AppGUIService& service) : AppGUIItem(service){}
}
