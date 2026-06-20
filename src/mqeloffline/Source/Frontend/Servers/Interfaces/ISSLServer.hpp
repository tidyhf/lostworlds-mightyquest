/*
    Initial author: Convery (tcn@hedgehogscience.com)
    Started: 09-01-2018
    License: MIT
    Notes:
        Provides an implementation of SSL over TCP parsing.
*/

#pragma once
#if __has_include(<openssl\ssl.h>)
#include "../../Stdinclude.hpp"
#include <openssl\bio.h>
#include <openssl\ssl.h>
#include <openssl\err.h>

struct ISSLServer : IStreamserver
{
    std::unordered_map<size_t, SSL_CTX *> Context;
    std::unordered_map<size_t, BIO *> Write_BIO;
    std::unordered_map<size_t, BIO *> Read_BIO;
    std::unordered_map<size_t, SSL *> State;
    X509 *SSLCertificate;
    EVP_PKEY *SSLKey;

    // SSL helper.
    virtual void Syncbuffers(const size_t Socket)
    {
        auto Buffer = std::make_unique<uint8_t []>(4096 * 1024);
        auto Readcount = BIO_read(Write_BIO[Socket], Buffer.get(), 4096 * 1024);
        if (Readcount > 0) IStreamserver::Send(Socket, Buffer.get(), Readcount);
    }
    virtual bool CreateSSLCert(std::string_view Hostname)
    {
        do
        {
            // Initialize OpenSSL.
            OpenSSL_add_ssl_algorithms();
            SSL_load_error_strings();

            // Allocate the PKEY.
            SSLKey = EVP_PKEY_new();
            if (!SSLKey) break;

            // Create the RSA key.
            RSA *RSAKey = RSA_new();
            BIGNUM *Exponent = BN_new();
            if (!BN_set_word(Exponent, 65537)) break;
            if (!RSA_generate_key_ex(RSAKey, 2048, Exponent, NULL)) break;
            if (!EVP_PKEY_assign_RSA(SSLKey, RSAKey)) break;

            // Allocate the x509 cert.
            SSLCertificate = X509_new();
            if (!SSLCertificate) break;

            // Cert details, valid for a year.
            X509_gmtime_adj(X509_get_notBefore(SSLCertificate), 0);
            ASN1_INTEGER_set(X509_get_serialNumber(SSLCertificate), 1);
            X509_gmtime_adj(X509_get_notAfter(SSLCertificate), 31536000L);

            // Set the public key.
            X509_set_pubkey(SSLCertificate, SSLKey);

            // Name information.
            X509_NAME *Name = X509_get_subject_name(SSLCertificate);
            X509_NAME_add_entry_by_txt(Name, "C",  MBSTRING_ASC, (unsigned char *)"SE", -1, -1, 0);
            X509_NAME_add_entry_by_txt(Name, "O",  MBSTRING_ASC, (unsigned char *)"Hedgehogscience", -1, -1, 0);
            X509_NAME_add_entry_by_txt(Name, "CN", MBSTRING_ASC, (unsigned char *)Hostname.data(), (int)Hostname.size(), -1, 0);

            // Set the issuer name.
            X509_set_issuer_name(SSLCertificate, Name);

            // Sign the certificate with the key.
            if (!X509_sign(SSLCertificate, SSLKey, EVP_sha1()))
            {
                X509_free(SSLCertificate);
                SSLCertificate = nullptr;
                EVP_PKEY_free(SSLKey);
                SSLKey = nullptr;
                break;
            }

            return true;
        } while (false);

        Infoprint(va("Failed to create a SSL certificate for \"%*s\"", Hostname.size(), Hostname.data()));
        SSLCertificate = nullptr;
        SSLKey = nullptr;
        return false;
    }

    // Usercode interactions.
    virtual void onStreamdecrypted(const size_t Socket, std::vector<uint8_t> &Stream) = 0;
    virtual void Send(const size_t Socket, const void *Databuffer, const uint32_t Datasize)
    {
        auto Lambda = [&](const size_t lSocket) -> void
        {
            SSL_write(State[lSocket], Databuffer, Datasize);
        };

        // If there is a socket, just enqueue to its stream.
        if (0 != Socket) return Lambda(Socket);

        // Else we treat it as a broadcast request.
        for (auto &Item : Validconnection)
            if (Item.second == true)
                Lambda(Item.first);
    }
    virtual void Send(const size_t Socket, std::string &Databuffer)
    {
        return Send(Socket, Databuffer.data(), uint32_t(Databuffer.size()));
    }
    virtual void onData(const size_t Socket, std::vector<uint8_t> &Stream)
    {
        int Readcount;
        int Writecount;

        // Insert the data into the SSL buffer.
        Writecount = BIO_write(Read_BIO[Socket], Stream.data(), (int)Stream.size());

        if (!SSL_is_init_finished(State[Socket]))
        {
            SSL_do_handshake(State[Socket]);
        }
        else
        {
            auto Buffer = std::make_unique<uint8_t []>(4096 * 1024);
            Readcount = SSL_read(State[Socket], Buffer.get(), 4096 * 1024);

            // Check errors.
            if (Readcount == 0)
            {
                size_t Error = SSL_get_error(State[Socket], 0);
                if (Error == SSL_ERROR_ZERO_RETURN)
                {
                    // Remake the SSL state.
                    {
                        SSL_free(State[Socket]);

                        Write_BIO[Socket] = BIO_new(BIO_s_mem());
                        Read_BIO[Socket] = BIO_new(BIO_s_mem());
                        BIO_set_nbio(Write_BIO[Socket], 1);
                        BIO_set_nbio(Read_BIO[Socket], 1);

                        State[Socket] = SSL_new(Context[Socket]);
                        if(!State[Socket]) Infoprint("OpenSSL error: Failed to create the SSL state.");

                        SSL_set_bio(State[Socket], Read_BIO[Socket], Write_BIO[Socket]);
                        SSL_set_verify(State[Socket], SSL_VERIFY_NONE, NULL);
                        SSL_set_accept_state(State[Socket]);
                    }

                    Stream.clear();
                    return;
                }
            }

            if (Readcount > 0)
            {
                std::vector<uint8_t> Request(Buffer.get(), Buffer.get() + Readcount);
                onStreamdecrypted(Socket, Request);
            }
        }

        Stream.erase(Stream.begin(), Stream.begin() + Writecount);
        Threadguard.unlock();
        Syncbuffers(Socket);
    }

    // Stream-based IO for protocols such as TCP.
    virtual void onDisconnect(const size_t Socket)
    {
        IStreamserver::onDisconnect(Socket);
        Write_BIO.erase(Socket);
        Read_BIO.erase(Socket);
        Context.erase(Socket);
        State.erase(Socket);
    }
    virtual void onConnect(const size_t Socket, const uint16_t Port)
    {
        unsigned long Resultcode;
        IStreamserver::onConnect(Socket, Port);

        // Initialize the context.
        {
            Context[Socket] = SSL_CTX_new(SSLv23_server_method());
            SSL_CTX_set_verify(Context[Socket], SSL_VERIFY_NONE, NULL);

            SSL_CTX_set_options(Context[Socket], SSL_OP_SINGLE_DH_USE);
            SSL_CTX_set_ecdh_auto(Context[Socket], 1);

            uint8_t ssl_context_id[16]{ 2, 3, 4, 5, 6 };
            SSL_CTX_set_session_id_context(Context[Socket], (const unsigned char *)&ssl_context_id, sizeof(ssl_context_id));
        }

        // Load the certificate and key for this server.
        {
            Resultcode = SSL_CTX_use_certificate(Context[Socket], SSLCertificate);
            if (Resultcode != 1) Infoprint(va("OpenSSL error: %s", ERR_error_string(Resultcode, NULL)).c_str());

            Resultcode = SSL_CTX_use_PrivateKey(Context[Socket], SSLKey);
            if (Resultcode != 1) Infoprint(va("OpenSSL error: %s", ERR_error_string(Resultcode, NULL)).c_str());

            Resultcode = SSL_CTX_check_private_key(Context[Socket]);
            if (Resultcode != 1) Infoprint(va("OpenSSL error: %s", ERR_error_string(Resultcode, NULL)).c_str());
        }

        // Create the BIO buffers.
        {
            Write_BIO[Socket] = BIO_new(BIO_s_mem());
            Read_BIO[Socket] = BIO_new(BIO_s_mem());
            BIO_set_nbio(Write_BIO[Socket], 1);
            BIO_set_nbio(Read_BIO[Socket], 1);
        }

        // Initialize the SSL state.
        {
            State[Socket] = SSL_new(Context[Socket]);
            if (!State[Socket]) Infoprint("OpenSSL error: Failed to create the SSL state.");

            SSL_set_bio(State[Socket], Read_BIO[Socket], Write_BIO[Socket]);
            SSL_set_verify(State[Socket], SSL_VERIFY_NONE, NULL);
            SSL_set_accept_state(State[Socket]);
        }
    }
};

#endif
