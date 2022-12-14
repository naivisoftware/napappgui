#pragma once

// Internal Includes
#include "appguiwidget.h"
#include "appguiitem.h"

// External Includes
#include <component.h>
#include <componentptr.h>
#include <nap/resourceptr.h>
#include <rtti/factory.h>

namespace nap
{
    //////////////////////////////////////////////////////////////////////////
    class AppGUIService;
    class AppGUIComponentInstance;

    class NAPAPI AppGUIComponent : public Component
    {
        friend class AppGUIComponentInstance;

        RTTI_ENABLE(Component)
        DECLARE_COMPONENT(AppGUIComponent, AppGUIComponentInstance)
    public:
        AppGUIComponent(AppGUIService& appGUIService);

        ~AppGUIComponent() = default;

        // properties
        ResourcePtr<AppGUIMenuItemGroup> mMenuItems;
    private:
        AppGUIService& mAppGUIService;
    };


    class NAPAPI AppGUIComponentInstance : public ComponentInstance
    {
        friend class AppGUICache;
        friend class AppGUIService;
    RTTI_ENABLE(ComponentInstance)
    public:
        AppGUIComponentInstance(EntityInstance& entity, Component& resource)
            : nap::ComponentInstance(entity, resource) { }

        virtual ~AppGUIComponentInstance() = default;

        // Initialize the component
        bool init(utility::ErrorState& errorState) override;

        void onDestroy() override;
    private:
        // Draws GUI, called from AppGUIService
        void draw(double deltaTime);

        bool constructMenuRecursive(AppGUIMenuItemGroup* group, std::vector<std::string>& group_ids, utility::ErrorState &errorState);

        void handleWidgetGroup(AppGUIMenuItemGroup* group);

        void handleWindowWidget(AppGUIWindow* mWidget);

        std::vector<std::string> getOpenWindowIDs();

        std::unordered_map<AppGUIWindow*, bool> mOpenWindows;

        AppGUIService* mAppGUIService;
    };

    class NAPAPI AppGUICache : public Resource
    {
        RTTI_ENABLE(Resource)
    public:
        AppGUICache() = default;
        AppGUICache(AppGUIComponentInstance& gui);

        ~AppGUICache() = default;

        bool isOpen(std::string widgetId);

        std::vector<std::string> mOpenWidgets;
    };

    using AppGUIComponentObjectCreator = rtti::ObjectCreator<AppGUIComponent, AppGUIService>;
}
