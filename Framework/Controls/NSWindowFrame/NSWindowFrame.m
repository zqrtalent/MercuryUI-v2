//
//  MyModalWindow.m
//  WindowCreationApp
//
//  Created by Zaqro Butskrikidze on 10/15/16.
//  Copyright © 2016 zaqro butskrikidze. All rights reserved.
//

#import "NSWindowFrame.h"
#import "NSWindowFrameView.h"

@implementation NSWindowFrame

-(id)initWithContentRect:(NSRect)contentRect styleMask:(NSUInteger)aStyle backing:(NSBackingStoreType)bufferingType defer:(BOOL)flag

{
    self = [super initWithContentRect:contentRect styleMask:aStyle backing:bufferingType defer:flag];
    NSLog(@"init");
   
    [self setAcceptsMouseMovedEvents:YES];
    return self;
}

-(void)keyDown:(NSEvent *)theEvent
{
    [super keyDown:theEvent];
    NSLog(@"event code %d", theEvent.keyCode);
}

-(void)mouseDown:(NSEvent *)theEvent
{
    [super mouseDown:theEvent];
    NSLog(@"mouse down");
}

-(void)mouseUp:(NSEvent *)theEvent
{
    [super mouseUp:theEvent];
    
    NSLog(@"mouse up");
}

-(void)rightMouseDown:(NSEvent *)theEvent
{
    [super rightMouseDown:theEvent];
    
    NSLog(@"right mouse down");
}

-(void)rightMouseUp:(NSEvent *)theEvent
{
    [super rightMouseUp:theEvent];
    
    NSLog(@"right mouse up");
}

-(void)mouseMoved:(NSEvent *)theEvent
{
    [super mouseMoved:theEvent];
    
    //NSLog(@"mouse moved");
}

-(void)mouseDragged:(NSEvent *)theEvent
{
    [super mouseDragged:theEvent];
    
    NSLog(@"mouse dragged");
}

-(void)scrollWheel:(NSEvent *)theEvent
{
    [super scrollWheel:theEvent];
    
    NSLog(@"scroll wheel");
}

-(void)magnifyWithEvent:(NSEvent*)theEvent
{
    [super magnifyWithEvent:theEvent];
    
    NSLog(@"magnify with event");
}

-(NSSize)windowWillResize:(NSWindow *)sender toSize:(NSSize)frameSize
{
    NSLog(@"resize event %f %f", frameSize.width, frameSize.height);
    [((NSWindowFrameView*)self.contentView) resizeEvent:frameSize];
    return frameSize;
}

-(void)windowWillMiniaturize:(NSNotification *)notification
{
    NSLog(@"on minimize");
}


 -(void)windowWillClose:(NSNotification *)notification
{
}

-(BOOL)windowShouldClose:(id)sender
{
    return YES;
}

@end
