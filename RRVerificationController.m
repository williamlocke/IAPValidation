/* RRVerificationController.m
 *
 *  Created by Evan Schoenberg on 7/20/12.
 *  Copyright 2012 Regular Rate and Rhythm Software. No rights reserved.
 *
 * Completed and fleshed out implementation of Apple's VerificationController, the Companion File to
 * TP40012484 ("In-App Purchase Receipt Validation on iOS" -  bit.ly/QiosJw)
 *
 * Distributed under the 3-Clause BSD License with a modification not to necessitate attribution in binary distributions. Feel free to give me credit or send brownies if you'd like:
 
 Redistribution and use in source and binary forms, with or without
 modification, are permitted provided that the following conditions are
 met:
 
 (1) Redistributions of source code must retain the above copyright
 notice, this list of conditions and the following disclaimer.
 
 (2) Redistributions in binary form MAY reproduce the above copyright
 notice, this list of conditions and the following disclaimer in
 the documentation and/or other materials provided with the
 distribution.
 
 (3)The name of the author may not be used to
 endorse or promote products derived from this software without
 specific prior written permission.
 
 THIS SOFTWARE IS PROVIDED BY THE AUTHOR ``AS IS'' AND ANY EXPRESS OR
 IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 DISCLAIMED. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY DIRECT,
 INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
 SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT,
 STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING
 IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 POSSIBILITY OF SUCH DAMAGE.
 *
 *  See README.md for implementation notes
 */

#if ! __has_feature(objc_arc)
#warning This file must be compiled with ARC. Use -fobjc-arc flag (or convert project to ARC).
#endif

#import "RRVerificationController.h"
#import "RRTransactionVerifier.h"

@implementation RRVerificationController

@synthesize itcContentProviderSharedSecret;

+ (RRVerificationController *)sharedInstance
{
	static RRVerificationController *singleton = nil;

	if (singleton == nil)
		singleton = [[RRVerificationController alloc] init];

	return singleton;
}


- (id)init
{
	if ((self = [super init])) {
		verificationsInProgress = [[NSMutableArray alloc] init];
	}

	return self;
}

#pragma mark Receipt Verification

#define RR_SYSTEM_VERSION_GREATER_THAN_OR_EQUAL_TO(v)  ([[[UIDevice currentDevice] systemVersion] compare:v options:NSNumericSearch] != NSOrderedAscending)

// This method should be called once a transaction gets to the SKPaymentTransactionStatePurchased or SKPaymentTransactionStateRestored state
- (BOOL)verifyPurchase:(SKPaymentTransaction *)transaction withDelegate:(NSObject<RRVerificationControllerDelegate> *)verificationDelegate error:(out NSError **)outError;
{
	if ((transaction.transactionState == SKPaymentTransactionStatePurchased) || (transaction.transactionState == SKPaymentTransactionStateRestored)) {
		RRTransactionVerifier *verifier = [[RRTransactionVerifier alloc] initWithPurchase:transaction delegate:verificationDelegate controller:self];
		[verificationsInProgress addObject:verifier];
		
		if (RR_SYSTEM_VERSION_GREATER_THAN_OR_EQUAL_TO(@"5.0")) {
			return [verifier beginVerificationWithError:outError];
		} else {
			/* On iOS < 5.0, simply reply that the transaction is valid on the next run loop */
			[self performSelector:@selector(returnValidForVerifier:) withObject:verifier afterDelay:0];
			return YES;
		}

	} else {
		/* Transaction wasn't in a state in which it can possibly be valid */
		if (outError)
			*outError = [NSError errorWithDomain:SKErrorDomain code:SKErrorPaymentInvalid userInfo:nil];
		return NO;
	}
}

- (void)transactionVerifier:(RRTransactionVerifier *)verifier didDetermineValidity:(BOOL)isValid
{
	[verifier.delegate verificationControllerDidVerifyPurchase:verifier.transaction isValid:isValid];
	[verificationsInProgress removeObject:verifier];
}

- (void)returnValidForVerifier:(RRTransactionVerifier *)verifier
{
	[self transactionVerifier:verifier didDetermineValidity:YES];
}

- (void)transactionVerifier:(RRTransactionVerifier *)verifier didFailWithError:(NSError *)error
{
	[verifier.delegate verificationControllerDidFailToVerifyPurchase:verifier.transaction error:error];
	[verificationsInProgress removeObject:verifier];
}


@end
