#import <UIKit/UIKit.h>
#import <BraintreeCore/BraintreeCore.h>
#import <BraintreeCard/BraintreeCard.h>
#import "BTThreeDSecureTokenizedCard.h"

BT_ASSUME_NONNULL_BEGIN

@protocol BTThreeDSecureDriverDelegate;

///  3D Secure Verification Driver
///
///  3D Secure is a protocol that enables cardholders and issuers to add a layer of security
///  to e-commerce transactions via password entry at checkout.
///
///  One of the primary reasons to use 3D Secure is to benefit from a shift in liability from the
///  merchant to the issuer, which may result in interchange savings. Please read our online
///  documentation (https://developers.braintreepayments.com/ios/guides/3d-secure) for a full explanation of 3D Secure.
///
///  After initializing this class with a Braintree client and delegate, you may verify Braintree
///  payment methods via the verifyCardWithNonce:amount: method. During verification, the delegate
///  may receive a request to present a view controller, as well as a success and failure messages.
///
///  Verification is associated with a transaction amount and your merchant account. To specify a
///  different merchant account, you will need to specify the merchant account id
///  when generating a client token (See https://developers.braintreepayments.com/ios/sdk/overview/generate-client-token ).
///
///  Your delegate must implement:
///    * paymentDriver:didCreatePaymentMethod:
///    * paymentDriver:didFailWithError:
///    * paymentDriver:requestsPresentationOfViewController:
///    * paymentDriver:requestsDismissalOfViewController:
///
///  When verification succeeds, the original payment method nonce is consumed, and you will receive
///  a new payment method nonce, which points to the original payment method, as well as the 3D
///  Secure Verification. Transactions created with this nonce are eligible for 3D Secure
///  liability shift.
///
///  When verification fails, the original payment method nonce is not consumed. While you may choose
///  to proceed with transaction creation, using the original payment method nonce, this transaction
///  will not be associated with a 3D Secure Verification.
///
///  @note The user authentication view controller is not always necessary to achieve the liabilty
///  shift. In these cases, your delegate will immediately receive paymentDriver:didCreatePaymentMethod:.

@interface BTThreeDSecureDriver : NSObject

///  Initializes a 3D Secure verification manager
///
///  @param apiClient The Braintree API Client
///
///  @return An initialized instance of BTThreeDSecureDriver
//- (BT_NULLABLE instancetype)initWithAPIClient:(BTAPIClient *)apiClient NS_DESIGNATED_INITIALIZER;

- (instancetype)initWithAPIClient:(BTAPIClient *)apiClient delegate:(id<BTViewControllerPresentingDelegate>)delegate;

- (BT_NULLABLE instancetype)init __attribute__((unavailable("Please use initWithAPIClient: instead.")));

/// Verify a card for a 3D Secure transaction, referring to the card by raw payment method nonce
///
/// This method is useful for implementations where 3D Secure verification occurs after generating
/// a payment method nonce from a vaulted credit card on your backend
///
/// @note This method performs an asynchronous operation and may request presentation of a view
///       controller via the delegate. It is the caller's responsibility to present an activity
///       indication to the user in the meantime.
///
/// @param nonce  A payment method nonce
/// @param amount The amount of the transaction in the current merchant account's currency
/// @param completionBlock This completion will be invoked exactly once when authorization is complete, is cancelled, or an error occurs.
/// On success, you will receive an instance of `BTTokenizedCard`. Typically, an implementation will send this tokenized card to your own
/// server for further use.
/// On failure, you will receive an error.
///
/// A failure may occur at any point during tokenization:
/// - Payment authorization is initiated with an incompatible configuration (e.g. no authorization
///   mechanism possible for specified provider)
/// - An authorization provider encounters an error
/// - A network or gateway error occurs
/// - The user-provided credentials led to a non-transactable payment method.
///
/// On user cancellation, you will receive `nil` for both parameters.
- (void)verifyCardWithNonce:(NSString *)nonce
                     amount:(NSDecimalNumber *)amount
                 completion:(void (^)(BTThreeDSecureTokenizedCard * __BT_NULLABLE tokenizedCard, NSError * __BT_NULLABLE error))completionBlock;

#pragma mark - Delegate

/// A delegate that presents and dismisses a view controller, as necessary, for the 3D Secure verification flow.
@property (nonatomic, weak) id<BTViewControllerPresentingDelegate> delegate;

@end

BT_ASSUME_NONNULL_END