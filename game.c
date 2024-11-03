#include <stdio.h>

char board[3][3]; // ゲームボード

void initializeBoard();
void displayBoard();
int checkWin();
void playerMove(char symbol);

int main() {
    int turn = 0; // ターン数
    int result = 0; // ゲーム結果（0: 継続、1: 勝利、2: 引き分け）
    char symbols[2] = {'X', 'O'}; // プレイヤーのシンボル

    initializeBoard();

    printf("〇×ゲームへようこそ！\n\n");

    while (1) {
        displayBoard();
        printf("プレイヤー %d (%c) の番です。\n", turn % 2 + 1, symbols[turn % 2]);
        playerMove(symbols[turn % 2]);

        result = checkWin();

        if (result == 1) {
            displayBoard();
            printf("プレイヤー %d (%c) の勝利です！\n", turn % 2 + 1, symbols[turn % 2]);
            break;
        } else if (result == 2) {
            displayBoard();
            printf("引き分けです！\n");
            break;
        }

        turn++;
    }

    return 0;
}

void initializeBoard() {
    int i, j;
    // ボードを初期化
    for (i = 0; i < 3; i++) {
        for (j = 0; j < 3; j++) {
            board[i][j] = '1' + i * 3 + j; // '1'から'9'の数字をセット
        }
    }
}

void displayBoard() {
    int i;
    printf("\n");
    // ボードを表示
    for (i = 0; i < 3; i++) {
        printf(" %c | %c | %c \n", board[i][0], board[i][1], board[i][2]);
        if (i != 2) {
            printf("---|---|---\n");
        }
    }
    printf("\n");
}

void playerMove(char symbol) {
    int choice;
    int row, col;

    while (1) {
        printf("配置する場所の番号を入力してください（1-9）: ");
        scanf("%d", &choice);

        if (choice >= 1 && choice <= 9) {
            row = (choice - 1) / 3;
            col = (choice - 1) % 3;

            if (board[row][col] != 'X' && board[row][col] != 'O') {
                board[row][col] = symbol;
                break;
            } else {
                printf("その場所は既に埋まっています。別の場所を選んでください。\n");
            }
        } else {
            printf("無効な入力です。1から9の数字を入力してください。\n");
        }
    }
}

int checkWin() {
    int i;
    // 横ラインのチェック
    for (i = 0; i < 3; i++) {
        if (board[i][0] == board[i][1] && board[i][1] == board[i][2]) {
            return 1;
        }
    }

    // 縦ラインのチェック
    for (i = 0; i < 3; i++) {
        if (board[0][i] == board[1][i] && board[1][i] == board[2][i]) {
            return 1;
        }
    }

    // 斜めラインのチェック
    if (board[0][0] == board[1][1] && board[1][1] == board[2][2]) {
        return 1;
    }
    if (board[0][2] == board[1][1] && board[1][1] == board[2][0]) {
        return 1;
    }

    // 引き分けのチェック
    for (i = 0; i < 3; i++) {
        int j;
        for (j = 0; j < 3; j++) {
            if (board[i][j] != 'X' && board[i][j] != 'O') {
                return 0; // まだ空きがあるのでゲーム続行
            }
        }
    }

    return 2; // 引き分け
}
