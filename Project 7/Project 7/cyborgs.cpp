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
    Cyborg* m_cyborgs[MAXCYBORGS];  //Array of cyborg pointers
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
    //Completed: return what col the cyborg is at
    return m_col;
}

int Cyborg::channel() const
{
    //Completed: return what channel the cyborg is at
    return m_channel;
}

bool Cyborg::isDead() const
{
    //Completed: return if Cyborg is dead
    return m_health <= 0;
}

void Cyborg::forceMove(int dir)
{

    //Completed: Move cyborg in indicated direction, if possible, suffering one unit of damage if moving fails.
    
    //Update row / col in order to move cyborg in Arena
    //Northmost row is 1
    if(dir == 0 && m_row-1 > 0 && !m_arena->hasWallAt(m_row-1, m_col)){
        m_row --;
    }
    //Eastmost column is max column
    else if(dir == 1 && m_col+1 <= m_arena->cols() && !m_arena->hasWallAt(m_row, m_col+1)){
        m_col ++;
    }
    //Southmost row is max row
    else if(dir == 2 && m_row + 1 <= m_arena->rows() && !m_arena->hasWallAt(m_row+1, m_col)){
        m_row ++;
    }
    //Westmost column is 1
    else if(dir == 3 && m_col-1 > 0 && !m_arena->hasWallAt(m_row, m_col-1)){
        m_col --;
    }
    //If all the above options weren't valid, the Cyborg must've run into something where it should take damage.
    else{
        m_health--;
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
    //Completed: Return row player is at
    return m_row;
}

int Player::col() const
{
    //Completed: Return col player is at
    return m_col;
}

string Player::stand()
{
    return "Player stands.";
}

string Player::move(int dir)
{

    //Completed: Move player one step in indicated direction accounting for walls, barriers, and cyborgs, returning appropriate lines

    //Update row / col in order to move player in Arena
    //Northmost row is 1
    if(dir == 0 && m_row-1 > 0 && !m_arena->hasWallAt(m_row-1, m_col) && m_arena->numberOfCyborgsAt(m_row-1, m_col) == 0){
        m_row --;
        return "Player moved north.";
    }
    //Eastmost column is max column
    else if(dir == 1 && m_col+1 <= m_arena->cols() && !m_arena->hasWallAt(m_row, m_col+1) && m_arena->numberOfCyborgsAt(m_row, m_col+1) == 0){
        m_col ++;
        return "Player moved east.";
    }
    //Southmost row is max row
    else if(dir == 2 && m_row + 1 <= m_arena->rows() && !m_arena->hasWallAt(m_row+1, m_col) && m_arena->numberOfCyborgsAt(m_row+1, m_col) == 0){
        m_row ++;
        return "Player moved south.";
    }
    //Westmost column is 1
    else if(dir == 3 && m_col-1 > 0 && !m_arena->hasWallAt(m_row, m_col-1) && m_arena->numberOfCyborgsAt(m_row, m_col-1) == 0){
        m_col --;
        return "Player moved west.";
    }
    //Kill player if it runs into a cyborg
    else if((dir == 0 && m_arena->numberOfCyborgsAt(m_row-1, m_col) != 0) || (dir == 1 && m_arena->numberOfCyborgsAt(m_row, m_col+1) != 0) || (dir == 2 && m_arena->numberOfCyborgsAt(m_row+1, m_col) != 0) || (dir == 3 && m_arena->numberOfCyborgsAt(m_row, m_col-1) != 0)){
        setDead();
        return "Player walked into a cyborg and died.";
    }
    

    return "Player couldn't move; player stands.";
}

bool Player::isDead() const
{
    //Completed: Return if player is dead
    return m_dead;
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
    //Completed: Release player and all remaining dynamically allocated cyborgs
    delete m_player;
    m_player = nullptr;
    
    for(int i = 0; i < m_nCyborgs; i++){
        delete m_cyborgs[i];
        m_cyborgs[i] = nullptr;
    }
}

int Arena::rows() const
{
    //Completed: Return number of rows in arena
    return m_rows;
}

int Arena::cols() const
{
    //Completed: Return number of cols in arena
    return m_cols;
}

Player* Arena::player() const
{
    return m_player;
}

int Arena::cyborgCount() const
{
    //Completed: Return number of cyborgs in arena
    return m_nCyborgs;
}

bool Arena::hasWallAt(int r, int c) const
{
    checkPos(r, c, "Arena::hasWallAt");
    return m_wallGrid[r-1][c-1];
}

int Arena::numberOfCyborgsAt(int r, int c) const
{
    //Completed: Return number of cyborgs at row r, column c
    int numCyborgs = 0;
    //Iterate through all the cyborgs and check how many are present at the specified row and column
    for(int i = 0; i < m_nCyborgs; i++){
        if(m_cyborgs[i]->row() == r && m_cyborgs[i]->col() == c)
            numCyborgs++;
    }
    
    //Debugging code
//    cout<<endl;
//    cout<< numCyborgs << " cyborgs at [" << r << "," << c << "]" << endl;
//    cout << endl;
    
    return numCyborgs;
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


    //Completed: For each cyborg, set grid cell to digit char representing its channel number
    for(int i = 0; i < m_nCyborgs; i++){
        char channel = '?';
        //Properly assign the channel to the cyborg
        switch(m_cyborgs[i]->channel()){
            case 1:
                channel = '1';
                break;
            case 2:
                channel = '2';
                break;
            case 3:
                channel = '3';
                break;
            default:
                channel = '?';
        }
        
        displayGrid[m_cyborgs[i]->row()-1][m_cyborgs[i]->col()-1] = channel;
        
        //Debugging code
//        cout << "Channel end = " << channel << endl;
//        cout << "Number of Cyborgs at " << m_cyborgs[i]->row() << "," << m_cyborgs[i]->col() << ": " << numberOfCyborgsAt(m_cyborgs[i]->row(), m_cyborgs[i]->col());
//        cout << endl;
//        cout << endl;
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

    //Completed: Move each cyborg, forcing cyborgs on same channel to move in indicated dir if willRespond is true. Otherwise, cyborg moves randomly. Player marked dead if applicable, dynamically allocated cyborgs released.
    
    for(int i = 0; i < m_nCyborgs; i++){
        
        //Listens to player
        if(willRespond && m_cyborgs[i]->channel() == channel){
            m_cyborgs[i]->forceMove(dir);
        }
        //Doesn't listen to player / is on different channel
        else{
            m_cyborgs[i]->move();
        }
        
        //If cyborg is dead, decrement overall cyborgs
        if(m_cyborgs[i]->isDead()){
            delete m_cyborgs[i];
            m_cyborgs[i] = nullptr;
            
            //Move null object to end of array
            for(int j = i; j < m_nCyborgs-1; j++){
                m_cyborgs[j] = m_cyborgs[j+1];
            }
            m_nCyborgs--;
            i--;
        }
        //If cyborg is on player, mark player as dead
        else if(m_cyborgs[i]->row() == m_player->row() && m_cyborgs[i]->col() == m_player->col()){
            m_player->setDead();
        }
        
    }
    
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

    //Completed: Function properly implemented.
    
    //North
    if(dir == 0 && r - 1 > 0 && !a.hasWallAt(r-1, c)){
        r--;
        return true;
    }
    //East
    else if(dir == 1 && c + 1 <= a.cols() && !a.hasWallAt(r, c+1)){
        c++;
        return true;
    }
    //South
    else if(dir == 2 && r + 1 <= a.rows() && !a.hasWallAt(r+1, c)){
        r++;
        return true;
    }
    //West
    else if(dir == 3 && c-1 > 0 && !a.hasWallAt(r, c-1)){
        c--;
        return true;
    }

    //None of the above was true. Return false.
    return false;
}

  // Recommend a move for a player at (r,c):  A false return means the
  // recommendation is that the player should stand; otherwise, bestDir is
  // set to the recommended direction to move.
bool recommendMove(const Arena& a, int r, int c, int& bestDir)
{
    //Completed: recommendMove implementation
    
    //Determine how many cyborgs are around the player.
    int north = 0;
    int east = 0;
    int south = 0;
    int west = 0;
    int standing = 0;
    
    //Can the player move in that direction
    //North
    if(a.numberOfCyborgsAt(r-1, c) != 0 || r-1 <= 0)
        //Instant veto the move
        north = 10;
    else if(!a.hasWallAt(r-1, c)){
        //No cyborgs directly north + valid move
        r--;
        
        //Count how many robots would be surrounding the player
        //North
        if(a.numberOfCyborgsAt(r-1, c) != 0)
            north++;
        //East
        if(a.numberOfCyborgsAt(r, c+1) != 0)
            north++;
        //West
        if(a.numberOfCyborgsAt(r, c-1) != 0)
            north++;
        
        r++;
    }
    
    //East
    if(a.numberOfCyborgsAt(r, c+1) != 0 || c+1 > a.cols())
        //Instant veto the move
        east = 10;
    else if(!a.hasWallAt(r, c+1)){
        //Valid East
        c++;
        
        //Count how many robots would be surrounding the player
        //North
        if(a.numberOfCyborgsAt(r-1, c) != 0)
            east++;
        //East
        if(a.numberOfCyborgsAt(r, c+1) != 0)
            east++;
        //South
        if(a.numberOfCyborgsAt(r+1, c) != 0)
            east++;
        
        c--;
    }
    
    //South
    if(a.numberOfCyborgsAt(r+1, c) != 0 || r + 1 > a.rows())
        //Instant veto the move
        south = 10;
    else if(!a.hasWallAt(r+1, c)){
        //Valid South
        r++;
        
        //Count how many robots would be surrounding the player
        //South
        if(a.numberOfCyborgsAt(r+1, c) != 0)
            south++;
        //East
        if(a.numberOfCyborgsAt(r, c+1) != 0)
            south++;
        //West
        if(a.numberOfCyborgsAt(r, c-1) != 0)
            south++;
        
        r--;
    }
    
    //West
    if(a.numberOfCyborgsAt(r, c-1) != 0 || c-1 <= 0)
        //Instant veto the move
        west = 10;
    else if(!a.hasWallAt(r, c-1)){
        //Valid West
        c--;
        
        //Count how many robots would be surroudning the player
        //North
        if(a.numberOfCyborgsAt(r-1, c) != 0)
            west++;
        //West
        if(a.numberOfCyborgsAt(r, c-1) != 0)
            west++;
        //South
        if(a.numberOfCyborgsAt(r+1, c) != 0)
            west++;
        
        c++;
    }
    
    //Standing
    //North
    if(a.numberOfCyborgsAt(r-1, c) != 0)
        standing++;
    //West
    if(a.numberOfCyborgsAt(r, c-1) != 0)
        standing++;
    //South
    if(a.numberOfCyborgsAt(r+1, c) != 0)
        standing++;
    //East
    if(a.numberOfCyborgsAt(r, c+1) != 0)
        standing++;
    
  
    //Debugging code
//    cerr << "north " << north << endl;
//    cerr << "south " << south << endl;
//    cerr << "east " << east << endl;
//    cerr << "west " << west << endl;
//    cerr << "standing " << standing << endl;
    
    
    if(north <= east && north <= south && north <= west && north <= standing){
        bestDir = 0;
        return true;
    }
    else if(east <= north && east <= south && east <= west && east <= standing){
        bestDir = 1;
        return true;
    }
    else if(south <= north && south <= east && south <= west && south <= standing){
        bestDir = 2;
        return true;
    }
    else if(west <= north && west <= east && west <= south && west <= standing){
        bestDir = 3;
        return true;
    }
    else
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
//    Game g(3,5,2);

//        Game g(10, 12, 10);
    
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
