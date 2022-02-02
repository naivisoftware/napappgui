#pragma once

#include <nap/resource.h>

namespace nap
{
    class NAPAPI AppGUIItem : public Resource
    {
        RTTI_ENABLE(Resource)
    public:
    };

    class NAPAPI AppGUIMenuItem : public AppGUIItem
    {
        RTTI_ENABLE(AppGUIItem)
    public:
    };
}
