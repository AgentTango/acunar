//  header files
#include <iostream>
#include <string>
#include <math.h>

using namespace std;

//  global variables
string s[14];              //  value of each string s1 .. s14
int coin[] = {0, 0, 0, 0}; //  value of each coin c1 .. c4

// function prototypes
void updateNetwork(long long &network, int pos, int player);
void coinIsCaptured(int coinNumber);
void updateStringValues(long long network);
void extraChance(long long &network, int player);
void changePlayer(int &player);
void addPlayerScore(long long &network, int player);
void displayNetwork(long long network);
bool stringStatus(long long network, int pos);
bool gameInProgress(long long network);
int playerScore(long long network, int player);
int turn(int player);
int isCoinCaptured(int coinNumber);

//  function used to check the progress of the game
//  is any string left uncut in the network
//  this function returns a bool value
bool gameInProgress(long long network)
{
    // need to discard the last two values in the network as they are scores of the players
    network /= 100; // local variable passed by call by value
    for (int i = 1; i <= 12; i++)
    {
        if (network % 10 == 1)
            return true;
        network /= 10;
    }
    return false;
}

//  function changes the value of coin
//  from 0 to 1
//  as 0 -> the coin is still not captured yet
//  and 1-> the coin is captured now
void coinIsCaptured(int coinNumber)
{
    if (coinNumber == 1)
        coin[0] = 1;
    else if (coinNumber == 2)
        coin[1] = 1;
    else if (coinNumber == 3)
        coin[2] = 1;
    else
        coin[3] = 1;
}

//  function to check is the coin captured or not
//  captured means is it already been taken as a point by any player
int isCoinCaptured(int coinNumber)
{
    if (coinNumber == 1)
        return coin[0];
    else if (coinNumber == 2)
        return coin[1];
    else if (coinNumber == 3)
        return coin[2];
    else
        return coin[3];
}

//  returns true if position pos of the game network still has a string
//  returns false otherwise
bool stringStatus(long long network, int pos)
{
    // initial network = 11111111111100
    network /= 100; // network/100 = 111111111111

    // looping till the pos in the string
    for (int i = 0; i < 12 - pos; i++)
        network /= 10;
    return (network % 10 != 0);
}

//  prints the network to the screen
void displayNetwork(long long network)
{
    int p1, p2;

    p1 = network / 10 % 10; // Player 1 score        // because the position of the score is s13
    p2 = network % 10;      // Player 2 score        // because position is s14

    updateStringValues(network);

    cout << "##########\n";
    cout << "#  " << s[0] << "  " << s[1] << "  #\n";
    cout << "#" << s[2] << "$" << s[3] << "$" << s[4] << "#\n";
    cout << "#  " << s[5] << "  " << s[6] << "  #\n";
    cout << "#" << s[7] << "$" << s[8] << "$" << s[9] << "#\n";
    cout << "#  " << s[10] << "  " << s[11] << "  # Player 1 score: " << p1 << "\n";
    cout << "########## Player 2 score: " << p2 << "\n";
}

//  function to update the values of the string
//  after player makes a move
void updateStringValues(long long network)
{
    s[0] = stringStatus(network, 1) ? "|" : " ";
    s[1] = stringStatus(network, 2) ? "|" : " ";
    s[2] = stringStatus(network, 3) ? "--" : "  ";
    s[3] = stringStatus(network, 4) ? "--" : "  ";
    s[4] = stringStatus(network, 5) ? "--" : "  ";
    s[5] = stringStatus(network, 6) ? "|" : " ";
    s[6] = stringStatus(network, 7) ? "|" : " ";
    s[7] = stringStatus(network, 8) ? "--" : "  ";
    s[8] = stringStatus(network, 9) ? "--" : "  ";
    s[9] = stringStatus(network, 10) ? "--" : "  ";
    s[10] = stringStatus(network, 11) ? "|" : " ";
    s[11] = stringStatus(network, 12) ? "|" : " ";
}

//  returns the score of Player p in network
int playerScore(long long network, int player)
{
    return player == 1 ? network / 10 % 10 : network % 10;
}

//  function to be called at each player's turn
int turn(int player)
{
    int pos;
    cout << "Player " << player << ", make your move (1-12): ";
    cin >> pos;
    while (pos < 1 || pos > 12)
    {
        cout << "Invalid. Try again!" << endl;
        pos = turn(player);
    }
    return pos;
}

//  function to change the player
void changePlayer(int &player)
{
    if (player == 1)
        player = 2;
    else
        player = 1;
}

//  function to compute the player's score
void addPlayerScore(long long &network, int player)
{
    // need to update the value at respective position in the network
    if (player == 1)
        network += 10;
    else
        network += 1;
}

//  function to give player extra chance when a point is added to his score
void extraChance(long long &network, int player)
{
    //  firstly check if the game is playable or not
    //  incase the game is not playable anymore display
    //  score and compute the winner
    //  otherwise give the player an extra chance
    if (!gameInProgress(network))
    {
        displayNetwork(network);
        return;
    }
    int pos;
    // notify which player gets an extra chance
    cout << "Player " << player << " scores! Gets another turn.\n";
    displayNetwork(network);
    pos = turn(player);
    updateNetwork(network, pos, player);
}

//  performs the task of player cutting a string at position in network.
//  The reference parameter network should get updated, and if any coins are
//  disconnected, the score of player should be incremented, to reflect the
//  new network configuration after the string is cut
void updateNetwork(long long &network, int pos, int player)
{
    //  check whether or not the position entered by player is already cut or not
    if (stringStatus(network, pos))
    {
        network = network - pow(10, 14 - pos); //  update the position of the string cut
        updateStringValues(network);
    }
    else
    {
        while (!stringStatus(network, pos))
        {
            cout << "Invalid. Try again!" << endl;
            pos = turn(player);
        }
        updateNetwork(network, pos, player);
    }

    //  check if any coin is captured or not
    if ((s[0] == " ") && (s[2] == "  ") && (s[3] == "  ") && (s[5] == " ") && !isCoinCaptured(1))
    {
        addPlayerScore(network, player); // add score to the respective player
        coinIsCaptured(1);               // make the coin captured
        extraChance(network, player);    // offer the player an extra chance
    }
    else if ((s[1] == " ") && (s[4] == "  ") && (s[3] == "  ") && (s[6] == " ") && !isCoinCaptured(2))
    {
        addPlayerScore(network, player); // add score to the respective player
        coinIsCaptured(2);               // make the coin captured
        extraChance(network, player);    // offer the player an extra chance
    }
    else if ((s[5] == " ") && (s[7] == "  ") && (s[8] == "  ") && (s[10] == " ") && !isCoinCaptured(3))
    {
        addPlayerScore(network, player); // add score to the respective player
        coinIsCaptured(3);               // make the coin captured
        extraChance(network, player);    // offer the player an extra chance
    }
    else if ((s[6] == " ") && (s[8] == "  ") && (s[9] == "  ") && (s[11] == " ") && !isCoinCaptured(4))
    {
        addPlayerScore(network, player); // add score to the respective player
        coinIsCaptured(4);               // make the coin captured
        extraChance(network, player);    // offer the player an extra chance
    }
}

//  execution begins here
int main()
{
    int pos, player = 1;
    long long network = 11111111111100; //initial network

    while (gameInProgress(network))
    {
        displayNetwork(network);
        pos = turn(player);
        updateNetwork(network, pos, player);
        changePlayer(player);
    }

    if (playerScore(network, 1) > playerScore(network, 2))
        cout << "Player 1 wins!\n";
    else if (playerScore(network, 2) > playerScore(network, 1))
        cout << "Player 2 wins!\n";
    else if (playerScore(network, 1) == playerScore(network, 2))
        cout << "Draw game!\n";
    return 0;
}