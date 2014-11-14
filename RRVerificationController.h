/* RRVerificationController.h
 *
 *  Created by Evan Schoenberg on 7/20/12.
 *  Copyright 2012 Regular Rate and Rhythm Software. No rights reserved.
 *
 * Completed and fleshed out implementation of Apple's VerificationController, the Companion File to
 * TP40012484 ("In-App Purchase Receipt Validation on iOS" -  bit.ly/QiosJw)
 *
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
 
 *  See README.md for implementation notes
 */

#import <Foundation/Foundation.h>
#import <StoreKit/StoreKit.h>

#define ITMS_PROD_VERIFY_RECEIPT_URL        @"https://buy.itunes.apple.com/verifyReceipt"
#define ITMS_SANDBOX_VERIFY_RECEIPT_URL     @"https://sandbox.itunes.apple.com/verifyReceipt"

#ifdef DEBUG_BUILD
	// Use ITMS_SANDBOX_VERIFY_RECEIPT_URL while testing against the sandbox.
	#define ITMS_ACTIVE_VERIFY_RECEIPT_URL	ITMS_SANDBOX_VERIFY_RECEIPT_URL
#else
	#define ITMS_ACTIVE_VERIFY_RECEIPT_URL	ITMS_PROD_VERIFY_RECEIPT_URL
#endif


@class RRTransactionVerifier;

@protocol RRVerificationControllerDelegate
/*!
 * @brief Verification with Apple's server completed successfully
 *
 * @param transaction The transaction being verified
 * @param isValid YES if Apple reported the transaction was valid; NO if Apple said it was not valid or if the server's validation reply was inconsistent with validity
 */
- (void)verificationControllerDidVerifyPurchase:(SKPaymentTransaction *)transaction isValid:(BOOL)isValid;


 /*!
  * @brief The attempt at verification could not be completed
  *
  * This does not mean that Apple reported the transaction was invalid, but
  * rather indicates a communication failure, a server error, or the like.
  *
  * @param transaction The transaction being verified
  * @param error An NSError describing the error. May be nil if the cause of the error was unknown (or if nobody has written code to report an NSError for that failure...)
  */
- (void)verificationControllerDidFailToVerifyPurchase:(SKPaymentTransaction *)transaction error:(NSError *)error;
@end

@interface RRVerificationController : NSObject {
    NSMutableDictionary *transactionsReceiptStorageDictionary;
	
	NSMutableArray *verificationsInProgress;
}

+ (RRVerificationController *) sharedInstance;

/* Must be set before attempting to verify. Generated via iTunes Connect -> Manage Apps -> your app -> Manage In App Purchases */
@property (strong) NSString *itcContentProviderSharedSecret;

/*!
 * @brief Verify a purchase
 *
 * This is an asynchronous process that requires checking with Apple's server to determine validity.
 * verificationDelegate will be sent verificationControllerDidVerifyPurchase:isValid: when verficiation is complete, if it begins at all
 *
 * This method should be called once a transaction gets to the SKPaymentTransactionStatePurchased or SKPaymentTransactionStateRestored state.
 *
 * @result YES if verification begins; NO if verification failed immediately, in which case the delegate will not be called.
 */
- (BOOL)verifyPurchase:(SKPaymentTransaction *)transaction withDelegate:(NSObject<RRVerificationControllerDelegate> *)verificationDelegate  error:(out NSError **)outError;

@end

@interface RRVerificationController (ForTransactionVerifierOnly)
- (void)transactionVerifier:(RRTransactionVerifier *)verifier didDetermineValidity:(BOOL)isValid;
- (void)transactionVerifier:(RRTransactionVerifier *)verifier didFailWithError:(NSError *)error;
@end
