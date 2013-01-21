//
//  LoginViewController.h
//  YaLive
//
//  Created by Jérôme BLONDON on 21/01/13.
//
//

#import <Foundation/Foundation.h>
#import "nui.h"
#import "Header.h"

@interface LoginViewController : NSObject
{
  Header *mpCB;
}

-(void)login:(Header*) cb;
-(void)logout;

@end
