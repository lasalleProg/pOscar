#include <stdio.h>
#include <string.h>

#define MAX_TXT 80
#define MAX_ACTORS 100
#define MAX_MOVIES 100

typedef struct{
    int id;
    char name[MAX_TXT];
}Actor;

typedef struct{
    char title[MAX_TXT];
    int year;
    char director[MAX_TXT];
    int actors_ids[MAX_ACTORS];
    int nb_actors;
}Movie;

int addActors(Actor actors[MAX_ACTORS]) {
    int count = 0;
    char choice;

    do {
        printf("Enter actor ID: ");
        scanf("%d", &actors[count].id);
        getchar(); 
        printf("Enter actor name: ");
        fgets(actors[count].name, MAX_TXT, stdin);
        actors[count].name[strcspn(actors[count].name, "\n")] = 0; // elimina el salto de línea

        count++;

        if (count >= MAX_ACTORS) {
            printf("Maximum number of actors reached.\n");
            break;
        }

        printf("Add another actor? (y/n): ");
        scanf(" %c", &choice);
    } while (choice == 'y' || choice == 'Y');

    return count;
}

int addMovies(Movie movies[MAX_MOVIES]) {
    int count = 0;
    char choice_movie;
    char choice_actor;

    do {
        printf("Enter movie title: ");
        getchar(); // limpia buffer
        fgets(movies[count].title, MAX_TXT, stdin);
        movies[count].title[strcspn(movies[count].title, "\n")] = 0;

        printf("Enter release year: ");
        scanf("%d", &movies[count].year);

        getchar(); // limpia buffer
        printf("Enter director name: ");
        fgets(movies[count].director, MAX_TXT, stdin);
        movies[count].director[strcspn(movies[count].director, "\n")] = 0;

        movies[count].nb_actors = 0;

        do {
            if (movies[count].nb_actors >= MAX_ACTORS) {
                printf("Maximum number of actors reached for this movie.\n");
                break;
            }

            printf("Enter actor ID (numeric): ");
            scanf("%d", &movies[count].actors_ids[movies[count].nb_actors]);
            movies[count].nb_actors++;

            printf("Add another actor to this movie? (y/n): ");
            scanf(" %c", &choice_actor);
        } while (choice_actor == 'y' || choice_actor == 'Y');

        count++;

        if (count >= MAX_MOVIES) {
            printf("Maximum number of movies reached.\n");
            break;
        }

        printf("Add another movie? (y/n): ");
        scanf(" %c", &choice_movie);
    } while (choice_movie == 'y' || choice_movie == 'Y');

    return count;
}

int getIdByName(Actor actors[], int num_actors, char name[]){
    for (int i = 0; i < num_actors; i++) {
        if (strcmp(actors[i].name, name) == 0) {
            return actors[i].id;
        }
    }
    return -1;
}

void getNameById(Actor actors[], int num_actors, char name[], int id){
    for (int i = 0; i < num_actors; i++) {
        if (actors[i].id == id) {
            strcpy(name, actors[i].name);
            return;
        }
    }
    strcpy(name, "Unknown");
}

void showMoviesByActor(Movie movies[], int num_movies, Actor actors[], int num_actors) {
    char actor_name[MAX_TXT];
    int actor_id;

    printf("Enter actor name: ");
    getchar();
    fgets(actor_name, MAX_TXT, stdin);
    actor_name[strcspn(actor_name, "\n")] = 0;

    actor_id = getIdByName(actors, num_actors, actor_name);
    if (actor_id == -1) {
        printf("Actor not found.\n");
        return;
    }

    printf("Movies with actor %s:\n", actor_name);
    for (int i = 0; i < num_movies; i++) {
        for (int j = 0; j < movies[i].nb_actors; j++) {
            if (movies[i].actors_ids[j] == actor_id) {
                printf("- %s (%d)\n", movies[i].title, movies[i].year);
                j = movies[i].nb_actors; // emula el break
            }
        }
    }
}

void reportActorsByDirector(Movie movies[], int num_movies, Actor actors[], int num_actors) {
    char director[MAX_TXT];
    FILE *f = fopen("reportABD.txt", "w");

    if (!f) {
        printf("Could not open file.\n");
        return;
    }

    printf("Enter director name: ");
    getchar();
    fgets(director, MAX_TXT, stdin);
    director[strcspn(director, "\n")] = 0;

    int used_ids[MAX_ACTORS] = {0};

    for (int i = 0; i < num_movies; i++) {
        if (strcmp(movies[i].director, director) == 0) {
            for (int j = 0; j < movies[i].nb_actors; j++) {
                int id = movies[i].actors_ids[j];
                if (!used_ids[id]) {
                    char name[MAX_TXT];
                    getNameById(actors, num_actors, name, id);
                    fprintf(f, "%s\n", name);
                    used_ids[id] = 1;
                }
            }
        }
    }

    fclose(f);
    printf("Report saved to reportABD.txt\n");
}

int deleteMovieByYear(Movie movies[], int num_movies) {
    int year;
    printf("Enter year of movies to delete: ");
    scanf("%d", &year);

    int new_count = 0;
    for (int i = 0; i < num_movies; i++) {
        if (movies[i].year != year) {
            movies[new_count++] = movies[i];
        }
    }

    printf("Movies from year %d deleted. Remaining movies: %d\n", year, new_count);
    return new_count;
}

void reportCurrentMovies(Movie movies[], int num_movies) {
    // TODO: Se ha de crear un fichero de binario llamado "reportCM.bin" con las pelñiculas que solo pertenezcan al año actual
}

int main() {
    Actor actors[MAX_ACTORS];
    Movie movies[MAX_MOVIES];
    int num_actors = 0, num_movies = 0;
    int option=0;

    do {
        printf("---- MENU ----\n");
        printf("1. Enter actors \n");
        printf("2. Enter movies \n");
        printf("3. Show movies by actor \n");
        printf("4. Report actors who have worked with x director... \n");
        printf("5. Delete movies by year \n");
        printf("6. Report current movies... \n");
        printf("7. Exit \n");
        printf("Enter option: ");
        scanf("%d", &option);

        switch (option) {
            case 1:
                num_actors = addActors(actors);
                break;
            case 2:
                num_movies = addMovies(movies);
                break;
            case 3:
                showMoviesByActor(movies, num_movies, actors, num_actors);
                break;
            case 4:
                reportActorsByDirector(movies, num_movies, actors, num_actors);
                break;
            case 5:
                num_movies = deleteMovieByYear(movies, num_movies);
                break;
            case 6:
                reportCurrentMovies(movies, num_movies);
                break;
            case 7:
                printf("Program closing...\n");
                break;
        }
        printf("\n");
    }while(option!=7);

    return 0;
}
