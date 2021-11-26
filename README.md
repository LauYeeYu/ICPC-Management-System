# ICPC Management System

This is an ICPC management system, also an assignment of programming course of SJTU ACM class 2025. For more information about this assignment, please click [here](https://github.com/ACM-Programming-2021/ICPC-Management-System-2021).

## Functions

### Add Team

#### Input Format:

`ADDTEAM team_name`

#### Implementation:

##### Before the competition starts: 

`void AddTeam();`

Check whether there is a identical name. If there is no identical name, emplace "Team" and "NameOfTeam".

##### After the competition starts: 

`std::cout << "[Error]Add failed: competition has started." << std::endl;`

### Start

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
