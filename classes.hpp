#include <iostream>
#include <sstream>
#include <iomanip>
#include <vector>
#include <string>
#include <cstring>
#include <map>
#include <memory>
#include <ctime>


using namespace std;

enum USER_TYPE {MEMBER, COACH, OFFICER};
enum ModificationType {CHANGE, CANCEL};

class ReservationSystem;
class Modification;

class Court {
public:
    // Constructor, initializes court with given court_number
    Court(int court_number);

    // Returns the court number
    int getCourtNumber();

    // setter
    void setCourtNumber(const int num);

private:
    int court_number;
};

class User {
public:
    // Virtual destructor for the abstract class
    virtual ~User() = default;

    // Returns the username
    string getUsername() const;

    // Returns the user type
    USER_TYPE getUserType() const;

    // 
    string getPassword() const;

    // setters
    void setUsername(string usernamne);

    void setUsertype(USER_TYPE usertype);

    void setPassword(string password);

    void viewReservations(ReservationSystem* r);


    // Abstract method to reserve a court for the user
    virtual void reserveCourt(int court_number, time_t reservation_time, ReservationSystem r) = 0;

    // Abstract method to join an existing reservation
    virtual void joinReservation(int reservation_id, ReservationSystem* r) = 0;

protected:
    // Constructor, initializes user with given username and user_type
    User(const string &username, const USER_TYPE &user_type, const string &password);

private:
    string username;
    USER_TYPE user_type;
    string password;
};

class Member : public User {
public:
    // Constructor, initializes member with given username and skill_level
    Member(const string &username, const string &password, char skill_level);

    // Returns the skill level of the member
    char getSkillLevel() const;

    // Requests a reservation modification, such as canceling or rescheduling
    void requestReservationModification(int reservation_id, ModificationType mtype, time_t time, ReservationSystem* r);

    // Reserves a court for the member
    void reserveCourt(int court_number, time_t reservation_time, ReservationSystem r) override;

    // Joins an existing reservation
    void joinReservation(int reservation_id, ReservationSystem* r) override;

private:
    char skill_level;
};

class Coach : public User {
public:
    // Constructor, initializes coach with given username
    Coach(const string &username, const string &password);

    // Requests a reservation modification, such as canceling or rescheduling
    void requestReservationModification(int reservation_id, ModificationType mtype, time_t time, ReservationSystem r);

    // Reserves a court for the coach
    void reserveCourt(int court_number, time_t reservation_time, ReservationSystem r) override;

    // Joins an existing reservation
    void joinReservation(int reservation_id, ReservationSystem* r) override;
};

class Officer : public User {
public:
    // Constructor, initializes officer with given username
    Officer(const string &username, const string &password);

    // Modifies a reservation based on the action provided (e.g., cancel, reschedule)
    void modifyReservation(Modification mod, ReservationSystem r);

    // Reserves a court for the officer
    void reserveCourt(int court_number, time_t reservation_time, ReservationSystem r) override;

    // Reserves a court for open play
    void reserveCourtOpenPlay(int court_number, time_t reservation_time, ReservationSystem r);

    // Joins an existing reservation
    void joinReservation(int reservation_id, ReservationSystem* r) override;
};

class Reservation {
public:
    // Constructor, initializes reservation with reservation_id, user_id, skill_level, and reservation_time
    Reservation(int reservation_id, int court_number, const string& username, time_t reservation_time, bool open_play);

    // Returns the reservation ID
    int getReservationId() const;

    // Returns the user ID of the reservation holder
    const string& getUserId() const;

    //Returns all the user IDs on the reservation
    const vector<string>& getUserIds() const;

    const vector<User*> getUsers() const;

    int getCourtNumber() const;

    // Returns the skill level of the reservation holder
    char getSkillLevel() const;

    // Returns the number of players in the reservation
    int getNumberOfPlayers() const;

    // Returns the reservation time
    time_t getReservationTime() const;

    // Adds a player to the reservation
    void addPlayer(const string &user_id);

    //Sets the time of the reservation
    void setTime(time_t time);

    //Is the court open play
    bool isOpenPlay();

private:
    int reservation_id;
    string username;
    int court_number;
    vector<string> users;
    time_t reservation_time;
    bool open_play;
};

class ReservationSystem {
public:

    ReservationSystem();

    // Enforces reservation rules, such as maximum reservations per user
    void enforceReservationRules();

    // Displays the daily schedule for all courts for a specific date
    void viewDailySchedule(time_t date);

    Reservation* getReservation(int court_number, time_t reservation_time);

    bool isLessThanTwoDaysApart(time_t time1, time_t time2);

    bool isLessThanSevenDaysApart(time_t time1, time_t time2);

    // Allows users to reserve a court
    void addReservation(User* user, int court_number, time_t reservation_time);


    void reserveOpenPlay(User* user, int court_number, time_t reservation_time);

    // Allows users to cancel a reservation
    void cancelReservation(int reservation_id);

    // Allows officers to modify reservations
    void modifyReservation(Officer* officer, int reservation_id, time_t time);

    // Return list of all users
    vector<User*> getUsers();

    //adds a user to the system
    void addUser(User* user);

    //adds multiple users to the system
    void addMultipleUsers(vector<User*> users);

    // Return list of all reservations
    vector<Reservation*> getReservations();

    // Return modifications
    vector<Modification> getModifications();

    // Add a modification
    void addModification(Modification mod);

    void viewModifications();

    // set reservations
    void setReservation(vector<Reservation*> res);

    // Check if any court is available at a time
    bool any_availibility(time_t time);

    // Checks if a court is available at a time
    bool court_availability(int court_number, time_t time);

    int getNumOfReservations(string username, time_t time, bool week);

    int createReservationID();

    void setCoachingHours(time_t start, time_t end);

    bool withinCoachingHours(time_t time);

    void setOpenPlayHours(time_t start, time_t end);

    bool withinOpenPlayHours(time_t time);

private:
    vector<Court> courts;
    vector<Reservation*> reservations;
    vector<User*> users;
    vector<Modification> modifications;
    time_t coaching_hours[2];
    time_t open_play[2];
};

class Modification {
public:
    Modification(int reservation_id, ModificationType mod, time_t time);
    int getReservationId();
    ModificationType getModificationType();
    time_t getTime();

private:
    int reservation_id;
    ModificationType mod;
    time_t time;
};

class FileHandler {
public:
    // Constructor, initializes the file handler with given file paths for users and reservations
    FileHandler(const string &users_file_path, const string &reservations_file_path);

    // Reads the user data from the file and returns a vector of pointers to User objects
    vector<User*> readUsersFromFile();

    // Writes the user data to the file
    void writeUsersToFile(const vector<User*> &users);

    // Reads the reservation data from the file and returns a vector of pointers to Reservation objects
    vector<Reservation*> readReservationsFromFile();

    // Writes the reservation data to the file
    void writeReservationsToFile(const vector<Reservation*> &reservations);
private:
    private:
    string users_file_path;
    string reservations_file_path;
};
