/*
    Initial author: Convery (tcn@hedgehogscience.com)
    Started: 09-01-2018
    License: MIT
    Notes:
        Provides a packet-based form of IO.
*/

#pragma once
#include "IServer.hpp"
#include <algorithm>
#include <string>
#include <queue>
#include <mutex>

struct IDatagramserver : IServer
{
    std::queue<std::string> Packetqueue;
    Address_t Hostinformation{};
    std::mutex Threadguard;

    // Usercode interaction.
    virtual void Send(std::string Databuffer)
    {
        // Enqueue the packet at the end of the queue.
        Threadguard.lock();
        {
            Packetqueue.push(Databuffer);
        }
        Threadguard.unlock();
    }
    virtual void Send(const void *Databuffer, const uint32_t Datasize)
    {
        return Send({ reinterpret_cast<const char *>(Databuffer), Datasize });
    }
    virtual void onData(const std::string &Packet) = 0;

    // Returns false if the request could not be completed for any reason.
    virtual bool onPacketread(Address_t &Server, void *Databuffer, uint32_t *Datasize)
    {
        // If there's no packets, return instantly.
        if (Packetqueue.empty()) return false;

        // Verify the pointers, although they should always be valid.
        if (!Databuffer || !Datasize) return false;

        // Copy the packet to the buffer.
        Threadguard.lock();
        {
            // Validate the state, unlikely to change.
            if (!Packetqueue.empty())
            {
                // Get the first packet in the queue.
                auto Packet = Packetqueue.front();
                Packetqueue.pop();

                // Copy as much data as we can fit in the buffer.
                *Datasize = std::min(*Datasize, uint32_t(Packet.size()));
                std::copy_n(Packet.begin(), *Datasize, reinterpret_cast<char *>(Databuffer));

                // Set the servers address.
                std::memcpy(&Server, &Hostinformation, sizeof(Address_t));
            }
        }
        Threadguard.unlock();

        return true;
    }
    virtual bool onPacketwrite(const Address_t &Server, const void *Databuffer, const uint32_t Datasize)
    {
        // Pass the packet to the usercode callback.
        Threadguard.lock();
        {
            // Update the servers address.
            std::memcpy(&Hostinformation, &Server, sizeof(Address_t));

            // Create a new string and let the compiler optimize it out.
            auto Pointer = reinterpret_cast<const char *>(Databuffer);
            auto Packet = std::string(Pointer, Datasize);
            onData(Packet);

            // Ensure that the mutex is locked as usercode is unpredictable.
            auto Discarded = Threadguard.try_lock();
            (void)Discarded;
        }
        Threadguard.unlock();

        return true;
    }

    // Nullsub the unused callbacks.
    virtual void onDisconnect(const size_t Socket)
    {
        (void)Socket;
    }
    virtual void onConnect(const size_t Socket, const uint16_t Port)
    {
        (void)Port;
        (void)Socket;
    }
    virtual bool onStreamread(const size_t Socket, void *Databuffer, uint32_t *Datasize)
    {
        (void)Socket;
        (void)Datasize;
        (void)Databuffer;

        return false;
    }
    virtual bool onStreamwrite(const size_t Socket, const void *Databuffer, const uint32_t Datasize)
    {
        (void)Socket;
        (void)Datasize;
        (void)Databuffer;

        return false;
    }
};
