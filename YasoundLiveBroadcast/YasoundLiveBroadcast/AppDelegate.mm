//
//  AppDelegate.m
//  YasoundLiveBroadcast
//
//  Created by matthieu campion on 5/7/12.
//  Copyright (c) 2012 MXP4. All rights reserved.
//

#import "AppDelegate.h"
#include "Broadcaster.h"

@implementation AppDelegate

@synthesize window = _window;

- (void)dealloc
{
  [super dealloc];
}

- (void)applicationDidFinishLaunching:(NSNotification *)aNotification
{
  // Insert code here to initialize your application
    
  Broadcaster* pBroadcaster = new Broadcaster();
}

@end
