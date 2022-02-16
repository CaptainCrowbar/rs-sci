#include "rs-sci/hash.hpp"

#if defined(__APPLE__)
    #pragma GCC diagnostic ignored "-Wdeprecated-declarations"
    #include <CommonCrypto/CommonDigest.h>
#elif defined(_XOPEN_SOURCE)
    #include <openssl/md5.h>
    #include <openssl/sha.h>
#else
    #include <windows.h>
    #include <wincrypt.h>
#endif

namespace RS::Sci {

    // Cryptographic hash functions

    #if defined(__APPLE__)

        #define HASH_CONTEXT(unix_fn_prefix, unix_ctx_prefix)  CC_ ## unix_fn_prefix ## _CTX context;
        #define HASH_INIT(unix_fn_prefix, win_fn_suffix)       CC_ ## unix_fn_prefix ## _Init(&impl_->context);
        #define HASH_UPDATE(unix_fn_prefix)                    CC_ ## unix_fn_prefix ## _Update(&impl_->context, ptr, uint32_t(len));
        #define HASH_FINAL(unix_fn_prefix)                     CC_ ## unix_fn_prefix ## _Final(data(), &impl_->context);

    #elif defined(_XOPEN_SOURCE)

        #define HASH_CONTEXT(unix_fn_prefix, unix_ctx_prefix)  unix_ctx_prefix ## _CTX context;
        #define HASH_INIT(unix_fn_prefix, win_fn_suffix)       unix_fn_prefix ## _Init(&impl_->context);
        #define HASH_UPDATE(unix_fn_prefix)                    unix_fn_prefix ## _Update(&impl_->context, ptr, uint32_t(len));
        #define HASH_FINAL(unix_fn_prefix)                     unix_fn_prefix ## _Final(data(), &impl_->context);

    #else

        #define HASH_CONTEXT(unix_fn_prefix, unix_ctx_prefix) \
            HCRYPTPROV hcprov = 0; \
            HCRYPTHASH hchash = 0; \
            DWORD hashlen = sizeof(result_type);
        #define HASH_INIT(unix_fn_prefix, win_fn_suffix) \
            CryptAcquireContextW(&impl_->hcprov, nullptr, MS_ENH_RSA_AES_PROV, PROV_RSA_AES, CRYPT_SILENT | CRYPT_VERIFYCONTEXT); \
            CryptCreateHash(impl_->hcprov, CALG_ ## win_fn_suffix, 0, 0, &impl_->hchash);
        #define HASH_UPDATE(unix_fn_prefix) \
            CryptHashData(impl_->hchash, static_cast<const uint8_t*>(ptr), uint32_t(len), 0);
        #define HASH_FINAL(unix_fn_prefix) \
            CryptGetHashParam(impl_->hchash, HP_HASHVAL, data(), &impl_->hashlen, 0); \
            CryptDestroyHash(impl_->hchash); \
            CryptReleaseContext(impl_->hcprov, 0);

    #endif

    #define IMPLEMENT_CRYPTOGRAPHIC_HASH_CLASS(ClassName, unix_fn_prefix, unix_ctx_prefix, win_fn_suffix) \
        struct ClassName::impl_type { \
            HASH_CONTEXT(unix_fn_prefix, unix_ctx_prefix) \
        }; \
        void ClassName::add(const void* ptr, size_t len) { \
            if (! impl_) { \
                impl_ = new impl_type; \
                HASH_INIT(unix_fn_prefix, win_fn_suffix) \
            } \
            HASH_UPDATE(unix_fn_prefix) \
        } \
        void ClassName::done() noexcept { \
            if (impl_) { \
                HASH_FINAL(unix_fn_prefix) \
                delete impl_; \
                impl_ = nullptr; \
            } \
        }

    IMPLEMENT_CRYPTOGRAPHIC_HASH_CLASS(Md5, MD5, MD5, MD5)
    IMPLEMENT_CRYPTOGRAPHIC_HASH_CLASS(Sha1, SHA1, SHA, SHA1)
    IMPLEMENT_CRYPTOGRAPHIC_HASH_CLASS(Sha256, SHA256, SHA256, SHA_256)
    IMPLEMENT_CRYPTOGRAPHIC_HASH_CLASS(Sha512, SHA512, SHA512, SHA_512)

}
