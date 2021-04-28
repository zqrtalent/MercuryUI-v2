//
//  MyModalWindow.h
//  WindowCreationApp
//
//  Created by Zaqro Butskrikidze on 10/15/16.
//  Copyright © 2016 zaqro butskrikidze. All rights reserved.
//

#import <Cocoa/Cocoa.h>
#include <functional>
#include "../../_platformCompat/PlatformFont.h"

@interface NSWindowFrame : NSWindow<NSWindowDelegate>
-(void) chooseFont:(_Font*)selectedFont withCallback:(std::function<void(void*,_Font*)>)callback andData:(void*)data;
@end
