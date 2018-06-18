Shared Libraries
================

## susucoinconsensus

The purpose of this library is to make the verification functionality that is critical to Susucoin's consensus available to other applications, e.g. to language bindings.

### API

The interface is defined in the C header `susucoinconsensus.h` located in  `src/script/susucoinconsensus.h`.

#### Version

`susucoinconsensus_version` returns an `unsigned int` with the API version *(currently at an experimental `0`)*.

#### Script Validation

`susucoinconsensus_verify_script` returns an `int` with the status of the verification. It will be `1` if the input script correctly spends the previous output `scriptPubKey`.

##### Parameters
- `const unsigned char *scriptPubKey` - The previous output script that encumbers spending.
- `unsigned int scriptPubKeyLen` - The number of bytes for the `scriptPubKey`.
- `const unsigned char *txTo` - The transaction with the input that is spending the previous output.
- `unsigned int txToLen` - The number of bytes for the `txTo`.
- `unsigned int nIn` - The index of the input in `txTo` that spends the `scriptPubKey`.
- `unsigned int flags` - The script validation flags *(see below)*.
- `susucoinconsensus_error* err` - Will have the error/success code for the operation *(see below)*.

##### Script Flags
- `susucoinconsensus_SCRIPT_FLAGS_VERIFY_NONE`
- `susucoinconsensus_SCRIPT_FLAGS_VERIFY_P2SH` - Evaluate P2SH ([BIP16](https://github.com/susucoin/bips/blob/master/bip-0016.mediawiki)) subscripts
- `susucoinconsensus_SCRIPT_FLAGS_VERIFY_DERSIG` - Enforce strict DER ([BIP66](https://github.com/susucoin/bips/blob/master/bip-0066.mediawiki)) compliance
- `susucoinconsensus_SCRIPT_FLAGS_VERIFY_NULLDUMMY` - Enforce NULLDUMMY ([BIP147](https://github.com/susucoin/bips/blob/master/bip-0147.mediawiki))
- `susucoinconsensus_SCRIPT_FLAGS_VERIFY_CHECKLOCKTIMEVERIFY` - Enable CHECKLOCKTIMEVERIFY ([BIP65](https://github.com/susucoin/bips/blob/master/bip-0065.mediawiki))
- `susucoinconsensus_SCRIPT_FLAGS_VERIFY_CHECKSEQUENCEVERIFY` - Enable CHECKSEQUENCEVERIFY ([BIP112](https://github.com/susucoin/bips/blob/master/bip-0112.mediawiki))
- `susucoinconsensus_SCRIPT_FLAGS_VERIFY_WITNESS` - Enable WITNESS ([BIP141](https://github.com/susucoin/bips/blob/master/bip-0141.mediawiki))

##### Errors
- `susucoinconsensus_ERR_OK` - No errors with input parameters *(see the return value of `susucoinconsensus_verify_script` for the verification status)*
- `susucoinconsensus_ERR_TX_INDEX` - An invalid index for `txTo`
- `susucoinconsensus_ERR_TX_SIZE_MISMATCH` - `txToLen` did not match with the size of `txTo`
- `susucoinconsensus_ERR_DESERIALIZE` - An error deserializing `txTo`
- `susucoinconsensus_ERR_AMOUNT_REQUIRED` - Input amount is required if WITNESS is used

### Example Implementations
- [NSusucoin](https://github.com/NicolasDorier/NSusucoin/blob/master/NSusucoin/Script.cs#L814) (.NET Bindings)
- [node-libsusucoinconsensus](https://github.com/bitpay/node-libsusucoinconsensus) (Node.js Bindings)
- [java-libsusucoinconsensus](https://github.com/dexX7/java-libsusucoinconsensus) (Java Bindings)
- [susucoinconsensus-php](https://github.com/Bit-Wasp/susucoinconsensus-php) (PHP Bindings)
