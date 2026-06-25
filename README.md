# Black-Jack-Game
Multi-player Blackjack game in C using 2D arrays, pointers &amp; structs. Implements hit(), stand(), dealCards(), calculateHandValue(), dealerDraw(), checkBlackjack() &amp; isBusted(). Features dealer AI, dynamic Ace scoring, Blackjack detection, bust logic &amp; winner declaration. camelCase, pass-by-reference throughout.
# 🃏 Blackjack Card Game — C Implementation

> **A fully playable multi-player Blackjack game engine** built in C using 2D arrays, pointers, structs, and function-based architecture. Implements complete casino-standard Blackjack rules with dealer AI, Ace handling, Blackjack detection, and bust logic.

---

## 📋 Table of Contents

- [Overview](#overview)
- [Features](#features)
- [Core Functions](#core-functions)
- [Data Structures](#data-structures)
- [Game Rules Engine](#game-rules-engine)
- [Getting Started](#getting-started)
- [Usage](#usage)
- [Project Architecture](#project-architecture)
- [Edge Cases Handled](#edge-cases-handled)
- [Contributing](#contributing)
- [License](#license)

---

## 🗂️ Overview

The **Blackjack Card Game** is a fully functional C-language implementation of the classic casino card game. The system manages multiple players simultaneously using **2D arrays** (`players[i][j]` where `i` = player index, `j` = card index), handles all standard card values including dynamic Ace scoring, and runs a rule-compliant dealer AI that hits on 16 or lower and stands on 17 or higher.

**Built to demonstrate:**
- Pass-by-value vs. pass-by-reference in C functions
- Pointer manipulation for hand and card management
- 2D array design for multi-player card tracking
- Struct-based player modeling
- Full game loop with input validation and winner declaration

---

## ✅ Features

| Feature | Description |
|---|---|
| 🎴 **2D Array Hand Management** | `players[i][j]` tracks every card for every player |
| 👆 **Pointer-Based Updates** | Player hands modified via pointers on hit/stand actions |
| 🏗️ **Struct-Driven Players** | `Player` struct holds name, hand, handValue, and status |
| 🤖 **Dealer AI** | Dealer hits on ≤16, stands on ≥17 — fully automated |
| 🃏 **Blackjack Detection** | Instantly detects Ace + 10-point card (21 in 2 cards) |
| 🔢 **Dynamic Ace Scoring** | Ace counted as 11 unless it causes a bust, then as 1 |
| 💥 **Bust Detection** | `isBusted()` checks and flags players exceeding 21 |
| 🏆 **Multi-Player Winner Logic** | Closest to 21 wins; tie declared on equal hand values |
| 🔁 **Bonus: Multi-Round Support** | Track win/loss records across multiple rounds |
| 📁 **Bonus: Game History Files** | Optionally save player stats and session history |

---

## ⚙️ Core Functions

### `dealCards(Player *players, int numPlayers)`
Deals exactly 2 cards to each player and the dealer at game start.
- Populates `players[i][0]` and `players[i][1]` for each player
- Passes player array **by reference** via pointer for in-place update

```c
void dealCards(Player *players, int numPlayers);
```

---

### `hit(Player *player)`
Draws one additional card and adds it to the player's hand.
- Passes `Player` struct **by reference** to directly update `hand[]` and `handValue`
- Triggers `isBusted()` check after each draw

```c
void hit(Player *player);
```

---

### `stand(Player *player)`
Locks the player's current hand and passes turn to the next player or dealer.
- Updates `player->status` to indicate the player is standing
- No card modification — demonstrates pass-by-reference with no mutation

```c
void stand(Player *player);
```

---

### `calculateHandValue(int *hand, int cardCount)`
Computes the total hand value with correct Ace handling.
- Cards 2–10 → face value
- J, Q, K → 10 points each
- Ace → 11 unless bust-causing, then recalculated as 1

```c
int calculateHandValue(int *hand, int cardCount);
```

---

### `dealerDraw(Player *dealer)`
Executes the dealer's automated drawing sequence per standard Blackjack rules.
- Draws cards until `handValue >= 17`
- Fully autonomous — no player input required

```c
void dealerDraw(Player *dealer);
```

---

### `checkBlackjack(Player *player)`
Detects a natural Blackjack (21 with exactly 2 cards: Ace + 10-point card).
- Returns `1` if condition is met, `0` otherwise
- Called immediately after initial `dealCards()` for early win declaration

```c
int checkBlackjack(Player *player);
```

---

### `isBusted(Player *player)`
Checks if a player's hand value exceeds 21.
- Returns `1` if bust, `0` if still active
- Updates `player->status` to `"busted"` on trigger

```c
int isBusted(Player *player);
```

---

### `declareWinner(Player *players, int numPlayers, Player *dealer)`
Evaluates all active (non-busted) players against the dealer and announces the result.
- Compares each `player->handValue` to `dealer->handValue`
- Handles ties when multiple players share the highest value

```c
void declareWinner(Player *players, int numPlayers, Player *dealer);
```

---

## 🏗️ Data Structures

### Player Struct

```c
typedef struct {
    char   name[50];       // Player's name
    int    hand[5];        // Cards in hand (max 5 cards)
    int    handValue;      // Computed total hand value
    char   status[10];     // "active", "standing", or "busted"
} Player;
```

### 2D Array — Multi-Player Hand Tracking

```c
// players[i][j] — i = player index, j = card index
int players[MAX_PLAYERS][MAX_CARDS];

// Example: Player 2's third card
int card = players[1][2];
```

---

## 🎮 Game Rules Engine

| Rule | Implementation |
|---|---|
| Initial deal | 2 cards per player via `dealCards()` |
| Player turn | Choose `hit()` or `stand()` each round |
| Ace value | 11 by default; recalculated to 1 if bust detected |
| Dealer rule | Hits on ≤16, stands on ≥17 via `dealerDraw()` |
| Blackjack | Ace + 10-point card in 2 cards → instant win |
| Bust | Any hand > 21 → player eliminated |
| Tie | Equal hand values → declared a tie |
| Winner | Closest to 21 among non-busted players wins |

---

## 🚀 Getting Started

### Prerequisites

- GCC compiler (version 9+ recommended)
- Unix/Linux terminal or Windows with MinGW
- `make` (optional, for Makefile builds)

### Compilation & Run

```bash
# Clone the repository
git clone https://github.com/your-username/blackjack-c.git
cd blackjack-c

# Compile
gcc -o blackjack main.c game.c player.c dealer.c utils.c -Wall -Wextra

# Run
./blackjack
```

### With Makefile

```bash
make
./blackjack
```

---

## 💻 Usage

```
===========================
     BLACKJACK - C GAME
===========================
Enter number of players: 3

Dealing cards...
Player 1 [Alice]: [A] [7]  → Hand Value: 18
Player 2 [Bob]:   [K] [9]  → Hand Value: 19
Player 3 [Carol]: [5] [6]  → Hand Value: 11
Dealer:           [Q] [?]  → Shows: 10

--- Player 1's Turn (Alice) ---
Hand Value: 18 | Action (hit/stand): stand

--- Player 2's Turn (Bob) ---
Hand Value: 19 | Action (hit/stand): stand

--- Player 3's Turn (Carol) ---
Hand Value: 11 | Action (hit/stand): hit
Carol drew: [8] → Hand Value: 19

--- Dealer's Turn ---
Dealer reveals: [Q] [4] → 14 → draws...
Dealer drew: [3] → Hand Value: 17 → STAND

===========================
        RESULTS
===========================
Alice  → 18 | LOSE
Bob    → 19 | WIN 🏆
Carol  → 19 | WIN 🏆  (TIE)
Dealer → 17
===========================
```

---

## 📁 Project Architecture

```
blackjack-c/
│
├── main.c              # Entry point, game loop, player count input
├── game.c              # Core game logic (dealCards, declareWinner)
├── game.h
│
├── player.c            # Player actions: hit(), stand(), isBusted()
├── player.h
│
├── dealer.c            # Dealer AI: dealerDraw()
├── dealer.h
│
├── cards.c             # Card values, deck management, shuffle logic
├── cards.h
│
├── utils.c             # calculateHandValue(), checkBlackjack(), input validation
├── utils.h
│
├── history/            # (Bonus) Saved game results and player stats
│   └── game_log.txt
│
├── Makefile
└── README.md
```

---

## 🧠 Edge Cases Handled

| Edge Case | Handling Strategy |
|---|---|
| Multiple Aces in hand | Each Ace re-evaluated to 1 if total > 21 |
| Player busts after hitting Ace | `calculateHandValue()` retroactively recounts Ace as 1 |
| All players bust | Dealer wins by default without drawing |
| Dealer and player tie at 21 | `declareWinner()` outputs explicit tie message |
| Blackjack vs. non-Blackjack 21 | Blackjack takes priority over regular 21 |
| Invalid input (not hit/stand) | Input validation loop re-prompts until valid |

---

## 🧪 Testing

```bash
# Compile with debug flags
gcc -o blackjack_debug main.c game.c player.c dealer.c utils.c -Wall -Wextra -g

# Run with Valgrind (memory check)
valgrind --leak-check=full ./blackjack_debug

# Run test cases
./run_tests.sh
```

---

## 🤝 Contributing

1. Fork the repository
2. Create a feature branch: `git checkout -b feature/add-split-hand`
3. Commit with Conventional Commits: `git commit -m "feat: implement hand splitting logic"`
4. Push and open a Pull Request

---

## 📄 License

This project is licensed under the [MIT License](LICENSE).

---

## 👤 Author

**Nejar Medhat**
- GitHub: [@nejarmedhat](https://github.com/nejarmedhat)

---

<p align="center">Built with pure C — no libraries, no shortcuts, just clean logic ♠️♥️♦️♣️</p>
