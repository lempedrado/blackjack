#include <iostream>
#include <cmath>
#include <vector>
#include <sstream>

using namespace std;


//Card class for individual cards
class Card
{
    private: 
        string suit;
        string rank;
        int val;

    public:
        //Card constructor
        Card(string rank, string suit, int val)
        {
            this->rank = rank;
            this->suit = suit;
            this->val = val;
        }

        //get methods
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

        //set method
        void setVal(int i)
        {
            val = i;
        }
};


//Deck class for a vector of Card objects
class Deck
{
    private:
        //contains all Cards in the Deck
        vector<Card> deck;

        //number of Cards left to be dealt
        int size;

        //string array of card ranks
        string ranks[13] = {"A", "2", "3", "4", "5", "6", "7", "8", "9","10", "J", "Q", "K"};

        //string array of card suits
        string suits[4] = {"♥", "♦", "♠", "♣"};
        
    public:
        //constructs Deck
        Deck()
        {
            //full Deck consists of 3 combined standard decks of cards
            for(int i = 0; i < 3; i++)
            {
                //iterates for each rank
                for(int j = 0; j < 13; j++)
                {
                    //iterates for each suit
                    for(int k = 0; k < 4; k++)
                    {
                        int val;
                        //ACE card value
                        if(j == 0)
                            val = 11;
                        //face card values
                        else if(j == 10 || j == 11 || j == 12)
                            val = 10;
                        //number card values
                        else
                            val = j + 1;

                        //creates a Card object and adds it to the deck
                        deck.push_back(Card(ranks[j], suits[k], val));
                    }
                }
            }

            size = deck.size();
            //shuffle the deck twice
            shuffle();
            shuffle();
        }

        //shuffles the deck
        void shuffle()
        {
            for(int j = size - 1; j > 0; j--)
            {
                int pos = rand() % (j + 1);
                Card temp = deck.at(j);
                deck.at(j) = deck.at(pos);
                deck.at(pos) = temp;
            }
        }

        //deals the next Card in the deck and decrements the size
        Card deal()
        {
            //if the deck is almost empty, reshuffle a new Deck and place it under the remaining Cards
            if(deck.size() < 10)
            {
                Deck d;
                deck.insert(deck.begin(), d.deck.begin(), d.deck.end());
            }
            //gets the Card at the top of the deck
            Card c = deck.back();
            
            //removes the Card from the deck
            deck.pop_back();
            size = deck.size();

            //returns dealt Card
            return c;
        }
};


//Hand class to store all Cards a player has been dealt
class Hand
{
    private:
        //Hand total initializes to 0
        int total = 0;
        vector<Card> hand;
    public:

        Hand()
        {
        }

        //adds the Card to this Hand
        void add(Card c)
        {
            //adds card to the Hand vector
            hand.push_back(c);

            //add the Card's value to the Hand total
            total += c.getVal();

            //check if there is an ACE in this Hand that makes the hand BUST
            int count = 0;
            for(auto &c : hand)
            {
                //if an ACE is in the hand that makes it bust, count it as a 1 instead of 11
                if(c.getVal() == 11 && total > 21)
                {
                    c.setVal(1);
                    //lowers total if there are multiple ACEs, so any other ACEs will still be 11 if legal
                    total -= 10;
                }
                count += c.getVal();
            }
            total = count;
        }

        //clears any Cards from the Hand and resets the Hand total
        void newHand()
        {
            hand.clear();
            total = 0;
        }

        //get Hand total
        int getTotal()
        {
            return total;
        }

        //overload the << operator for printing a Hand
        friend ostream& operator<<(ostream &strm, Hand h);
};

ostream& operator<<(ostream &strm, Hand h)
{
    for(Card c : h.hand)
        strm << c.getRank() << c.getSuit() << " ";
    return strm;
}

//class to run a game of Blackjack
class BlackjackRunner
{
    private:
        Hand player, dealer;    //stores Cards in each players Hand
        Deck d;                 //Deck of Cards

    public:

        BlackjackRunner()
        {
        }

        //deals first two cards to DEALER and PLAYER to start hand
        void startGame()
        {
            for(int i = 0; i < 4; i++)
            {
                //PLAYER gets dealt first
                if(i % 2 == 0)
                    player.add(d.deal());
                else 
                    dealer.add(d.deal());
            }
        }
        
        
        //updates the DEALER and PLAYER Hands printed to console
        void update()
        {
            //hides DEALER's second Card (hole card)
            ostringstream s;
            s << dealer;
            string d = s.str().substr(0, s.str().find(" "));
            //prints dealer's and player's hands
            cout << "\n\nDEALER:\n" << d << " []\n" << "\nYOU:\n" << player << "\n" << player.getTotal() << "\n\n";
        }

        //checks Hands of DEALER and PLAYER for a winner
        int compareHands()
        {
            //get player and dealer hand totals
            int pTotal = player.getTotal();
            int dTotal = dealer.getTotal();
            
            //checks if player BUST
            if(pTotal > 21)
            {   
                cout << "\n\nDEALER:\n" << dealer << "\n" << dTotal << "\nYOU:\n" << player << "\n" << pTotal << "\n\n";
                cout << "YOU LOSE\n";
                return 1;
            }
            //checks if dealer has BLACKJACK
            else if(dTotal == 21)
            {
                cout << "\n\nDEALER:\n" << dealer << "\n" << dTotal << "\nYOU:\n" << player << "\n" << pTotal << "\n\n";
                //compares dealer hand to player hand
                if(pTotal != 21)
                    cout << "DEALER has BLACKJACK\nYOU LOSE\n";
                else
                    cout << "YOU and DEALER have BLACKJACK\n\tDRAW\n";
                return 1;
            }
            //checks if player got BLACKJACK
            else if(pTotal == 21)
            {
                cout << "\n\nDEALER:\n" << dealer << "\n" << dTotal << "\nYOU:\n" << player << "\n" << pTotal << "\n\n";
                cout << "YOU have BLACKJACK\nYOU WIN\n";
                return 1;
            }
            return 0;
        }

        //when player has a valid hand and decides to stand, determines winner
        void stand()
        {            
            //DEALER must hit while below 17
            while(dealer.getTotal() < 17)
                dealer.add(d.deal());

            int dTotal = dealer.getTotal(), pTotal = player.getTotal();
            cout << "\n\nDEALER:\n" << dealer << "\n" << dTotal << "\nYOU:\n" << player << "\n" << pTotal << "\n\n";
            //checks if DEALER BUST
            if(dTotal > 21)
            {
                cout << "DEALER BUST.\tYOU WIN\n";
                return;
            }
            //if DEALER stands
            if(pTotal > dTotal)
                cout << "YOU WIN\n";
            else if(dTotal > pTotal)
                cout << "YOU LOSE\n";
            else
                cout << "DRAW\n";
            compareHands();
        }

        //plays a game of Blackjack
        void play()
        {
            string input;
            player.newHand();
            dealer.newHand();

            startGame();          //deals starting hands to begin game
            update();             //prints updated Hands to console
            
            //after cards are dealt, check if anyone has BLACKJACK
            if(compareHands())
                return;

            //runs if nobody has BLACKJACK on the deal
            while(input != "Stand" && input != "s")
            {
                cout << "'Hit' or 'Stand': ";
                cin >> input;

                //validates user input
                while(input != "Hit" && input != "Stand" && input != "h" && input != "s")
                {
                    cout << "Invalid. Enter 'Hit' or 'Stand': ";
                    cin >> input;
                }

                //player HITS
                if(input == "Hit" || input == "h")
                {
                    player.add(d.deal());
                    update();
                    if(compareHands())
                        return;
                }
            }
            stand();
        }

};


//main function to start playing BLACKJACK
int main()
{
    srand(time(0));
    
    BlackjackRunner bjr;
    string input;
    do
    {
        bjr.play();
        cout << "Would you like to play another hand? y/n?: ";
        cin >> input;
        //input validation
        while(input != "y" && input != "yes" && input != "n" && input != "no")
        {
            cout << "Invalid input. Please enter 'y' or 'n': ";
            cin >> input;
        }
    }
    while(input != "n" && input != "no");
    cout << "\n\nThank you for playing :)\n\n\n";

    return 0;
}
