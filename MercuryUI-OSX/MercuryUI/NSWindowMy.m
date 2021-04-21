//
//  NSWindowMy.m
//  OSXWindowEvents
//
//  Created by Zaqro Butskhrikidze on 8/14/14.
//  Copyright (c) 2014 Zaqro Butskhrikidze. All rights reserved.
//

#import "NSWindowMy.h"
#import "NSViewMy.h"

@implementation NSWindowMy

/*
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
}*/


-(IBAction) onClick:(NSButton*)sender
{
    NSLog(@"button click");
    
    
    NSOpenPanel* openDialog = [NSOpenPanel openPanel];
    openDialog.allowedFileTypes = [[NSArray alloc] initWithObjects:@"des", @"png",nil];
    openDialog.allowsMultipleSelection = NO;
    [openDialog setDirectoryURL:[NSURL URLWithString:@"/Volumes/OSX-DATA/Dev/ESPoker_06.01.2014/"]];
    
    NSLog(@"%@", [[[NSBundle mainBundle] bundleURL] URLByDeletingLastPathComponent]);
    
    if( [openDialog runModal] == NSOKButton )
    {
       NSArray* arrChosenFiles = [openDialog URLs];
        for(int i=0; i<arrChosenFiles.count; i++)
        {
            NSLog(@"%@", [[arrChosenFiles objectAtIndex:i] path]);
        }
    }
    
}

/*
-(NSSize)windowWillResize:(NSWindow *)sender toSize:(NSSize)frameSize
{
    NSLog(@"resize event %f %f", frameSize.width, frameSize.height);
    [((NSViewMy*)_contentView) resizeEvent:frameSize];
    
    return frameSize;
}

-(void)windowWillMiniaturize:(NSNotification *)notification
{
    NSLog(@"on minimize");
}*/

@end
