/*
    Initial author: Convery (tcn@hedgehogscience.com)
    Started: 31-01-2018
    License: MIT
    Notes:
        Provides notification tracking.
*/

#include "../Stdinclude.hpp"

namespace Backend
{
    namespace Notification
    {
        // Internal state.
        std::queue<MQEL_json> Localqueue;
        std::queue<MQEL_json> Globalqueue;

        // Notification tracking.
        std::vector<MQEL_json> Dequeuelocals()
        {
            std::vector<MQEL_json> Result;

            while (!Localqueue.empty())
            {
                auto Local = Localqueue.front();
                Result.push_back(Local);
                Localqueue.pop();
            }

            return std::move(Result);
        }
        std::vector<MQEL_json> Dequeueglobals()
        {
            std::vector<MQEL_json> Result;

            while (!Globalqueue.empty())
            {
                auto Global = Globalqueue.front();
                Result.push_back(Global);
                Globalqueue.pop();
            }

            return std::move(Result);
        }
        void Enqueuelocal(MQEL_json Notification)
        {
            Localqueue.push(Notification);
        }
        void Enqueueglobal(MQEL_json Notification)
        {
            Globalqueue.push(Notification);
        }
    }
}
