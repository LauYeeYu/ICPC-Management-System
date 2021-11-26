// 1347 ICPC Management System
// 2021.10.29 13:02 GMT+8:00
// Lau YeeYu, all rights reserved
// Using ISO cpp standard

#include <iostream>
#include <algorithm>
#include <set>
#include <string>
#include <unordered_map>

enum SubmitStatus {None, // For initializing or representing all status
                   Accepted,
                   Wrong_Answer,
                   Runtime_Error,
                   Time_Limit_Exceed};
int DurationTime;
int ProblemCount = 0;
bool StartState = false; // To indicate whether the competition starts
bool Frozen = false; // To indicate whether the rank list is frozen
bool NeedFlush = true; // Frozen ranking won't change, just one flush needed

void AddTeam();
void Start();
void Submit();
void Flush();
void Freeze();
void PrintList();
void Scroll();
void QueryRanking();
void QuerySubmission();
void PrintStatus(SubmitStatus Status);

class Team {
public:
    std::string TeamName;
    int Rank = 0;
    int FrozenCount = 0;
    int PassedCount = 0;
    int Penalty = 0;

private:
    SubmitStatus LastSubmitStatus = None;
    int LastSubmitProblem;
    int LastSubmitTime;
    int FailTimes[26] = {0};
    bool Passed[26] = {false};
    int PassedTimeUp[26] = {0}; // Record the time of Passed in ascending order
    int FirstAccepted[26] = {0};
    int LastWrongAnswer[26] = {0};
    int LastRuntimeError[26] = {0};
    int LastTimeLimitExceed[26] = {0};
    int LastAcceptedProblem = -1;
    int LastWrongAnswerProblem = -1;
    int LastRuntimeErrorProblem = -1;
    int LastTimeLimitExceedProblem = -1;
    SubmitStatus LastProblemStatus[26] = {None};

    //For Frozen
    int FrozenFailTimes[26] = {0};
    int FrozenTryTimes[26] = {0};
    bool FrozenProblem[26] = {false};
    int FrozenFirstAccepted[26] = {0};
    int FrozenLastAccepted[26] = {0};
    SubmitStatus FrozenLastProblemStatus[26];

public:
    explicit Team(std::string& Name) : TeamName(Name) {}
    Team() = default;
    ~Team() = default;

    void TeamSubmit(char Problem, SubmitStatus NewStatus, int Time)
    {
        LastProblemStatus[Problem - 65] = NewStatus;
        LastSubmitStatus = NewStatus;
        LastSubmitProblem = Problem - 65;
        LastSubmitTime = Time;
        if (!Frozen) {
            if (NewStatus == Accepted) {
                LastAcceptedProblem = Problem - 65;
                if (Passed[Problem - 65]) {
                    FrozenLastAccepted[Problem - 65] = Time;
                }
                else {
                    PassedTimeUp[PassedCount] = Time;
                    Passed[Problem - 65] = true;
                    ++PassedCount;
                    FirstAccepted[Problem - 65] = Time;
                    FrozenFirstAccepted[Problem - 65] = Time;
                    FrozenLastAccepted[Problem - 65] = Time;
                    Penalty += 20 * FailTimes[Problem - 65]
                             + FirstAccepted[Problem - 65];
                }
            }
            else {
                if (!Passed[Problem - 65]) {
                    ++FailTimes[Problem - 65];
                }
                if (NewStatus == Wrong_Answer) {
                    LastWrongAnswerProblem = Problem - 65;
                    LastWrongAnswer[Problem - 65] = Time;
                }
                if (NewStatus == Runtime_Error) {
                    LastRuntimeErrorProblem = Problem - 65;
                    LastRuntimeError[Problem - 65] = Time;
                }
                if (NewStatus == Time_Limit_Exceed) {
                    LastTimeLimitExceedProblem = Problem - 65;
                    LastTimeLimitExceed[Problem - 65] = Time;
                }
            }
        }

        // After FREEZE Command
        else {
            // For problems not to be frozen
            if (Passed[Problem - 65]) {
                if (NewStatus == Accepted) {
                    LastAcceptedProblem = Problem - 65;
                    FrozenLastAccepted[Problem - 65] = Time;
                }// Since the problem is passed before FREEZE command
                if (NewStatus == Wrong_Answer) {
                    LastWrongAnswerProblem = Problem - 65;
                    LastWrongAnswer[Problem - 65] = Time;
                }
                if (NewStatus == Runtime_Error) {
                    LastRuntimeErrorProblem = Problem - 65;
                    LastRuntimeError[Problem - 65] = Time;
                }
                if (NewStatus == Time_Limit_Exceed) {
                    LastTimeLimitExceedProblem = Problem - 65;
                    LastTimeLimitExceed[Problem - 65] = Time;
                }
            }
            // For problem frozen or to be frozen
            else {
                FrozenLastProblemStatus[Problem - 65] = NewStatus;
                if (!FrozenProblem[Problem - 65]) {
                    ++FrozenCount;
                    FrozenProblem[Problem - 65] = true;
                }
                ++FrozenTryTimes[Problem - 65];
                if (NewStatus == Accepted) {
                    LastAcceptedProblem = Problem - 65;
                    if (FrozenFirstAccepted[Problem - 65] != 0) {
                        FrozenLastAccepted[Problem - 65] = Time;
                    }
                    else {
                        FrozenFirstAccepted[Problem - 65] = Time;
                        FrozenLastAccepted[Problem - 65] = Time;
                    }
                }
                else {
                    if (FrozenFirstAccepted[Problem - 65] == 0) {
                        ++FrozenFailTimes[Problem - 65];
                    }
                    if (NewStatus == Wrong_Answer) {
                        LastWrongAnswerProblem = Problem - 65;
                        LastWrongAnswer[Problem - 65] = Time;
                    }
                    if (NewStatus == Runtime_Error) {
                        LastRuntimeErrorProblem = Problem - 65;
                        LastRuntimeError[Problem - 65] = Time;
                    }
                    if (NewStatus == Time_Limit_Exceed) {
                        LastTimeLimitExceedProblem = Problem - 65;
                        LastTimeLimitExceed[Problem - 65] = Time;
                    }
                }
            }
        }
    }

    void Print() const
    {
        std::cout << TeamName << " " << Rank << " " << PassedCount << " "
                  << Penalty;
        for (int i = 0; i < ProblemCount; ++i) {
            if (Passed[i]) {
                if (FailTimes[i] == 0) {
                    std::cout << " +";
                }
                else {
                    std::cout << " +" << FailTimes[i];
                }
            }
            else {
                if (FrozenProblem[i]) {
                    if (FailTimes[i] == 0) {
                        std::cout << " 0/" << FrozenTryTimes[i];
                    }
                    else {
                        std::cout << " -" << FailTimes[i] << "/"
                                  << FrozenTryTimes[i];
                    }
                }
                else {
                    if (FailTimes[i] == 0) {
                        std::cout << " .";
                    }
                    else {
                        std::cout << " -" << FailTimes[i];
                    }
                }
            }
        }
        std::cout << std::endl;
    }

    void TeamQuerySubmission(char Problem, SubmitStatus QueryStatus) const
    {
        char ProblemName;
        if (Problem == 'a') {
            if (QueryStatus == None) {
                if (LastSubmitStatus == None) {
                    std::cout << "Cannot find any submission." << std::endl;
                    return;
                }
                else {
                    ProblemName = (LastSubmitProblem + 65);
                    std::cout << TeamName << " " << ProblemName << " ";
                    PrintStatus(LastSubmitStatus);
                    std::cout << " " << LastSubmitTime << std::endl;
                    return;
                }
            }
            if (QueryStatus == Accepted) {
                if (LastAcceptedProblem == -1) {
                    std::cout << "Cannot find any submission." << std::endl;
                    return;
                }
                else {
                    ProblemName = (LastAcceptedProblem + 65);
                    std::cout << TeamName << " " << ProblemName << " Accepted "
                              << FrozenLastAccepted[LastAcceptedProblem]
                              << std::endl;
                    return;
                }
            }
            if (QueryStatus == Wrong_Answer) {
                if (LastWrongAnswerProblem == -1) {
                    std::cout << "Cannot find any submission." << std::endl;
                    return;
                }
                else {
                    ProblemName = (LastWrongAnswerProblem + 65);
                    std::cout << TeamName << " " << ProblemName << " Wrong_Answer "
                              << LastWrongAnswer[LastWrongAnswerProblem]
                              << std::endl;
                    return;
                }
            }
            if (QueryStatus == Runtime_Error) {
                if (LastRuntimeErrorProblem == -1) {
                    std::cout << "Cannot find any submission." << std::endl;
                    return;
                }
                else {
                    ProblemName = (LastRuntimeErrorProblem + 65);
                    std::cout << TeamName << " " << ProblemName << " Runtime_Error "
                              << LastRuntimeError[LastRuntimeErrorProblem]
                              << std::endl;
                    return;
                }
            }
            if (QueryStatus == Time_Limit_Exceed) {
                if (LastTimeLimitExceedProblem == -1) {
                    std::cout << "Cannot find any submission." << std::endl;
                    return;
                }
                else {
                    ProblemName = (LastTimeLimitExceedProblem + 65);
                    std::cout << TeamName << " " << ProblemName << " Time_Limit_Exceed "
                              << LastTimeLimitExceed[LastTimeLimitExceedProblem]
                              << std::endl;
                    return;
                }
            }
        }
        else {
            if (QueryStatus == None) {
                if (LastProblemStatus[Problem - 65] == None) {
                    std::cout << "Cannot find any submission." << std::endl;
                    return;
                }
                else {
                    std::cout << TeamName << " " << Problem << " ";
                    PrintStatus(LastProblemStatus[Problem - 65]);
                    if (LastProblemStatus[Problem - 65] == Accepted) {
                        std::cout << " " << FrozenLastAccepted[Problem - 65]
                                  << std::endl;
                        return;
                    }
                    if (LastProblemStatus[Problem - 65] == Runtime_Error) {
                        std::cout << " " << LastRuntimeError[Problem - 65]
                                  << std::endl;
                        return;
                    }
                    if (LastProblemStatus[Problem - 65] == Wrong_Answer) {
                        std::cout << " " << LastWrongAnswer[Problem - 65]
                                  << std::endl;
                        return;
                    }
                    if (LastProblemStatus[Problem - 65] == Time_Limit_Exceed) {
                        std::cout << " " << LastTimeLimitExceed[Problem - 65]
                                  << std::endl;
                        return;
                    }
                }
            }
            if (QueryStatus == Accepted) {
                if (FrozenLastAccepted[Problem - 65] == 0) {
                    std::cout << "Cannot find any submission." << std::endl;
                    return;
                }
                else {
                    std::cout << TeamName << " " << Problem << " Accepted "
                              << FrozenLastAccepted[Problem - 65] << std::endl;
                    return;
                }
            }
            if (QueryStatus == Wrong_Answer) {
                if (LastWrongAnswer[Problem - 65] == 0) {
                    std::cout << "Cannot find any submission." << std::endl;
                    return;
                }
                else {
                    std::cout << TeamName << " " << Problem << " Wrong_Answer "
                              << LastWrongAnswer[Problem - 65] << std::endl;
                    return;
                }
            }
            if (QueryStatus == Runtime_Error) {
                if (LastRuntimeError[Problem - 65] == 0) {
                    std::cout << "Cannot find any submission." << std::endl;
                    return;
                }
                else {
                    std::cout << TeamName << " " << Problem << " Runtime_Error "
                              << LastRuntimeError[Problem - 65] << std::endl;
                    return;
                }
            }
            if (QueryStatus == Time_Limit_Exceed) {
                if (LastTimeLimitExceed[Problem - 65] == 0) {
                    std::cout << "Cannot find any submission." << std::endl;
                    return;
                }
                else {
                    std::cout << TeamName << " " << Problem << " Time_Limit_Exceed "
                              << LastTimeLimitExceed[Problem - 65] << std::endl;
                    return;
                }
            }
        }
    }

    bool TeamScroll()
    {
        for (int i = 0; i < ProblemCount; ++i) {
            if (FrozenProblem[i]) {
                FailTimes[i] += FrozenFailTimes[i];
                FrozenProblem[i] = false;
                FrozenTryTimes[i] = 0;
                FrozenFailTimes[i] = 0;
                --FrozenCount;
                LastProblemStatus[i] = FrozenLastProblemStatus[i];
                if (LastWrongAnswer[i] != 0) {
                    if (LastSubmitTime < LastWrongAnswer[i]) {
                        LastSubmitTime = LastWrongAnswer[i];
                        LastSubmitProblem = i;
                        LastSubmitStatus = Wrong_Answer;
                    }
                }
                if (LastRuntimeError[i] != 0) {
                    if (LastSubmitTime < LastRuntimeError[i]) {
                        LastSubmitTime = LastRuntimeError[i];
                        LastSubmitProblem = i;
                        LastSubmitStatus = Runtime_Error;
                    }
                }
                if (LastTimeLimitExceed[i] != 0) {
                    if (LastSubmitTime < LastTimeLimitExceed[i]) {
                        LastSubmitTime = LastTimeLimitExceed[i];
                        LastSubmitProblem = i;
                        LastSubmitStatus = Time_Limit_Exceed;
                    }
                }
                if (FrozenFirstAccepted[i] != 0) {
                    FirstAccepted[i] = FrozenFirstAccepted[i];
                    InsertPassedTime(FrozenFirstAccepted[i]);
                    ++PassedCount;
                    Penalty += 20 * FailTimes[i]
                               + FirstAccepted[i];
                    Passed[i] = true;
                    return true;
                }
            }
        }
        return false;
    }

    void InsertPassedTime(int Time)
    {
        if (PassedCount > 0) {
            int Head = PassedCount - 1;
            int End = 0;
            if (Time > PassedTimeUp[PassedCount - 1]) {
                PassedTimeUp[PassedCount] = Time;
            }
            else {
                while (Head != End) {
                    if (Time > PassedTimeUp[(Head + End) / 2]) End = (Head + End) / 2 + 1;
                    else Head = (Head + End) / 2;
                }

                for (int i = PassedCount; i > Head; --i) {
                    PassedTimeUp[i] = PassedTimeUp[i - 1];
                }
                PassedTimeUp[Head] = Time;
            }
        }
        else {
            PassedTimeUp[0] = Time;
        }
    }

    bool operator<(const Team& that) const
    {
        if (!StartState) {
            return (this->TeamName < that.TeamName);
        }
        if (this->PassedCount != that.PassedCount) {
            return (this->PassedCount > that.PassedCount);
        }
        if (this->Penalty != that.Penalty) {
            return (this->Penalty < that.Penalty);
        }
        for (int i = PassedCount - 1; i >= 0; --i) {
            if (this->PassedTimeUp[i] != that.PassedTimeUp[i]) {
                return (this->PassedTimeUp[i] < that.PassedTimeUp[i]);
            }
        }
        return (this->TeamName < that.TeamName);
    }
};

std::unordered_map<std::string, Team> TeamMap;

class NameOfTeam {
public:
    std::string TeamName;
    decltype(TeamMap.begin()) Pointer;

    explicit NameOfTeam(std::string& Name) : TeamName(Name)
    {
        Pointer = TeamMap.find(TeamName);
    }
    NameOfTeam() = default;

    bool operator<(const NameOfTeam& that) const
    {
        return (this->Pointer->second < that.Pointer->second);
    }
};

std::set<NameOfTeam> TeamRank; // In real time rank
std::string DelayedTeamRanking[20000];
std::string Rubbish; // Used for deleting some useless input

int main()
{
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);
    std::cout.tie(nullptr);
    std::string Command;
    std::cin >> Command;
    while (Command == "ADDTEAM") {
        AddTeam();
        std::cin >> Command;
    }
    if (Command == "START") {
        Start();
        std::cin >> Command;
    }
    while (Command != "END") {
        if (Command == "ADDTEAM") {
            std::cout << "[Error]Add failed: competition has started."
                      << std::endl;
            std::cin >> Rubbish;
        }

        if (Command == "START") {
            std::cout << "[Error]Start failed: competition has started."
                      << std::endl;
        }

        if (Command == "SUBMIT") {
            Submit();
        }

        if (Command == "FLUSH") {
            Flush();
            std::cout << "[Info]Flush scoreboard." << std::endl;
        }

        if (Command == "FREEZE") {
            Freeze();
        }

        if (Command == "SCROLL") {
            Scroll();
        }

        if (Command == "QUERY_RANKING") {
            QueryRanking();
        }

        if (Command == "QUERY_SUBMISSION") {
            QuerySubmission();
        }
        std::cin >> Command;
    }

    // Ending
    if (Command == "END") {
        std::cout << "[Info]Competition ends." << std::endl;
    }

    return 0;
}

/// ADDTEAM (Before START command): Check whether there is a identical name.
/// If there is no identical name, emplace "Team" and "NameOfTeam".
void AddTeam()
{
    std::string Name;
    std::cin >> Name;
    if (TeamMap.count(Name)) {
        std::cout << "[Error]Add failed: duplicated team name." << std::endl;
    }
    else {
        TeamMap.emplace(Name, Team(Name));
        std::cout << "[Info]Add successfully." << std::endl;
        TeamRank.emplace(NameOfTeam(Name));
    }
}

/// START (Before the competition starts): Get "DurationTime" and
/// "ProblemNumber". Then flush the ranking in name order, turn on StartState.
void Start()
{
    std::string SubCommand;
    std::cin >> SubCommand;
    std::cin >> DurationTime;
    std::cin >> SubCommand;
    std::cin >> ProblemCount;
    Flush();
    StartState = true;
    std::cout << "[Info]Competition starts." << std::endl;
}

///Submit: Load data and call the "TeamSubmit" function in class
void Submit()
{
    char Problem;
    int Time;
    std::string TeamName, StatusName;
    SubmitStatus NewStatus;
    // Load data (Rubbish is used to remove words "BY", "With" and "AT".)
    std::cin >> Problem >> Rubbish >> TeamName >> Rubbish
             >> StatusName >> Rubbish >> Time;
    if (StatusName == "Accepted") {
        if (Frozen) {
            NewStatus = Accepted;
            TeamMap[TeamName].TeamSubmit(Problem, NewStatus, Time);
        }
        else {
            TeamRank.erase(NameOfTeam(TeamName));
            NewStatus = Accepted;
            TeamMap[TeamName].TeamSubmit(Problem, NewStatus, Time);
            TeamRank.emplace(NameOfTeam(TeamName));
        }
        return;
    }
    if (StatusName == "Wrong_Answer") NewStatus = Wrong_Answer;
    if (StatusName == "Runtime_Error") NewStatus = Runtime_Error;
    if (StatusName == "Time_Limit_Exceed") NewStatus = Time_Limit_Exceed;
    TeamMap[TeamName].TeamSubmit(Problem, NewStatus, Time);
}

/// Update the DelayedTeamRanking and Rank in TeamMap
void Flush()
{
    if (!Frozen || NeedFlush) {
        int Rank = 1;
        for (const auto& Name: TeamRank) {
            Name.Pointer->second.Rank = Rank;
            DelayedTeamRanking[Rank] = Name.TeamName;
            ++Rank;
        }
    }
    if (Frozen) {
        NeedFlush = false;
    }
}

void Freeze()
{
    if (Frozen) {
        std::cout << "[Error]Freeze failed: scoreboard has been frozen."
                  << std::endl;
    }
    else {
        Frozen = true;
        std::cout << "[Info]Freeze scoreboard." << std::endl;
    }
}

void PrintList()
{
    for (int i = 1; i <= TeamRank.size(); ++i) {
        TeamMap[DelayedTeamRanking[i]].Print();
    }
}

/// Flush and print list first. Then unfreeze the frozen team in adverse order.
/// Since the set iterator may die after erase, so two iterator is needed.
/// (Member following TempMember)
void Scroll()
{
    if (Frozen) {
        std::cout << "[Info]Scroll scoreboard." << std::endl;
        Flush();
        Frozen = false;
        PrintList();
        auto Member = TeamRank.end();
        auto TempMember = Member;
        --TempMember;
        while (TempMember != TeamRank.begin()) {
            if (TeamMap[TempMember->TeamName].FrozenCount != 0) {
                std::string TempName = TempMember->TeamName;
                auto Former = TempMember;
                --Former;
                TeamRank.erase(NameOfTeam(TempName));
                bool HavePassed = TeamMap[TempName].TeamScroll();
                TeamRank.emplace(NameOfTeam(TempName));
                auto NewFormer = TeamRank.find(NameOfTeam(TempName));
                if (NewFormer != TeamRank.begin()) {
                    --NewFormer;
                    if (HavePassed) {
                        if (Former->TeamName != NewFormer->TeamName) {
                            auto Team2 = NewFormer;
                            ++Team2;
                            auto Team1 = Team2;
                            ++Team2;
                            std::cout << Team1->TeamName << " " << Team2->TeamName
                                      << " " << TeamMap[TempName].PassedCount << " "
                                      << TeamMap[TempName].Penalty << std::endl;
                        }
                        TempMember = Member;
                        --TempMember;
                    }
                    else {
                        --Member;
                        TempMember = Member;
                        --TempMember;
                    }
                }
                else {
                    auto Team2 = TeamRank.begin();
                    ++Team2;
                    std::cout << TeamRank.begin()->TeamName << " "
                              << Team2->TeamName << " "
                              << TeamMap[TempName].PassedCount << " "
                              << TeamMap[TempName].Penalty << std::endl;
                    TempMember = Member;
                    --TempMember;
                }

            }
            else {
                --Member;
                --TempMember;
            }
        }
        while (TeamRank.begin()->Pointer->second.FrozenCount != 0) {
            TeamRank.begin()->Pointer->second.TeamScroll();
        }
        Flush();
        PrintList();
        NeedFlush = true;
    }
    else {
        std::cout << "[Error]Scroll failed: scoreboard has not been frozen."
                  << std::endl;
    }
}

void QueryRanking()
{
    std::string TempName;
    std::cin >> TempName;
    auto Member = TeamMap.find(TempName);
    if (Member != TeamMap.end()) {
        std::cout << "[Info]Complete query ranking." << std::endl;
        if (Frozen) {
            std::cout << "[Warning]Scoreboard is frozen. The ranking may be inaccurate until it were scrolled."
                      << std::endl;
        }
        std::cout << TempName << " NOW AT RANKING "
                  << Member->second.Rank << std::endl;
    }
    else {
        std::cout << "[Error]Query ranking failed: cannot find the team."
                  << std::endl;
    }
}

void QuerySubmission()
{
    std::string TempName, ProblemName, StatusName;
    SubmitStatus QueryStatus;
    std::cin >> TempName;
    auto Member = TeamMap.find(TempName);
    if (Member != TeamMap.end()) {
        std::cout << "[Info]Complete query submission." << std::endl;
        // Get 15 characters
        for (int i = 0; i < 15; ++i) {
            std::cin.get();
        }

        std::cin >> ProblemName;
        // Get 12 characters
        for (int i = 0; i < 12; ++i) {
            std::cin.get();
        }

        std::cin >> StatusName;
        if (StatusName == "Accepted") QueryStatus = Accepted;
        if (StatusName == "Wrong_Answer") QueryStatus = Wrong_Answer;
        if (StatusName == "Runtime_Error") QueryStatus = Runtime_Error;
        if (StatusName == "Time_Limit_Exceed") QueryStatus = Time_Limit_Exceed;
        if (StatusName == "ALL") QueryStatus = None;
        if (ProblemName == "ALL") {
            Member->second.TeamQuerySubmission('a', QueryStatus);
        }
        else {
            Member->second.TeamQuerySubmission(ProblemName[0], QueryStatus);
        }
    }
    else {
        std::cout << "[Error]Query submission failed: cannot find the team."
                  << std::endl;
    }
}

void PrintStatus(SubmitStatus Status)
{
    if (Status == Accepted) {
        std::cout << "Accepted";
    }
    if (Status == Wrong_Answer) {
        std::cout << "Wrong_Answer";
    }
    if (Status == Runtime_Error) {
        std::cout << "Runtime_Error";
    }
    if (Status == Time_Limit_Exceed) {
        std::cout << "Time_Limit_Exceed";
    }
}
