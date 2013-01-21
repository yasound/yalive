//
//  LoginViewControllerUIKit.m
//  YaLive
//
//  Created by Jérôme BLONDON on 21/01/13.
//
//

#import "LoginViewControllerUIKit.h"

@implementation LoginViewControllerUIKit
@synthesize mpWebView;

- (id)initWithNibName:(NSString *)nibNameOrNil bundle:(NSBundle *)nibBundleOrNil
{
    self = [super initWithNibName:nibNameOrNil bundle:nibBundleOrNil];
    if (self) {
        // Custom initialization
    }
    return self;
}

- (void)viewDidLoad
{
  [super viewDidLoad];
  // Do any additional setup after loading the view from its nib.
  NSString *urlAddress = @"http://www.google.com";
  //Create a URL object.
  NSURL *url = [NSURL URLWithString:urlAddress];

  //URL Requst Object
  NSURLRequest *requestObj = [NSURLRequest requestWithURL:url];

  //Load the request in the UIWebView.
  [mpWebView loadRequest:requestObj];
}

- (void)didReceiveMemoryWarning
{
    [super didReceiveMemoryWarning];
    // Dispose of any resources that can be recreated.
}

- (BOOL)shouldAutorotate
{
  return YES;
}

- (BOOL)shouldAutorotateToInterfaceOrientation:(UIInterfaceOrientation)toInterfaceOrientation
{
  return YES;
}

@end
