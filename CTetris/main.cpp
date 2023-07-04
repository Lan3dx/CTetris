/*

  Lan3 - 2023
  Telegram - https://t.me/Lan3_Tg

*/

#include <iostream>
#include <vector>
#include <string>
#include <Windows.h>

namespace Shapes {
  // { {x, x, x, x}, {y, y, y, y} }
  std::vector<std::vector<int>> I = { {5, 5, 5, 5}, {0, 1, 2, 3} };
  std::vector<std::vector<int>> L = { {5, 5, 5, 6}, {0, 1, 2, 2} };
  std::vector<std::vector<int>> J = { {6, 6, 6, 5}, {0, 1, 2, 2} };
  std::vector<std::vector<int>> O = { {6, 6, 5, 5}, {1, 0, 1, 0} };
  std::vector<std::vector<int>> T = { {4, 5, 5, 6}, {1, 1, 0, 1} };
  std::vector<std::vector<int>> Z = { {4, 5, 5, 6}, {0, 0, 1, 1} };
  std::vector<std::vector<int>> S = { {4, 5, 5, 6}, {1, 1, 0, 0} };
  std::vector<std::vector<std::vector<int>>> shapes = { I,L,J,O,T,Z,S };
}

namespace CD {
  int fall = 250; // cooldown for gravity
  int key = 55; // cooldown for keyboard
}

void cfg() {
  void* activeBuffer = GetStdHandle(STD_OUTPUT_HANDLE);  // get active buffer
  SMALL_RECT zeroWindow = { 0, 0, 0, 0 };
  COORD zBuffer = { 1, 1 };
  SetConsoleWindowInfo(activeBuffer, TRUE, &zeroWindow);
  SetConsoleScreenBufferSize(activeBuffer, zBuffer);

  COORD bufferSize = { 74, 26 }; // set buffer size
  SMALL_RECT windowSize = { 0, 0, 74 - 1, 26 - 1 };  // set window size
  SetConsoleScreenBufferSize(activeBuffer, bufferSize);
  SetConsoleWindowInfo(activeBuffer, TRUE, &windowSize);

  CONSOLE_CURSOR_INFO structCursorInfo; // Disabling the cursor 
  GetConsoleCursorInfo(activeBuffer, &structCursorInfo);
  structCursorInfo.bVisible = FALSE;
  SetConsoleCursorInfo(activeBuffer, &structCursorInfo);

  SetConsoleTextAttribute(activeBuffer, 15); // set console color
}

void clear() {
  SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), { 0,0 }); // move cursor position to 0:0
}

// create clear board
std::vector<std::vector<int>> board_init() {
  std::vector<std::vector<int>> board(20, std::vector<int>(10));
  for (size_t i = 0; i < board.size(); i++) {
    for (size_t j = 0; j < board[i].size(); j++) {
      board[i][j] = 0;
    }
  }
  return board;
}

// output
void render(std::vector<std::vector<int>>&  board, int Score, std::vector<std::vector<int>> nextShape, int nsindex) {
  for (size_t i = 0; i < nextShape[0].size(); i++) {
    nextShape[0][i] -= 3;
    if (nsindex != 0) {
      nextShape[1][i] += 1;
    }
  }
  // local board of the next shape
  std::vector<std::vector<int>> board_ns(6, std::vector<int>(5));
  for (size_t i = 0; i < board_ns.size(); i++) {
    for (size_t j = 0; j < board_ns[i].size(); j++) {
      board_ns[i][j] = 0;
    }
  }
  // fill in the board of the next shape according to the coordinates
  for (size_t y = 0; y < nextShape[0].size(); y++) {
    board_ns[nextShape[1][y]][nextShape[0][y]] = 1;
  }
  std::cout << '\n' << '\n';
  for (size_t i = 0; i < board.size(); i++) {
    std::string line = "                         ";
    // main window
    for (size_t j = 0; j < board[i].size(); j++) {
      if (j == 0) {
        line += "<!";
      }
      // default shape
      if (board[i][j] == 1) {
        line += "[]";
      }
      // shape shadow
      else if (board[i][j] == 2) {
          line += "()";
      }
      // space
      else {
          line += " .";
      }
      if (j == board[i].size() - 1) {
        line += "!>";
      }
    }
    if (i == 3) {
      line += "       Score:";
      line += std::to_string(Score);
    }

    // next shape window
    else if (i == 5) {
      line += "       Next:";
    }
    else if (i == 7) {
      line += "       ";
      for (size_t k = 0; k < board_ns[0].size(); k++) {
        if (board_ns[0][k] == 1) {
          line += "[]";
        }
        else {
          line += " .";
        }
      }
    }
    else if (i == 8) {
      line += "       ";
      for (size_t k = 0; k < board_ns[1].size(); k++) {
        if (board_ns[1][k] == 1) {
          line += "[]";
        }
        else {
          line += " .";
        }
      }
    }
    else if (i == 9) {
      line += "       ";
      for (size_t k = 0; k < board_ns[2].size(); k++) {
        if (board_ns[2][k] == 1) {
          line += "[]";
        }
        else {
          line += " .";
        }
      }
    }
    else if (i == 10) {
      line += "       ";
      for (size_t k = 0; k < board_ns[3].size(); k++) {
        if (board_ns[3][k] == 1) {
          line += "[]";
        }
        else {
          line += " .";
        }
      }
    }
    else {
      line += "                         ";
    }

    // output line
    std::cout << line << '\n';
  }
  std::string line = "                         <!====================!>                         ";
  std::string line1 = "                           \\/\\/\\/\\/\\/\\/\\/\\/\\/\\/                           ";
  std::cout << line << '\n' << line1 << '\n';
}

class Shape {
public:
  bool isAlive; // boolean variable to check if shape has not collided with an object or wall
  int shape_i; // index (check namespace Shapes)
  std::vector<std::vector<int>> shape; // shape coordinates

  Shape(std::vector<std::vector<int>> c_shape, int c_shape_i) {
    isAlive = true;
    shape = c_shape;
    shape_i = c_shape_i;
  }

  bool getStatus() {
    return isAlive;
  }

  // replacing the coordinates of the shape and its index from namespace Shapes
  void reset(std::vector<std::vector<int>> r_shape, int r_shape_i) {
    shape = r_shape;
    shape_i = r_shape_i;
    isAlive = true;
  }

  // Collision with figures
  bool shape_clsn(std::vector<std::vector<int>> board, char type) {
    // b - bottom
    if (type == 'b') {
      std::vector<std::vector<int>> nshape = shape;
      for (size_t y = 0; y < nshape[0].size(); y++) {
        nshape[1][y] += 1;
      }
      for (size_t x = 0; x < nshape[0].size(); x++) {
        if (board[nshape[1][x]][nshape[0][x]] == 1) {
          isAlive = false;
          return true;
        }
      }
    }
    // r - right
    if (type == 'r') {
      std::vector<std::vector<int>> nshape = shape;
      for (size_t y = 0; y < nshape[0].size(); y++) {
        nshape[0][y] += 1;
      }
      for (size_t x = 0; x < nshape[0].size(); x++) {
        if (board[nshape[1][x]][nshape[0][x]] == 1) {
          return true;
        }
      }
    }
    // l - left
    if (type == 'l') {
      std::vector<std::vector<int>> nshape = shape;
      for (size_t y = 0; y < nshape[0].size(); y++) {
        nshape[0][y] -= 1;
      }
      for (size_t x = 0; x < nshape[0].size(); x++) {
        if (board[nshape[1][x]][nshape[0][x]] == 1) {
          return true;
        }
      }
    }
    return false;
  }

  // Collision with walls
  bool wall_clsn(std::vector<std::vector<int>>& board, char type)
  {
    int right = -1;
    int left = 11;
    int bottom = -1;

    for (size_t i = 0; i < shape[0].size(); i++) {
      if (shape[0][i] > right) {
        right = shape[0][i];
      }
      if (shape[0][i] < left) {
        left = shape[0][i];
      }
      if (shape[1][i] > bottom) {
        bottom = shape[1][i];
      }
    }

    if (right == board[0].size()-1) {
      // r - right
      if(type == 'r')
        return true;
    }
    if (left == 0) {
      // l - left
      if(type == 'l')
        return true;
    }
    if (bottom == board.size()-1) {
      // b - bottom
      if (type == 'b') {
        isAlive = false;
        return true;
      }
    }

    return false;
  }

  // Lower the figure down by 1 element
  void lower(std::vector<std::vector<int>>& board) {
    if (wall_clsn(std::ref(board), 'b') == false) {
      if (shape_clsn(std::ref(board), 'b') == false) { 
        for (size_t y = 0; y < shape[0].size(); y++) {
          shape[1][y] += 1;
        }
      }
    }
  }

  // Move the figure left or right
  void move(int side, std::vector<std::vector<int>>& board) {
    char type = 'n';
    if (side == 1) {
      // r - right
      type = 'r';
    }
    else {
      // l - left
      type = 'l';
    }
    if (wall_clsn(std::ref(board), type) == false) {
      if (shape_clsn(std::ref(board), type) == false) {
        for (size_t x = 0; x < shape[0].size(); x++) {
          shape[0][x] += side;
        }
      }
    }
  }

  // rotate figure
  void rotate(std::vector<std::vector<int>>& board) {
    // index 3 == { O } shape
    if (shape_i != 3) {
      std::vector<std::vector<int>> d_shape = shape;
      int cx = shape[0][1];
      int cy = shape[1][1];

      for (size_t x = 0; x < shape[0].size(); x++) {
        int x0 = shape[1][x] - cy;
        int y0 = shape[0][x] - cx;
        shape[0][x] = cx - x0;
        shape[1][x] = cy + y0;
      }

      // check collisions with others figures or walls
      for (size_t x = 0; x < shape[0].size(); x++) {
        if (shape[0][x] > board[0].size() - 1) {
          shape = d_shape;
        }
        if (shape[0][x] < 0) {
          shape = d_shape;
        }
        if (shape[1][x] > board.size() - 1) {
          shape = d_shape;
        }
        if (board[shape[1][x]][shape[0][x]] == 1) {
          shape = d_shape;
        }
      }

    }
  }

  // remove shape from board
  void remove(std::vector<std::vector<int>>& board) {
    for (size_t y = 0; y < shape[0].size(); y++) {
      board[shape[1][y]][shape[0][y]] = 0;
    }
  }

  // spawn shape on board
  void spawn(std::vector<std::vector<int>>& board, int symbol) {
    for (size_t y = 0; y < shape[0].size(); y++) {
      // symbol:
      // 1 - default shape
      // 2 - shape shadow
      board[shape[1][y]][shape[0][y]] = symbol;
    }
  }
};

// reducing cooldowns by 1
void cdset(std::vector<int>& values) {
  for (size_t i = 0; i < values.size(); i++) {
    values[i] -= 1;
  }
}

// remove line
// returns true if one line was burned
// this is necessary to calculate the number of points for the burned lines
bool burnline(std::vector<std::vector<int>>& board) {
  int min = 0;
  bool flag = false;
  for (int i = 0; i < board.size(); i++) {
    flag = false;
    for (size_t j = 0; j < board[0].size(); j++) {
      if (board[i][j] != 1) {
        flag = true;
      }
    }
    if (flag == false) {
      min = i;
      break;
    }
  }
  if (flag == true) {
    return false;
  }
  for (size_t i = 0; i < board[0].size(); i++) {
    board[min][i] = 0;
  }
  for (size_t i = min; i > 0; i--) {
    for (size_t j = 0; j < board[0].size(); j++) {
      board[i][j] = board[i - 1][j];
      board[i - 1][j] = 0;
    }
  }
  return true;
}

// determining where the shape shadow will be located
void shadowcalculate(Shape* shape, Shape* shapeshadow, std::vector<std::vector<int>>& board, int random_shape, bool isRotate) {
  shapeshadow->remove(std::ref(board));
  // delete the default shape again so that there are no errors in the calculations
  shape->remove(std::ref(board));
  *shapeshadow = *shape;
  // check whether the user flips the shape to calculate the coordinates accurately
  if (isRotate) {
    shapeshadow->rotate(std::ref(board));
  }
  // just lower the shape
  for (size_t i = 0; i < 20; i++) {
    shapeshadow->remove(std::ref(board));
    shapeshadow->lower(std::ref(board));
    shapeshadow->spawn(std::ref(board), 2);
  }
  // spawn default shape after calculating
  shape->spawn(std::ref(board), 1);
}

int main() {
  cfg();
  srand((unsigned int)time(NULL));

  int Score = 0;
  int lines = 0; // number of burnt lines
  auto board = board_init();
  bool isFrozen = false;
  bool isBurned = false;
  int random_shape = rand() % 7;
  Shape shape(Shapes::shapes[random_shape], random_shape);
  Shape shapeshadow(Shapes::shapes[random_shape], random_shape);
  int next_random_shape = rand() % 7;
  std::vector<std::vector<int>> nextShape = Shapes::shapes[next_random_shape];
  std::vector<int> values = { CD::fall, CD::key };

  while (true) {
    isFrozen = !shape.getStatus();

    // control
    if (GetAsyncKeyState((unsigned short)'D') || GetAsyncKeyState((unsigned short)VK_RIGHT))
    {
      if (values[1] <= 0) {
        shapeshadow.remove(std::ref(board));
        shapeshadow.move(1, std::ref(board));
        shapeshadow.spawn(std::ref(board), 2);
        shape.remove(std::ref(board));
        shape.move(1, std::ref(board));
        shape.spawn(std::ref(board), 1);
        values[1] = CD::key;
      }
    }
    if (GetAsyncKeyState((unsigned short)'A') || GetAsyncKeyState((unsigned short)VK_LEFT))
    {
      if (values[1] <= 0) {
        shapeshadow.remove(std::ref(board));
        shapeshadow.move(-1, std::ref(board));
        shapeshadow.spawn(std::ref(board), 2);
        shape.remove(std::ref(board));
        shape.move(-1, std::ref(board));
        shape.spawn(std::ref(board), 1);
        values[1] = CD::key;
      }
    }
    if (GetAsyncKeyState((unsigned short)'S') || GetAsyncKeyState((unsigned short)VK_DOWN))
    {
      if (values[1] <= 0) {
        shape.remove(std::ref(board));
        shape.lower(std::ref(board));
        shape.spawn(std::ref(board), 1);
        values[1] = CD::key;
      }
    }
    if (GetAsyncKeyState((unsigned short)'W') || GetAsyncKeyState((unsigned short)VK_UP) || GetAsyncKeyState((unsigned short)VK_SPACE))
    {
      if (values[1] <= 0) {
        shadowcalculate(&shape, &shapeshadow, std::ref(board), random_shape, true);
        shape.remove(std::ref(board));
        shape.rotate(std::ref(board));
        shape.spawn(std::ref(board), 1);
        values[1] = CD::key;
      }
    }

    // lower shape
    if (shape.getStatus()) {
      if (values[0] <= 0) {
        shape.remove(std::ref(board));
        shape.lower(std::ref(board));
        shape.spawn(std::ref(board), 1);
        values[0] = CD::fall;
      }
      shadowcalculate(&shape, &shapeshadow, std::ref(board), random_shape, false);
    }

    // Checking if an event has occurred when the game should end
    if (shape.getStatus() == false) {
      if (board[0][5] == 1) {
        clear();
        std::cout << "GAME OVER" << std::endl;
        exit(0);
      }
    }

    // Check if there is a burnt line
    if (shape.getStatus() == false) {
      shapeshadow = shape;
      isBurned = burnline(std::ref(board));
      if (isBurned) {
        lines += 1;
        while (isBurned) {
          isBurned = burnline(std::ref(board));
          if (isBurned)
          {
            lines += 1;
          }
          render(std::ref(board), Score, nextShape, next_random_shape);
          Sleep(100);
          clear();
        }
      }
    }

    // if shape collision with other figure
    if (shape.getStatus() == false) {
      shapeshadow.reset(nextShape, next_random_shape);
      shape.reset(nextShape, next_random_shape);
      next_random_shape = rand() % 7;
      nextShape = Shapes::shapes[next_random_shape];
      isFrozen = false;
    }

    // scoring according to tetris standards
    if (lines == 1) {
      Score += 100;
      lines = 0;
    }
    else if (lines == 2) {
      Score += 300;
      lines = 0;  
    }
    else if (lines == 3) {
      Score += 700;
      lines = 0;
    } 
    else if(lines == 4){
      Score += 1500;
      lines = 0;
    }

    // cooldowns -= 1
    cdset(std::ref(values));

    // output
    render(std::ref(board), Score, nextShape, next_random_shape);
    clear();
  }

  return 0;
}