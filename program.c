#include <unistd.h>
#include <stdlib.h>
#include <ncurses.h>  // include ncurses for the initscr() function

typedef struct {
    int x;
    int y;
} vec2;

vec2 segments[256];  // array to store the snake segments

int score = 0;

void draw_border(int screen_width, int screen_height) {
    for (int x = 0; x < screen_width; x++) {
        mvaddch(0, x * 2, '*');  // Top border
        mvaddch(screen_height - 1, x * 2, '*');  // Bottom border
    }
    for (int y = 0; y < screen_height; y++) {
        mvaddch(y, 0, '*');  // Left border
        mvaddch(y, (screen_width - 1) * 2, '*');  // Right border
    }
}

int main() {

    int screen_width = 40;
    int screen_height = 20;

    // Initialize A Screen
    WINDOW *win = initscr();

    // Take player input
    keypad(win, true);
    nodelay(win, true);
    curs_set(0);

    // Snake's initial head position and direction
    vec2 head = {screen_width / 2, screen_height / 2};
    vec2 dir = {1, 0};

    // Berry (food) position
    vec2 berry = {rand() % (screen_width - 2) + 1, rand() % (screen_height - 2) + 1};

    while (true) {

        int presses = wgetch(win);

        // Player input to change direction
        if (presses == KEY_LEFT) {
            if (dir.x == 1) continue;
            dir.x = -1;
            dir.y = 0;
        }
        if (presses == KEY_RIGHT) {
            if (dir.x == -1) continue;
            dir.x = 1;
            dir.y = 0;
        }
        if (presses == KEY_UP) {
            if (dir.y == 1) continue;
            dir.x = 0;
            dir.y = -1;
        }
        if (presses == KEY_DOWN) {
            if (dir.y == -1) continue;
            dir.x = 0;
            dir.y = 1;
        }
        if (presses == '\e') {  // ESC key to quit the game
            break;
        }

        // Move the snake's body segments
        for (int i = score; i > 0; i--) {
            segments[i] = segments[i - 1];
        }
        segments[0] = head;  // update the segment where the head was

        // Move the head in the current direction
        head.x += dir.x;
        head.y += dir.y;

        // Check for collision with the body
        for (int i = 0; i < score; i++) {
            if (head.x == segments[i].x && head.y == segments[i].y) {
                mvprintw(screen_height / 2, screen_width / 2, "I lose");
                refresh();
                usleep(2000000);  // wait for 2 seconds to display the message
                endwin();
                return 0;
            }
        }

        // Check if the snake eats the berry
        if (head.x == berry.x && head.y == berry.y) {
            score += 1;
            berry.x = rand() % (screen_width - 2) + 1;
            berry.y = rand() % (screen_height - 2) + 1;
        }

        // Check for collision with the border
        if (head.x == 0 || head.x == screen_width - 1 || head.y == 0 || head.y == screen_height - 1) {
            mvprintw(screen_height / 2, screen_width / 2, "I lose");
            refresh();
            usleep(2000000);  // wait for 2 seconds to display the message
            endwin();
            return 0;
        }

        // Clear the screen and draw the border
        erase();
        draw_border(screen_width, screen_height);

        // Draw the score
        mvprintw(0, (screen_width * 2) / 2 - 5, "Score: %d", score);

        // Draw the berry and the snake
        mvaddch(berry.y, berry.x * 2, '@');
        for (int i = 0; i < score; i++) {
            mvaddch(segments[i].y, segments[i].x * 2, 'o');
        }
        mvaddch(head.y, head.x * 2, 'O');

        usleep(125000);  // Control the speed of the snake
    }

    endwin();

    return 0;
}



