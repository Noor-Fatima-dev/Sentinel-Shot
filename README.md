# Sentinel Shot 🕹️

Sentinel Shot is a retro-style, ASCII-based platformer built entirely in C++. Inspired by classic games like Super Mario, it challenges players to navigate through complex mazes, eliminate enemies with tactical shooting, and collect points to unlock the path to the next level.

## 🚀 Features
* **Dynamic Physics:** Implementation of gravity, jumping, and collision detection in a console environment.
* **Multi-Level Progression:** 4 distinct levels with increasing difficulty and unique maze layouts.
* **Floating Platforms:** Moving tiles that require precise timing to navigate.
* **Combat System:** Use projectiles to eliminate enemies. Note: The exit door only unlocks once the area is cleared!
* **Randomized Collectibles:** Score-boosting "pills" spawn dynamically across the map.
* **Rich Console Graphics:** Utilizes UTF-8 characters and Windows API colors for an enhanced visual experience.

## 🎮 How to Play
1.  **Objective:** Eliminate all enemies on the screen to unlock the **Winning Door** (`|[]`).
2.  **Controls:**
    * **Arrow Keys:** Move Left/Right and Jump.
    * **Spacebar:** Shoot bullets.
3.  **Health:** Avoid colliding with enemies (`{O}`). Each hit costs **20 HP**. If health hits 0, it's Game Over!
4.  **Scoring:** Kill enemies and collect pills (` ` `) to increase your final score.

## 🛠️ Technical Details
The game is built using:
* **Language:** C++
* **Libraries:** `windows.h` (for console manipulation), `iostream`, `conio.h`.
* **Core Concepts:** Coordinate-based rendering (`gotoxy`), ASCII character buffer reading, and array-based entity management.
1. Clone the repository:
   ```bash
   git clone [https://github.com/YourUsername/Sentinel-Shot.git](https://github.com/YourUsername/Sentinel-Shot.git)
