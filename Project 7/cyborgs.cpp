//
//  cyborgs.cpp
//  cyborgs
//
//  Created by Chris Head on 5/29/22.
//

// cyborgs.cpp

// Portions you are to complete are marked with a TODO: comment.
// We've provided some incorrect return statements (so indicated) just
// to allow this skeleton program to compile and run, albeit incorrectly.
// The first thing you probably want to do is implement the utterly trivial
// functions (marked TRIVIAL).  Then get Arena::display going.  That gives
// you more flexibility in the order that you choose to tackle the rest of
// the functionality.  As you finish implementing each TODO: item, remove
// its TODO: comment; that makes it easier to find what you have left to do.

#include <iostream>
#include <string>
#include <random>
#include <utility>
#include <cstdlib>
#include <cctype>
#include <cassert>
using namespace std;

///////////////////////////////////////////////////////////////////////////
// Manifest constants
///////////////////////////////////////////////////////////////////////////

const int MAXROWS = 20;              // max number of rows in the arena
const int MAXCOLS = 20;              // max number of columns in the arena
const int MAXCYBORGS = 100;          // max number of cyborgs allowed
const int MAXCHANNELS = 3;           // max number of channels
const int INITIAL_CYBORG_HEALTH = 3; // initial cyborg health
const double WALL_DENSITY = 0.11;    // density of walls

const int NORTH = 0;
const int EAST  = 1;
const int SOUTH = 2;
const int WEST  = 3;
const int NUMDIRS = 4;
const int BADDIR = -1;


///////////////////////////////////////////////////////////////////////////
// Type definitions
///////////////////////////////////////////////////////////////////////////

class Arena;  // This is needed to let the compiler know that Arena is a
              // type name, since it's mentioned in the Cyborg declaration.

bool attemptMove(const Arena& a, int dir, int& r, int& c); //added so I could use in other functions

class Cyborg
{
  public:
      // Constructor
    Cyborg(Arena* ap, int r, int c, int channel);

      // Accessors
    int  row() const;
    int  col() const;
    int  channel() const;
    bool isDead() const;

      // Mutators
    void forceMove(int dir);
    void move();

  private:
    Arena* m_arena;
    int    m_row;
    int    m_col;
    int    m_channel;
    int    m_health;
};

class Player
{
  public:
      // Constructor
    Player(Arena* ap, int r, int c);

      // Accessors
    int  row() const;
    int  col() const;
    bool isDead() const;

      // Mutators
    string stand();
    string move(int dir);
    void   setDead();

  private:
    Arena* m_arena;
    int    m_row;
    int    m_col;
    bool   m_dead;
};

class Arena
{
  public:
      // Constructor/destructor
    Arena(int nRows, int nCols);
    ~Arena();

      // Accessors
    int     rows() const;
    int     cols() const;
    Player* player() const;
    int     cyborgCount() const;
    bool    hasWallAt(int r, int c) const;
    int     numberOfCyborgsAt(int r, int c) const;
    void    display(string msg) const;

      // Mutators
    void   placeWallAt(int r, int c);
    bool   addCyborg(int r, int c, int channel);
    bool   addPlayer(int r, int c);
    string moveCyborgs(int channel, int dir);

  private:
    bool    m_wallGrid[MAXROWS][MAXCOLS];
    int     m_rows;
    int     m_cols;
    Player* m_player;
    Cyborg* m_cyborgs[MAXCYBORGS];
    int     m_nCyborgs;

      // Helper functions
    void checkPos(int r, int c, string functionName) const;
    bool isPosInBounds(int r, int c) const;
};

class Game
{
  public:
      // Constructor/destructor
    Game(int rows, int cols, int nCyborgs);
    ~Game();

      // Mutators
    void play();

  private:
    Arena* m_arena;

      // Helper functions
    string takePlayerTurn();
    string takeCyborgsTurn();
};

///////////////////////////////////////////////////////////////////////////
//  Auxiliary function declarations
///////////////////////////////////////////////////////////////////////////

int decodeDirection(char ch);
int randInt(int lowest, int highest);
bool attemptMove(const Arena& a, int dir, int& r, int& c);
bool recommendMove(const Arena& a, int r, int c, int& bestDir);
void clearScreen();

///////////////////////////////////////////////////////////////////////////
//  Cyborg implementation
///////////////////////////////////////////////////////////////////////////

Cyborg::Cyborg(Arena* ap, int r, int c, int channel)
{
    if (ap == nullptr)
    {
        cout << "***** A cyborg must be created in some Arena!" << endl;
        exit(1);
    }
    if (r < 1  ||  r > ap->rows()  ||  c < 1  ||  c > ap->cols())
    {
        cout << "***** Cyborg created with invalid coordinates (" << r << ","
             << c << ")!" << endl;
        exit(1);
    }
    if (channel < 1  ||  channel > MAXCHANNELS)
    {
        cout << "***** Cyborg created with invalid channel " << channel << endl;
        exit(1);
    }
    m_arena = ap;
    m_row = r;
    m_col = c;
    m_channel = channel;
    m_health = INITIAL_CYBORG_HEALTH;
}

int Cyborg::row() const
{
    return m_row;
}

int Cyborg::col() const
{
    return m_col;
}

int Cyborg::channel() const
{
    return m_channel;
}

bool Cyborg::isDead() const
{
      if (m_health <= 0)
          return true;
      else
          return false;
        
}

void Cyborg::forceMove(int dir)
{
    int currentRow = m_row;
    int currentCol = m_col;

    switch(dir)
    {
        case NORTH:
            if (!attemptMove(*m_arena, NORTH, currentRow, currentCol))
                m_health--;
            else
                m_row--;
            break;
        case SOUTH:
            if (!attemptMove(*m_arena, SOUTH, currentRow, currentCol))
                m_health--;
            else
                m_row++;
            break;
        case EAST:
            if (!attemptMove(*m_arena, EAST, currentRow, currentCol))
                m_health--;
            else
                m_col++;
            break;
        case WEST:
            if (!attemptMove(*m_arena, WEST, currentRow, currentCol))
                m_health--;
            else
                m_col--;
            break;
    }
        
            
    
}

void Cyborg::move()
{
      // Attempt to move in a random direction; if cannot move, don't move
    if (!isDead())
        attemptMove(*m_arena, randInt(0, NUMDIRS-1), m_row, m_col);
}

///////////////////////////////////////////////////////////////////////////
//  Player implementation
///////////////////////////////////////////////////////////////////////////

Player::Player(Arena* ap, int r, int c)
{
    if (ap == nullptr)
    {
        cout << "***** The player must be created in some Arena!" << endl;
        exit(1);
    }
    if (r < 1  ||  r > ap->rows()  ||  c < 1  ||  c > ap->cols())
    {
        cout << "**** Player created with invalid coordinates (" << r
             << "," << c << ")!" << endl;
        exit(1);
    }
    m_arena = ap;
    m_row = r;
    m_col = c;
    m_dead = false;
}

int Player::row() const
{
    return m_row;
}

int Player::col() const
{
    return m_col;
}

string Player::stand()
{
    return "Player stands.";
}

string Player::move(int dir)
{
      
      //        direction.  If this fails,
      //        return "Player couldn't move; player stands."
      //        A player who moves onto a cyborg dies, and this
      //        returns "Player walked into a cyborg and died."
      //        Otherwise, return one of "Player moved north.",
      //        "Player moved east.", "Player moved south.", or
      //        "Player moved west."
    
   
    int currentRow = m_row;
    int currentCol = m_col;
    
    
    switch(dir)
    {
        case NORTH:
            if (!attemptMove(*m_arena, dir, currentRow, currentCol))
                return "Player couldn't move; player stands.";
            else if (m_arena->numberOfCyborgsAt(m_row-1, m_col) > 0){
                m_row--;
                m_dead = true;
                return "Player walked into a cyborg and died.";
            }
            else
            {
                m_row--;
                return "Player moved north.";
            }
            break;
        case SOUTH:
            if (!attemptMove(*m_arena, dir, currentRow, currentCol))
                return "Player couldn't move; player stands.";
            else if (m_arena->numberOfCyborgsAt(m_row+1, m_col) > 0){
                m_row++;
                m_dead = true;
                return "Player walked into a cyborg and died.";
            }
            else
            {
                m_row++;
                return "Player moved south.";
            }
            break;
        case EAST:
            if (!attemptMove(*m_arena, dir, currentRow, currentCol))
                return "Player couldn't move; player stands.";
            else if (m_arena->numberOfCyborgsAt(m_row, m_col+1) > 0){
                m_col++;
                m_dead = true;
                return "Player walked into a cyborg and died.";
            }
            else
            {
                m_col++;
                return "Player moved east.";
            }
            break;
        case WEST:
            if (!attemptMove(*m_arena, dir, currentRow, currentCol))
                return "Player couldn't move; player stands.";
            else if (m_arena->numberOfCyborgsAt(m_row, m_col-1) > 0){
                m_col--;
                m_dead = true;
                return "Player walked into a cyborg and died.";
            }
            else
            {
                m_col--;
                return "Player moved west.";
            }
            break;
    }
    return "FAILED";
    
}

bool Player::isDead() const
{
      if (m_dead)
          return true;
      else
          return false;
}

void Player::setDead()
{
    m_dead = true;
}

///////////////////////////////////////////////////////////////////////////
//  Arena implementation
///////////////////////////////////////////////////////////////////////////

Arena::Arena(int nRows, int nCols)
{
    if (nRows <= 0  ||  nCols <= 0  ||  nRows > MAXROWS  ||  nCols > MAXCOLS)
    {
        cout << "***** Arena created with invalid size " << nRows << " by "
             << nCols << "!" << endl;
        exit(1);
    }
    m_rows = nRows;
    m_cols = nCols;
    m_player = nullptr;
    m_nCyborgs = 0;
    for (int r = 1; r <= m_rows; r++)
        for (int c = 1; c <= m_cols; c++)
            m_wallGrid[r-1][c-1] = false;
}

Arena::~Arena()
{
    delete m_player;
    
    for (int k = 0; k < m_nCyborgs; k++)
        delete m_cyborgs[k];
}

int Arena::rows() const
{
    return m_rows;
}

int Arena::cols() const
{
    return m_cols;
}

Player* Arena::player() const
{
    return m_player;
}

int Arena::cyborgCount() const
{
    return m_nCyborgs;
}

bool Arena::hasWallAt(int r, int c) const
{
    checkPos(r, c, "Arena::hasWallAt");
    return m_wallGrid[r-1][c-1];
}

int Arena::numberOfCyborgsAt(int r, int c) const
{
    int numOfCyborgs = 0;
    
    for (int k = 0; k < m_nCyborgs; k++)
    {
        if (m_cyborgs[k]->col() == c && m_cyborgs[k]->row() == r)
            numOfCyborgs++;
    }
    
    return numOfCyborgs;
    
}

void Arena::display(string msg) const
{
    char displayGrid[MAXROWS][MAXCOLS];
    int r, c;
    
      // Fill displayGrid with dots (empty) and stars (wall)
    for (r = 1; r <= rows(); r++)
        for (c = 1; c <= cols(); c++)
            displayGrid[r-1][c-1] = (hasWallAt(r, c) ? '*' : '.');

      // Indicate cyborg positions by their channels.  If more than one cyborg
      // occupies a cell, show just one (any one will do).
    
    for (int k = 0; k < m_nCyborgs; k++)
    {
        if (m_cyborgs[k] != nullptr)
        {
        int channelNum = m_cyborgs[k]->channel();
        char c;
        
        switch(channelNum)
        {
            case 1:
                c = '1';
                break;
            case 2:
                c = '2';
                break;
            case 3:
                c = '3';
                break;
            default:
                break;
        }
        
           
            
                displayGrid[m_cyborgs[k]->row()-1][m_cyborgs[k]->col()-1] = c;
            
            
        }
    }

      // Indicate player's position
    if (m_player != nullptr)
        displayGrid[m_player->row()-1][m_player->col()-1] = (m_player->isDead() ? 'X' : '@');

      // Draw the grid
    clearScreen();
    for (r = 1; r <= rows(); r++)
    {
        for (c = 1; c <= cols(); c++)
            cout << displayGrid[r-1][c-1];
        cout << endl;
    }
    cout << endl;

      // Write message, cyborg, and player info
    if (msg != "")
        cout << msg << endl;
    cout << "There are " << cyborgCount() << " cyborgs remaining." << endl;
    if (m_player == nullptr)
        cout << "There is no player!" << endl;
    else if (m_player->isDead())
        cout << "The player is dead." << endl;
}

void Arena::placeWallAt(int r, int c)
{
    checkPos(r, c, "Arena::placeWallAt");
    m_wallGrid[r-1][c-1] = true;
}

bool Arena::addCyborg(int r, int c, int channel)
{
    if (! isPosInBounds(r, c)  ||  hasWallAt(r, c))
        return false;
    if (m_player != nullptr  &&  m_player->row() == r  &&  m_player->col() == c)
        return false;
    if (channel < 1  ||  channel > MAXCHANNELS)
        return false;
    if (m_nCyborgs == MAXCYBORGS)
        return false;
    m_cyborgs[m_nCyborgs] = new Cyborg(this, r, c, channel);
    m_nCyborgs++;
    return true;
}

bool Arena::addPlayer(int r, int c)
{
    if (m_player != nullptr  ||  ! isPosInBounds(r, c)  ||  hasWallAt(r, c))
        return false;
    if (numberOfCyborgsAt(r, c) > 0)
        return false;
    m_player = new Player(this, r, c);
    return true;
}

string Arena::moveCyborgs(int channel, int dir)
{
      // Cyborgs on the channel will respond with probability 1/2
    bool willRespond = (randInt(0, 1) == 0);

      // Move all cyborgs
    int nCyborgsOriginally = m_nCyborgs;

      //        Move each cyborg.  Force cyborgs listening on the channel to
      //        move in the indicated direction if willRespond is true.  If
      //        willRespond is false, or if the cyborg is listening on a
      //        different channel, it just moves.  Mark the player as dead
      //        if necessary.  Release any dead dynamically allocated cyborgs.
    
        for (int k = 0; k < m_nCyborgs; k++)
             {
                 if (m_cyborgs[k] != nullptr)
                 {
                 if (willRespond && m_cyborgs[k]->channel() == channel)
                     m_cyborgs[k]->forceMove(dir);
                 else
                     m_cyborgs[k]->move();
                 
             }
             }
    
    for (int k = 0; k < m_nCyborgs; k++)
         {
            if (m_cyborgs[k]->isDead())
            {
                delete m_cyborgs[k];
                m_cyborgs[k] = m_cyborgs[m_nCyborgs-1];
                m_nCyborgs--;
            }
         }
    
    if (numberOfCyborgsAt(m_player->row(), m_player->col()) > 0)
        m_player->setDead();
    
    

    if (m_nCyborgs < nCyborgsOriginally)
        return "Some cyborgs have been destroyed.";
    else
        return "No cyborgs were destroyed.";
}

bool Arena::isPosInBounds(int r, int c) const
{
    return (r >= 1  &&  r <= m_rows  &&  c >= 1  &&  c <= m_cols);
}

void Arena::checkPos(int r, int c, string functionName) const
{
    if (!isPosInBounds(r, c))
    {
        cout << "***** " << "Invalid arena position (" << r << ","
             << c << ") in call to " << functionName << endl;
        exit(1);
    }
}

///////////////////////////////////////////////////////////////////////////
//  Game implementation
///////////////////////////////////////////////////////////////////////////

Game::Game(int rows, int cols, int nCyborgs)
{
    if (nCyborgs < 0  ||  nCyborgs > MAXCYBORGS)
    {
        cout << "***** Game created with invalid number of cyborgs:  "
             << nCyborgs << endl;
        exit(1);
    }
    int nEmpty = rows * cols - nCyborgs - 1;  // 1 for Player
    if (nEmpty < 0)
    {
        cout << "***** Game created with a " << rows << " by "
             << cols << " arena, which is too small too hold a player and "
             << nCyborgs << " cyborgs!" << endl;
        exit(1);
    }

      // Create arena
    m_arena = new Arena(rows, cols);

      // Add some walls in WALL_DENSITY of the empty spots
    assert(WALL_DENSITY >= 0  &&  WALL_DENSITY <= 1);
    int nWalls = static_cast<int>(WALL_DENSITY * nEmpty);
    while (nWalls > 0)
    {
        int r = randInt(1, rows);
        int c = randInt(1, cols);
        if (m_arena->hasWallAt(r, c))
            continue;
        m_arena->placeWallAt(r, c);
        nWalls--;
    }

      // Add player
    int rPlayer;
    int cPlayer;
    do
    {
        rPlayer = randInt(1, rows);
        cPlayer = randInt(1, cols);
    } while (m_arena->hasWallAt(rPlayer, cPlayer));
    m_arena->addPlayer(rPlayer, cPlayer);

      // Populate with cyborgs
    while (nCyborgs > 0)
    {
        int r = randInt(1, rows);
        int c = randInt(1, cols);
        if (m_arena->hasWallAt(r, c)  ||  (r == rPlayer && c == cPlayer))
            continue;
        m_arena->addCyborg(r, c, randInt(1, MAXCHANNELS));
        nCyborgs--;
    }
}

Game::~Game()
{
    delete m_arena;
}

string Game::takePlayerTurn()
{
    for (;;)
    {
        cout << "Your move (n/e/s/w/x or nothing): ";
        string playerMove;
        getline(cin, playerMove);

        Player* player = m_arena->player();
        int dir;

        if (playerMove.size() == 0)
        {
            if (recommendMove(*m_arena, player->row(), player->col(), dir))
                return player->move(dir);
            else
                return player->stand();
        }
        else if (playerMove.size() == 1)
        {
            if (tolower(playerMove[0]) == 'x')
                return player->stand();
            else
            {
                dir = decodeDirection(tolower(playerMove[0]));
                if (dir != BADDIR)
                    return player->move(dir);
            }
        }
        cout << "Player move must be nothing, or 1 character n/e/s/w/x." << endl;
    }
}

string Game::takeCyborgsTurn()
{
    for (;;)
    {
        cout << "Broadcast (e.g., 2n): ";
        string broadcast;
        getline(cin, broadcast);
        if (broadcast.size() != 2)
        {
            cout << "You must specify a channel followed by a direction." << endl;
            continue;
        }
        else if (broadcast[0] < '1'  ||  broadcast[0] > '0'+MAXCHANNELS)
            cout << "Channel must be a digit in the range 1 through "
                 << MAXCHANNELS << "." << endl;
        else
        {
            int dir = decodeDirection(tolower(broadcast[1]));
            if (dir == BADDIR)
                cout << "Direction must be n, e, s, or w." << endl;
            else
                return m_arena->moveCyborgs(broadcast[0]-'0', dir);
        }
    }
}

void Game::play()
{
    m_arena->display("");
    Player* player = m_arena->player();
    if (player == nullptr)
        return;
    while ( ! player->isDead()  &&  m_arena->cyborgCount() > 0)
    {
        string msg = takePlayerTurn();
        m_arena->display(msg);
        if (player->isDead())
            break;
        msg = takeCyborgsTurn();
        m_arena->display(msg);
    }
    if (player->isDead())
        cout << "You lose." << endl;
    else
        cout << "You win." << endl;
}

///////////////////////////////////////////////////////////////////////////
//  Auxiliary function implementations
///////////////////////////////////////////////////////////////////////////

int decodeDirection(char dir)
{
    switch (dir)
    {
      case 'n':  return NORTH;
      case 'e':  return EAST;
      case 's':  return SOUTH;
      case 'w':  return WEST;
    }
    return BADDIR;  // bad argument passed in!
}

  // Return a random int from min to max, inclusive
int randInt(int min, int max)
{
    if (max < min)
        swap(max, min);
    static random_device rd;
    static default_random_engine generator(rd());
    uniform_int_distribution<> distro(min, max);
    return distro(generator);
}

  // Return false without changing anything if moving one step from (r,c)
  // in the indicated direction would hit a wall or run off the edge of the
  // arena.  Otherwise, update r and c to the position resulting from the
  // move and return true.
bool attemptMove(const Arena& a, int dir, int& r, int& c)
{
      
      // Delete the following line and replace it with the correct code.
      // This implementation compiles, but is incorrect.
    
    switch(dir)
    {
        case NORTH:
            if (r < 2)
                return false;
            else if (a.hasWallAt(r-1, c))
                return false;
            else
                r--;
            break;
        case SOUTH:
            if (r == a.rows())
                return false;
            else if (a.hasWallAt(r+1, c))
                return false;
            else
                r++;
            break;
        case EAST:
            if (c == a.cols())
                return false;
            else if (a.hasWallAt(r, c+1))
                return false;
            else
                c++;
            break;
        case WEST:
            if (c < 2)
                return false;
            else if (a.hasWallAt(r, c-1))
                return false;
            else
                c--;
            break;
    }
    return true;
    
    
}

  // Recommend a move for a player at (r,c):  A false return means the
  // recommendation is that the player should stand; otherwise, bestDir is
  // set to the recommended direction to move.
bool recommendMove(const Arena& a, int r, int c, int& bestDir)
{
      // Delete the following line and replace it with a better strategy than
      // to always recommend standing.
      // Recommend standing
    
    //the following bool variables check to see if any moves are valid moves
    bool move_n = false;
    bool move_s = false;
    bool move_e = false;
    bool move_w = false;
    int curRow = r;
    int curCol = c;
    int currentCybs;  //current number of cyborgs player is standing next to
    
    //number of cyborgs standing next to given any move- starts at a large number so we won't compare to moves that player is unable to make (moving west, for example might put the player next to less cyborgs, but there might be a cyborg one space west of the player)
    int nCybs = 9999999;
    int sCybs = 9999999;
    int eCybs = 9999999;
    int wCybs = 9999999;
    
    //the following variables establish the row/column numbers that a move to the n/s/e/w would put the player in
    int n_row = r-1;
    int s_row = r+1;
    int e_col = c+1;
    int w_col = c-1;
    
    if ((a.numberOfCyborgsAt(r-1, c) == 0) && attemptMove(a, NORTH, curRow, curCol))
        move_n = true;
    if ((a.numberOfCyborgsAt(r+1, c) == 0) && attemptMove(a, SOUTH, curRow, curCol))
        move_s = true;
    if ((a.numberOfCyborgsAt(r, c+1) == 0) && attemptMove(a, EAST, curRow, curCol))
        move_e = true;
    if ((a.numberOfCyborgsAt(r, c-1) == 0) && attemptMove(a, WEST, curRow, curCol))
        move_w = true;
    
    
    if (!(move_n || move_s || move_e || move_w)) //if none are valid moves, stay standing
        return false;
    
    // if all moves are valid, you are not standing next to any cyborgs, and standing still is the best move- moving in any direction could only put you in equal or more danger
    
    if (move_n && move_s && move_e && move_w)
        return false;
    
    //find current number of cyborgs standing next to and potential number of cyborgs standing next to for each move- only makes this calculation if move is valid
    
    currentCybs = a.numberOfCyborgsAt(r-1, c) + a.numberOfCyborgsAt(r+1, c) + a.numberOfCyborgsAt(r, c+1) + a.numberOfCyborgsAt(r, c-1);
    if (move_n)
        nCybs = a.numberOfCyborgsAt(n_row-1, c) + a.numberOfCyborgsAt(n_row+1, c) + a.numberOfCyborgsAt(n_row, c+1) + a.numberOfCyborgsAt(n_row, c-1);
    if (move_s)
        sCybs = a.numberOfCyborgsAt(s_row-1, c) + a.numberOfCyborgsAt(s_row+1, c) + a.numberOfCyborgsAt(s_row, c+1) + a.numberOfCyborgsAt(s_row, c-1);
    if (move_e)
        eCybs = a.numberOfCyborgsAt(r-1, e_col) + a.numberOfCyborgsAt(r+1, e_col) + a.numberOfCyborgsAt(r, e_col+1) + a.numberOfCyborgsAt(r, e_col-1);
    if (move_w)
        wCybs = a.numberOfCyborgsAt(r-1, w_col) + a.numberOfCyborgsAt(r+1, w_col) + a.numberOfCyborgsAt(r, w_col+1) + a.numberOfCyborgsAt(r, w_col-1);
    
    
    
    //if staying still leaves player standing next to least amount of cyborgs- in the following cases- standing still is values above everything else, followed by moving north, south, east, then west
    if (currentCybs <= nCybs && currentCybs <= sCybs && currentCybs <= eCybs && currentCybs <= wCybs)
        return false;
    //if moving north leaves player next to least amount of cyborgs and it is a valid move
    if (move_n && (nCybs < currentCybs && nCybs <= sCybs && nCybs <= eCybs && nCybs <= wCybs))
    {
            bestDir = NORTH;
            return true;
    }
    
    //if moving south leaves player next to least amount of cyborgs and it is a valid move
    if (move_s && (sCybs < currentCybs && sCybs <= nCybs && sCybs <= eCybs && nCybs <= wCybs))
    {
            bestDir = SOUTH;
            return true;
    }
    
    //if moving east leaves player next to least amount of cyborgs and it is a valid move
    if (move_e && (eCybs < currentCybs && eCybs <= nCybs && eCybs <= sCybs && eCybs <= wCybs))
    {
            bestDir = EAST;
            return true;
    }
    
    //if moving west leaves player next to least amount of cyborgs and it is a valid move
    if (move_w && (wCybs < currentCybs && wCybs <= nCybs && wCybs <= sCybs && wCybs <= eCybs))
    {
            bestDir = WEST;
            return true;
    }
    
    

      // Your replacement implementation should do something intelligent.
      // For example, if you're standing next to four cyborgs, and moving
      // north would put you next to two cyborgs, but moving east would put
      // you next to none, moving east is a safer choice than standing or
      // moving north.
    return false;
}



///////////////////////////////////////////////////////////////////////////
// main()
///////////////////////////////////////////////////////////////////////////

int main()
{
      // Create a game
      // Use this instead to create a mini-game:   Game g(3, 5, 2);
    Game g(10, 12, 50);

      // Play the game
    g.play();
}

///////////////////////////////////////////////////////////////////////////
//  clearScreen implementation
///////////////////////////////////////////////////////////////////////////

// DO NOT MODIFY OR REMOVE ANYTHING BETWEEN HERE AND THE END OF THE FILE!!!
// THE CODE IS SUITABLE FOR VISUAL C++, XCODE, AND g++/g31 UNDER LINUX.

// Note to Xcode users:  clearScreen() will just write a newline instead
// of clearing the window if you launch your program from within Xcode.
// That's acceptable.  (The Xcode output window doesn't have the capability
// of being cleared.)

#ifdef _WIN32

#pragma warning(disable : 4005)
#include <windows.h>

void clearScreen()
{
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    GetConsoleScreenBufferInfo(hConsole, &csbi);
    DWORD dwConSize = csbi.dwSize.X * csbi.dwSize.Y;
    COORD upperLeft = { 0, 0 };
    DWORD dwCharsWritten;
    FillConsoleOutputCharacter(hConsole, TCHAR(' '), dwConSize, upperLeft,
                                                        &dwCharsWritten);
    SetConsoleCursorPosition(hConsole, upperLeft);
}

#else  // not _WIN32

#include <iostream>
#include <cstring>
#include <cstdlib>

void clearScreen()  // will just write a newline in an Xcode output window
{
    static const char* term = getenv("TERM");
    if (term == nullptr  ||  strcmp(term, "dumb") == 0)
        cout << endl;
    else
    {
        static const char* ESC_SEQ = "\x1B[";  // ANSI Terminal esc seq:  ESC [
        cout << ESC_SEQ << "2J" << ESC_SEQ << "H" << flush;
    }
}

#endif
