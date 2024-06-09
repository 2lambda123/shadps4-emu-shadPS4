// SPDX-FileCopyrightText: Copyright 2024 shadPS4 Emulator Project
// SPDX-License-Identifier: GPL-2.0-or-later

#include <array>
#include "crypto.h"

CryptoPP::RSA::PrivateKey Crypto::key_pkg_derived_key3_keyset_init() {
    CryptoPP::InvertibleRSAFunction params;
    params.SetPrime1(CryptoPP::Integer(pkg_derived_key3_keyset.Prime1, 0x80));
    params.SetPrime2(CryptoPP::Integer(pkg_derived_key3_keyset.Prime2, 0x80));

    params.SetPublicExponent(CryptoPP::Integer(pkg_derived_key3_keyset.PublicExponent, 4));
    params.SetPrivateExponent(CryptoPP::Integer(pkg_derived_key3_keyset.PrivateExponent, 0x100));

    params.SetModPrime1PrivateExponent(CryptoPP::Integer(pkg_derived_key3_keyset.Exponent1, 0x80));
    params.SetModPrime2PrivateExponent(CryptoPP::Integer(pkg_derived_key3_keyset.Exponent2, 0x80));

    params.SetModulus(CryptoPP::Integer(pkg_derived_key3_keyset.Modulus, 0x100));
    params.SetMultiplicativeInverseOfPrime2ModPrime1(
        CryptoPP::Integer(pkg_derived_key3_keyset.Coefficient, 0x80));

    CryptoPP::RSA::PrivateKey privateKey(params);

    return privateKey;
}

CryptoPP::RSA::PrivateKey Crypto::FakeKeyset_keyset_init() {
    CryptoPP::InvertibleRSAFunction params;
    params.SetPrime1(CryptoPP::Integer(FakeKeyset_keyset.Prime1, 0x80));
    params.SetPrime2(CryptoPP::Integer(FakeKeyset_keyset.Prime2, 0x80));

    params.SetPublicExponent(CryptoPP::Integer(FakeKeyset_keyset.PublicExponent, 4));
    params.SetPrivateExponent(CryptoPP::Integer(FakeKeyset_keyset.PrivateExponent, 0x100));

    params.SetModPrime1PrivateExponent(CryptoPP::Integer(FakeKeyset_keyset.Exponent1, 0x80));
    params.SetModPrime2PrivateExponent(CryptoPP::Integer(FakeKeyset_keyset.Exponent2, 0x80));

    params.SetModulus(CryptoPP::Integer(FakeKeyset_keyset.Modulus, 0x100));
    params.SetMultiplicativeInverseOfPrime2ModPrime1(
        CryptoPP::Integer(FakeKeyset_keyset.Coefficient, 0x80));

    CryptoPP::RSA::PrivateKey privateKey(params);

    return privateKey;
}

CryptoPP::RSA::PrivateKey Crypto::DebugRifKeyset_init() {
    CryptoPP::InvertibleRSAFunction params;
    params.SetPrime1(
        CryptoPP::Integer(DebugRifKeyset_keyset.Prime1, sizeof(DebugRifKeyset_keyset.Prime1)));
    params.SetPrime2(
        CryptoPP::Integer(DebugRifKeyset_keyset.Prime2, sizeof(DebugRifKeyset_keyset.Prime2)));

    params.SetPublicExponent(CryptoPP::Integer(DebugRifKeyset_keyset.PrivateExponent,
                                               sizeof(DebugRifKeyset_keyset.PrivateExponent)));
    params.SetPrivateExponent(CryptoPP::Integer(DebugRifKeyset_keyset.PrivateExponent,
                                                sizeof(DebugRifKeyset_keyset.PrivateExponent)));

    params.SetModPrime1PrivateExponent(CryptoPP::Integer(DebugRifKeyset_keyset.Exponent1,
                                                         sizeof(DebugRifKeyset_keyset.Exponent1)));
    params.SetModPrime2PrivateExponent(CryptoPP::Integer(DebugRifKeyset_keyset.Exponent2,
                                                         sizeof(DebugRifKeyset_keyset.Exponent2)));

    params.SetModulus(
        CryptoPP::Integer(DebugRifKeyset_keyset.Modulus, sizeof(DebugRifKeyset_keyset.Modulus)));
    params.SetMultiplicativeInverseOfPrime2ModPrime1(CryptoPP::Integer(
        DebugRifKeyset_keyset.Coefficient, sizeof(DebugRifKeyset_keyset.Coefficient)));

    CryptoPP::RSA::PrivateKey privateKey(params);

    return privateKey;
}

void Crypto::RSA2048Decrypt(std::span<CryptoPP::byte, 32> dec_key,
                            std::span<const CryptoPP::byte, 256> ciphertext,
                            bool is_dk3) { // RSAES_PKCS1v15_
    // Create an RSA decryptor
    CryptoPP::RSA::PrivateKey privateKey;
    if (is_dk3) {
        privateKey = key_pkg_derived_key3_keyset_init();
    } else {
        privateKey = FakeKeyset_keyset_init();
    }

    CryptoPP::RSAES_PKCS1v15_Decryptor rsaDecryptor(privateKey);

    // Allocate memory for the decrypted data
    std::array<CryptoPP::byte, 256> decrypted;

    // Perform the decryption
    CryptoPP::AutoSeededRandomPool rng;
    CryptoPP::DecodingResult result =
        rsaDecryptor.Decrypt(rng, ciphertext.data(), decrypted.size(), decrypted.data());
    std::copy(decrypted.begin(), decrypted.begin() + dec_key.size(), dec_key.begin());
}

void Crypto::ivKeyHASH256(std::span<const CryptoPP::byte, 64> cipher_input,
                          std::span<CryptoPP::byte, 32> ivkey_result) {
    CryptoPP::SHA256 sha256;
    std::array<CryptoPP::byte, CryptoPP::SHA256::DIGESTSIZE> hashResult;
    auto array_sink = new CryptoPP::ArraySink(hashResult.data(), CryptoPP::SHA256::DIGESTSIZE);
    auto filter = new CryptoPP::HashFilter(sha256, array_sink);
    CryptoPP::ArraySource r(cipher_input.data(), cipher_input.size(), true, filter);
    std::copy(hashResult.begin(), hashResult.begin() + ivkey_result.size(), ivkey_result.begin());
}

void Crypto::aesCbcCfb128Decrypt(std::span<const CryptoPP::byte, 32> ivkey,
                                 std::span<const CryptoPP::byte, 256> ciphertext,
                                 std::span<CryptoPP::byte, 256> decrypted) {
    std::array<CryptoPP::byte, CryptoPP::AES::DEFAULT_KEYLENGTH> key;
    std::array<CryptoPP::byte, CryptoPP::AES::DEFAULT_KEYLENGTH> iv;

    std::copy(ivkey.begin() + 16, ivkey.begin() + 16 + key.size(), key.begin());
    std::copy(ivkey.begin(), ivkey.begin() + iv.size(), iv.begin());

    CryptoPP::AES::Decryption aesDecryption(key.data(), CryptoPP::AES::DEFAULT_KEYLENGTH);
    CryptoPP::CBC_Mode_ExternalCipher::Decryption cbcDecryption(aesDecryption, iv.data());

    for (size_t i = 0; i < decrypted.size(); i += CryptoPP::AES::BLOCKSIZE) {
        cbcDecryption.ProcessData(decrypted.data() + i, ciphertext.data() + i,
                                  CryptoPP::AES::BLOCKSIZE);
    }
}

void Crypto::aesCbcCfb128DecryptEntry(std::span<const CryptoPP::byte, 32> ivkey,
                                      std::span<CryptoPP::byte> ciphertext,
                                      std::span<CryptoPP::byte> decrypted) {
    std::array<CryptoPP::byte, CryptoPP::AES::DEFAULT_KEYLENGTH> key;
    std::array<CryptoPP::byte, CryptoPP::AES::DEFAULT_KEYLENGTH> iv;

    std::copy(ivkey.begin() + 16, ivkey.begin() + 16 + key.size(), key.begin());
    std::copy(ivkey.begin(), ivkey.begin() + iv.size(), iv.begin());

    CryptoPP::AES::Decryption aesDecryption(key.data(), CryptoPP::AES::DEFAULT_KEYLENGTH);
    CryptoPP::CBC_Mode_ExternalCipher::Decryption cbcDecryption(aesDecryption, iv.data());

    for (size_t i = 0; i < decrypted.size(); i += CryptoPP::AES::BLOCKSIZE) {
        cbcDecryption.ProcessData(decrypted.data() + i, ciphertext.data() + i,
                                  CryptoPP::AES::BLOCKSIZE);
    }
}

void Crypto::decryptEFSM(std::span<CryptoPP::byte, 16> NPcommID,
                         std::span<CryptoPP::byte, 16> efsmIv, std::span<CryptoPP::byte> ciphertext,
                         std::span<CryptoPP::byte> decrypted) {

    std::vector<CryptoPP::byte> TrophyKey = {0x21, 0xF4, 0x1A, 0x6B, 0xAD, 0x8A, 0x1D, 0x3E,
                                             0xCA, 0x7A, 0xD5, 0x86, 0xC1, 0x01, 0xB7, 0xA9};
    std::vector<CryptoPP::byte> TrophyIV = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
    // step 1: Encrypt NPcommID
    CryptoPP::CBC_Mode<CryptoPP::AES>::Encryption encrypt;
    encrypt.SetKeyWithIV(TrophyKey.data(), TrophyKey.size(), TrophyIV.data());

    std::vector<CryptoPP::byte> trpKey(16);

    encrypt.ProcessData(trpKey.data(), NPcommID.data(), 16);
    // step 2: decrypt efsm.
    CryptoPP::CBC_Mode<CryptoPP::AES>::Decryption decrypt;
    decrypt.SetKeyWithIV(trpKey.data(), trpKey.size(), efsmIv.data());

    for (size_t i = 0; i < decrypted.size(); i += CryptoPP::AES::BLOCKSIZE) {
        decrypt.ProcessData(decrypted.data() + i, ciphertext.data() + i, CryptoPP::AES::BLOCKSIZE);
    }
}

void Crypto::PfsGenCryptoKey(std::span<const CryptoPP::byte, 32> ekpfs,
                             std::span<const CryptoPP::byte, 16> seed,
                             std::span<CryptoPP::byte, 16> dataKey,
                             std::span<CryptoPP::byte, 16> tweakKey) {
    CryptoPP::HMAC<CryptoPP::SHA256> hmac(ekpfs.data(), ekpfs.size());

    CryptoPP::SecByteBlock d(20); // Use Crypto++ SecByteBlock for better memory management

    // Copy the bytes of 'index' to the 'd' array
    uint32_t index = 1;
    std::memcpy(d, &index, sizeof(uint32_t));

    // Copy the bytes of 'seed' to the 'd' array starting from index 4
    std::memcpy(d + sizeof(uint32_t), seed.data(), seed.size());

    // Allocate memory for 'u64' using new
    std::vector<CryptoPP::byte> data_tweak_key(hmac.DigestSize());

    // Calculate the HMAC
    hmac.CalculateDigest(data_tweak_key.data(), d, d.size());
    std::copy(data_tweak_key.begin(), data_tweak_key.begin() + dataKey.size(), tweakKey.begin());
    std::copy(data_tweak_key.begin() + tweakKey.size(),
              data_tweak_key.begin() + tweakKey.size() + dataKey.size(), dataKey.begin());
}

void Crypto::decryptPFS(std::span<const CryptoPP::byte, 16> dataKey,
                        std::span<const CryptoPP::byte, 16> tweakKey, std::span<const u8> src_image,
                        std::span<CryptoPP::byte> dst_image, u64 sector) {
    // Start at 0x10000 to keep the header when decrypting the whole pfs_image.
    for (int i = 0; i < src_image.size(); i += 0x1000) {
        const u64 current_sector = sector + (i / 0x1000);
        CryptoPP::ECB_Mode<CryptoPP::AES>::Encryption encrypt(tweakKey.data(), tweakKey.size());
        CryptoPP::ECB_Mode<CryptoPP::AES>::Decryption decrypt(dataKey.data(), dataKey.size());

        std::array<CryptoPP::byte, 16> tweak{};
        std::array<CryptoPP::byte, 16> encryptedTweak;
        std::array<CryptoPP::byte, 16> xorBuffer;
        std::memcpy(tweak.data(), &current_sector, sizeof(u64));

        // Encrypt the tweak for each sector.
        encrypt.ProcessData(encryptedTweak.data(), tweak.data(), 16);

        for (int plaintextOffset = 0; plaintextOffset < 0x1000; plaintextOffset += 16) {
            xtsXorBlock(xorBuffer.data(), src_image.data() + i + plaintextOffset,
                        encryptedTweak.data());                          // x, c, t
            decrypt.ProcessData(xorBuffer.data(), xorBuffer.data(), 16); // x, x
            xtsXorBlock(dst_image.data() + i + plaintextOffset, xorBuffer.data(),
                        encryptedTweak.data()); //(p)  c, x , t
            xtsMult(encryptedTweak);
        }
    }
}
