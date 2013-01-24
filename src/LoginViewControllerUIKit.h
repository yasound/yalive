//
//  LoginViewControllerUIKit.h
//  YaLive
//
//  Created by Jérôme BLONDON on 21/01/13.
//
//

#import <UIKit/UIKit.h>
#import "LoginViewController.h"

@interface LoginViewControllerUIKit : UIViewController<UIWebViewDelegate>
{
  UIWebView *mpWebView;
  UIToolbar *mpToolbar;
  id <LoginEventsDelegate> delegate;
}

@property (retain) id delegate;
@property (nonatomic, retain) UIWebView *mpWebView;
@property (nonatomic, retain) UIToolbar *mpToolbar;

- (void)loginCompleted:(NSString *)sessionId;
- (void)loginCanceled;

@end
