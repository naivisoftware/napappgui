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

        virtual void setAppGUIService(AppGUIService& service);

        AppGUIService& getAppGUIService() const;
    protected:
        AppGUIService* mService;
    };

    class NAPAPI AppGUIMenuItem : public AppGUIItem
    {
        RTTI_ENABLE(AppGUIItem)
    public:
        // default constructor & destructor
        AppGUIMenuItem() = default;
        virtual ~AppGUIMenuItem() = default;
    };
}
