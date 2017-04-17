#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_image.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <stdbool.h>
#include <math.h>

#define STRING_SIZE 100       // max size for some strings
#define TABLE_SIZE 650        // main game space size
#define LEFT_BAR_SIZE 150     // left white bar size
#define WINDOW_POSX 200       // initial position of the window: x
#define WINDOW_POSY 200       // initial position of the window: y
#define SQUARE_SEPARATOR 8    // square separator in px
#define BOARD_SIZE_PER 0.7f   // board size in % wrt to table size 
#define MAX_BOARD_POS 11      // maximum size of the board
#define MAX_LEVELS 23
#define MARGIN 5

// declaration of the functions related to graphical issues
void InitEverything(int , int , TTF_Font **, TTF_Font **, SDL_Surface **, SDL_Window ** , SDL_Renderer ** );
void InitSDL();
void InitFont();
SDL_Window* CreateWindow(int , int );
SDL_Renderer* CreateRenderer(int , int , SDL_Window *);
int RenderText(int, int, const char *, TTF_Font *, SDL_Color *, SDL_Renderer *);
int RenderLogo(int, int, SDL_Surface *, SDL_Renderer *);
void RenderTable(int, int *, int *, TTF_Font *, SDL_Surface **, SDL_Renderer *);
void RenderBoard(int [][MAX_BOARD_POS], SDL_Surface **, int, int, int, SDL_Renderer *);
void RenderStats( SDL_Renderer *, TTF_Font *, int , int , int );
void RenderPress(SDL_Renderer *, TTF_Font *);
void LoadValues(SDL_Surface **);
void UnLoadValues(SDL_Surface **);


/* All the parameters of each function are explained respectively,
* according to the order of their declaration
*/

void Parameters(int *, char *, int*);
void SaveResults(char name[STRING_SIZE] , int vec_stats[121], int);
void GenStart(int board[][MAX_BOARD_POS], int, int *);
void GenPiece(int board[][MAX_BOARD_POS], int, int *);
int MovPieceUp(int board[][MAX_BOARD_POS], int, int *, int *, int *, int *, int last_board[][MAX_BOARD_POS]);
int MovPieceDown(int board[][MAX_BOARD_POS], int, int *, int *, int *, int *, int last_board[][MAX_BOARD_POS]);
int MovPieceLeft(int board[][MAX_BOARD_POS], int, int *, int *, int *, int *, int last_board[][MAX_BOARD_POS]);
int MovPieceRight(int board[][MAX_BOARD_POS], int, int *, int *, int *, int *, int last_board[][MAX_BOARD_POS]);
void CopyPlay(int board[][MAX_BOARD_POS], int, int last_board[][MAX_BOARD_POS]);
int Undo(int board[][MAX_BOARD_POS], int, int *, int, int *, int, int last_board[][MAX_BOARD_POS]);
int CheckWin(int, int, int, int board[][MAX_BOARD_POS]);
void NewGame(int board[][MAX_BOARD_POS], int);
void RenderInfoRect(SDL_Renderer *_renderer, TTF_Font *,int);
int Load(int board[][MAX_BOARD_POS], int *, char name[STRING_SIZE], int *, int *, int *);
void Save(int board[][MAX_BOARD_POS], int, char name[STRING_SIZE], int, int, int, int);


// definition of some strings: they cannot be changed when the program is executed !
const char myName[] = "Francisco Branco";
const char myNumber[] = "IST425347";

/**
 * main function: entry point of the program
 * only to invoke other functions !
 */
int main( int argc, char* args[] )
{
    SDL_Window *window = NULL;
    SDL_Renderer *renderer = NULL;
    TTF_Font *serif = NULL, *sans = NULL;
    SDL_Surface *array_of_numbers[MAX_LEVELS], *imgs[2];
    SDL_Event event;
    int delay = 300;
    int quit = 0;
    int width = (TABLE_SIZE + LEFT_BAR_SIZE);
    int height = TABLE_SIZE;
    int square_size_px, board_size_px;
    int board[MAX_BOARD_POS][MAX_BOARD_POS] = {{0}};

    int last_board[MAX_BOARD_POS][MAX_BOARD_POS] = {{0}};

    int vec_stats[121];
    int board_size = 0;
    char name[STRING_SIZE], answer;
    int difficulty = 0;

    // i is the position saved in the vec_stats and game is the flag which says if the game is active or not
    int i = 0, game = 0;

    /* last_points is the points before the latest play, last_pc is the previous highest piece, max_pc is the current highest piece*
    *begin is the machine's time when the game starts and baggage is the game time of the saved game*/
    int points = 0, last_points = 0, max_pc = 0, last_pc = 0, game_time = 0, begin = 0, baggage = 0;

    /* test is a flag to test if a piece is generated or not, win tests the game if the player already won or lost, has_undo tests the undo option*
    * first is a flag that tests if n has been pressed for the first time*/
    int test = 0, win = 0, has_undo = 1, first = 0;






    // Initializes the function rand()
    srand(time(NULL));

    printf("Resume game?(y/n) ");
    scanf("%c", &answer);

    // Loads the game if there is a game to be resumed (same parameters as the previous)
    baggage = Load(board, &board_size, name, &difficulty, &points, &max_pc);

    // This is only to render the game time from the load
    game_time = baggage;

    // Asks for parameters (if answer = y and there is no game to resume, asks for parameters anyway)
    if(answer == 'n' || max_pc == 0) Parameters(&board_size, name, &difficulty);

    // initialize graphics
    InitEverything(width, height, &serif, &sans, imgs, &window, &renderer);
    // loads numbers as images
    LoadValues(array_of_numbers);

    begin = time(NULL);

    while( quit == 0 )
    {
        // If the player won or lost, the game stops
        if(win != 0) 
        {
            game = 0;
            baggage = 0;
        }
        // The game only counts the time if the game is active
        if(game == 1) game_time = time(NULL) - begin + baggage;


        // while there's events to handle
        while( SDL_PollEvent( &event ) )
        {
            if( event.type == SDL_QUIT )
            {
                quit = 1;   // quit !
            }
            else if ( event.type == SDL_KEYDOWN )
            {
                switch ( event.key.keysym.sym )
                {
                    case SDLK_q:
                        quit = 1;
                        break;
                
                    case SDLK_n:
                        begin = time(NULL);

                            // Saves the stat's vector for the writing of all the stats in a file
                            if(baggage == 0)
                            {
                                if(first != 0)
                                {
                                    vec_stats[i] = points;
                                    vec_stats[i + 1] = max_pc;
                                    vec_stats[i + 2] = game_time;
                                    i += 3;
                                }

                                NewGame(board, board_size);
                                GenStart(board, board_size, &max_pc);

                                // Restarts variables
                                points = 0;
                                max_pc = 0;
                                win = 0;
                            }


                        // Variable which decides when the game starts
                        game = 1;
                        // Says that the button n has been pushed, so that the vector is saved with new games
                        first = 1;
                        break;

                    case SDLK_u:
                        // Verifies if the game has already undo once, and if not, undos
                        if(game == 1)
                        {
                            if(has_undo == 0) has_undo = Undo(board, board_size, &points, last_points, &max_pc, last_pc, last_board);
                        }
                        break;

                    case SDLK_UP:
                        if(game == 1)
                        {
                            // Moves piece if possible
                            test = MovPieceUp(board, board_size, &points, &last_points, &max_pc, &last_pc, last_board);

                            // Generates another piece, puts undo ready and checks if the player won
                            if(test != 0) 
                            {
                                GenPiece(board, board_size, &max_pc);
                                has_undo = 0;
                                win = CheckWin(max_pc, difficulty, board_size, board);
                            }
                        }

                        break;

                    case SDLK_DOWN:
                        if(game == 1)
                        {
                            test = MovPieceDown(board, board_size, &points, &last_points, &max_pc, &last_pc, last_board);

                            if(test != 0) 
                            {
                                GenPiece(board, board_size, &max_pc);
                                has_undo = 0;
                                win = CheckWin(max_pc, difficulty, board_size, board);
                            }
                        }

                        break;
                    
                    case SDLK_LEFT:
                        if(game == 1)
                        {
                            test = MovPieceLeft(board, board_size, &points, &last_points, &max_pc, &last_pc, last_board);

                            if(test != 0) 
                            {
                                GenPiece(board, board_size, &max_pc);
                                has_undo = 0;
                                win =CheckWin(max_pc, difficulty, board_size, board);
                            }
                        }

                        break;
                    
                    case SDLK_RIGHT:
                        if(game == 1)
                        {
                            test = MovPieceRight(board, board_size, &points, &last_points, &max_pc, &last_pc, last_board);

                            if(test != 0)
                            {   
                                GenPiece(board, board_size, &max_pc);
                                has_undo = 0;
                                win = CheckWin(max_pc, difficulty, board_size, board);
                            }
                        }

                        break;
                    
                    default:
                        break;
                    
                }
            }
        }

        // render game table
        RenderTable(board_size, &board_size_px, &square_size_px, serif, imgs, renderer);
        // render board 
        RenderBoard(board, array_of_numbers, board_size, board_size_px, square_size_px, renderer);
        // renders stats
        RenderStats(renderer, sans, difficulty, points, game_time);
        // renders win/lose rectangle
        if(win != 0) RenderInfoRect(renderer, serif, win);
        // Renders Press n to start
        if(game == 0) RenderPress(renderer, serif);
        // render in the screen all changes above
        SDL_RenderPresent(renderer);
        // add a delay
        SDL_Delay( delay );
    }

    // Saves the game if the game didn't end
    if(win == 0) Save(board, board_size, name, difficulty, points, max_pc, game_time);
    else remove("save.txt");

    // free memory allocated for images and textures and closes everything including fonts
    UnLoadValues(array_of_numbers);
    TTF_CloseFont(serif);
    SDL_FreeSurface(imgs[0]);
    SDL_FreeSurface(imgs[1]);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    game_time = time(NULL) - begin + baggage;

    vec_stats[i] = points;
    vec_stats[i + 1] = max_pc;
    vec_stats[i + 2] = game_time;
    i += 3;

    // Saves the game's stats
    SaveResults(name, vec_stats, i);

    return EXIT_SUCCESS;
}








/* Parameters: Asks the user for the required parameters for the game to work 
*  Board size, player's name and difficulty
*/
void Parameters(int *_board_size, char name[STRING_SIZE], int *_difficulty)
{
    int var = 0;

    int board_size = *_board_size, difficulty = *_difficulty;   // Auxilliary variables to help the management of parameters

    // Asks for the board_size
    while(var == 0)
    {
        printf("Choose size for board: ");
        scanf("%d", &board_size);

        if(board_size < 2 || board_size > 11) printf("The board size must be between 2 and 11.\n");
        else var = 1;
    }

    var = 0;

    // Asks for the name of the player
    while(var == 0)
    {
        printf("\nChoose your name: ");
        scanf("%s", name);

        if(strlen(name) > 8 || strlen(name) < 1) printf("The name must have at least 8 characters.\n");
        else var = 1;
    }

    var = 0;

    // Asks for the difficulty of the game
    while(var == 0)
    {
        printf("\nChoose difficulty (from 4 to 23): ");
        scanf("%d", &difficulty);

        if(difficulty < 4 || difficulty > 23) printf("The difficulty must be between 4 and 23.\n");
        else var = 1;
    }

    *_board_size = board_size;
    *_difficulty = difficulty;
}

/* GenStart: generates the start of the game
*  The board's matrix and it's size
*/
void GenStart(int board[][MAX_BOARD_POS], int _board_size, int *max_pc)
{
    int pos1, pos2, pos3, pos4, val;

    // Initializes the random position and value (2 or 4) for the first pieces
    pos1 = rand() % _board_size;
    pos2 = rand() % _board_size;

    val = (rand() % 2) + 1;

    board[pos1][pos2] = val;

    if(*max_pc < val) *max_pc = val;

    // We need to be sure that the two first pieces don't overlap
    while(1)
    {
        pos3 = rand() % _board_size;
        pos4 = rand() % _board_size;

        if(pos3 != pos1 || pos4 != pos2) break;
    }

    val = (rand() % 2) + 1;

    board[pos3][pos4] = val;

    if(*max_pc < val) *max_pc = val;
}

/* GenPiece: Generates a new piece on the board
*  The board's matrix and it's size
*/
void GenPiece(int board[][MAX_BOARD_POS], int _board_size, int *max_pc)
{
    int pos1 = 0, pos2 = 0, val, aux = 1;

    // Initializes the random position and value (2 or 4) for the first pieces
    while(board[pos1][pos2] != 0 || aux == 1)
    {
        pos1 = rand() % _board_size;
        pos2 = rand() % _board_size;

        aux = 0;
    }

    val = (rand() % 2) + 1;

    board[pos1][pos2] = val;

    if(*max_pc < val) *max_pc = val;
}




/* Functions to calculate the piece's movement */


/* MovPieceUp: Caculates the movements of the pieces on the board, including adding pieces
*  The board's matrix and it's size
*/
int MovPieceUp(int board[][MAX_BOARD_POS], int _board_size, int *points, int *last_points, int *max_pc, int *last_pc, int last_board[][MAX_BOARD_POS])
{
    int i, j, l, aux, test = 0;

    CopyPlay(board, _board_size, last_board);

    *last_pc = *max_pc;

    for(i = 0; i < _board_size; i++)
    {
        for(j = 0; j < _board_size; j++)
        {
            if(board[i][j] != 0)
            {
                aux = board[i][j];

                // other variables to increment: l
                for(l = j - 1; l >= 0 && board[i][l] == 0; l--){}

                // tests if a piece moved or not
                if(j != l + 1)
                {
                    test = 1;
                }

                // substitution of the values so the piece appears to move
                board[i][j] = 0;
                board[i][l + 1] = aux;

                // Verifies if there are pieces to be added
                if(l > -1 && board[i][l + 1] == board[i][l] && board[i][l] != 0)
                {
                    board[i][l + 1] = 0;
                    board[i][l] += 1;

                    *last_points = *points;

                    *points += pow(2, board[i][l]);

                    if(*max_pc < board[i][l]) *max_pc = board[i][l];

                    // Make sure the piece is generated if the pieces involved in the fusion are right next ot each other
                    test = 1;
                }
            }
        }
    }

    return test;
}



/* MovPieceDown: Caculates the movements of the pieces on the board
*  The board's matrix and it's size
*/
int MovPieceDown(int board[][MAX_BOARD_POS], int _board_size, int *points, int *last_points, int *max_pc, int *last_pc, int last_board[][MAX_BOARD_POS])
{
    int i, j, l, aux, test = 0;

    CopyPlay(board, _board_size, last_board);

    *last_pc = *max_pc;

    for(i = 0; i < _board_size; i++)
    {
        for(j = _board_size - 1; j >= 0; j--)
        {
            if(board[i][j] != 0)
            {
                aux = board[i][j];

                // other variables to increment: l
                for(l = j + 1; l < _board_size && board[i][l] == 0; l++){}

                // tests if a piece moved or not
                if(j != l - 1)
                {
                    test = 1;
                }

                // substitution of the values so the piece appears to move
                board[i][j] = 0;
                board[i][l - 1] = aux;

                // Verifies if there are pieces to be added
                if(l < _board_size && board[i][l - 1] == board[i][l] && board[i][l] != 0) 
                {
                    board[i][l - 1] = 0;
                    board[i][l] += 1;

                    *last_points = *points;

                    *points += pow(2, board[i][l]);

                    if(*max_pc < board[i][l]) *max_pc = board[i][l];

                    // Make sure the piece is generated if the pieces involved in the fusion are right next ot each other
                    test = 1;
                }
            }
        }
    }

    return test;
}

/* MovPieceLeft: Caculates the movements of the pieces on the board
*  The board's matrix and it's size
*/
int MovPieceLeft(int board[][MAX_BOARD_POS], int _board_size, int *points, int *last_points, int *max_pc, int *last_pc, int last_board[][MAX_BOARD_POS])
{
    int i, j, l, aux, test = 0;
                    
    CopyPlay(board, _board_size, last_board);

    *last_pc = *max_pc;

    for(i = 0; i < _board_size; i++)
    {
        for(j = 0; j < _board_size; j++)
        {
            if(board[i][j] != 0)
            {
                aux = board[i][j];

                // other variables to increment: l
                for(l = i - 1; l >= 0 && board[l][j] == 0; l--){}

                // tests if a piece moved or not
                if(i != l + 1)
                {
                    test = 1;
                }

                // substitution of the values so the piece appears to move
                board[i][j] = 0;
                board[l + 1][j] = aux;

                // Verifies if there are pieces to be added
                if(l > -1 && board[l + 1][j] == board[l][j] && board[l][j] != 0) 
                {
                    board[l + 1][j] = 0;
                    board[l][j] += 1;

                    *last_points = *points;

                    *points += pow(2, board[l][j]);

                    if(*max_pc < board[l][j]) *max_pc = board[l][j];

                    // Make sure the piece is generated if the pieces involved in the fusion are right next ot each other
                    test = 1;
                }
            }
        }
    }

    return test;
}

/* MovPieceRight: Caculates the movements of the pieces on the board
*  The board's matrix and it's size
*/
int MovPieceRight(int board[][MAX_BOARD_POS], int _board_size, int *points, int *last_points, int *max_pc, int *last_pc, int last_board[][MAX_BOARD_POS])
{
    int i, j, l, aux, test = 0;

    CopyPlay(board, _board_size, last_board);

    *last_pc = *max_pc;

    for(i = _board_size; i >= 0; i--)
    {
        for(j = 0; j < _board_size; j++)
        {
            if(board[i][j] != 0)
            {
                aux = board[i][j];

                // other variables to increment: l
                for(l = i + 1; l < _board_size && board[l][j] == 0; l++) {}

                // tests if a piece moved or not
                if(i != l - 1)
                {
                    test = 1;
                }

                // substitution of the values so the piece appears to move
                board[i][j] = 0;
                board[l - 1][j] = aux;

                // Verifies if there are pieces to be added
                if(l < _board_size && board[l - 1][j] == board[l][j] && board[l][j] != 0) 
                {
                    board[l - 1][j] = 0;
                    board[l][j] += 1;

                    *last_points = *points;

                    *points += pow(2, board[l][j]);

                    if(*max_pc < board[l][j]) *max_pc = board[l][j];

                    // Make sure the piece is generated if the pieces involved in the fusion are right next ot each other
                    test = 1;
                }
            }
        }
    }

    return test;
}








/* CopyPlay: Copies the last play, to make the undo functionality work
*  The board's matrix, the board's size and the last board before the current one
*/
void CopyPlay(int board[][MAX_BOARD_POS], int _board_size, int last_board[][MAX_BOARD_POS])
{
    int i, j;

    for(i = 0; i < _board_size; i++)
    {
        for(j = 0; j < _board_size; j++)
        {
            last_board[i][j] = board[i][j];
        }
    }
}

/* Undo: Puts the board's pieces as the previous board's pieces
*  The board's matrix, the board's size, game's points, previous points and the last board before the current one
*/
int Undo(int board[][MAX_BOARD_POS], int _board_size, int *points, int last_points, int *max_pc, int last_pc, int last_board[][MAX_BOARD_POS])
{
    int i, j;

    for(i = 0; i < _board_size; i++)
    {
        for(j = 0; j < _board_size; j++)
        {
            board[i][j] = last_board[i][j];
        }
    }

    *points = last_points;
    *max_pc = last_pc;

    return 1;
}

/* CheckWin: Verifies if player has won or lost
*  Highest piece achieved, difficulty, the board's size and matrix
*/
int CheckWin(int _max_pc, int _difficulty, int _board_size, int board[][MAX_BOARD_POS])
{
    int i, j;

    if(_max_pc == _difficulty) return 1;

    for(i = 0; i < _board_size; i++)
    {
        for(j = 0; j < _board_size; j++)
        {
            if(board[i][j] != 0)
            {
                if(i - 1 >= 0 && board[i][j] == board[i - 1][j]) return 0;

                if(i + 1 < _board_size && board[i][j] == board[i + 1][j]) return 0;

                if(j - 1 >= 0 && board[i][j] == board[i][j - 1]) return 0;

                if(j + 1 < _board_size && board[i][j] == board[i][j + 1]) return 0;
            }
            else return 0;
        }
    }

    return -1;
}

/* NewGame: creates a new game. Points and time reset 
*  The board's matrix and size
*/
void NewGame(int board[][MAX_BOARD_POS], int _board_size)
{
    int i, j;

    for(i = 0; i < _board_size; i++)
    {
        for(j = 0; j < _board_size; j++)
        {
            board[i][j] = 0;
        }
    }
}

/* SaveResults: Writes or creates if necessary, a file with all the player's stats
*  Player's name, points, a vector with their points, the highest piece they achieved and the total game time; also, the last position occupied in the vector
*/
void SaveResults(char name[STRING_SIZE], int vec_stats[121], int i)
{
    int j, aux;

    FILE *results = NULL;

    results = fopen("stats.txt", "w");

    if(results == NULL)
    {
        printf("The file failed to open\n");
        exit(EXIT_FAILURE);
    }

    fprintf(results, "%s\n", name);

    for(j = 0; j < i; j += 3)
    {
        aux = pow(2, vec_stats[j + 1]);
        fprintf(results, "Points - %d\nHighest piece - %d\nTime - %d\n\n", vec_stats[j], aux, vec_stats[j + 2]);
    }

    fclose(results);
}



/* Special functions implemented */


/* Load: Loads the last saved game
*  The board's martix, the board's size, the player's name, tha game's difficulty, points and highest piece achieved
*/
int Load(int board[][MAX_BOARD_POS], int *board_size, char name[STRING_SIZE], int *difficulty, int *points, int *max_pc)
{
    int i, j, _board_size, _difficulty, _points, _max_pc, baggage = 0;

    FILE *file_save = NULL;

    file_save = fopen("save.txt", "r");

    if(file_save == NULL)
    {
        printf("There is no game to resume.\n");
        return baggage;
    }

    fscanf(file_save,"%s", name);

    fscanf(file_save,"%d %d", &_board_size, &_difficulty);

    for(j = 0; j < _board_size; j++)
    {
        for(i = 0; i < _board_size; i++)
        {
            fscanf(file_save,"%d", &board[i][j]);
        }
    }

    fscanf(file_save,"%d %d %d", &_points, &_max_pc, &baggage);

    *board_size = _board_size;
    *difficulty = _difficulty;
    *points = _points;
    *max_pc = _max_pc;

    fclose(file_save);

    return baggage;
}

/* Save: saves the actual state of the game
*  
*/
void Save(int board[][MAX_BOARD_POS], int _board_size, char name[STRING_SIZE], int difficulty, int points, int max_pc, int game_time)
{
    int i, j;

    FILE *file_save = NULL;

    file_save = fopen("save.txt", "w");

    if(file_save == NULL)
    {
        printf("The file didn't open correctly\n");
        exit(EXIT_FAILURE);
    }

    fprintf(file_save,"%s\n", name);

    fprintf(file_save,"%d %d\n", _board_size, difficulty);

    for(j = 0; j < _board_size; j++)
    {
        for(i = 0; i < _board_size; i++)
        {
            fprintf(file_save,"%d ", board[i][j]);
        }
        fprintf(file_save,"\n");
    }

    fprintf(file_save, "%d %d %d\n", points, max_pc, game_time);

    fclose(file_save);
}






/* Graphic's Developed */




/*
 * Shows some information about the game:
 * - Level to achieve (difficulty)
 * - Score of the game
 * - Time spent in seconds
 * \param _renderer renderer to handle all rendering in a window
 * \param _level current level
 * \param _score actual score
 * \param _time time to display in seconds
 */
void RenderStats( SDL_Renderer *_renderer, TTF_Font *_font, int _level, int _score, int _time )
{
    char info1[STRING_SIZE], info2[STRING_SIZE], info3[STRING_SIZE];

    SDL_Color orange = {205, 105, 0};
    SDL_Rect stats_rect1;
    SDL_Rect stats_rect2;
    SDL_Rect stats_rect3;

    SDL_SetRenderDrawColor(_renderer, 225, 225, 225, 255 );
    stats_rect1.x = 98;
    stats_rect1.y = 135;
    stats_rect1.w = 125;
    stats_rect1.h = 35;
    SDL_RenderFillRect(_renderer, &stats_rect1);


    stats_rect2.x = 262;
    stats_rect2.y = 135;
    stats_rect2.w = 125;
    stats_rect2.h = 35;
    SDL_RenderFillRect(_renderer, &stats_rect2);


    stats_rect3.x = 425;
    stats_rect3.y = 135;
    stats_rect3.w = 125;
    stats_rect3.h = 35;
    SDL_RenderFillRect(_renderer, &stats_rect3);

    _level = pow(2, _level);

    sprintf(info1,"Level: %d", _level);

    sprintf(info2,"Score: %d", _score);

    sprintf(info3,"Time: %d", _time);

    RenderText(105, 140, info1, _font, &orange, _renderer);

    RenderText(267, 140, info2, _font, &orange, _renderer);

    RenderText(430, 140, info3, _font, &orange, _renderer);
}

/*  RenderInfoRect: Renders the rectangles with the information if the player won or lost
*   The renderer, font and the win variable
*/
void RenderInfoRect(SDL_Renderer *_renderer, TTF_Font *_font, int _win)
{
    SDL_Rect info_rect;

    SDL_Color black = {0, 0, 0};

    if(_win == 1) SDL_SetRenderDrawColor(_renderer, 0, 255, 0, 200 );
    else SDL_SetRenderDrawColor(_renderer, 255, 0, 0, 200 );
    info_rect.x = 175;
    info_rect.y = 250;
    info_rect.w = 300;
    info_rect.h = 300;
    SDL_RenderFillRect(_renderer, &info_rect);

    if(_win == 1)
    {
        char status[] = "YOU WON!";

        RenderText(280, 380, status, _font, &black, _renderer);
    }

    else
    {
        char status[] = "YOU LOST!";

        RenderText(280, 380, status, _font, &black, _renderer);
    }
}

/* RenderPress: Renders the instructions to press n to start
*  Renderer and font
*/
void RenderPress(SDL_Renderer *_renderer, TTF_Font *_font)
{
    SDL_Color black = {0, 0, 0};

    char instructions[STRING_SIZE] = "Press n to start";

    RenderText(670, 300, instructions, _font, &black, _renderer);
}











/**
 * LoadCards: Loads all images with the values
 * \param _array_of_numbers vector with all loaded numbers
 */
void LoadValues(SDL_Surface **_array_of_numbers)
{
    int i;
    char filename[STRING_SIZE];

    // loads all images to an array
    for ( i = 0 ; i < MAX_LEVELS; i++ )
    {
        // create the filename !
        sprintf(filename, ".//numbers//value_%02d.jpg", i+1);
        // loads the image !
        _array_of_numbers[i] = IMG_Load(filename);
        // check for errors: deleted files ?
        if (_array_of_numbers[i] == NULL)
        {
            printf("Unable to load image: %s\n", SDL_GetError());
            exit(EXIT_FAILURE);
        }
    }
}


/**
 * UnLoadCards: unloads all values images of the memory
 * \param _array_of_numbers vector with all loaded image numbers
 */
void UnLoadValues(SDL_Surface **_array_of_numbers)
{
    // unload all cards of the memory: +1 for the card back
    for ( int i = 0 ; i < MAX_LEVELS; i++ )
    {
        SDL_FreeSurface(_array_of_numbers[i]);
    }
}

/**
 * RenderBoard: renders the board
 * \param _board multidimensional array with the board numbers
 * \param _array_of_numbers vector with all loaded images
 * \param _board_pos number of positions in the board
 * \param _board_size_px size of the board in pixels
 * \param _square_size_px size of each square
 * \param _renderer renderer to handle all rendering in a window
 */
void RenderBoard(int _board[][MAX_BOARD_POS], SDL_Surface **_array_of_numbers, int _board_pos,
        int _board_size_px, int _square_size_px, SDL_Renderer *_renderer )
{
    int x_corner, y_corner;
    SDL_Rect boardPos;
    SDL_Texture *number_text;

    // corner of the board
    x_corner = (TABLE_SIZE - _board_size_px) >> 1;
    y_corner = (TABLE_SIZE - _board_size_px - 15);

    // renders the squares where the numbers will appear
    for ( int i = 0; i < _board_pos; i++ )
    {
        for ( int j = 0; j < _board_pos; j++ )
        {
            // basic check
            if ( _board[i][j] > 23 || _board[i][j] < 0 ) 
            {
                printf("Invalid board value: RenderBoard\n");
                exit(EXIT_FAILURE);
            }
            // only draws a number if it corresponds to a non-empty space
            if ( _board[i][j] != 0 )
            {
                // define the size and copy the image to display
                boardPos.x = x_corner + (i+1)*SQUARE_SEPARATOR + i*_square_size_px;  
                boardPos.y = y_corner + (j+1)*SQUARE_SEPARATOR + j*_square_size_px;
                boardPos.w = _square_size_px;
                boardPos.h = _square_size_px;
                number_text = SDL_CreateTextureFromSurface( _renderer, _array_of_numbers[ _board[i][j] - 1] );
                SDL_RenderCopy( _renderer, number_text, NULL, &boardPos );
                // destroy texture 
                SDL_DestroyTexture(number_text);
            }
        }
    }
}


/*
 * RenderTable: Draws the table where the game will be played, namely:
 * -  some texture for the background
 * -  the right part with the IST logo and the student name and number
 * -  the grid for game board with squares and seperator lines
 * \param _board_pos number of squares in the board 
 * \param _font font used to render the text
 * \param _img surfaces with the table background and IST logo (already loaded)
 * \param _renderer renderer to handle all rendering in a window
 */
void RenderTable( int _board_pos, int *_board_size_px, int *_square_size_px, TTF_Font *_font, SDL_Surface *_img[], SDL_Renderer* _renderer )
{
    SDL_Color black = { 0, 0, 0 }; // black
    SDL_Color light = { 205, 193, 181 };
    SDL_Color dark = { 120, 110, 102 };
    SDL_Texture *table_texture;
    SDL_Rect tableSrc, tableDest, board, board_square;
    int height, board_size_px, square_size_px;

    // set color of renderer to some color
    SDL_SetRenderDrawColor( _renderer, 255, 255, 255, 255 );

    // clear the window
    SDL_RenderClear( _renderer );

    tableDest.x = tableSrc.x = 0;
    tableDest.y = tableSrc.y = 0;
    tableSrc.w = _img[0]->w;
    tableSrc.h = _img[0]->h;
    tableDest.w = TABLE_SIZE;
    tableDest.h = TABLE_SIZE;

    // draws the table texture
    table_texture = SDL_CreateTextureFromSurface(_renderer, _img[0]);
    SDL_RenderCopy(_renderer, table_texture, &tableSrc, &tableDest);

    // render the IST Logo
    height = RenderLogo(TABLE_SIZE, 0, _img[1], _renderer);

    // render the student name
    height += RenderText(TABLE_SIZE+3*MARGIN, height, myName, _font, &black, _renderer);

    // this renders the student number
    RenderText(TABLE_SIZE+3*MARGIN, height, myNumber, _font, &black, _renderer);

    // compute and adjust the size of the table and squares
    board_size_px = (int)(BOARD_SIZE_PER*TABLE_SIZE);
    square_size_px = (board_size_px - (_board_pos+1)*SQUARE_SEPARATOR) / _board_pos; 
    board_size_px -= board_size_px % (_board_pos*(square_size_px+SQUARE_SEPARATOR));
    board_size_px += SQUARE_SEPARATOR;

    // renders the entire board background 
    SDL_SetRenderDrawColor(_renderer, dark.r, dark.g, dark.b, dark.a );
    board.x = (TABLE_SIZE - board_size_px) >> 1;
    board.y = (TABLE_SIZE - board_size_px - 15);
    board.w = board_size_px;
    board.h = board_size_px;
    SDL_RenderFillRect(_renderer, &board);

    // renders the squares where the numbers will appear
    SDL_SetRenderDrawColor(_renderer, light.r, light.g, light.b, light.a );

    // iterate over all squares
    for ( int i = 0; i < _board_pos; i++ )
    {
        for ( int j = 0; j < _board_pos; j++ )
        {
            board_square.x = board.x + (i+1)*SQUARE_SEPARATOR + i*square_size_px;  
            board_square.y = board.y + (j+1)*SQUARE_SEPARATOR + j*square_size_px;
            board_square.w = square_size_px;
            board_square.h = square_size_px;
            SDL_RenderFillRect(_renderer, &board_square);
        }
    }

    // destroy everything
    SDL_DestroyTexture(table_texture);
    // store some variables for later use
    *_board_size_px = board_size_px;
    *_square_size_px = square_size_px;
}

/**
 * RenderLogo function: Renders the IST logo on the app window 
 * \param x X coordinate of the Logo
 * \param y Y coordinate of the Logo
 * \param _logoIST surface with the IST logo image to render
 * \param _renderer renderer to handle all rendering in a window
 */
int RenderLogo(int x, int y, SDL_Surface *_logoIST, SDL_Renderer* _renderer)
{
    SDL_Texture *text_IST;
    SDL_Rect boardPos;

    // space occupied by the logo
    boardPos.x = x;
    boardPos.y = y;
    boardPos.w = _logoIST->w;
    boardPos.h = _logoIST->h;

    // render it
    text_IST = SDL_CreateTextureFromSurface(_renderer, _logoIST);
    SDL_RenderCopy(_renderer, text_IST, NULL, &boardPos);

    // destroy associated texture !
    SDL_DestroyTexture(text_IST);
    return _logoIST->h;
}

/**
 * RenderText function: Renders some text on a position inside the app window
 * \param x X coordinate of the text
 * \param y Y coordinate of the text
 * \param text string with the text to be written
 * \param _font TTF font used to render the text
 * \param _color color of the text
 * \param _renderer renderer to handle all rendering in a window
 */
int RenderText(int x, int y, const char *text, TTF_Font *_font, SDL_Color *_color, SDL_Renderer* _renderer)
{
    SDL_Surface *text_surface;
    SDL_Texture *text_texture;
    SDL_Rect solidRect;

    solidRect.x = x;
    solidRect.y = y;
    // create a surface from the string text with a predefined font
    text_surface = TTF_RenderText_Blended(_font,text,*_color);
    if(!text_surface)
    {
        printf("TTF_RenderText_Blended: %s\n", TTF_GetError());
        exit(EXIT_FAILURE);
    }
    // create texture
    text_texture = SDL_CreateTextureFromSurface(_renderer, text_surface);
    // obtain size
    SDL_QueryTexture( text_texture, NULL, NULL, &solidRect.w, &solidRect.h );
    // render it !
    SDL_RenderCopy(_renderer, text_texture, NULL, &solidRect);
    // clear memory
    SDL_DestroyTexture(text_texture);
    SDL_FreeSurface(text_surface);
    return solidRect.h;
}

/**
 * InitEverything: Initializes the SDL2 library and all graphical components: font, window, renderer
 * \param width width in px of the window
 * \param height height in px of the window
 * \param _font font that will be used to render the text
 * \param _img surface to be created with the table background and IST logo
 * \param _window represents the window of the application
 * \param _renderer renderer to handle all rendering in a window
 */
void InitEverything(int width, int height, TTF_Font **_font1, TTF_Font **_font2, SDL_Surface *_img[], SDL_Window** _window, SDL_Renderer** _renderer)
{
    InitSDL();
    InitFont();
    *_window = CreateWindow(width, height);
    *_renderer = CreateRenderer(width, height, *_window);

    // load the table texture
    _img[0] = IMG_Load("table_texture.png");
    if (_img[0] == NULL)
    {
        printf("Unable to load image: %s\n", SDL_GetError());
        exit(EXIT_FAILURE);
    }

    // load IST logo
    _img[1] = SDL_LoadBMP("ist_logo.bmp");
    if (_img[1] == NULL)
    {
        printf("Unable to load bitmap: %s\n", SDL_GetError());
        exit(EXIT_FAILURE);
    }
    // this opens (loads) a font file and sets a size
    *_font1 = TTF_OpenFont("FreeSerif.ttf", 16);
    if(!*_font1)
    {
        printf("TTF_OpenFont: %s\n", TTF_GetError());
        exit(EXIT_FAILURE);
    }

    *_font2 = TTF_OpenFont("OpenSans.ttf", 19);
    if(!*_font2)
    {
        printf("TTF_OpenFont: %s\n", TTF_GetError());
        exit(EXIT_FAILURE);
    }
}

/**
 * InitSDL: Initializes the SDL2 graphic library
 */
void InitSDL()
{
    // init SDL library
    if ( SDL_Init( SDL_INIT_EVERYTHING ) == -1 )
    {
        printf(" Failed to initialize SDL : %s\n", SDL_GetError());
        exit(EXIT_FAILURE);
    }
}

/**
 * InitFont: Initializes the SDL2_ttf font library
 */
void InitFont()
{
    // Init font library
    if(TTF_Init()==-1)
    {
        printf("TTF_Init: %s\n", TTF_GetError());
        exit(EXIT_FAILURE);
    }
}

/**
 * CreateWindow: Creates a window for the application
 * \param width width in px of the window
 * \param height height in px of the window
 * \return pointer to the window created
 */
SDL_Window* CreateWindow(int width, int height)
{
    SDL_Window *window;
    // init window
    window = SDL_CreateWindow( "1024", WINDOW_POSX, WINDOW_POSY, width, height, 0 );
    // check for error !
    if ( window == NULL )
    {
        printf("Failed to create window : %s\n", SDL_GetError());
        exit(EXIT_FAILURE);
    }
    return window;
}

/**
 * CreateRenderer: Creates a renderer for the application
 * \param width width in px of the window
 * \param height height in px of the window
 * \param _window represents the window for which the renderer is associated
 * \return pointer to the renderer created
 */
SDL_Renderer* CreateRenderer(int width, int height, SDL_Window *_window)
{
    SDL_Renderer *renderer;
    // init renderer
    renderer = SDL_CreateRenderer( _window, -1, 0 );

    if ( renderer == NULL )
    {
        printf("Failed to create renderer : %s", SDL_GetError());
        exit(EXIT_FAILURE);
    }

    // set size of renderer to the same as window
    SDL_RenderSetLogicalSize( renderer, width, height );

    return renderer;
}
