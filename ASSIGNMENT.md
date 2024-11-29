> Copied from the [submission page](https://online.saddleback.edu/courses/21562/assignments/706060).

# Lab #12 (Hash Table)

Netflix Movie Tracker and Recommendation:

You are tasked with designing a comprehensive movie management system for a
Netflix-like platform. The program should manage user accounts, track watched
and favorite movies, recommend new movies based on user preferences, and allow
users to rate movies. Additionally, the movie database should be persistent
across sessions, stored in a text file.

## Features and Requirements

### User Authentication

1.  Registration:
    * Users must register to use the platform.
    * Registration required

2.  Login:
    * Registered users must log in using their ID.
    * The ID is validated against the registered user database.
    * If the ID is not found, the user is prompted to register.

### Main Menu

Once authenticated, users can access the main menu with the following options:

1. Mark a Movie as Watched:
    * Users can mark any movie from the database as "watched."

2. Add a Movie to Favorites:
    * Users can add any movie from the database to their favorites list.
    * Movies in the favorites list will show whether they have been watched.

3. Search for a Movie:
    * Users can search for a specific movie by its title.
    * If found, users can optionally add it to their favorites.

4. Display Favorite Movies:
    * Display all favorite movies with their watched status.

5. Rate a Movie:
    * Users can rate movies as:
        * Thumbs Down
        * One Thumbs Up
        * Two Thumbs Up
    * When movies rated "Two Thumbs Up”, other movies in the same category are
      randomly added to the recommendation list.

6. View Recommendations:
    * Recommendations are based on:
        * Randomly add movies from the category most frequently watched
          by the user.
        * Movies rated "Two Thumbs Up" by the user.

### Movie Database

The program uses a persistent text file (movie_database.txt) containing 500
movies equally distributed across five categories:

* Action
* Comedy
* Drama
* Sci-Fi
* Horror

### Constraints

1. Only the vector STL is allowed for data storage.

2. The hash table must be implemented manually (no STL unordered_map).

3. Movie titles are unique.

4. Recommendations are generated dynamically and stored in the recommendations list.

### Persistence

User data (registration, watched movies, favorites, ratings, recommendations)
is saved to a text file and loaded at the start of the program.

### Input and Output

Input:
* Commands from the user to interact with the system (e.g., "mark watched,"
  "add to favorites," etc.).
* Movie titles and user ratings.

Output:
* Feedback for each user action (e.g., "Movie marked as watched").
* Display of favorite movies and recommendations.


## Hint use the following modules:

1. Authentication Module:
    * Manages user registration and login.
    * Validates IDs against a database of ID’s from the text file.

2. Movie Tracker Module:
    * Tracks watched and favorite movies using a custom hash table.
    * Searches for movies in the database.

3. Recommendation Module:
    * Generates recommendations based on user preferences.
    * Dynamically updates recommendations based on user ratings.

4. File I/O Module:
    * Handles reading from and writing to persistent text files.

5. Hash Table Implementation:
    * Implements hashing functionality for efficient movie lookup.
