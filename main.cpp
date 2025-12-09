#include <iostream>
#include <string>
#include <iomanip>
#include <fstream>
#include <chrono>
#include <stdlib.h>
#include <thread>
#include <ctime>
#include <future>
#include <optional>
#include <unistd.h>

using namespace std;

// function prototypes
void header(string, char);
void colours(char, string = "0");
int displayMenu();
void gameMode(string &, string &, string &);
string displayCategories();
void displayQuestions(string, string, string, int &, int &, int &);
int displayLifelines();
void endScreen(int, int, int, string);
void recordScore(string, string, int);
void displayHS();
bool getTimedAnswer(int &, int, int &);
void shuffleQuestions(string[], string[], int[], int);
void shuffleOptions(string[], int &, int);
string getCurrentDate();

// clear console using ansi escape sequence
void clearConsole()
{
    cout << "\033[2J\033[H";
}

// main function
int main()
{
    srand(time(0));
    displayMenu();
    clearConsole();
}

// return date in dd-mm-yyyy format
string getCurrentDate()
{
    long now = time(0);
    int day = localtime(&now)->tm_mday;
    int month = 1 + localtime(&now)->tm_mon;
    int year = 1900 + localtime(&now)->tm_year;
    string dayStr = (day < 10) ? "0" + to_string(day) : to_string(day);
    string monthStr = (month < 10) ? "0" + to_string(month) : to_string(month);
    return dayStr + "-" + monthStr + "-" + to_string(year);
}

// makes the text in console coloured using ansi codes
// parameters: colour, weight (0 = regular, 1 = bold, 4 underline)
void colours(char c, string weight)
{
    // ansi escape sequences
    switch (c)
    {
    case 'r':
        cout << "\033[" + weight + ";31m"; // red
        break;
    case 'g':
        cout << "\033[" + weight + ";32m"; // green
        break;
    case 'b':
        cout << "\033[" + weight + ";36m"; // cyan
        break;
    case 'o':
        cout << "\033[" + weight + ";33m"; // orange
        break;
    case 'p':
        cout << "\033[" + weight + ";35m"; // purple
        break;
    case 'B':
        cout << "\033[" + weight + ";34m"; // blue
        break;
    case 'w':
        cout << "\033[0m"; // reset
        break;
    }
}

// displays a header with text centered
// parameters: str (text to display in the header), colour (color of header)
void header(string str, char colour = 'g')
{
    colours(colour, "1");
    cout << "===============================================" << endl;
    // centering string
    int spaces = (45 - str.length()) / 2; // 45 is the number of '='

    cout << "=";
    for (int i = 0; i < spaces; i++)
    {
        cout << " ";
    }
    cout << str;
    for (int i = 0; i < spaces; i++)
    {
        cout << " ";
    }

    cout << "=" << endl;
    cout << "===============================================" << endl;
    colours('w');
}

// records player score to high_scores.txt file
// parameters: (player name), category (quiz category), score (final score)
void recordScore(string name, string category, int score)
{
    string fileName = "high_scores.txt";
    ofstream outputFile(fileName, ios::app);

    outputFile << "\n" + name + "/" + getCurrentDate() + "/" << score;
}

// displays the main menu and handles user navigation
// returns elected option number (e.g 1 to start, 2 to view highscores, 3 to exit)
int displayMenu()
{
    string mode, difficulty, name;
    int option;
    header("Welcome to Quiz Game");
        colours('o', "1");
    cout << "\nPlease enter your name: ";
    getline(cin, name);
    colours('b', "1");
    cout << "\nWelcome, " << name;
    cout << endl;
    cout << "1. Start Game" << endl
         << "2. View High Scores" << endl
         << "3. Exit Game" << endl;
    colours('w');
    cout << "\nPlease select an option (1-3): ";

    cin >> option;

    switch (option)
    {
    case 1:
    {
        clearConsole();
        gameMode(mode, difficulty, name);
        break;
    }
    case 2:
    {
        displayHS();
        break;
    }

    case 3:
    {
        string msg = "Goodbye, " + name + " :(";
        header(msg, 'r');
        cout << "\nPress Enter to exit.";
        cin.ignore(1000, '\n');
        cin.get();
        return 0;
    }
    }

    return option;
}

// displays all the quiz categories and returns selected category
// returns category name as string (geography, computer, history, sports, iq), used for file handling later
string displayCategories()
{
    int option;
    string mode;

    colours('o', "1");
    cout << "A) Pick a Category" << endl;
    colours('b', "1");
    cout << "1. Geography" << endl;
    cout << "2. Computer Science" << endl;
    cout << "3. History" << endl;
    cout << "4. Sports" << endl;
    cout << "5. IQ/Logic" << endl;
    colours('w');
    cout << "\nPlease select an option (1-5): ";
    cin >> option;

    switch (option)
    {
    case 1:
        mode = "geography";
        break;
    case 2:
        mode = "computer";
        break;
    case 3:
        mode = "history";
        break;
    case 4:
        mode = "sports";
        break;
    case 5:
        mode = "iq";
        break;
    }

    return mode;
}

// displays top 5 high scores
void displayHS()
{
    string names[5000];
    string dates[5000];
    int scores[5000];

    string question;
    string fileName = "high_scores.txt";

    ifstream hsFile(fileName);
    string line;

    int index = 0;
    int lines = 0;

    // read scores from "high_scores.txt" file
    while (getline(hsFile, line))
    {
        int slashCount = 0;
        int start = 0;

        // extract name and score from the line into their arrays for sorting
        for (int i = 0; i <= line.length(); i++) // <= to handle last element as well (last '/')
        {
            if (i == line.length() || line[i] == '/') // Check end of string or slash
            {
                string portion = line.substr(start, i - start);

                if (slashCount == 0) // first entry is name
                {
                    names[index] = portion;
                }
                else if (slashCount == 1) // second entry is date
                {
                    dates[index] = portion;
                }
                else if (slashCount == 2) // third entry is score
                {
                    scores[index] = stoi(portion);
                }

                start = i + 1;
                slashCount++;
            }
        }

        index++;
    }

    // bubble sort according to scores (highest first)
    for (int i = 0; i < index - 1; i++)
    {
        for (int j = 0; j < index - 1; j++)
        {
            if (scores[j] < scores[j + 1])
            {
                swap(scores[j], scores[j + 1]);
                swap(names[j], names[j + 1]);
                swap(dates[j], dates[j + 1]);
            }
        }
    }

    // display the final sorted scores
    string heading = "Highscores";
    header(heading, 'b');
    colours('g');
    int spacing = 15;
    // alignment
    cout << left << setw(spacing + 10) << "Name" << setw(spacing) << "Score" << "Date" << endl;
    colours('w');
    for (int i = 0; i < 5; i++)
    {
        if (scores[i] != 0)
        {
            cout << left << setw(spacing + 10) << names[i] << setw(spacing) << scores[i] << dates[i] << endl;
        }
    }

    colours('r', "1");
    cout << "\nPress Enter to return to main menu.";
    colours('w');
    cin.ignore(1000, '\n');
    cin.get();
    clearConsole();
    displayMenu();
}

// handles game mode selection (category via displayCategories() function and difficulty)
// parameters: mode, difficulty, name
void gameMode(string &mode, string &difficulty, string &name)
{
    int correct, incorrect, score;
    int option;
    // gamemode selection
    header("Choose a Gamemode");
    mode = displayCategories();

    // difficulty selection
    cout << endl;

    cout << "B) Pick a Difficulty" << endl;
    colours('g', "1");
    cout << "1. ";
    cout << "Easy " << endl;
    colours('o', "1");
    cout << "2. ";
    cout << "Medium " << endl;
    colours('r', "1");
    cout << "3. ";
    cout << "Hard " << endl;
    colours('w');
    cout << "\nPlease select an option (1-3): ";
    cin >> option;

    switch (option)
    {
    case 1:
        difficulty = "easy";
        break;
    case 2:
        difficulty = "medium";
        break;
    case 3:
        difficulty = "hard";
        break;
    }

    displayQuestions(mode, difficulty, name, correct, incorrect, score);
}

// display lifeline options menu
// returns elected lifeline option (1-4)(50/50, skip, replace ques, increase  time)
int displayLifelines()
{
    int option;
    cout << endl;
    header("Pick a lifeline", 'o');
    colours('b', "1");
    cout << "1. ";
    colours('g', "1");
    cout << "50/50" << endl;
    colours('b', "1");
    cout << "2. ";
    colours('g', "1");
    cout << "Skip Question" << endl;
    colours('b', "1");
    cout << "3. ";
    colours('g', "1");
    cout << "Replace Question" << endl;
    colours('b', "1");
    cout << "4. ";
    colours('g', "1");
    cout << "Extra Time" << endl;
    colours('w');
    cout << "\nPlease select an option (1-4): ";
    cin >> option;

    return option;
}

// gets user input with a time limit
// parameters: answer (stores user input), timeLimitSeconds (time allowed)
// returns true if answered in time or false otherwise
bool getTimedAnswer(int &answer, int timeLimitSeconds, int &answerTime)
{
    cout << "You have " << timeLimitSeconds << " seconds to answer: " << flush;

    answer = -1;
    bool hasInput = false;
    int userInput;

    thread inputThread([&]()
                       {
            cin >> userInput;
            hasInput = true; });

    inputThread.detach();

    for (int i = timeLimitSeconds; i > 0; i--)
    {
        if (i == 10)
        {
            colours('g');
            cout << "\r" << string(50, ' ') << "\r"; // Clear line
            cout << "You have " << i << " seconds to answer: " << flush;
        }
        else if (i == 5)
        {
            colours('o');
            cout << "\r" << string(50, ' ') << "\r"; // Clear line
            cout << "You have " << i << " seconds to answer: \n**PANIC MODE ACTIVATED**" << flush;
        }
        else if (i == 3)
        {
            colours('r');
            cout << "\r" << string(50, ' ') << "\r"; // Clear line
            cout << "You have " << i << " seconds to answer: \n**JUST PICK SOMETHING!!!**" << flush;
        }

        // Check every 100ms if input arrived
        for (int j = 0; j < 10; j++)
        {
            if (hasInput)
            {
                answer = userInput;
                answerTime = i;
                return true;
            }
            this_thread::sleep_for(chrono::milliseconds(100));
        }
    }

    cout << endl;
    return false;
}

// Function: Shuffles questions randomly using Fisher-Yates algorithm
// Parameters: questions array, options array, answers array, size (number of questions)
// void shuffleQuestions(string questions[], string opt[], int answers[], int size)
// {
//     for (int i = size - 1; i > 0; i--)
//     {
//         int j = rand() % (i + 1);
//         swap(questions[i], questions[j]);
//         swap(opt[i], opt[j]);
//         swap(answers[i], answers[j]);
//     }
// }

// Function: Shuffles answer options randomly and updates correct answer position
// Parameters: options array, correctAnswer (updated by reference), size (number of options)
// void shuffleOptions(string options[], int& correctAnswer, int size)
// {
//     string correctOption = options[correctAnswer - 1];

//     for (int i = size - 1; i > 0; i--)
//     {
//         int j = rand() % (i + 1);
//         swap(options[i], options[j]);
//     }

//     for (int i = 0; i < size; i++)
//     {
//         if (options[i] == correctOption)
//         {
//             correctAnswer = i + 1;
//             break;
//         }
//     }
// }

// main gameplay - displays questions, handles answers, calculates score
// parameters: mode (category), difficulty level, player name, correct/incorrect/score
void displayQuestions(string mode, string difficulty, string name, int &correct, int &incorrect, int &score)
{
    clearConsole();
    correct = 0, incorrect = 0, score = 0;
    int skipped = 0;

    // arrays to store data fetched from files
    string options[5000];
    string questions[5000];
    string opt[5000];
    int answers[5000];

    // arrays to store wrong answers for review at the end
    string wrongQuestions[10];
    string wrongCorrectAnswers[10];
    string wrongUserAnswers[10];
    int wrongCount = 0;

    string question;
    string fileName = difficulty + "/" + mode + ".txt";
    cout << mode << endl;

    ifstream inputFile(fileName);

    int scoreMultiplier;
    int deduction = 2;

    char colour;

    // score multiplier and deduction based on difficulty
    if (difficulty == "easy")
    {
        scoreMultiplier = 1;
        deduction = 2;
        colour = 'g';
    }
    else if (difficulty == "medium")
    {
        scoreMultiplier = 2;
        deduction = 3;
        colour = 'o';
    }
    else
    {
        scoreMultiplier = 3;
        deduction = 5;
        colour = 'r';
    }

    mode[0] = toupper(mode[0]);
    difficulty[0] = toupper(difficulty[0]);

    header(mode + " - " + difficulty, colour);
    cout << endl;

    int q = 1;

    int streak = 0;
    int bonus = 0;

    // flags to keep track of lifelines
    bool used5050 = false;
    bool usedSkip = false;
    bool usedReplace = false;
    bool usedExtraTime = false;

    int baseTime = 10;
    int extraTime = 0;

    bool skipQ = false;

    ifstream ansArray(fileName);
    int ind = 0;
    int ansIndex = 0;
    int optIndex = 0;
    int qLine = 0;

    string statement;
    // fetch all questions, options, and answers from file
    while (getline(ansArray, statement))
    {
        if (qLine % 3 == 0) // question
        {
            questions[ind] = statement;
            ind++;
        }
        else if ((qLine + 1) % 3 == 0) // correct answer number
        {
            answers[ansIndex] = stoi(statement);
            ansIndex++;
        }
        else // all options
        {
            opt[optIndex] = statement;
            optIndex++;
        }
        qLine++;
    }

    // Shuffle questions randomly (only shuffle if we have enough questions)
    // if (ind >= 10)
    // {
    //     shuffleQuestions(questions, opt, answers, ind);
    // }

    string dadJokes[20] = {
        "What do you call a fish with no eyes?",
        "Why can't a bicycle stand on its own?",
        "What do you call a sleeping bull?",
        "What do you call a belt made out of watches?",
    };
    string djAnswers[20] = {
        "Fsh.",
        "Because it is two-tired!",
        "A bulldozer",
        "A waist of time",
    };

    // main loop to display 10 questions
    while (q <= 10 && q <= ind) // number of questions (10) but don't exceed available questions
    {
        // display a dad joke every 3 questions
        if (q % 3 == 0)
        {
            header("DAD JOKE TIME!!!", 'o');
            int randomSerial = rand() % 4;
            colours('b');
            cout << "Question: ";
            cout << dadJokes[randomSerial];

            colours('p');
            cout << "\nAnswer coming in: " << endl;
            for (int k = 3; k > 0; k--)
            {
                cout << k << endl;
                this_thread::sleep_for(chrono::seconds(1));
            }
            cout << endl;
            header(djAnswers[randomSerial], 'b');
            colours('g', "1");
            cout << "\nPress enter to suffer through more questions :)";
            cin.get();
            colours('w');
            cout << endl;
        }

        colours('o', "1");
        cout << "Question " << q << ". " << questions[q - 1] << endl;

        string option;
        int index = 0;

        int answerSerial = answers[q - 1];

        string answerString;

        int currentSerial = 1;
        int st = 0;
        string currentOptions = opt[q - 1];

        // check if currentOptions is empty
        if (currentOptions.empty())
        {
            cout << "Error: Missing options for this question!" << endl;
            q++;
            continue;
        }

        // store all the options in an array + store correct option as a string
        for (int i = 0; i <= currentOptions.length(); i++)
        {
            if (i == currentOptions.length() || currentOptions[i] == '/')
            {
                string portion = currentOptions.substr(st, i - st);
                options[index] = portion;
                if (currentSerial == answerSerial)
                {
                    answerString = portion;
                }
                index++;
                st = i + 1;
                currentSerial++;
            }
        }

        // Shuffle options randomly
        // shuffleOptions(options, answerSerial, 4);

        // Update answer string after shuffle
        // for (int i = 0; i < 4; i++)
        // {
        //     if (i == answerSerial - 1)
        //     {
        //         answerString = options[i];
        //     }
        // }

        // display all options
        for (int optNum = 0; optNum < 4; optNum++)
        {
            colours('b', "1");
            cout << optNum + 1 << ". ";
            colours('p', "1");
            cout << options[optNum] << endl;
        }

        colours('w');
        int answer, ansTime;
        cout << "\nEnter your answer (1-4) or 5 for lifeline: " << endl;
        bool answeredInTime = getTimedAnswer(answer, baseTime + extraTime, ansTime);
        extraTime = 0; // reset after use

        // procedure in case player runs out of time
        if (answeredInTime == false)
        {
            header("⏰ *BEEP BEEP BEEP* ⏰", 'r');
            incorrect++;
            streak = 0;
            score -= deduction;
            cout << "We could've grown a garden in the time you took!" << endl
                 << deduction << " points deducted.\n\n";

            // store it as a wrong answer
            wrongQuestions[wrongCount] = questions[q - 1];
            wrongCorrectAnswers[wrongCount] = answerString;
            wrongUserAnswers[wrongCount] = "No answer (timeout)";
            wrongCount++;

            q++;
            this_thread::sleep_for(chrono::seconds(1));
            continue;
        }

        skipQ = false;
        // lifeline handling
        if (answer == 5)
        {
            int lifeline = displayLifelines();

            // check if lifeline already used
            if (lifeline == 1 && used5050)
            {
                cout << "50/50 already used!\n";
                cin >> answer;
            }
            else if (lifeline == 2 && usedSkip)
            {
                cout << "Skip already used!\n";
                cin >> answer;
            }
            else if (lifeline == 3 && usedReplace)
            {
                cout << "Replace already used!\n";
                cin >> answer;
            }
            else if (lifeline == 4 && usedExtraTime)
            {
                cout << "Extra Time already used!\n";
                cin >> answer;
            }
            else
            {
                // marks lifeline flags as true
                if (lifeline == 1)
                    used5050 = true;
                if (lifeline == 2)
                    usedSkip = true;
                if (lifeline == 3)
                    usedReplace = true;
                if (lifeline == 4)
                    usedExtraTime = true;

                // Lifeline 1: 50/50, remove two wrong options
                if (lifeline == 1)
                {
                    string newOptions[3];
                    int randomSerial = rand() % 4;

                    while (randomSerial == answerSerial - 1)
                    {
                        randomSerial = rand() % 4;
                    }

                    cout << endl;
                    header("Cutting odds of failure in half", 'b');

                    for (int i = 0; i < 4; i++)
                    {
                        if (i == randomSerial || i == answerSerial - 1)
                        {
                            colours('b', "1");
                            cout << i + 1 << ". ";
                            colours('p', "1");
                            cout << options[i] << endl;
                        }
                    }
                    cin.ignore(1000, '\n');
                    colours('w');
                    cout << "\nEnter your answer (1-4): ";
                    cin >> answer;
                }
                // lifeline 2: skip current question
                else if (lifeline == 2)
                {
                    header("Avoiding commitment, I see. Next!", 'b');
                    answer = answerSerial;
                    correct++;
                    skipQ = true;
                }
                // lifeline 3: replace current question with a new question
                else if (lifeline == 3)
                {
                    header("Couldn't handle the heat?", 'b');
                    int randomSerial = rand() % ind;
                    string newQ = questions[randomSerial];

                    colours('o', "1");
                    cout << "Question " << q << ". " << newQ << endl;

                    int strt = 0;
                    int indice = 0;
                    string newOption = opt[randomSerial];
                    answerSerial = answers[randomSerial];

                    string replaceOptions[4];
                    int replaceIndex = 0;

                    for (int j = 0; j <= newOption.length(); j++)
                    {
                        if (j == newOption.length() || newOption[j] == '/')
                        {
                            string portion = newOption.substr(strt, j - strt);
                            replaceOptions[replaceIndex] = portion;
                            if (replaceIndex + 1 == answerSerial)
                            {
                                answerString = portion;
                            }
                            replaceIndex++;
                            strt = j + 1;
                        }
                    }

                    // shuffleOptions(replaceOptions, answerSerial, 4);

                    // for (int i = 0; i < 4; i++)
                    // {
                    //     if (i == answerSerial - 1)
                    //     {
                    //         answerString = replaceOptions[i];
                    //     }
                    // }

                    for (int i = 0; i < 4; i++)
                    {
                        colours('b', "1");
                        cout << i + 1 << ". ";
                        colours('p', "1");
                        cout << replaceOptions[i] << endl;
                    }
                    cin.ignore(1000, '\n');
                    colours('w');
                    cout << "\nEnter your answer (1-4): ";
                    cin >> answer;
                }
                // lifeline 4: add extra time (10 seconds)
                else if (lifeline == 4)
                {
                    header("Buying more time won't buy knowledge.", 'b');
                    cout << "You received +10 seconds.\n";
                    extraTime = 10;

                    cin.ignore(1000, '\n');

                    int ansTime;
                    cout << "\nRe-enter your answer (1-4): " << endl;
                    bool answeredInTime = getTimedAnswer(answer, baseTime + extraTime, ansTime);
                    extraTime = 0;

                    if (!answeredInTime)
                    {
                        header("TIME UP!", 'r');
                        incorrect++;
                        streak = 0;
                        score -= deduction;
                        cout << "Even extra time wasn't enough. Sad. -" << deduction << " points.\n";

                        wrongQuestions[wrongCount] = questions[q - 1];
                        wrongCorrectAnswers[wrongCount] = answerString;
                        wrongUserAnswers[wrongCount] = "No answer (timeout)";
                        wrongCount++;

                        q++;
                        continue;
                    }
                }
            }
        }

        // messages to display after answers
        string insults[20] = {
            "That wasn't even a trick question...",
            "My cat knows better.",
            "Bold, yet totally wrong.",
            "Zero points for effort.",
            "Confidence over competence.",
            "Awake? Debatable.",
            "You almost got it. Almost.",
            "Error 404: Answer Not Found.",
            "That's a new kind of wrong.",
        };

        string quickIncorrect[20] = {
            "Didn't even try, did you?",
            "Speed doesn't equal smarts.",
            "That was impressively fast, and awful.",
            "Confidence impressive, answer not :)",
            "Immediate failure.",
        };

        string correctAns[20] = {
            "Did you Google that?",
            "Stop flexing on me.",
            "Pure luck.",
            "Accidents happen.",
            "Did you cheat?",
            "Don't get used to that.",
            "Must be a fluke.",
        };

        // check if the answer is correct
        if (answer == answerSerial)
        {
            if (!skipQ)
            {
                int messages = 6;
                int serial = rand() % messages;
                string correctMessage = correctAns[serial];

                if (ansTime < 4)
                {
                    correctMessage = "Very slow, we're not impressed but ok";
                }

                header(correctMessage, 'g');
                correct++;
                streak++;

                bonus = 0;
                // streak rewards
                if (streak == 3)
                {
                    colours('g');
                    cout << "Streak of 3: Did you actually study for this?" << endl;
                    bonus = 5;
                    cout << "+" << bonus << "bonus points";
                }
                else if (streak == 5)
                {
                    colours('g');
                    cout << "Streak of 5: This is suspicious. Are you being fed answers?" << endl;
                    bonus = 15;
                    cout << "+" << bonus << "bonus points";
                }

                colours('w');
                cout << "Good Job! You earned " << 10 * scoreMultiplier + bonus << " points" << endl;

                score += 10 * scoreMultiplier + bonus;
            }
        }
        else // wrong answer
        {
            int messages = 8;
            int insultSerial = rand() % messages;
            string message = insults[insultSerial];
            streak = 0;
            if (ansTime >= 8)
            {
                insultSerial = rand() % 5; // 5 messages for quick incorrect answer
                message = quickIncorrect[insultSerial];
            }

            header(message, 'r');
            incorrect++;

            // store wrong answer for review at the end
            wrongQuestions[wrongCount] = questions[q - 1];
            wrongCorrectAnswers[wrongCount] = answerString;
            wrongUserAnswers[wrongCount] = options[answer - 1];
            wrongCount++;

            cout << "\nThe correct answer is ";
            colours('g', "1");
            cout << answerSerial << ". " << answerString << endl;
            colours('r', "1");
            cout << "You lost " << deduction << " points" << endl;
            colours('w');
            score -= deduction; // negative marking
        }

        colours('w');
        q++;
        cout << endl;
        this_thread::sleep_for(chrono::seconds(1));
        cin.ignore(1000, '\n');
    }

    // review wrong answers
    if (wrongCount > 0)
    {
        cout << "\nWould you like to review incorrect answers? (y/n): ";
        char review;
        cin >> review;

        if (review == 'y' || review == 'Y')
        {
            clearConsole();
            header("Review Incorrect Answers", 'r');

            for (int i = 0; i < wrongCount; i++)
            {
                colours('o', "1");
                cout << "\n"
                     << i + 1 << ". " << wrongQuestions[i] << endl;
                colours('r', "1");
                cout << "Your Answer: " << wrongUserAnswers[i] << endl;
                colours('g', "1");
                cout << "Correct Answer: " << wrongCorrectAnswers[i] << endl;
                colours('w');
                cout << endl;
            }

            cout << "\nPress Enter to continue...";
            cin.ignore(1000, '\n');
            cin.get();
            clearConsole();
        }
    }

    // store the score in high_scores.txt
    recordScore(name, mode, score);
    endScreen(correct, incorrect, score, name);
}

// displays end screen with final score and stats
// parameters: correct answers, incorrect answers, final score, player name
void endScreen(int correct, int incorrect, int score, string name)
{
    // log the quiz result in file
    ofstream logsFile("quiz_logs.txt", ios::app);
    logsFile << "\n" + name + "/" + getCurrentDate() + "/" + to_string(correct) + "/" + to_string(incorrect) + "/" + to_string(score) + "/";

    header("Quiz Completed! - Summary", 'B');
    int spacing = 15;
    colours('b', "1");
    cout << left << setw(spacing + 5) << "Final Score" << setw(spacing) << "Correct" << "Incorrect" << endl;
    colours('p');
    cout << left << setw(spacing + 5) << score << setw(spacing) << correct << incorrect << endl;

    colours('w');
    cout << "\nThank you for playing, " << name << endl;

    // replay feature
    char playAgain;
    colours('g', "1");
    cout << "\nWould you like to play again? (y/n): ";
    colours('w');
    cin >> playAgain;

    if (playAgain == 'y' || playAgain == 'Y')
    {
        cin.ignore(1000, '\n');
        clearConsole();
        displayMenu();
    }
    else
    {
        cout << "\nPress Enter to return to main menu.";
        cin.ignore(1000, '\n');
        cin.get();
        clearConsole();
        displayMenu();
    }
}
