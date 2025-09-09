#include <iostream>
#include <string>
using namespace std;

const string TARGET_STATE = "111110111100*110000100000";

const int GRID_SIZE = 5;
const int DIRECTION_X[] = {2, -2, 2, -2, 1, -1, 1, -1};
const int DIRECTION_Y[] = {1, 1, -1, -1, 2, 2, -2, -2};

string current_board;
int search_limit = 0;

int calculateHeuristic(const string& board) {
    int difference = 0;
    for (size_t i = 0; i < board.length(); ++i) {
        if (board[i] != TARGET_STATE[i] && board[i] != '*') difference++;
    }
    return difference;
}

bool judgeBoundary(int row, int col) {
    return row >= 0 && row < GRID_SIZE && col >= 0 && col < GRID_SIZE;
}
bool depthFirstSearch(int last_position, int current_depth) {
    if (current_board == TARGET_STATE) {
        return true;
    }

    // 用启发式函数剪枝，因为换到目标状态的最小步数不会超过当前深度加上启发式值
    int heuristic_value = calculateHeuristic(current_board);
    if (current_depth + heuristic_value > search_limit) {
        return false;
    }

    size_t space_index = current_board.find('*');
    int row = space_index / GRID_SIZE;
    int col = space_index % GRID_SIZE;// 算出横纵坐标

    for (int i = 0; i < 8; ++i) {
        int new_row = row + DIRECTION_X[i];
        int new_col = col + DIRECTION_Y[i];
        int new_position = new_row * GRID_SIZE + new_col;

        // 检查新位置是否在棋盘范围内，且不是上一步的位置
        if (judgeBoundary(new_row,new_col) && new_col < GRID_SIZE && new_position != last_position) {
            swap(current_board[space_index], current_board[new_position]);
            
            if (depthFirstSearch(space_index, current_depth + 1)) {
                return true;
            }

            //这里是回溯，将棋盘状态恢复到上一步，继续尝试其他方向
            swap(current_board[space_index], current_board[new_position]);
        }
    }
    return false;
}

int main() {
    int T;
    cin >> T;

    while (T--) {
        current_board = "";
        search_limit = 0;

        for (int i = 0; i < GRID_SIZE; ++i) {
            string row_input;
            cin >> row_input;
            current_board += row_input;
        }

        while (true) {
            if (depthFirstSearch(-1, 0)) {
                cout << search_limit << endl;
                break;
            }
            search_limit++;
            if (search_limit > 20) {
                cout << -1 << endl;
                break;
            }
        }
    }
    return 0;
}