/*---------------------------------------------------------*\
| | SkydimoControllerDetect.cpp                             |
| |                                                         |
| |   Detector for Skydimo LED controllers.                 |
| |                                                         |
| |   Skydimo Team                              08 Aug 2025 |
| |                                                         |
| |   This file is part of the OpenRGB project.             |
| |   SPDX-License-Identifier: GPL-2.0-only                 |
\*---------------------------------------------------------*/

#include <vector>
#include <memory>
#include <string>
#include <hidapi.h>
#include "Detector.h"
#include "find_usb_serial_port.h"
#include "LogManager.h"

// Headers for HID controller
#include "SkydimoHIDController/SkydimoHIDController.h"
#include "SkydimoHIDController/RGBController_SkydimoHID.h"

// Headers for Serial controller
#include "SkydimoSerialController/SkydimoSerialController.h"
#include "SkydimoSerialController/RGBController_SkydimoSerial.h"

// Device identifiers
#define SKYDIMO_HID_VID     0x1A86
#define SKYDIMO_HID_PID     0xE316
#define SKYDIMO_SERIAL_VID  0x1A86
#define SKYDIMO_SERIAL_PID  0x7523

// Default device name constant
#define DEFAULT_DEVICE_NAME "Skydimo LED Strip"

/**
 * @brief Generic device registration function.
 *
 * Appends an identifier to the default name for better device recognition
 * and registers the controller with the ResourceManager.
 *
 * @tparam RGBControllerType The type of the RGBController.
 * @param rgb_controller Pointer to the RGB controller instance.
 * @param identifier A string identifier for the device (e.g., path or port).
 */
template<typename RGBControllerType>
void RegisterControllerWithIdentifier(RGBControllerType* rgb_controller, const std::string& identifier)
{
    if(rgb_controller->name == DEFAULT_DEVICE_NAME)
    {
        rgb_controller->name += " " + identifier;
    }
    ResourceManager::get()->RegisterRGBController(rgb_controller);
}

/**
 * @brief Template function to detect and register a controller.
 *
 * This function abstracts the common logic of creating a hardware controller,
 * initializing it, wrapping it in an RGBController, and registering it.
 *
 * @tparam ControllerType The hardware controller class (e.g., SkydimoHIDController).
 * @tparam RGBWrapperType The RGBController wrapper class (e.g., RGBController_SkydimoHID).
 * @tparam InitArgType The type of the initialization argument (e.g., device path).
 * @param init_arg The argument required to initialize the controller.
 * @param id_suffix A suffix to append to the device name for identification.
 * @return true if detection and registration were successful, false otherwise.
 */
template<class ControllerType, class RGBWrapperType, class InitArgType>
bool DetectAndRegisterController(const InitArgType& init_arg, const std::string& id_suffix)
{
    std::unique_ptr<ControllerType> controller(new ControllerType());

    // Initialize the controller. The `Initialize` method must be implemented
    // in the respective controller class.
    if(!controller->Initialize(init_arg))
    {
        return false;
    }

    // Create the RGBController wrapper, passing ownership of the hardware controller.
    RGBWrapperType* rgb_controller = new RGBWrapperType(std::move(controller));

    // Register the controller with a descriptive identifier.
    RegisterControllerWithIdentifier(rgb_controller, id_suffix);

    return true;
}

/**
 * @brief Detects Skydimo HID LED controller devices.
 *
 * This function is a callback invoked by OpenRGB for each matching HID device.
 *
 * @param info Pointer to the HID device information structure.
 * @param name The name of the device.
 */
void DetectSkydimoHIDControllers(hid_device_info* info, const std::string& /*name*/)
{
    try
    {
        DetectAndRegisterController<SkydimoHIDController, RGBController_SkydimoHID>(
            info->path, "(HID: " + std::string(info->path) + ")"
        );
    }
    catch(const std::exception& e)
    {
        LOG_ERROR("Failed to initialize Skydimo HID controller at path %s: %s", info->path, e.what());
    }
}

/**
 * @brief Detects Skydimo Serial LED controller devices.
 *
 * This function is called once by OpenRGB to detect all serial-based devices.
 */
void DetectSkydimoSerialControllers()
{
    std::vector<std::string*> ports = find_usb_serial_port(SKYDIMO_SERIAL_VID, SKYDIMO_SERIAL_PID);

    for(size_t port_idx = 0; port_idx < ports.size(); port_idx++)
    {
        std::unique_ptr<std::string> port(ports[port_idx]);
        if(!port || port->empty())
        {
            continue;
        }

        try
        {
            DetectAndRegisterController<SkydimoSerialController, RGBController_SkydimoSerial>(
                *port, "(Serial: " + *port + ")"
            );
        }
        catch(const std::exception& e)
        {
            LOG_ERROR("Failed to initialize Skydimo Serial controller on port %s: %s", port->c_str(), e.what());
        }
    }
}

/*---------------------------------------------------------------------------------------------------------*\
| Register detectors                                                                                      |
\*---------------------------------------------------------------------------------------------------------*/

REGISTER_HID_DETECTOR("Skydimo HID LED", DetectSkydimoHIDControllers, SKYDIMO_HID_VID, SKYDIMO_HID_PID);

REGISTER_DETECTOR("Skydimo Serial LED", DetectSkydimoSerialControllers);
