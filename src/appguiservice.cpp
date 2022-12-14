// Local Includes
#include "appguiservice.h"
#include "appguicomponent.h"
#include "appguiwindowmenucomponent.h"

// External Includes
#include <iostream>
#include <rtti/jsonwriter.h>
#include <nap/logger.h>
#include <utility/fileutils.h>
#include <nap/projectinfo.h>
#include <rtti/deserializeresult.h>
#include <rtti/epropertyvalidationmode.h>
#include <rtti/defaultlinkresolver.h>
#include <nap/core.h>
#include <rtti/jsonwriter.h>
#include <rtti/jsonreader.h>
#include <imguiservice.h>
#include <sceneservice.h>

RTTI_BEGIN_CLASS_NO_DEFAULT_CONSTRUCTOR(nap::AppGUIService)
    RTTI_CONSTRUCTOR(nap::ServiceConfiguration*)
RTTI_END_CLASS

namespace nap
{
    bool AppGUIService::init(nap::utility::ErrorState& errorState)
    {
        // Try to load .ini file and extract saved settings, allowed to fail
        nap::utility::ErrorState ini_error;
        if (!loadIni(getIniFilePath(), ini_error))
        {
            ini_error.fail("Unable to load: %s", getIniFilePath().c_str());
            nap::Logger::warn(errorState.toString());
        }

        return true;
    }


    void AppGUIService::update(double deltaTime)
    {
        for(auto* gui : mGUIs)
        {
            gui->draw(deltaTime);
        }
    }


    void AppGUIService::getDependentServices(std::vector<rtti::TypeInfo>& dependencies)
    {
        dependencies.emplace_back(RTTI_OF(IMGuiService));
        dependencies.emplace_back(RTTI_OF(SceneService));
    }


    void AppGUIService::preShutdown()
    {
        utility::ErrorState write_error;
        if (!writeIni(getIniFilePath(), write_error))
        {
            write_error.fail("Unable to write: %s", getIniFilePath().c_str());
            nap::Logger::warn(write_error.toString());
        }
    }


    bool AppGUIService::writeIni(const std::string& path, utility::ErrorState error)
    {
        // Create ego gui caches to write to disk
        std::vector<std::unique_ptr<AppGUICache>> caches;
        nap::rtti::ObjectList resources;

        caches.reserve(mGUIs.size());
        resources.reserve(mGUIs.size());

        for (auto* gui : mGUIs)
        {
            if(gui->get_type().is_derived_from<AppGUIWindowMenuComponentInstance>())
            {
                auto* window_menu = static_cast<AppGUIWindowMenuComponentInstance*>(gui);

                // Create cache
                auto new_cache = std::make_unique<AppGUICache>(*window_menu);

                resources.emplace_back(new_cache.get());
                caches.emplace_back(std::move(new_cache));
            }

        }

        // Serialize current set of parameters to json
        rtti::JSONWriter writer;
        if (!rtti::serializeObjects(resources, writer, error))
            return false;

        // Get ini file path, create directory if it doesn't exist
        std::string dir = utility::getFileDir(path);
        if (!error.check(utility::ensureDirExists(dir), "unable to write %s file(s) to directory: %s", projectinfo::iniExtension, dir.c_str()))
            return false;

        // Open output file
        std::ofstream output_stream(path, std::ios::binary | std::ios::out);
        if (!error.check(output_stream.is_open() && output_stream.good(), "Failed to open %s for writing", path.c_str()))
            return false;

        // Write to disk
        std::string json = writer.GetJSON();
        output_stream.write(json.data(), json.size());
        return true;
    }


    bool AppGUIService::loadIni(const std::string& path, utility::ErrorState error)
    {
        // Ensure file exists
        mCache.clear();
        if (!utility::fileExists(path))
            return true;

        // Read file
        rtti::DeserializeResult result;
        rtti::Factory& factory = getCore().getResourceManager()->getFactory();
        if (!deserializeJSONFile(
                path, rtti::EPropertyValidationMode::DisallowMissingProperties,
                rtti::EPointerPropertyMode::OnlyRawPointers,
                factory, result, error))
            return false;

        // Resolve links
        if (!rtti::DefaultLinkResolver::sResolveLinks(result.mReadObjects, result.mUnresolvedPointers, error))
            return false;

        // Move found items
        mCache.reserve(result.mReadObjects.size());
        for (auto& item : result.mReadObjects)
        {
            // Ensure it's a window cache
            if (item->get_type().is_derived_from(RTTI_OF(AppGUICache)))
                mCache.emplace_back(std::move(item));
        }
        return true;
    }


    void AppGUIService::registerAppGUIComponentInstance(AppGUIComponentInstance *appGUIComponentInstance)
    {
        mGUIs.emplace_back(appGUIComponentInstance);

        // Find cache associated with given appGUI
        for (const auto& object : mCache)
        {
            // Make sure it's a app gui cache object
            if (!object->get_type().is_derived_from(RTTI_OF(AppGUICache)))
                continue;

            auto* cache = static_cast<AppGUICache*>(object.get());
            if (appGUIComponentInstance->mID == cache->mID)
            {
                if(appGUIComponentInstance->get_type().is_derived_from<AppGUIWindowMenuComponentInstance>())
                {
                    auto* window_menu = static_cast<AppGUIWindowMenuComponentInstance*>(appGUIComponentInstance);
                    for(auto& pair : window_menu->mOpenWindows)
                    {
                        auto open_windows_it = std::find(cache->mOpenWidgets.begin(), cache->mOpenWidgets.end(), pair.first->mID);
                        if(open_windows_it!=cache->mOpenWidgets.end())
                            pair.second = true;
                    }
                }
            }
        }
    }


    void AppGUIService::unregisterAppGUIComponentInstance(AppGUIComponentInstance *appGUIComponentInstance)
    {
        auto it = std::find(mGUIs.begin(), mGUIs.end(), appGUIComponentInstance);
        if(it != mGUIs.end())
        {
            mGUIs.erase(it);
        }
    }


    void AppGUIService::registerObjectCreators(rtti::Factory &factory)
    {
        factory.addObjectCreator(std::make_unique<AppGUIComponentObjectCreator>(*this));
    }
}
