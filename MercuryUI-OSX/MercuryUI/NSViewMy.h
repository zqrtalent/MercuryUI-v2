//
//  NSViewMy.h
//  OSXWindowEvents
//
//  Created by Zaqro Butskhrikidze on 9/12/14.
//  Copyright (c) 2014 Zaqro Butskhrikidze. All rights reserved.
//

#import <Cocoa/Cocoa.h>
#import "Controls/NSWindowFrame/NSWindowFrameView.h"

@interface NSViewMy : NSWindowFrameView

-(void)initializeMercuryView:(NSRect)rcView wnd:(NSWindow*)wnd;
-(void)resizeEvent:(NSSize)frameSize;
-(void)viewDidMoveToWindow;
 
@end
