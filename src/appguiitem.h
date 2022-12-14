#pragma once

#include <nap/resource.h>

namespace nap
{
    class AppGUIService;
    class AppGUIComponentInstance;

    class NAPAPI AppGUIItem : public Resource
    {
        RTTI_ENABLE(Resource)
    public:
        // default constructor & destructor
        AppGUIItem() = default;
        virtual ~AppGUIItem() = default;

        virtual bool setup(AppGUIService& service, AppGUIComponentInstance& appGuiComponent, utility::ErrorState& errorState);

        virtual void draw(double deltaTime) = 0;
    };
}
