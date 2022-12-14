#pragma once

// Internal Includes
#include "appguiwidget.h"
#include "appguiwindow.h"

// External Includes
#include <component.h>
#include <componentptr.h>
#include <nap/resourceptr.h>
#include <rtti/factory.h>

namespace nap
{
    //////////////////////////////////////////////////////////////////////////

    // forward declares
    class AppGUIService;
    class AppGUIWindowMenuComponentInstance;

    class NAPAPI AppGUIWindowMenuComponent : public Component
    {
        friend class AppGUIWindowMenuComponentInstance;

        RTTI_ENABLE(Component)
        DECLARE_COMPONENT(AppGUIWindowMenuComponent, AppGUIWindowMenuComponentInstance)
    public:
        AppGUIWindowMenuComponent(AppGUIService& appGUIService);

        virtual ~AppGUIWindowMenuComponent() = default;

        // properties
        std::vector<ResourcePtr<AppGUIWindowGroup>> mWindowGroups;
    private:
        AppGUIService& mAppGUIService;
    };


    class NAPAPI AppGUIWindowMenuComponentInstance : public ComponentInstance
    {
        friend class AppGUICache;
        friend class AppGUIService;
    RTTI_ENABLE(ComponentInstance)
    public:
        AppGUIWindowMenuComponentInstance(EntityInstance& entity, Component& resource)
            : nap::ComponentInstance(entity, resource) { }

        virtual ~AppGUIWindowMenuComponentInstance() = default;

        // Initialize the component
        bool init(utility::ErrorState& errorState) override;

        void onDestroy() override;
    private:
        // Draws GUI, called from AppGUIService
        void draw(double deltaTime);

        bool constructMenuRecursive(AppGUIWindowGroup* group, std::vector<std::string>& group_ids, utility::ErrorState &errorState);

        void handleWindowGroup(AppGUIWindowGroup* group);

        void handleWindow(AppGUIWindow* mWidget);

        std::vector<std::string> getOpenWindowIDs();

        std::unordered_map<AppGUIWindow*, bool> mOpenWindows;

        AppGUIService* mAppGUIService;
    };

    class NAPAPI AppGUICache : public Resource
    {
        RTTI_ENABLE(Resource)
    public:
        AppGUICache() = default;
        AppGUICache(AppGUIWindowMenuComponentInstance& gui);

        ~AppGUICache() = default;

        bool isOpen(const std::string& windowID);

        std::vector<std::string> mOpenWidgets;
    };

    using AppGUIComponentObjectCreator = rtti::ObjectCreator<AppGUIWindowMenuComponent, AppGUIService>;
}
