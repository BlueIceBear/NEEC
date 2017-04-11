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
void InitEverything(int , int , TTF_Font **, SDL_Surface **, SDL_Window ** , SDL_Renderer ** );
void InitSDL();
void InitFont();
SDL_Window* CreateWindow(int , int );
SDL_Renderer* CreateRenderer(int , int , SDL_Window *);
int RenderText(int, int, const char *, TTF_Font *, SDL_Color *, SDL_Renderer *);
int RenderLogo(int, int, SDL_Surface *, SDL_Renderer *);
void RenderTable(int, int *, int *, TTF_Font *, SDL_Surface **, SDL_Renderer *);
void RenderBoard(int [][MAX_BOARD_POS], SDL_Surface **, int, int, int, SDL_Renderer *);
void RenderStats( SDL_Renderer *, TTF_Font *, int , int , int );
void LoadValues(SDL_Surface **);
void UnLoadValues(SDL_Surface **);


/* All the parameters of each function are explained respectively,
* according to the order of their declaration
*/

void Parameters(int *, char *, int*);
void SaveResults(char name[STRING_SIZE] , int, int, int);
void GenStart(int board[][MAX_BOARD_POS], int, int *);
void GenPiece(int board[][MAX_BOARD_POS], int, int *);
int MovPieceUp(int board[][MAX_BOARD_POS], int, int *, int *);
int MovPieceDown(int board[][MAX_BOARD_POS], int, int *, int *);
int MovPieceLeft(int board[][MAX_BOARD_POS], int, int *, int *);
int MovPieceRight(int board[][MAX_BOARD_POS], int, int *, int *);
void NewGame(int board[][MAX_BOARD_POS], int);

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
    TTF_Font *serif = NULL;
    SDL_Surface *array_of_numbers[MAX_LEVELS], *imgs[2];
    SDL_Event event;
    int delay = 300;
    int quit = 0;
    int width = (TABLE_SIZE + LEFT_BAR_SIZE);
    int height = TABLE_SIZE;
    int square_size_px, board_size_px, board_pos = 5;
    int board[MAX_BOARD_POS][MAX_BOARD_POS] = {{0}};

    int board_size;
    char name[STRING_SIZE];
    int difficulty;

    int game = 0, begin =0;

    int points = 0, max_points = 0, max_pc = 0, game_time = 0;

    int test = 0;

    max_points = 0;


    
    // some examples
    //board[1][0] = 1;
    //board[1][2] = 2;
    //board[3][3] = 3;
    //board[4][4] = 4;
    //board[4][0] = 5;
    //board[3][0] = 8;
    //board[0][1] = 10;
    

    // Initializes the function rand()
    srand(time(NULL));

    // Asks for parameters
    Parameters(&board_size, name, &difficulty);

    // initialize graphics
    InitEverything(width, height, &serif, imgs, &window, &renderer);
    // loads numbers as images
    LoadValues(array_of_numbers);

    begin = time(NULL);

    while( quit == 0 )
    {
        if(game == 1) game_time = time(NULL) - begin;
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

                        if(game == 1) SaveResults(name, points, max_pc, game_time);

                        NewGame(board, board_size);
                        GenStart(board, board_size, &max_pc);

                        points = 0;
                        max_pc = 0;

                        // Variable which decides when the game starts
                        game = 1;
                        break;

                    case SDLK_u:
                        break;

                    case SDLK_UP:
                        if(game == 1)
                        {
                            printf("Up\n");
                            test = MovPieceUp(board, board_size, &points, &max_pc);

                            if(test != 0) GenPiece(board, board_size, &max_pc);
                        }

                        break;

                    case SDLK_DOWN:
                        if(game == 1)
                        {
                            printf("Down\n");
                            test = MovPieceDown(board, board_size, &points, &max_pc);

                            if(test != 0) GenPiece(board, board_size, &max_pc);
                        }

                        break;
                    
                    case SDLK_LEFT:
                        if(game == 1)
                        {
                            printf("Left\n");
                            test = MovPieceLeft(board, board_size, &points, &max_pc);

                            if(test != 0) GenPiece(board, board_size, &max_pc);
                        }

                        break;
                    
                    case SDLK_RIGHT:
                        if(game == 1)
                        {
                            printf("Right\n");
                            test = MovPieceRight(board, board_size, &points, &max_pc);

                            if(test != 0) GenPiece(board, board_size, &max_pc);
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

        printf("%d\n", game_time);
        // renders stats
        RenderStats(renderer, serif, difficulty, points, game_time);
        // render in the screen all changes above
        SDL_RenderPresent(renderer);
        // add a delay
        SDL_Delay( delay );
    }

    // free memory allocated for images and textures and closes everything including fonts
    UnLoadValues(array_of_numbers);
    TTF_CloseFont(serif);
    SDL_FreeSurface(imgs[0]);
    SDL_FreeSurface(imgs[1]);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    game_time = time(NULL) - begin;

    max_pc = pow(2, max_pc);

    SaveResults(name, points, max_pc, game_time);

    return EXIT_SUCCESS;
}








/* Parameters: Asks the user for the required parameters for the game to work 
*  Board size, player's name and difficulty
*/
void Parameters(int *_board_size, char *_name, int *_difficulty)
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
        scanf("%s", _name);

        if(strlen(_name) > 8 || strlen(_name) < 1) printf("The name must have at least 8 characters.\n");
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


/* MovPieceUp: Caculates the movements of the pieces on the board, including adding pieces
*  The board's matrix and it's size
*/
int MovPieceUp(int board[][MAX_BOARD_POS], int _board_size, int *points, int *max_pc)
{
    int i, j, l, aux, test = 0;

    for(i = 0; i < _board_size; i++)
    {
        printf("i = %d\n", i);
        for(j = 0; j < _board_size; j++)
        {
            printf("j = %d\n", j);
            if(board[i][j] != 0)
            {
                aux = board[i][j];

                // other variables to increment: l
                for(l = j - 1; l >= 0 && board[i][l] == 0; l--){}

                // substitution of the values so the piece appears to move
                board[i][j] = 0;
                board[i][l + 1] = aux;

                // Verifies if there are pieces to be added
                if(l > -1 && board[i][l + 1] == board[i][l] && board[i][l] != 0)
                {
                    board[i][l + 1] = 0;
                    board[i][l] += 1;

                    *points += pow(2, board[i][l]);

                    printf("points = %d\n", *points);

                    if(*max_pc < board[i][l]) *max_pc = board[i][l];

                    // Make sure the piece is generated if the pieces involved in the fusion are right next ot each other
                    test = 1;
                }

                // tests if a piece moved or not
                if(j != l + 1) test = 1;

                printf("i,l + 1= %d %d\n", i, l + 1);
            }
        }
    }

    return test;
}



/* MovPieceDown: Caculates the movements of the pieces on the board
*  The board's matrix and it's size
*/
int MovPieceDown(int board[][MAX_BOARD_POS], int _board_size, int *points, int *max_pc)
{
    int i, j, l, aux, test = 0;

    for(i = 0; i < _board_size; i++)
    {
        printf("i = %d\n", i);
        for(j = _board_size - 1; j >= 0; j--)
        {
            printf("j = %d\n", j);
            if(board[i][j] != 0)
            {
                aux = board[i][j];

                // other variables to increment: l
                for(l = j + 1; l < _board_size && board[i][l] == 0; l++)

                // substitution of the values so the piece appears to move
                board[i][j] = 0;
                board[i][l - 1] = aux;

                // Verifies if there are pieces to be added
                if(l < _board_size && board[i][l - 1] == board[i][l] && board[i][l] != 0) 
                {
                    board[i][l - 1] = 0;
                    board[i][l] += 1;

                    *points += pow(2, board[i][l]);

                    printf("points = %d", *points);

                    if(*max_pc < board[i][l]) *max_pc = board[i][l];

                    // Make sure the piece is generated if the pieces involved in the fusion are right next ot each other
                    test = 1;
                }

                // tests if a piece moved or not
                if(j != l - 1) test = 1;

                printf("i,l - 1= %d %d\naux = %d\n", i, l - 1, aux);
            }
        }
    }

    return test;
}

/* MovPieceLeft: Caculates the movements of the pieces on the board
*  The board's matrix and it's size
*/
int MovPieceLeft(int board[][MAX_BOARD_POS], int _board_size, int *points, int *max_pc)
{
    int i, j, l, aux, test = 0;

    for(i = 0; i < _board_size; i++)
    {
        printf("i = %d\n", i);
        for(j = 0; j < _board_size; j++)
        {
            printf("j = %d\n", j);
            if(board[i][j] != 0)
            {
                aux = board[i][j];

                // other variables to increment: l
                for(l = i - 1; l >= 0 && board[l][j] == 0; l--)

                // substitution of the values so the piece appears to move
                board[i][j] = 0;
                board[l + 1][j] = aux;

                // Verifies if there are pieces to be added
                if(l > -1 && board[l + 1][j] == board[l][j] && board[l][j] != 0) 
                {
                    board[l + 1][j] = 0;
                    board[l][j] += 1;

                    *points += pow(2, board[l][j]);

                    printf("points = %d", *points);

                    if(*max_pc < board[i][l]) *max_pc = board[i][l];

                    // Make sure the piece is generated if the pieces involved in the fusion are right next ot each other
                    test = 1;
                }

                // tests if a piece moved or not
                if(i != l + 1) test = 1;

                printf("j,l + 1= %d %d", j, l + 1);
            }
        }
    }

    return test;
}

/* MovPieceRight: Caculates the movements of the pieces on the board
*  The board's matrix and it's size
*/
int MovPieceRight(int board[][MAX_BOARD_POS], int _board_size, int *points, int *max_pc)
{
    int i, j, l, aux, test = 0;

    for(i = _board_size; i >= 0; i--)
    {
        printf("i = %d\n", i);
        for(j = 0; j < _board_size; j++)
        {
            printf("j = %d\n", j);
            if(board[i][j] != 0)
            {
                aux = board[i][j];

                // other variables to increment: l
                for(l = i + 1; l < _board_size && board[l][j] == 0; l++) {}

                // substitution of the values so the piece appears to move
                board[i][j] = 0;
                board[l - 1][j] = aux;

                // Verifies if there are pieces to be added
                if(l < _board_size && board[l - 1][j] == board[l][j] && board[l][j] != 0) 
                {
                    board[l - 1][j] = 0;
                    board[l][j] += 1;

                    *points += pow(2, board[l][j]);

                    printf("points = %d", *points);

                    if(*max_pc < board[i][l]) *max_pc = board[i][l];

                    // Make sure the piece is generated if the pieces involved in the fusion are right next ot each other
                    test = 1;
                }

                // tests if a piece moved or not
                if(i != l - 1) test = 1;

                printf("i,l + 1= %d %d", i, l + 1);
            }
        }
    }

    return test;
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
*  Player's name, points, the highest piece they achieved and the total game time
*/
void SaveResults(char name[STRING_SIZE], int points, int max_pc, int game_time)
{
    FILE *results;

    results = fopen("stats.txt", "a");

    fprintf(results, "%s\nPoints - %d\nHighest piece - %d\nTime - %d\n\n", name, points, max_pc, game_time);

    fclose(results);
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
    char info[STRING_SIZE];

    SDL_Color orange = {255, 150, 0};

    _level = pow(2, _level);

    sprintf(info,"Level: %d                          Score: %d                              Time: %d", _level, _score, _time);

    RenderText(120, 150, info, _font, &orange, _renderer);
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
void InitEverything(int width, int height, TTF_Font **_font, SDL_Surface *_img[], SDL_Window** _window, SDL_Renderer** _renderer)
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
    *_font = TTF_OpenFont("FreeSerif.ttf", 16);
    if(!*_font)
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
