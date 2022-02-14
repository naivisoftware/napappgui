#pragma once

#include <nap/resource.h>

namespace nap
{
    class AppGUIService;

    class NAPAPI AppGUIItem : public Resource
    {
        RTTI_ENABLE(Resource)
    public:
        AppGUIItem(AppGUIService& service);

        AppGUIService& mAppGUIService;
    };

    class NAPAPI AppGUIMenuItem : public AppGUIItem
    {
        RTTI_ENABLE(AppGUIItem)
    public:
        AppGUIMenuItem(AppGUIService& service);
    };
}
