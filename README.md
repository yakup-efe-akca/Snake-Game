# Snake Game in C

A classic Snake Game developed in C using the raylib graphics library and a Circular Queue data structure.

The project demonstrates how queues and a 2D occupancy array can be used to manage snake movement, growth, and collision detection.

## Features

- Snake movement using a Circular Queue
- Keyboard controls with arrow keys
- Food generation at random unoccupied positions
- Snake growth when food is eaten
- Score tracking
- Wall and self-collision detection
- Game Over screen
- Restart functionality
- Prevention of reverse movement and multiple direction changes between updates

## Data Structures

**Circular Queue**

The snake's body is stored in a circular queue:

- Front represents the tail.
- Rear represents the head.
- Enqueue adds a new head position.
- Dequeue removes the oldest tail position.

During normal movement, the tail is removed and a new head is added. When food is eaten, the tail remains, allowing the snake to grow.

**Occupancy Array**

A 2D boolean array tracks occupied board cells, allowing O(1) collision checks.

## Technologies

- C
- raylib
- Circular Queue
- 2D Arrays

## How to Run

**Requirements**

- C compiler (Clang or GCC)
- raylib
- pkg-config

**Installation on macOS**

Install the dependencies using Homebrew:

`brew install raylib pkg-config`

**Compile**

Open a terminal in the project directory and run:

`clang Snake.c -o Snake $(pkg-config --cflags --libs raylib)`

**Run**

`./Snake`

## Controls

| Key | Action |
|---|---|
| ↑ | Move Up |
| ↓ | Move Down |
| ← | Move Left |
| → | Move Right |
| R | Restart after Game Over |
| ESC | Exit |

## Game Rules

1. Control the snake using the arrow keys.
2. Eat food to increase your score and grow.
3. Avoid hitting the walls or your own body.
4. Press R to restart after Game Over.

## Learning Objectives

This project focuses on applying fundamental data structures to a graphical application, including circular queues, array indexing, collision detection, and game-loop logic.