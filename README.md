# Quiz Battle Arena

An offline multiplayer quiz game written in C++ for 2–4 players sharing one computer.

## Features
- 2–4 local players; no login, server, database, or internet required
- Randomized question order from a text question bank
- One 50-50 lifeline per player (remaining choices are enforced)
- +10 points for a correct answer
- Stable-sorted leaderboard with tie handling
- Saves previous game results to `results.txt`
- Loads questions from `questions.txt`; uses built-in questions if the file is missing or invalid

## DSA concepts used
| Concept | Use |
|---|---|
| `vector` | Stores players, options, and questions |
| `queue` | Delivers questions in FIFO order |
| `unordered_map` | Keeps a quick score lookup by player name |
| `stable_sort` | Ranks players while preserving order for ties |
| `shuffle` | Randomizes question order |
| File handling | Reads `questions.txt` and appends `results.txt` |

## Compile and run

### Windows (MinGW)
```bash
g++ -std=c++11 -Wall -Wextra -pedantic main.cpp -o quiz.exe
quiz.exe
```

### Linux / macOS
```bash
g++ -std=c++11 -Wall -Wextra -pedantic main.cpp -o quiz
./quiz
```

## Custom questions
Each line in `questions.txt` must contain exactly 8 fields separated by `|`:

`Question|Option A|Option B|Option C|Option D|Correct letter|Category|Difficulty`

Correct letter must be `A`, `B`, `C`, or `D`. Avoid using `|` inside a field.

## Project scope
This is a console-based academic DSA mini-project. Timer, GUI, online multiplayer, and database features are intentionally outside the current version.
