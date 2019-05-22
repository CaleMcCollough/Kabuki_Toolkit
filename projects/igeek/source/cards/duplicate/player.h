/** kabuki::cards
    @file       ~/source/kabuki/cards/player.h
    @author  Cale McCollough <cale.mccollough@gmail.com>
    @license Copyright (C) 2017 Cale McCollough <calemccollough.github.io>;
             All right reserved (R). Licensed under the Apache License, Version 
             2.0 (the "License"); you may not use this file except in 
             compliance with the License. You may obtain a copy of the License 
             [here](http://www.apache.org/licenses/LICENSE-2.0). Unless 
             required by applicable law or agreed to in writing, software
             distributed under the License is distributed on an "AS IS" BASIS,
             WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or 
             implied. See the License for the specific language governing 
             permissions and limitations under the License.
*/

#ifndef KABUKI_BLACKJACK_HAND_H
#define KABUKI_BLACKJACK_HAND_H

#include "../global.h"

#include "Hand.h"
#include "CardStack.h"

namespace KabukiCardGames
{
class Player
{
public:
    //++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

    /** Default Constructor. */
    Player (String thisPlayerName = "You", int startPoints = 10, bool playerOrDealer = false);
    virtual ~Player () {}                       //< Virtual destructor.

    virtual bool playOrPass (Hand& other) = 0;  /*< Virtual function that determines if this Player plays, or passes. */
    virtual void playHand () = 0;               //< Virtual function that performs a Player's turn.
    virtual Array<Card*> discard ();            //< Virtual function that performs the discard action.

    String& getName ();                         //< Returns the playerName.
    void setName (const String& newName);       //< Sets the playerName to the newName.
    
    Hand& getHand ();                           //< Returns the player's hand.
    void setHand (Hand& newHand);               //< Sets the hand to the newHand.

    int getNumPoints ();                        //< Returns the point total.
    int addPoints (int num_points);             /*< Adds a specified numPoints to the players point total.
                                                    @return Returns 0 upon success, and -1 if numPoints is less than 1. */
    int removePoints (int num_points);          /*< Removes the specified numPoints from the players point total.
                                                    @return Returns 0 upon success, -1 if thesePoints is less than 0, or returns the number of missing points if the 
                                                        player doesn't have enough points to take. */
    void resetWins ();                          //< Resets the numWins to 0.
    int getNumWins ();                          //< Returns the number of wins.
    void addWin ();                             //< Adds a win to the players numWins.

    void dealHand (Hand& newHand);              //< Resets the number of wins to 0.

    String toString ();                         //< Returns a String reprentation of this object.

private:
    //------------------------------------------------------------------------------

    String name;                                //< The Player's name.

    bool isDealer;                              //< Stores if this player is the dealer or not.

    int numPoints,                              //< The number of points.
        numWins;                                //< The total number of wins.

    Hand& hand;                                 //< The Player's Hand.
};
}
#endif // DOJOCARDGAMES__PLAYER_H_INCLUDED
