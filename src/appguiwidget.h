#pragma once

// External Includes
#include <nap/resource.h>
#include <nap/resourceptr.h>
#include <rtti/factory.h>
#include <nap/group.h>

// Local Includes
#include "appguiitem.h"

namespace nap
{
    //////////////////////////////////////////////////////////////////////////
    // forward declares
    class AppGUIService;
    class AppGUIWidget;

    // AppGUIWidgetGroup group type definition
    using AppGUIWidgetGroup = Group<AppGUIWidget>;

    class NAPAPI AppGUIWidget : public AppGUIItem
    {
        RTTI_ENABLE(AppGUIItem)
    public:
        void draw(double deltaTime) override;
    protected:
        virtual void drawContent(double deltaTime) = 0;
    };
}
