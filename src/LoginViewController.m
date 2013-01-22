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

- (id) initWithCB:(Header *) cb
{
  self = [super init];
  mpCB = cb;
  
#ifdef _COCOA_
  LoginViewControllerCocoa *pImpl = [[LoginViewControllerCocoa alloc] init];
  pImpl.delegate = self;
  mpImpl = pImpl;
# endif
  
#ifdef NUI_IPHONE
  LoginViewControllerUIKit *pImpl = [[LoginViewControllerUIKit alloc] initWithNibName:nil bundle:nil];
  pImpl.delegate = self;
  mpImpl = pImpl;
#endif
  
  return self;
}

-(void) dealloc
{
  if (mpImpl)
  {
    [mpImpl dealloc];
  }
  [super dealloc];
}

-(void)login:(Header*) cb
{
}

-(void)logout
{
#ifdef _COCOA_
  [mpImpl logout];
#endif
  
}

#pragma mark LoginEventsDelegate handlers

- (void) loginCompleted: (NSString *)sessionId
{
  mpCB->OnLoginReceived(self, [sessionId cStringUsingEncoding:NSASCIIStringEncoding]);
}


@end
