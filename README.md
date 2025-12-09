# MINDCRAFT - Quiz Game
This repository contains the source code for Mindcraft, a console-based quiz game developed as part of our end semester project at FAST-NUCES.

The Catch: Sticking to the project's foundational requirements, this game was developed completely using fundamental C++ programming constructs like functions, loops, and arrays. No classes, structs, or pointers were used.

## Team
1) Muhammad Hassan Tasneem
2) Syed Muhammad Ali
3) Mahad Itqaan Naser

## Project Overview:
The game allows users to test their general knowledge across multiple categories (e.g. Geography, IQ, History) with varying difficulty levels (Easy, Medium, Hard). The goal of this project was to create a scalable game capable of handling diverse question sets and providing an engaging user experience.

# Key Features by Phase:

## Phase 1: Basic Quiz System Setup
This phase established the foundational and essential features: 


- **Main Menu:**
Implemented a functional menu with options to Start Quiz, View High Scores

- **Question Bank and File Handling:**
Configured the system to load questions from separate category files (geography.txt, history.txt, etc.). 

File structure includes the questions statement, four multiple-choice options, and the correct answer. 

- **Basic Gameplay:** 
Displays 10 randomly selected questions per session, provides immediate feedback (Correct/Incorrect), and tracks the player's score.

## Phase 2: Modularization and Advanced Gameplay Features

This phase focused on expanding functionality and features.

- **Varying Difficulty:** 
Implemented three difficulty levels (Easy, Medium, Hard) with dynamic adjustment of the question pool based on the user's choice. 

- **Timed Gameplay:** 
Introduced a per-question timer (e.g 10 seconds) to add a time-based challenge, utilizing threads.

- **Lifelines System:**
Incorporated four KBC-inspired lifelines, each usable once per quiz: 50/50, Skip Question, Replace Question, and Extra Time. 

- **Streak Bonus System:**
Rewards players with bonus points (+5 or +15) for consecutive correct answers to increase engagement and reward skill. 

- **Negative Marking:**
Implemented realistic challenge by deducting points for wrong answers, with the penalty adjusted by the difficulty level (e.g -5 for Hard level).


## Phase 3: File Handling and Persistent Features

This phase focused on data storage and replay functionality: 

- **High Scores:**
Implemented persistent storage of the top 5 high scores to high_scores.txt, including the player's name and session date for the leaderboard display. 

- **Quiz Logging:**
Developed a logging feature to save complete session data—including player name, date, and question performance—to quiz_logs.txt. 

- **Review and Replay:**
Provides options for players to review incorrectly answered questions and replay the quiz.


This project serves as a showcase of our ability to deliver a complex, feature-complete game using fundamental C++.