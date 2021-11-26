# INC 2021 - Problem Repository

## Repository Structure

Each problem (from PA to PD for trial and A to M for contest) has their own folder. The folder name is the `slug` of the problem, which is the codename we used when we internally prepare the problems. The following table describes the slug for each problem

| Problem Code | Problem Slug | Problem Title                        |
| ------------ | ------------ | ------------------------------------ |
| PA           | frequent     | Frequent Letter                      |
| PB           | trees        | Visible Trees                        |
| PC           | cube         | Sum of Three Cubes                   |
| PD           | short        | Shortsighted                         |
| A            | sus          | Suspicious Event                     |
| B            | fence        | Wooden Fence                         |
| C            | card         | Card Game                            |
| D            | team         | Forming a Team                       |
| E            | quest        | Quests from the Queen                |
| F            | split        | Splitology                           |
| G            | treasure     | Treasure Hunter                      |
| H            | convex-hull  | Convex Hull                          |
| I            | ability      | Unique Ability                       |
| J            | smpa         | Sum Mod Pair of A                    |
| K            | sequence     | Yet Another Sequence Related Problem |
| L            | domino       | Concealed Domino                     |
| M            | tennis       | Tennis Game                          |

In the problem folder, there should be exactly the following file/folder:

- `description.pdf`. The problem statement distributed to the contestants during the contest.
- `data/`. A folder consisting of all testcases. The sample inputs given in the problem statement will be in the form of `sample/<slug>_sample_<testcase number>.in/ans`, while the hidden testcases will be in the form of `secret/<slug>_1_<testcase number>.in/ans`.
- `solution.cpp`. The official solution used to generate the outputs of the testcases
- `scorer.cpp`. A validator used to validate contestants' output. The code accepts three arguments.
  - The first parameter will be used as the filename of the input file
  - The second parameter will be used as the filename of the judge's output file
  - The third parameter will be used as the filename of the contestant's output file
  - This code will print `WA` to stdout if the contestant's output is intended to be judged `WRONG ANSWER`, while it will print `AC` to stdout if the contestants's output is intended to be judged `CORRECT`.
- `verifier.py`. A verifier used to validate the input of the testcases. The code will crash if the input violates the problem constraint, while it will exit gracefully and will not output anything if the input satisfies the problem constraint.

## Limitations

The following table describes the time limit for each problem, as broadcasted to all teams at the beginning of the contest:

| Problem Code | Problem Slug | Time Limit |
| ------------ | ------------ | ---------- |
| PA           | frequent     | 1 sec      |
| PB           | trees        | 1 sec      |
| PC           | cube         | 1 sec      |
| PD           | short        | 2 sec      |
| A            | sus          | 1 sec      |
| B            | fence        | 2 sec      |
| C            | card         | 2 sec      |
| D            | team         | 3 sec      |
| E            | quest        | 3 sec      |
| F            | split        | 1 sec      |
| G            | treasure     | 2 sec      |
| H            | convex-hull  | 1 sec      |
| I            | ability      | 2 sec      |
| J            | smpa         | 1.5 sec    |
| K            | sequence     | 1 sec      |
| L            | domino       | 1 sec      |
| M            | tennis       | 1 sec      |

The memory limit for all problems is 256MB, as broadcasted to all teams at the beginning of the contest.

## Solutions

Note that while there is only one official published solution, we have a lot of other solutions prepared for our internal testing. These other solutions include, but not limited to:

- Solutions in several other languages
- Solutions that is intended to be incorrect (e.g. WA, TLE, etc.)
- Other correct solutions (especially on problems with more than one correct outputs for each input)

## Errata

None.
