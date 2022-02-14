#pragma once

// External Includes
#include <nap/resource.h>
#include <nap/resourceptr.h>
#include <rtti/factory.h>

// Local Includes
#include "appguiitem.h"

namespace nap
{
    //////////////////////////////////////////////////////////////////////////
    class AppGUIService;

    class NAPAPI AppGUIWidget : public AppGUIItem
    {
        RTTI_ENABLE(AppGUIItem)
    public:
        AppGUIWidget(AppGUIService& service);

        virtual ~AppGUIWidget() = default;

        void draw(double deltaTime);
    protected:
        virtual void drawContent(double deltaTime) = 0;
    };

    class NAPAPI AppGUIWindow : public AppGUIMenuItem
    {
        RTTI_ENABLE(AppGUIMenuItem)
    public:
        AppGUIWindow(AppGUIService& service);

        virtual ~AppGUIWindow() = default;

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
        AppGUIWindowGroup(AppGUIService& service);

        virtual ~AppGUIWindowGroup() = default;

        // properties
        std::vector<ResourcePtr<AppGUIMenuItem>> mItems;
        std::string mName;
    };

    using AppGUIWindowGroupObjectCreator = rtti::ObjectCreator<AppGUIWindowGroup, AppGUIService>;
}
