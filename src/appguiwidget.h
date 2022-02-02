#pragma once

// External Includes
#include <nap/resource.h>
#include <nap/resourceptr.h>

// Local Includes
#include "appguiitem.h"

namespace nap
{
    //////////////////////////////////////////////////////////////////////////

    class NAPAPI AppGuiWidget : public AppGUIItem
    {
        RTTI_ENABLE(AppGUIItem)
    public:
        virtual ~AppGuiWidget(){};

        void draw(double deltaTime);
    protected:
        virtual void drawContent(double deltaTime) = 0;
    };

    class NAPAPI AppGuiWindow : public AppGUIMenuItem
    {
        RTTI_ENABLE(AppGUIMenuItem)
    public:
        virtual ~AppGuiWindow(){};

        void draw(double deltaTime);

        // properties
        std::string mName;
    protected:
        virtual void drawContent(double deltaTime) = 0;
    };

    class NAPAPI AppGUIMenuItems : public AppGUIMenuItem
    {
        RTTI_ENABLE(AppGUIMenuItem)
    public:
        virtual ~AppGUIMenuItems(){};

        // properties
        std::vector<ResourcePtr<AppGUIMenuItem>> mItems;
        std::string mName;
    };
}
