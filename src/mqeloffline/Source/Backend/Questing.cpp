/*
    Initial author: Convery (tcn@hedgehogscience.com)
    Started: 31-01-2018
    License: MIT
    Notes:
        Provides quest tracking and management.
*/

#include "../Stdinclude.hpp"

namespace Backend
{
    namespace Quest
    {
        // The clients progress.
        std::unordered_map<uint32_t /* QuestID */, bool /* Complete */> *Questmap;

        // Notify the frontend when successful.
        void Notifyassignment(uint32_t QuestID, uint32_t ActionID)
        {
            MQEL_json Notification = MQEL_json::object();
            Notification["$type"] = "HyperQuest.GameServer.Contracts.ServerAssignmentActionCompletedNotification, HyperQuest.GameServer.Contracts";

            Notification["AssignmentId"] = QuestID;
            Notification["AssignmentActionIndex"] = ActionID;

            Notification["NotificationType"] = 74;
            Backend::Notification::Enqueuelocal(Notification);
        }

        // Update the clients progress.
        void Start(uint32_t QuestID)
        {
            Infoprint(va("Client started quest %i", QuestID));
            (*Questmap)[QuestID] = false;
        }
        void Complete(uint32_t QuestID)
        {
            Infoprint(va("Client completed quest %i", QuestID));
            (*Questmap)[QuestID] = true;
        }
        std::vector<uint32_t> Getcompleted()
        {
            std::vector<uint32_t> Result;

            for (auto &Item : *Questmap)
            {
                if (Item.second)
                {
                    Result.push_back(Item.first);
                }
            }

            return Result;
        }
        void Update(uint32_t QuestID, uint32_t ActionID)
        {
            Infoprint(va("Client modified quest %i with action %i", QuestID, ActionID));
            Notifyassignment(QuestID, ActionID);

            /* TODO(Convery): Reverse-engineer the actions. */
        }

        // Load the quest progress on startup.
        void Savequests()
        {
            // Serialize the map.
            Bytebuffer Writer;
            for (auto &Item : *Questmap)
            {
                Writer.Write(Item.first);
                Writer.Write(Item.second);
            }

            // Save to the archive.
            std::string Buffer((char *)Writer.Data(), Writer.Size());
            Package::Write("Quests.BB", Buffer);
        }
        void Loadquests()
        {
            // Initialize the map on startup.
            if (Questmap == nullptr)
            {
                Questmap = new std::unordered_map<uint32_t, bool>();
            }

            // Save the map on exit.
            std::atexit(Savequests);

            // Load the file from the archive.
            auto Filebuffer = Package::Read("Quests.BB");
            if (Filebuffer.size() == 0) return;

            // Deserialize the buffer.
            Bytebuffer Reader(Filebuffer);
            while (true)
            {
                uint32_t QuestID;
                bool Status;

                if (!Reader.Read(QuestID)) break;
                if (!Reader.Read(Status)) break;

                (*Questmap)[QuestID] = Status;
            }
        }
        namespace { struct Startup { Startup() { Loadquests(); }; }; static Startup Loader{}; }
    }
}
