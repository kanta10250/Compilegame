#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <termios.h>
#include <sys/select.h>

#define WIDTH 40
#define HEIGHT 25

char field[HEIGHT][WIDTH]; // ゲームフィールド

int paddle_x = WIDTH / 2 - 4; // パドルの位置
int ball_x = WIDTH / 2;       // ボールの位置（X座標）
int ball_y = HEIGHT - 4;      // ボールの位置（Y座標）
int ball_dir_x = 1;           // ボールの移動方向（X）
int ball_dir_y = -1;          // ボールの移動方向（Y）

void initializeField() {
    int i, j;
    // フィールドを空白で初期化
    for (i = 0; i < HEIGHT; i++)
        for (j = 0; j < WIDTH; j++)
            field[i][j] = ' ';

    // ブロックを配置
    for (i = 2; i < 8; i++) {
        for (j = 2; j < WIDTH - 2; j++) {
            field[i][j] = '#';
        }
    }
}

void drawField() {
    int i, j;
    system("clear"); // 画面クリア（Unix環境）

    // ANSIエスケープシーケンスで色を設定
    const char *color_reset = "\x1b[0m";
    const char *color_paddle = "\x1b[44m";
    const char *color_ball = "\x1b[41m";
    const char *color_block = "\x1b[42m";

    for (i = 0; i < HEIGHT; i++) {
        printf("|");
        for (j = 0; j < WIDTH; j++) {
            // パドル描画
            if (i == HEIGHT - 2 && j >= paddle_x && j < paddle_x + 8) {
                printf("%s ", color_paddle);
            }
            // ボールの描画
            else if (i == ball_y && j == ball_x) {
                printf("%s ", color_ball);
            }
            // ブロック描画
            else if (field[i][j] == '#') {
                printf("%s ", color_block);
            }
            // フィールドの描画
            else {
                printf("%s ", color_reset);
            }
        }
        printf("%s|\n", color_reset);
    }
    // 下の壁
    for (j = 0; j < WIDTH + 2; j++)
        printf("-");
    printf("\n");
}

int kbhit(void) {
    struct termios oldt, newt;
    int ch;
    int oldf;
    struct timeval tv = {0L, 0L};
    fd_set readfds;

    // ターミナル設定を変更
    tcgetattr(STDIN_FILENO, &oldt);
    newt = oldt;
    newt.c_lflag &= ~(ICANON | ECHO);
    tcsetattr(STDIN_FILENO, TCSANOW, &newt);

    // ファイル記述子セット
    FD_ZERO(&readfds);
    FD_SET(STDIN_FILENO, &readfds);

    int retval = select(STDIN_FILENO+1, &readfds, NULL, NULL, &tv);

    // ターミナル設定を元に戻す
    tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
    return retval;
}

int getch(void) {
    struct termios oldt, newt;
    int ch;
    // ターミナル設定を変更
    tcgetattr(STDIN_FILENO, &oldt);
    newt = oldt;
    newt.c_lflag &= ~(ICANON | ECHO); // 非カノニカル、エコーオフ
    tcsetattr(STDIN_FILENO, TCSANOW, &newt);
    ch = getchar();
    // ターミナル設定を元に戻す
    tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
    return ch;
}

void updateBall() {
    // ボールの位置を更新
    int next_x = ball_x + ball_dir_x;
    int next_y = ball_y + ball_dir_y;

    // 左右の壁に当たった場合
    if (next_x <= 0 || next_x >= WIDTH - 1)
        ball_dir_x = -ball_dir_x;

    // 上の壁に当たった場合
    if (next_y <= 2)
        ball_dir_y = -ball_dir_y;

    // パドルに当たった場合
    if (next_y == HEIGHT - 3 && next_x >= paddle_x && next_x < paddle_x + 8) {
        ball_dir_y = -ball_dir_y;
    }

    // ブロックに当たった場合
    if (field[next_y][next_x] == '#') {
        field[next_y][next_x] = ' ';
        ball_dir_y = -ball_dir_y;
    }

    // ボールを移動
    ball_x += ball_dir_x;
    ball_y += ball_dir_y;
}

int main() {
    initializeField();

    while (1) {
        // キー入力処理
        if (kbhit()) {
            char key = getch();
            if ((key == 'a' || key == 'A') && paddle_x > 0)
                paddle_x -= 8;
            else if ((key == 'd' || key == 'D') && paddle_x < WIDTH - 8)
                paddle_x += 8;
        }

        updateBall();

        drawField();

        // gameover
        if (ball_y >= HEIGHT - 1) {
            printf("GAME OVER！\n");
            break;
        }

        // すべてのブロックを破壊したか判定
        int blocks_left = 0;
        for (int i = 0; i < HEIGHT; i++)
            for (int j = 0; j < WIDTH; j++)
                if (field[i][j] == '#')
                    blocks_left++;

        if (blocks_left == 0) {
            printf("ゲームクリア！\n");
            break;
        }

        usleep(40000); // 速度
    }

    return 0;
}
