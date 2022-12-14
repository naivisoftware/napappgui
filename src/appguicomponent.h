#pragma once

// Internal Includes
#include "appguiwidget.h"
#include "appguiwindow.h"

// External Includes
#include <component.h>
#include <componentptr.h>
#include <nap/resourceptr.h>
#include <rtti/factory.h>
#include <entity.h>
#include <component.h>

namespace nap
{
    //////////////////////////////////////////////////////////////////////////

    // forward declares
    class AppGUIService;
    class AppGUIComponentInstance;

    class NAPAPI AppGUIComponent : public Component
    {
        friend class AppGUIComponentInstance;

        RTTI_ENABLE(Component)
        DECLARE_COMPONENT(AppGUIComponent, AppGUIComponentInstance)
    public:
        AppGUIComponent(AppGUIService& appGUIService);

        virtual ~AppGUIComponent() = default;
    private:
        AppGUIService& mAppGUIService;
    };

    class NAPAPI AppGUIComponentInstance : public ComponentInstance
    {
        friend class AppGUICache;
        friend class AppGUIService;

        RTTI_ENABLE(ComponentInstance)
    public:
        AppGUIComponentInstance(EntityInstance& entity, Component& resource);

        virtual ~AppGUIComponentInstance() = default;

        virtual void draw(double deltaTime) = 0;

        bool init(utility::ErrorState &errorState) override final;

        void onDestroy() override final;

        template<typename T>
        T* findComponentInstance(const std::string& componentID);
    protected:
        virtual bool initInternal(utility::ErrorState& errorState) = 0;

        virtual void onDestroyInternal() = 0;

        AppGUIService* mAppGUIService;
    };

    template<typename T>
    T* AppGUIComponentInstance::findComponentInstance(const std::string& componentID)
    {
        // get root entity
        auto* root_entity = getEntityInstance();
        auto* parent = root_entity;
        while(parent!= nullptr)
        {
            root_entity = parent;
            parent = root_entity->getParent();
        }

        // find all component instances and their resources and try to match componentID
        std::vector<T*> component_instances;
        root_entity->getComponentsOfTypeRecursive(component_instances);
        for(auto* component_instance : component_instances)
        {
            if(component_instance->getComponent()->mID==componentID)
            {
                return component_instance;
            }
        }

        // couldn't find component instance with component id
        return nullptr;
    }
}
