#pragma once

// External Includes
#include <nap/service.h>

namespace nap
{
    //
    class AppGUI;

    class NAPAPI AppGUIService : public Service
    {
        RTTI_ENABLE(Service)
    public:
        // Default Constructor
        AppGUIService(ServiceConfiguration* configuration) : Service(configuration)	{ }

        /**
         * Use this call to register service dependencies
         * A service that depends on another service is initialized after all it's associated dependencies
         * This will ensure correct order of initialization, update calls and shutdown of all services
         * @param dependencies rtti information of the services this service depends on
         */
        virtual void getDependentServices(std::vector<rtti::TypeInfo>& dependencies) override;

        /**
         * Initializes the service
         * @param errorState contains the error message on failure
         * @return if the video service was initialized correctly
         */
        virtual bool init(nap::utility::ErrorState& errorState) override;

        /**
         * Invoked by core in the app loop. Update order depends on service dependency
         * This call is invoked after the resource manager has loaded any file changes but before
         * the app update call. If service B depends on A, A:s:update() is called before B::update()
         * @param deltaTime: the time in seconds between calls
        */
        virtual void update(double deltaTime) override;

        virtual void preShutdown() override;

        /**
         * Writes the render service .ini file to disk
         * The .ini file is used to (re)-store render settings in between sessions.
         * @param path path to file to write
         * @param error contains the error if the write operation fails
         * @return if write operation succeeded
         */
        bool writeIni(const std::string& path, utility::ErrorState error);

        /**
         * Loads settings from the .ini file.
         * The .ini file is used to (re)-store render settings in between sessions.
         * @param path path to file to load
         * @param error contains the error if the load operation fails.
         * @return if the file is read
         */
        bool loadIni(const std::string& path, utility::ErrorState error);

        void registerAppGUI(AppGUI* appGui);

        void unregisterAppGUI(AppGUI* appGui);
    protected:
        void registerObjectCreators(rtti::Factory &factory) override;

        std::vector<AppGUI*> mGUIs;

        // Cache read from ini file, contains saved settings
        std::vector<std::unique_ptr<rtti::Object>> mCache;
    };
}
