#pragma once

// External Includes
#include <nap/resource.h>
#include <nap/resourceptr.h>

// Local Includes
#include "appguiitem.h"

namespace nap
{
    //////////////////////////////////////////////////////////////////////////

    class NAPAPI AppGUIWidget : public AppGUIItem
    {
        RTTI_ENABLE(AppGUIItem)
    public:
        virtual ~AppGUIWidget(){};

        void draw(double deltaTime);
    protected:
        virtual void drawContent(double deltaTime) = 0;
    };

    class NAPAPI AppGUIWindow : public AppGUIMenuItem
    {
        RTTI_ENABLE(AppGUIMenuItem)
    public:
        virtual ~AppGUIWindow(){};

        void draw(double deltaTime);

        // properties
        std::string mName;
    protected:
        virtual void drawContent(double deltaTime) = 0;
    };

    class NAPAPI AppGUIWindowGroup : public AppGUIMenuItem
    {
        RTTI_ENABLE(AppGUIMenuItem)
    public:
        virtual ~AppGUIWindowGroup(){};

        // properties
        std::vector<ResourcePtr<AppGUIMenuItem>> mItems;
        std::string mName;
    };
}
