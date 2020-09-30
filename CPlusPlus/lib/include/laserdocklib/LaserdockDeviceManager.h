//
// Created by Guoping Huang on 9/6/16.
//

#ifndef LASERDOCKLIB_LASERDOCKDEVICEMANAGER_H
#define LASERDOCKLIB_LASERDOCKDEVICEMANAGER_H

#include <memory>
#include <vector>

#include <laserdocklib/Laserdocklib.h>

class LaserdockDevice;
class LaserdockDeviceManagerPrivate;

class LASERDOCKLIB_EXPORT LaserdockDeviceManager
{
public:
    static LaserdockDeviceManager& getInstance();

    std::vector<std::unique_ptr<LaserdockDevice> > get_laserdock_devices();

    // helper methods
    void print_laserdock_devices();
    LaserdockDevice *get_next_available_device();

private:
    explicit LaserdockDeviceManager();
    virtual ~LaserdockDeviceManager();

private:
    std::unique_ptr<LaserdockDeviceManagerPrivate> d;
};

#endif //LASERDOCKLIB_LASERDOCKDEVICEMANAGER_H
