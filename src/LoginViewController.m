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
#ifdef NUI_IPHONE
#import "LoginViewControllerUIKit.h"
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
#ifdef NUI_IPHONE
  LoginViewControllerUIKit *pImpl = [[LoginViewControllerUIKit alloc] initWithNibName:@"LoginViewControllerUIKit" bundle:nil];
  UIWindow *mainWindow = (UIWindow *)([[UIApplication sharedApplication].windows objectAtIndex:0]);
  [mainWindow addSubview:[pImpl view]];
#endif
}

-(void)logout
{
#ifdef _COCOA_
  LoginViewControllerCocoa *pImpl = [[LoginViewControllerCocoa alloc] initWithCB:nil];
  [pImpl dealloc];
#endif
  
}

@end
