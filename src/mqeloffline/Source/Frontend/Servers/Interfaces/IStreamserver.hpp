/*
    Initial author: Convery (tcn@hedgehogscience.com)
    Started: 09-01-2018
    License: MIT
    Notes:
        Provides a stream-based form of IO.
*/

#pragma once
#include <unordered_map>
#include "IServer.hpp"
#include <algorithm>
#include <vector>
#include <mutex>

struct IStreamserver : IServer
{
    // Per socket state-information where the Berkeley socket is the key.
    std::unordered_map<size_t, std::vector<uint8_t>> Incomingstream;
    std::unordered_map<size_t, std::vector<uint8_t>> Outgoingstream;
    std::unordered_map<size_t, bool> Validconnection;
    std::mutex Threadguard;

    // Usercode interaction.
    virtual void Send(const size_t Socket, const void *Databuffer, const uint32_t Datasize)
    {
        auto Lambda = [&](const size_t lSocket) -> void
        {
            // Enqueue the data at the end of the stream.
            Threadguard.lock();
            {
                auto Pointer = reinterpret_cast<const uint8_t *>(Databuffer);
                std::copy_n(Pointer, Datasize, std::back_inserter(Outgoingstream[lSocket]));
            }
            Threadguard.unlock();
        };

        // If there is a socket, just enqueue to its stream.
        if (0 != Socket) return Lambda(Socket);

        // Else we treat it as a broadcast request.
        for (auto &Item : Validconnection)
            if (Item.second == true)
                Lambda(Item.first);
    }
    virtual void Send(const size_t Socket, std::string Databuffer)
    {
        return Send(Socket, Databuffer.data(), uint32_t(Databuffer.size()));
    }
    virtual void onData(const size_t Socket, std::vector<uint8_t> &Stream) = 0;

    // Stream-based IO for protocols such as TCP.
    virtual void onDisconnect(const size_t Socket)
    {
        Threadguard.lock();
        {
            // Clear the incoming stream, but keep the outgoing.
            Incomingstream[Socket].clear();
            Incomingstream[Socket].shrink_to_fit();

            // Set the connection-state.
            Validconnection[Socket] = false;
        }
        Threadguard.unlock();
    }
    virtual void onConnect(const size_t Socket, const uint16_t Port)
    {
        Threadguard.lock();
        {
            // Clear the streams to be ready for new data.
            Incomingstream[Socket].clear();
            Outgoingstream[Socket].clear();

            // Set the connection-state.
            Validconnection[Socket] = true;
        }
        Threadguard.unlock();
    }
    virtual bool onStreamread(const size_t Socket, void *Databuffer, uint32_t *Datasize)
    {
        // To support lingering sockets, we transmit data even if the socket is disconnected.
        if (0 == Outgoingstream[Socket].size()) return false;

        // Verify the pointers, although they should always be valid.
        if (!Databuffer || !Datasize) return false;

        // Copy the stream to the buffer.
        Threadguard.lock();
        {
            // Validate the state, unlikely to change.
            if (0 != Outgoingstream[Socket].size())
            {
                // Copy as much data as we can fit in the buffer.
                *Datasize = std::min(*Datasize, uint32_t(Outgoingstream[Socket].size()));
                std::copy_n(Outgoingstream[Socket].begin(), *Datasize, reinterpret_cast<char *>(Databuffer));
                Outgoingstream[Socket].erase(Outgoingstream[Socket].begin(), Outgoingstream[Socket].begin() + *Datasize);
            }
        }
        Threadguard.unlock();

        return true;
    }
    virtual bool onStreamwrite(const size_t Socket, const void *Databuffer, const uint32_t Datasize)
    {
        // If there is no valid connection, we just ignore the data.
        if (Validconnection[Socket] == false) return false;

        // Append the data to the stream and notify usercode.
        Threadguard.lock();
        {
            auto Pointer = reinterpret_cast<const uint8_t *>(Databuffer);
            std::copy_n(Pointer, Datasize, std::back_inserter(Incomingstream[Socket]));
            onData(Socket, Incomingstream[Socket]);

            // Ensure that the mutex is locked as usercode is unpredictable.
            auto Discarded = Threadguard.try_lock();
            (void)Discarded;
        }
        Threadguard.unlock();

        return true;
    }

    // Nullsub the unused callbacks.
    virtual bool onPacketread(Address_t &Server, void *Databuffer, uint32_t *Datasize)
    {
        (void)Server;
        (void)Datasize;
        (void)Databuffer;

        return false;
    }
    virtual bool onPacketwrite(const Address_t &Server, const void *Databuffer, const uint32_t Datasize)
    {
        (void)Server;
        (void)Datasize;
        (void)Databuffer;

        return false;
    }
};
