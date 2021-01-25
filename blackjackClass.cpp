#include <iostream>
#include <cmath>
#include <vector>
using namespace std;




class Card
{
    private: 
        string suit;
        string rank;
        int val;

    public:

        Card(string rank, string suit, int val)
        {
            this->rank = rank;
            this->suit = suit;
            this->val = val;
        }

        string getRank()
        {
            return rank;
        }

        string getSuit()
        {
            return suit;
        }

        int getVal()
        {
            return val;
        }
};

ostream& operator<<(ostream &strm, Card &c) 
{
    return strm << c.getRank() << c.getSuit();
}

class Deck
{
    private:
        vector<Card> cards;

    public:

        Deck(string ranks[], string suits[], int values[])
        {
            int rlen = sizeof(ranks) / sizeof(ranks[0]), slen = sizeof(suits) / sizeof(suits[0]);
            for(int j = 0; j < rlen; j++)
            {
                for(int k = 0; k < slen; k++)
                {
                    int val;
                    if(j == 1)
                        val = 11;
                    else if(j == 11 || j == 12 || j == 0)
                        val = 10;
                    else
                        val = j;
                    cards.push_back(Card(ranks[j], suits[k], val));   
                }
            }
        }
};

/*

string cards[52] = {""};                    //initializes empty deck of cards
string suits[4] = {"♥", "♦", "♠", "♣"};     //array of card suits
int cardsDealt[52] = {0};                   //placeholder array for each card if dealt
int pHand, dHand;                           //stores numerical value of hands
string player, dealer;                      //string output to display hands
bool hasAce;                                //determines if player has a preexisting ace to treat next drawn ace as 1
bool handEnd;                               //determines when a hand is finished




*
                !!!! FUTURE NOTE !!!!

    maybe implement split hands, but bets cant be implemented so 
    maybe split hands arent necessary and wold require making separate 
    strings for each hand or keeping track of where first hand ends
*




//builds the deck of cards to be used
void makeDeck()
{
    //initializes deck and dealt cards to default values
    for(int i = 0; i < 52; i++)
    {
        cards[i] = "";
        cardsDealt[i] = 0;
    }

    //assigns cards values to deck, cards[]
    for(int k = 0; k < 4; k++)
    {
        for(int i = 0; i < 13; i++)
        {
            int index = (13 * k) + (i % 13);

            if(i == 1)
                cards[index] += "A";
            else if (i == 11) 
                cards[index] += "J";
            else if (i == 12) 
                cards[index] += "Q";
            else if(i == 0)
                cards[index] += "K";
            else
                cards[index] += to_string(i); 

            cards[index] += suits[k];
        }
    }
}



//draws a cards from the deck and adds it to respective player's hand
string hit(int p)
{
    int i = rand() % 52;
    //cout << "DEBUGGING: " << i << endl; //debugging

    while(cardsDealt[i] == 1)
    {
        i = rand() % 52;
        //cout << "DEBUGGING: " << i << endl; //debugging
    }

    int val = i % 13;

    if(p)
    {   
        //decides if a dealt ace will equal 1 or 11
        if(val == 1)
        {
            hasAce = true;
            if(pHand + 11 > 21)
                pHand += val;
            else
                pHand += 11;
        }
        //makes all face cards value 10
        else if(val == 0 || val > 10)
            pHand += 10;
        else
            pHand += val;
    }
    else
    {
        if(val == 1)
        {
            if(dHand + 11 > 21)
                dHand += val;
            else
                dHand += 11;
        }
        else if(val == 0 || val > 10)
            dHand += 10;
        else
            dHand += val;
    }
    cardsDealt[i] = 1;

    return cards[i];
}



//deals first two cards to DEALER and PLAYER to start hand
void startGame()
{
    for(int i = 0; i < 4; i++)
    {
        if(i % 2 == 0)
            dealer += hit(0) + " ";
        else 
            player += hit(1) + " ";
    }
   
    //cout << "DEBUGGING: " << d << endl;
}



//updates the DEALER and PLAYER hands printed to console
void update()
{
    string s = dealer.substr(0, dealer.find(" "));
    cout << "\n\nDEALER:\n" << s << " []\n" << "\nYOU:\n" << player << "\n" << pHand << "\n\n";
}



//checks hands of DEALER and PLAYER for a winner
void compareHands(int end)
{
    
    //checks if player has a playable hand
    if(pHand > 21)
    {   
        cout << "\n\nDEALER:\n" << dealer << endl << dHand << "\nYOU:\n" << player << "\n" << pHand << "\n\n";
        cout << "YOU LOSE\n";
        handEnd = true;
    }


    //checks for BLACKJACK
    if(dHand == 21)
    {
        cout << "\n\nDEALER:\n" << dealer << endl << dHand << "\nYOU:\n" << player << "\n" << pHand << "\n\n";
        handEnd = true;
        if(pHand != 21)
        {
            cout << "DEALER has BLACKJACK\nYOU LOSE\n";
            return;
        }
        cout << "YOU and DEALER have BLACKJACK\n\tDRAW\n";
        return;
    }


    if(pHand == 21)
    {
        cout << "\n\nDEALER:\n" << dealer << endl << dHand << "\nYOU:\n" << player << "\n" << pHand << "\n\n";
        handEnd = true;
        cout << "YOU have BLACKJACK\nYOU WIN\n";
        return;
    }


    //for starting hands dealt
    if(!end)
        return;

    
    cout << "\n\nDEALER:\n" << dealer << endl << dHand << "\nYOU:\n" << player << "\n" << pHand << "\n\n";

    if(dHand > 21)
    {
        cout << "DEALER BUST.\tYOU WIN\n";
        return;
    }

    handEnd = true;
    //determines which hand wins
    if(pHand > dHand)
    {
        cout << "YOU WIN\n";
    }
    else if(dHand > pHand)
        cout << "YOU LOSE\n";
    else
        cout << "DRAW\n";
}



//when player has a valid hand and decides to stand, determines winner
void stand()
{
    //cout << "DEALER:\n" << dealer << endl << dHand << "\nYOU:\n" << player << "\n" << pHand << "\n\n";
    
    //DEALER must hit while below 17
    while(dHand < 17)
    {
        dealer += hit(0) + " ";
        //cout << "\n\nDEALER:\n" << dealer << endl << dHand << "\nYOU:\n" << player << "\n" << pHand << "\n\n";
    }

    compareHands(1);
}



//function that calls all previous functions and initializes variables to play blackjack
void play()
{
    makeDeck(); //builds deck of cards
    pHand = 0, dHand = 0;
    hasAce = false;
    handEnd = false;
    string input;

    //deals starting hand to begin game
    player = "", dealer = "";    //output to display daelt hands
    startGame();          //daels starting hands to begin game
    update();             //prints updated hands to console

    
    //after cards are dealt, check if anyone has blackjack
    compareHands(0);

    //runs if nobody has BLACKJACK on the draw
    if(!handEnd)
    {
        while(input != "Stand" && input != "s")
        {
            cout << "'Hit' or 'Stand': ";
            cin >> input;
            while(input != "Hit" && input != "Stand" && input != "h" && input != "s")
            {
                cout << "Invalid. Enter 'Hit' or 'Stand': ";
                cin >> input;
            }
            if(input == "Hit" || input == "h")
            {
                player += hit(1) + " ";  
                //if player draws a second ace, make the value of the first one equal 1 instead of 11
                if(hasAce && pHand > 21)
                {
                    hasAce = false;
                    pHand -= 10;
                }
                update();
                compareHands(0);
                if(handEnd)
                    return;
            }
        }
        stand();
    }
}

*/

//main function to start playing BLACKJACK and potential future hands
int main()
{
    srand(time(0));
    /*
    play();

    string input = "";
    cout << "Would you like to play another hand? y/n?: ";
    cin >> input;
    while(input != "y" && input != "n")
    {
        cout << "Invalid. Please enter 'y' or 'n': ";
        cin >> input;
    }
    if(input == "y")
        main();
*/
    return 0;
}
