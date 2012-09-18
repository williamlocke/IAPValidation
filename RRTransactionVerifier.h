//
//  RRTransactionVerifier.h
//  Rowmote
//
//  Created by Evan Schoenberg on 7/20/12.
//
//

#import <Foundation/Foundation.h>
#import <StoreKit/StoreKit.h>
#import "RRVerificationController.h"

@interface RRTransactionVerifier : NSObject
{
	SKPaymentTransaction *transaction;
    BOOL sandboxTransaction;
}

#define ERROR_SANDBOX_RECEIPT_IN_PROD   31415

@property (nonatomic, assign, getter=isSandboxTransaction) BOOL sandboxTransaction;
@property (nonatomic, strong) SKPaymentTransaction *transaction;
@property (nonatomic, assign) NSObject<RRVerificationControllerDelegate> *delegate;
@property (nonatomic, assign) RRVerificationController *controller;

@property (nonatomic, strong) NSDictionary *originalPurchaseInfoDict;
@property (nonatomic, strong) NSMutableData *receivedData;


- (id)initWithPurchase:(SKPaymentTransaction *)inTransaction delegate:(NSObject<RRVerificationControllerDelegate> *)inVerificationDelegate controller:(RRVerificationController *)inController;

- (BOOL)beginVerificationWithError:(out NSError **)outError;

@end
