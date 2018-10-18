//
//  DeviceInfo.m
//  vncomp
//
//  Created by Роман on 16.09.18.
//
//

#import "DeviceInfo.h"
#import "DeviceInfo-C.h"
#import <UIKit/UIKit.h>

wchar_t * GetDeviceModel()
{
    NSString * model = [UIDevice currentDevice].model;
    return [DeviceInfo convertNSStringToWchar:model];
}

wchar_t * GetSytemInfo()
{
    NSString * systemName = [UIDevice currentDevice].systemName;
    return [DeviceInfo convertNSStringToWchar:systemName];
}

wchar_t * GetSystemVersion()
{
    NSString * systemVersion = [UIDevice currentDevice].systemVersion;
    return [DeviceInfo convertNSStringToWchar:systemVersion];
}

uint16_t GetBatteryLevel()
{
    UIDevice * device = [UIDevice currentDevice];
    
    if (![device isBatteryMonitoringEnabled]) {
        [device setBatteryMonitoringEnabled:YES];
    }
    
    return device.batteryLevel * 100;
}

wchar_t * GetIdForVendor()
{
    NSString * identifierForVendor = [[UIDevice currentDevice].identifierForVendor UUIDString];
    
    return [DeviceInfo convertNSStringToWchar:identifierForVendor];
}

@implementation DeviceInfo

+(wchar_t *)convertNSStringToWchar: (NSString*) string
{
  return (wchar_t *) [string cStringUsingEncoding:NSUTF32StringEncoding];
}

@end
