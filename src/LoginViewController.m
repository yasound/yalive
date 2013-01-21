//
//  LoginViewController.m
//  YaLive
//
//  Created by Jérôme BLONDON on 21/01/13.
//
//

#import "LoginViewController.h"
#ifdef _COCOA_
#import "LoginViewControllerCocoa.h"
#endif

@implementation LoginViewController

- (id) init
{
  self = [super init];
  return self;
}

-(void)login:(Header*) cb
{
#ifdef _COCOA_
  [[LoginViewControllerCocoa alloc] initWithCB: cb];
#endif
}

-(void)logout
{
#ifdef _COCOA_
  LoginViewControllerCocoa *pImpl = [[LoginViewControllerCocoa alloc] initWithCB:nil];
  [pImpl logout];
  [pImpl dealloc];
#endif
}

@end
