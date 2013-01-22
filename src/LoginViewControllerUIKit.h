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
  IBOutlet UIWebView *mpWebView;
  id <LoginEventsDelegate> delegate;
}

@property (retain) id delegate;
@property (nonatomic, retain) IBOutlet UIWebView *mpWebView;

- (void)loginCompleted:(NSString *)sessionId;

@end
