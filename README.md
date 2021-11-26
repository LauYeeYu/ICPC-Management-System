# Structure:

### Main Elements

##### class Team: Storing Almost All Data of Each Team

public:

- std::string TeamName
- int Rank: Delayed ranking (update in FLUSH)
- int FrozenCount : Showing the number of frozen problems
- int PassedCount: Showing the number of passed problems
- int Penalty: Penalty

private:

- SubmitStatus LastSubmitStatus
- int LastSubmitProblem
- int LastSubmitTime
- int FailTimes[26]
- bool Passed[26]
- int PassedTimeUp[26]       : Showing the pass time in ascending order
- int FirstAccepted[26]      : First accepted time
- int LastWrongAnswer[26]    : Last wrong answer time
- int LastRuntimeError[26]   : Last runtime error time
- int LastTimeLimitExceed[26]: Last time limit exceed time
- int LastAcceptedProblem
- int LastWrongAnswerProblem
- int LastRuntimeErrorProblem
- int LastTimeLimitExceedProblem
- SubmitStatus LastProblemStatus[26]
- int FrozenFailTimes[26]
- int FrozenTryTimes[26]
- bool FrozenProblem[26]
- int FrozenFirstAccepted[26]
- int FrozenLastAccepted[26]
- SubmitStatus FrozenLastProblemStatus[26]

##### std::map< std::string, Team > TeamMap: Gathering ALL Team Data

##### class NameOfTeam

- std::string TeamName
- std::unordered_map<std::string, Team> Pointer : Pointing to TeamMap

##### std::set< NameOfTeam > TeamRank: Real time rank list

##### std::string DelayedTeamRanking[20000]: Delayed rank list

# Function:

## Add Team

#### Input Format:

`ADDTEAM team_name`

#### Implementation:

##### Before the competition starts: 

`void AddTeam();`

Check whether there is a identical name. If there is no identical name, emplace "Team" and "NameOfTeam".

##### After the competition starts: 

`std::cout << "[Error]Add failed: competition has started." << std::endl;`



## Start

#### Input Format:

`START DURATION duration_time PROBLEM problem_count`

#### Implementation:

##### Before the competition starts: 

`void Start();`

Get "DurationTime" and "ProblemNumber". Then flush the ranking in name order, turn on StartState.

After the competition starts: 

`std::cout << "[Error]Start failed: competition has started." << std::endl;`



## Submit

#### Input Format:

`SUBMIT problem_name BY team_name WITH submit_status AT time`

#### Implementation:

`void Submit();`

Load data and call the "TeamSubmit" function in class



## Flush

#### Input Format:

`FLUSH`

#### Implementation:

`void Flush();`

Update the DelayedTeamRanking and Rank in TeamMap



## Freeze

#### Input Format:

`FREEZE`

#### Implementation:

`void Freeze();`



## Scroll

#### Input Format:

`SCROLL`

#### Implementation:

`void Scroll();`

Flush and print list first. Then unfreeze the frozen team in adverse order. Since the set iterator may die after erase, so two iterator is needed. (Member following TempMember)



## Query Ranking

#### Input Format:

`QUERY_RANKING team_name`

#### Implementation:

`void QueryRanking();`



## Query Submission

#### Input Format:

`QUERY_SUBMISSION team_name WHERE PROBLEM=problem_name AND STATUS=status`

#### Implementation:

`void QuerySubmission();`



## End

#### Input Format:

`END`
