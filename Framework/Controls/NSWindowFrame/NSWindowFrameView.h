//
//  MyModalView.h
//  WindowCreationApp
//
//  Created by Zaqro Butskrikidze on 10/15/16.
//  Copyright © 2016 zaqro butskrikidze. All rights reserved.
//

#import <Cocoa/Cocoa.h>
class MercuryBaseView;

@interface NSWindowFrameView : NSView
{
@protected
    MercuryBaseView* _pMercuryView;
    MercuryBaseView* _pMercuryViewToDisplay;
}

-(void)initializeMercuryView:(NSRect)rcView wnd:(NSWindow*)wnd mercuryView:(MercuryBaseView*) pMercuryView;
-(void)resizeEvent:(NSSize)frameSize;
-(void)viewDidMoveToWindow;

@end
