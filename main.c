#include "header.h"

void init() {
    // buat baca file
    FILE *fp = fopen("dictionary", "r");
    int i;
    // menyimpan kata dari file sebanyak variabel
    for(i = 0; i < WORDSCOUNT; ++i) {
        fscanf(fp, "%s\n", databaseKata[i]);
    }
}
 
void makeNewText(char* text) {
    /* menampilkan teks di typethis menyimpan teks */
    char output[255];

    int index = 0;

    while(strlen(output) < 60) {
        int random = rand() % WORDSCOUNT;

        char temp[50];

        strcpy(temp, databaseKata[random]);
        
        for(int i = 0; i < strlen(temp); ++i, ++index) {
            output[index] = temp[i];
        }

        output[index] = ' ';
        index++;
    }

    output[index - 1] = '\0';
    strcpy(text, output);
}

int menu() {
    // menampilkan menu pilihan
    initscr();
    noecho();
    // initialize window
    WINDOW* win = newwin(SCREENHEIGHT, SCREENWIDTH, START_Y, START_X);  
    refresh();
    box(win, 0, 0);
    wrefresh(win);

    // menu functionality
    keypad(win, TRUE);
    mvprintw(0, 1, "created by React group");
    refresh();

    int c;
    int choice = 0;
    int highlighted = 1;

    print_menu(win, highlighted);
    /* untuk pilihan atas bawah
    kalau melebihi dari pilihan yang ada 
    akan kembali ke paling bawah 
    selagi paling atas akan berkurang*/ 
    while(1) {
        c = wgetch(win);
        switch(c) {
            case KEY_UP:
                if(highlighted == 1) {
                    highlighted = choices_count; // last
                } else {
                    --highlighted;
                }
            break;

            case KEY_DOWN:
                if(highlighted == choices_count) {
                    highlighted = 1;
                } else {
                    ++highlighted;
                }
            break;

            case 10: // enter
                choice = highlighted;
            break;
        }
        print_menu(win, highlighted);
        if(choice != 0) {
           break;
        }
    }

    endwin();

    return choice;
}

void print_menu(WINDOW *menu_win, int highlight) {
	int x, y, i;	

	x = y = 2;
	box(menu_win, 0, 0);
	for(i = 0; i < choices_count; ++i) {	
        if(highlight == i + 1) {
			wattron(menu_win, A_REVERSE); 
			mvwprintw(menu_win, y, x, "%s", menu_choices[i]);
			wattroff(menu_win, A_REVERSE);
		} else {
			mvwprintw(menu_win, y, x, "%s", menu_choices[i]);
        }
		++y;
	}
	wrefresh(menu_win);
}

void renderSinglePlayer() {

    for(int i = 0; i < typedLetterIndex; ++i) {
        if(typedLetter[i] == typeThis[i]) {
            wattron(win, A_BOLD);
            mvwprintw(win, 1, i + 1, "%c", typeThis[i]);
            wattroff(win, A_BOLD);
        } else {
            wattron(win, A_DIM);
            mvwprintw(win, 1, i + 1, "%c", typeThis[i]);
            wattroff(win, A_DIM);
        }
    }
    wclrtoeol(win);


    for(int i = typedLetterIndex; i < strlen(typeThis); ++i) {
        mvwprintw(win, 1, i + 1, "%c", typeThis[i]);
    }
    wclrtoeol(win);


    // print current typedLetter
    for(int i = 0; i < typedLetterIndex + 1; ++i) {
        mvwprintw(win, 5, 1 + i, "%c", typedLetter[i]);
    }
    wclrtoeol(win);

    mvwprintw(win, 12, 1, "%d %s", countCorrectLetterTyped, "Correct Letter");
    wclrtoeol(win);

    mvwprintw(win, 13, 1, "%d %s", countWrongLetterTyped, "Wrong Letter");
    wclrtoeol(win);

    wmove(win, 5, typedLetterIndex + 1);

    box(win, 0, 0);
    wrefresh(win);
}

void* print_info() {
    time(&current_time);

    timeSpent = current_time - starting_time;
    int totalLetter = countCorrectLetterTyped + countWrongLetterTyped;
    mvwprintw(win, 15, 1, "time spent: %d seconds     ", timeSpent);

    mvwprintw(win, 16, 1, "%.2f CPM     ", (float)totalLetter * 60/ timeSpent);

    mvwprintw(win, 17, 1, "%.2f%s     ", (float) countCorrectLetterTyped*100/totalLetter, "% accuracy" );

    wrefresh(win);
    wmove(win, 5, typedLetterIndex + 1);
    wrefresh(win);

    sleep(1);
    if(timeSpent < TIMELIMIT) {
        print_info();
    } else {
        pthread_cancel(input_thread);
        ch = '\0';
        pthread_exit(NULL);
    }
}

void* print_info_multiplayer() {
    
    time(&current_time);
    timeSpent = current_time - starting_time;
    // sent info to server

    // get info from server

    int totalLetter = countCorrectLetterTyped + countWrongLetterTyped;
    float yourCPM = (float)totalLetter * 60/ timeSpent;
    float yourAccuracy = (float) countCorrectLetterTyped*100/totalLetter;
    float yourScore = (float) yourCPM*yourAccuracy;
    mvwprintw(win, 15, 1, "time remaining: %d seconds", TIMELIMIT - timeSpent);
    wclrtoeol(win);

    mvwprintw(win, 18, 1, "YOUR DETAILS:");
    mvwprintw(win, 17, 1, "%.2f CPM with %.2f%s ", yourCPM, yourAccuracy, "% accuracy");
    wclrtoeol(win);
    mvwprintw(win, 18, 1, "Your SCORE: %.f", yourScore);
    wclrtoeol(win);

    float enemyCPM = 300;
    float enemyAccuracy = 98/100;
    float enemyScore = 10000;

    mvwprintw(win, 20, 1, "ENEMY DETAILS:");
    mvwprintw(win, 21, 1, "%.2f CPM with %.2f%s ", enemyCPM, enemyAccuracy, "% accuracy");
    wclrtoeol(win);

    mvwprintw(win, 22, 1, "Enemey SCORE: %.f", enemyScore);
    wclrtoeol(win);

    wmove(win, 5, typedLetterIndex + 1);
    wrefresh(win);

    sleep(1);
    if(timeSpent < TIMELIMIT) {
        print_info();
    } else {
        pthread_cancel(input_thread);
        pthread_exit(NULL);
    }

}

void* input() {
    ch = getch();
    // read(STDIN_FILENO, &ch, sizeof(char));
}

void singleplayer() {
    // menjalankan screen
    initscr();
    // clear();
    // refresh();
    //fungsi random
    time_t t;
    // membuat fungsi random
    srand((unsigned) time(&t));
    not_playing = false;
    // memulai waktu dan timer
    // waktu
    time(&starting_time);
    pthread_create(&time_thread, NULL, print_info, NULL);

    /* buat window , nampilkan windows, buat box dan menampilkan windows */
    win = newwin(SCREENHEIGHT, SCREENWIDTH, START_Y, START_X);  
    wrefresh(win);
    clear();
    refresh();

    wmove(win, 0, 0);
    wrefresh(win);
    box(win, 0, 0);
    wrefresh(win);
    /* saat memberi masukan jadi ketikan ilang, menampilkan manual */
    noecho();
    /* menyimpan yang diketik, buat kosongkan stringnya */
    // generate
    typedLetterIndex = 0;
    for(int i = 0; i < 255; ++i) {
        typedLetter[i] = '\0';
    }
    /* buat teks baru simpan di typethis */
    makeNewText(typeThis);
    /* inisialisasi */
    countCorrectLetterTyped = 0;
    countWrongLetterTyped = 0;
    /* merender */
    renderSinglePlayer();
    /* buat time spent time limitnya 60 */
    while(timeSpent < TIMELIMIT) {
        /* buat input lalu nunggu selesai input,  */
        pthread_create(&input_thread, NULL, input, NULL);
        pthread_join(input_thread, NULL);
        switch(ch) {
            case 127 : //delete
                if(typedLetterIndex <= 0) break; // tidak nge delete
                // yang di ketik sebelumnya benar
                if(typedLetter[typedLetterIndex - 1] == typeThis[typedLetterIndex - 1]) {
                    countCorrectLetterTyped--; // betul kurang
                }
                // salah berkurang
                else {
                    countWrongLetterTyped--;
                }
                typedLetterIndex -= 1;
                /* yang diketik berkurang satu nah kosong lalu dibreak */
                typedLetter[typedLetterIndex] = '\0';
                break;
            // 
            case 10 : //enter
                if(typedLetterIndex != strlen(typeThis)) break; // ketika kata yang muncul sudah diketikkan
                // lalu, ambil data baru
                makeNewText(typeThis); //buat kata-kata yang baru
                // semua di kosong
                for(int i = 0; i < 200; ++i) {
                    typedLetter[i] = '\0'; // kosongin variabel
                }
                // ulangi dari awal
                typedLetterIndex = 0;
                break;
            // kasus biasa refresh semua typed letter, lalu jalanin pita baru
            // 
            default : 
            // cuma pgn ada enter
                if(typedLetterIndex == strlen(typeThis)) break; 
                if(ch == typeThis[typedLetterIndex]) {
                    countCorrectLetterTyped++;
                    // jika char sesuai dg tampilan jumlah kata benar nambah
                } else {
                    countWrongLetterTyped++;
                }
                // char yang di ketik disimpan ke var ch untuk apa
                typedLetter[typedLetterIndex] = ch; // yang di ketik jadi ch
                typedLetterIndex += 1; // index bertambah
            
        }
        // render lagi
        renderSinglePlayer();
    }
    
    not_playing = true;
    pthread_join(time_thread, NULL);

    wclear(win);
    wrefresh(win);

    float CPM = (float)(countWrongLetterTyped + countCorrectLetterTyped) * 60/ timeSpent;
    float Accuracy = (float) countCorrectLetterTyped*100/(countWrongLetterTyped + countCorrectLetterTyped);
    mvwprintw(win, 6, 10, "%.2f CPM", CPM );
    mvwprintw(win, 7, 10, "%.2f%s", Accuracy , "% accuracy" );
    mvwprintw(win, 8, 10, "final score: %.f", CPM*Accuracy);

    mvwprintw(win, 10, 10, "Done!, press enter to continue");
    wrefresh(win);

    timeSpent = 0;

    do {
        ch = getch();
    } while(ch != 10);
    endwin();

    main();
}

int main() {
    init();
    int game = menu();

    switch(game) {
        case 1 :
            singleplayer();
        break;

        case 2 : 
            return 0;
    }
    return 0;
}
