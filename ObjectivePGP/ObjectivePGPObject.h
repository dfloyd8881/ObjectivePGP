//
//  Copyright (c) Marcin Krzyżanowski. All rights reserved.
//
//  THIS SOURCE CODE AND ANY ACCOMPANYING DOCUMENTATION ARE PROTECTED BY
//  INTERNATIONAL COPYRIGHT LAW. USAGE IS BOUND TO THE LICENSE AGREEMENT.
//  This notice may not be removed from this file.
//

#import <ObjectivePGP/PGPKey.h>
#import <ObjectivePGP/PGPKeyring.h>
#import <Foundation/Foundation.h>

NS_ASSUME_NONNULL_BEGIN

/**
 ObjectivePGP - The Leading OpenPGP Framework for iOS and macOS.
 This is the configuration object for framework-global settings.

 @note The ObjectivePGP shared object is a global, thread-safe key/value store.
 Use `setValue:forKey:` and `valueForKey:` or the subscripted variants to set/get properties.
 */
@interface ObjectivePGP : NSObject

/**
 The shared ObjectivePGP configuration instance.
 @note This is the default instance.
 */
@property (class, atomic, readonly) ObjectivePGP *sharedInstance;

/**
 Default, shared keyring instance. Not used internally.
 */
@property (class, atomic, readonly) PGPKeyring *defaultKeyring;

/**
 Read binary or armored (ASCII) PGP keys from the input.

 @param data Key data or keyring data.
 @return Array of read keys.
 */
+ (NSArray<PGPKey *> *)readKeysFromData:(NSData *)data NS_SWIFT_NAME(readKeys(from:));

/**
 Read binary or armored (ASCII) PGP keys from the input.

 @param path Path to the file with keys.
 @return Array of read keys.
 */
+ (NSArray<PGPKey *> *)readKeysFromFile:(NSString *)path NS_SWIFT_NAME(readKeys(from:));

/**
 Sign data using a given key. Use passphrase to unlock the key if needed.
 If `detached` is true, output with the signature only. Otherwise, return signed data in PGP format.

 @param data Input data.
 @param detached Whether result in only signature (not signed data)
 @param key Key to be used to sign.
 @param passphrase Optional. Passphrase for the `key`.
 @param error Optional. Error.
 @return Signed data, or `nil` if fail.
 */
+ (nullable NSData *)sign:(NSData *)data detached:(BOOL)detached usingKeys:(NSArray<PGPKey *> *)keys passphraseForKey:(nullable NSString * _Nullable(^NS_NOESCAPE)(PGPKey *key))passphraseBlock error:(NSError * __autoreleasing _Nullable *)error;

/**
 Verify signed data using given key.

 @param data Signed data.
 @param detached Detached signature data (Optional). If not provided, `data` is expected to be signed.
 @param keys Public keys. The provided keys should match the signatures.
 @param passphraseBlock Optional. Handler for passphrase protected keys. Return passphrase for a key in question.
 @param error Optional. Check error code for details about the error.
 @return YES on success.
 */
+ (BOOL)verify:(NSData *)data withSignature:(nullable NSData *)detached usingKeys:(NSArray<PGPKey *> *)keys passphraseForKey:(nullable NSString * _Nullable(^NS_NOESCAPE)(PGPKey *key))passphraseBlock error:(NSError * __autoreleasing _Nullable *)error;

/**
 Encrypt data using given keys. Output in binary.

 @param data Data to encrypt.
 @param sign Whether message should be encrypte and signed.
 @param keys Keys to use to encrypte `data`
 @param passphraseBlock Optional. Handler for passphrase protected keys. Return passphrase for a key in question.
 @param error Optional. Error.
 @return Encrypted data in requested format.

 @note Use `PGPArmor` to convert binary `data` format to the armored (ASCII) format:

 ```
 [[PGPArmor armored:data as:PGPArmorTypeMessage] dataUsingEncoding:NSUTF8StringEncoding];
 ```

 */
+ (nullable NSData *)encrypt:(NSData *)data addSignature:(BOOL)sign usingKeys:(NSArray<PGPKey *> *)keys passphraseForKey:(nullable NSString * _Nullable(^NS_NOESCAPE)(PGPKey *key))passphraseBlock error:(NSError * __autoreleasing _Nullable *)error;

/**
 Decrypt PGP encrypted data.

 @param data data to decrypt.
 @param keys private keys to use.
 @param passphraseBlock Optional. Handler for passphrase protected keys. Return passphrase for a key in question.
 @param verifySignature `YES` if should verify the signature used during encryption, if message is encrypted and signed.
 @param error Optional. Error.
 @return Decrypted data, or `nil` if failed.
 */
+ (nullable NSData *)decrypt:(NSData *)data usingKeys:(NSArray<PGPKey *> *)keys passphraseForKey:(nullable NSString * _Nullable(^NS_NOESCAPE)(PGPKey * _Nullable key))passphraseBlock verifySignature:(BOOL)verifySignature error:(NSError * __autoreleasing _Nullable *)error;

@end

NS_ASSUME_NONNULL_END
