#pragma once

#include <nap/resource.h>

namespace nap
{
    class AppGUIService;

    class NAPAPI AppGUIItem : public Resource
    {
        RTTI_ENABLE(Resource)
    public:
        // default constructor & destructor
        AppGUIItem() = default;
        virtual ~AppGUIItem() = default;

        virtual void draw(double deltaTime) = 0;

        void setAppGUIService(AppGUIService& service);

        AppGUIService& getAppGUIService() const;
    protected:
        AppGUIService* mService;
    };
}
