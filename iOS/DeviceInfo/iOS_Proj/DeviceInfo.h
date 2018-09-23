//
//  DeviceInfo.h
//  vncomp
//
//  Created by Роман on 16.09.18.
//
//

#ifndef DeviceInfo_h
#define DeviceInfo_h

#import <Foundation/Foundation.h>

@interface DeviceInfo : NSObject

//+(wchar_t *)GetDeviceModel;
//+(wchar_t *)GetSytemInfo;
//+(wchar_t *)GetSystemVersion;
//+(uint16_t)GetBatteryLevel;
+(wchar_t *)convertNSStringToWchar: (NSString*) string;

@end

#endif /* DeviceInfo_h */
