#include <iostream>
#include <fstream>
#include <sstream>
#include <iomanip>
#include <vector>
#include <string>
#include <cstring>
#include <map>
#include <memory>
#include <ctime>
#include <cmath>
#include <algorithm>
#include "classes.hpp"

// Court class methods

Court::Court(const int court_number) : court_number(court_number) {}

int Court::getCourtNumber() {
    return court_number;
}

void Court::setCourtNumber(const int num) {
    court_number = num;
}

// User class methods

string User::getUsername() const {
    return username;
}

USER_TYPE User::getUserType() const {
    return user_type;
}

string User:: getPassword() const {
    return password;
}

void User::setUsername(string username) {
    this->username = username;
}

void User::setUsertype(USER_TYPE userType) {
    this->user_type = userType;
}

void User::setPassword(string password) {
    this->password = password;
}

void User::viewReservations(ReservationSystem* r) {
    cout << "Reservations: reservationID, court, time" << endl;
    for (auto res : r->getReservations()) {
        for (auto uid : res->getUserIds()) {
            if (uid == this->getUsername()) {
                time_t reservationTime = res->getReservationTime();
                cout << "             " << res->getReservationId() << ", " << res->getCourtNumber() << ", " << ctime(&reservationTime) << endl;
            }
        }
    }
}

User::User(const string &username, const USER_TYPE &user_type, const string &password) : username(username), user_type(user_type), password(password) {}

// Member class methods

Member::Member(const string &username, const string &password, char skill_level) : User(username, MEMBER, password), skill_level(skill_level) {}

char Member::getSkillLevel() const {
    return skill_level;
}

void Member::requestReservationModification(int reservation_id, ModificationType mtype, time_t time, ReservationSystem* r) {
    Modification mod = Modification(reservation_id, mtype, time);
    r->addModification(mod);
}

void Member::reserveCourt(int court_number, time_t reservation_time, ReservationSystem r) {
    r.addReservation(this, court_number, reservation_time);
}

void Member::joinReservation(int reservation_id, ReservationSystem* r) {
    for (auto x : r->getReservations()) {
        if (x->getReservationId() == reservation_id) {
            if (x->getUserIds().size() > 1) {
                cout << "Maximum players exceeded" << endl;
            }
            else {
                x->addPlayer(this->getUsername());
            }
            
        }
    }
}

// Coach class methods

Coach::Coach(const string &username, const string &password) : User(username, COACH, password) {}

void Coach::requestReservationModification(int reservation_id, ModificationType mtype, time_t time, ReservationSystem r) {
    Modification mod = Modification(reservation_id, mtype, time);
    r.addModification(mod);
}

void Coach::reserveCourt(int court_number, time_t reservation_time, ReservationSystem r) {
    r.addReservation(this, court_number, reservation_time);
}

void Coach::joinReservation(int reservation_id, ReservationSystem* r) {
    for (auto x : r->getReservations()) {
        if (x->getReservationId() == reservation_id) {
            x->addPlayer(this->getUsername());
        }
    }
}
// Officer class methods

Officer::Officer(const string &username, const string &password) : User(username, OFFICER, password) {}

void Officer::modifyReservation(Modification mod, ReservationSystem r) {
    for (auto x : r.getReservations()) {
        if (x->getReservationId() == mod.getReservationId()) {
            if (mod.getModificationType() == CANCEL) {
                r.cancelReservation(mod.getReservationId());
            }
            else {
                r.modifyReservation(this, mod.getReservationId(), mod.getTime());
            }
        }
    }
}

void Officer::reserveCourt(int court_number, time_t reservation_time, ReservationSystem r) {
    r.addReservation(this, court_number, reservation_time);
}

void Officer::reserveCourtOpenPlay(int court_number, time_t reservation_time, ReservationSystem r) {
    r.reserveOpenPlay(this, court_number, reservation_time);
}

void Officer::joinReservation(int reservation_id, ReservationSystem* r) {
    for (auto x : r->getReservations()) {
        if (x->getReservationId() == reservation_id) {
            x->addPlayer(this->getUsername());
        }
    }
}

// Reservation class methods


Reservation::Reservation(int reservation_id, int court_number, const string& username, time_t reservation_time, bool open_play)
         : reservation_id(reservation_id), court_number(court_number), username(username), reservation_time(reservation_time), open_play(open_play) {
           users.push_back(username);
         }

int Reservation::getReservationId() const {
    return reservation_id;
}

const string& Reservation::getUserId() const {
    return users.front();
}

const vector<string>& Reservation::getUserIds() const {
    return users;
}

int Reservation::getCourtNumber() const {
    return this->court_number;
}

int Reservation::getNumberOfPlayers() const {
    return users.size();
}

time_t Reservation::getReservationTime() const {
    return reservation_time;
}

void Reservation::addPlayer(const string &user_id) {
    users.push_back(user_id);
}

void Reservation::setTime(time_t time) {
    this->reservation_time = time;
}

bool Reservation::isOpenPlay() {
    return open_play;
}

// ReservationSystem class methods

    // default reservation constructor
ReservationSystem::ReservationSystem() {
    // Initialize courts
    for (int i = 1; i <= 3; i++) {
        courts.emplace_back(i);
    }

    // Initialize pointers to nullptr
    reservations.clear();
    users.clear();

    // Initialize modifications to an empty vector
    modifications.clear();

    // Initialize coaching hours
    struct tm coaching_start = {};
    struct tm coaching_end = {};

    coaching_start.tm_hour = 13; // 13:00
    coaching_start.tm_min = 0;
    coaching_end.tm_hour = 15; // 15:00
    coaching_end.tm_min = 0;

    coaching_hours[0] = mktime(&coaching_start);
    coaching_hours[1] = mktime(&coaching_end);

    // Initialize open play hours
    struct tm open_play_start = {}; // Set the desired start time for open play
    struct tm open_play_end = {};   // Set the desired end time for open play

    open_play_start.tm_hour = 22; // Example: 0:00
    open_play_start.tm_min = 0;
    open_play_end.tm_hour = 23;  // Example: 23:59
    open_play_end.tm_min = 59;

    open_play[0] = mktime(&open_play_start);
    open_play[1] = mktime(&open_play_end);
}

void ReservationSystem::enforceReservationRules() {
    // Implement enforceReservationRules
}

void ReservationSystem::viewDailySchedule(time_t date) {
    // Set up table header
    cout << "                                      Court1                         Court2                         Court3" << endl;

    struct tm *date_info = localtime(&date);
    date_info->tm_hour = 6;
    date_info->tm_min = 0;
    date_info->tm_sec = 0;

    for (int i = 1; i <= 36; ++i) {
        time_t slot_start = mktime(date_info);
        date_info->tm_min += 30;
        time_t slot_end = mktime(date_info);

        char time_buffer[20];
        strftime(time_buffer, sizeof(time_buffer), "%H:%M", localtime(&slot_start));
        cout << "Timeslot " << i << " - " << time_buffer << " - ";
        strftime(time_buffer, sizeof(time_buffer), "%H:%M", localtime(&slot_end));
        cout << time_buffer;
        
        for (int court = 1; court <= 3; ++court) {
            if (!(court_availability(court, slot_start))) {
                Reservation* res = this->getReservation(court, slot_start);
                for (auto u : this->users) {
                    for (auto i : res->getUserIds()) {
                        if (u->getUsername() == i) {
                            cout << " ";
                            cout << "           " << u->getUsername();

                        }
                    }
                }
            }
            else {
                cout << "              ";
            }
        }
        cout << endl;
    }
}

Reservation* ReservationSystem::getReservation(int court_number, time_t reservation_time) {
    for (Reservation* res : reservations) {
        if (res->getCourtNumber() == court_number && res->getReservationTime() == reservation_time) {
            return res;
        }
    }
    return nullptr; // If no reservation is found, return nullptr
}

bool ReservationSystem::isLessThanTwoDaysApart(time_t time1, time_t time2) {
    const double seconds_in_a_day = 60 * 60 * 24;
    double time_difference_seconds = abs(difftime(time1, time2));

    return time_difference_seconds < (2 * seconds_in_a_day);
}

bool ReservationSystem::isLessThanSevenDaysApart(time_t time1, time_t time2) {
    const double seconds_in_a_day = 60 * 60 * 24;
    double time_difference_seconds = abs(difftime(time1, time2));

    return time_difference_seconds < (7 * seconds_in_a_day);
}

void ReservationSystem::addReservation(User* user, int court_number, time_t reservation_time) {
    int res_id = 0;
    int courtnum = 0;
    cout << "Inputted Court Number: " << court_number << endl;
    time_t current_time = time(nullptr);
    if (!(court_availability(court_number, reservation_time))) {
        cout << "Court is not availible at this time" << endl;
    }
    else {
        if (user->getUserType() == MEMBER) {
            if ((this->getNumOfReservations(user->getUsername(), reservation_time, true) < 3) 
            && (this->getNumOfReservations(user->getUsername(), reservation_time, false) < 2) 
            && (isLessThanTwoDaysApart(reservation_time, current_time))) {
                Reservation* new_res = new Reservation(createReservationID(), court_number, user->getUsername(), reservation_time, false);
                this->reservations.push_back(new_res);
                res_id = new_res->getReservationId();
                courtnum = new_res->getCourtNumber();
            }
            else if (this->withinCoachingHours(reservation_time)) {
                cout << "Members cannot reserve a court during coaching hours" << endl;
            }
            else {
                cout << "User has exceeded reservation limit" << endl;
            }
        }
        else if (user->getUserType() == COACH) {
            if (this->withinCoachingHours(reservation_time) && !(this->isLessThanTwoDaysApart(current_time, reservation_time)) && this->isLessThanSevenDaysApart(current_time, reservation_time)) {
                Reservation* new_res = new Reservation(createReservationID(), court_number, user->getUsername(), reservation_time, false);
                this->reservations.push_back(new_res);
                res_id = new_res->getReservationId();
                courtnum = new_res->getCourtNumber();
            }
            else {
                cout << "Reservation could not be made. Please check that reservation requirements are being followed" << endl;
            }
        }
        else if (user->getUserType() == OFFICER) {
            if (!(this->isLessThanSevenDaysApart(reservation_time, current_time))) {
                cout << "Reservations cannot be made more than 7 days in advance" << endl;
            }
            else {
                Reservation* new_res = new Reservation(createReservationID(), court_number, user->getUsername(), reservation_time, false);
                this->reservations.push_back(new_res);
                res_id = new_res->getReservationId();
                courtnum = new_res->getCourtNumber();
            }
        }
        else {
            cout << "User type not defined" << endl;
        }
    }
    if (res_id != 0) {
        cout << "Reservation successfully added: " << res_id << ", " << courtnum << endl;
    }
}

void ReservationSystem::reserveOpenPlay(User* user, int court_number, time_t reservation_time) {
    if (!(this->court_availability(court_number, reservation_time))) {
        cout << "Court is not availible at this time" << endl;
    }
    else if (!(this->withinOpenPlayHours(reservation_time))){
        cout << "Reservation is not within Open Play time" << endl;
    }
    else {
        Reservation* new_res = new Reservation(createReservationID(), court_number, user->getUsername(), reservation_time, true);
        reservations.push_back(new_res);
    }
}

void ReservationSystem::cancelReservation(int reservation_id) {
    for (auto it = reservations.begin(); it != reservations.end(); ++it) {
        if ((*it)->getReservationId() == reservation_id) {
            reservations.erase(it);
            return;
        }
    }
    cout << "Reservation not found." << endl;
}

void ReservationSystem::modifyReservation(Officer* officer, int reservation_id, time_t time) {
    for (auto it = reservations.begin(); it != reservations.end(); ++it) {
        if ((*it)->getReservationId() == reservation_id) {
            (*it)->setTime(time);
            return;
        }
    }
    cout << "Reservation not found." << endl;
}

vector<User*> ReservationSystem::getUsers() {
    return this->users;
}


void ReservationSystem::addUser(User* user) {
    this->users.push_back(user);
}

// set reservations
 void ReservationSystem::setReservation(vector<Reservation*> res) {
    this->reservations = res;
 }


void ReservationSystem::addMultipleUsers(vector<User*> users) {
    for (auto u : users) {
        this->addUser(u);
    }
}


vector<Reservation*> ReservationSystem::getReservations() {
    return this->reservations;
}


vector<Modification> ReservationSystem::getModifications() {
    return this->modifications;
}

void ReservationSystem::addModification(Modification mod) {
    this->modifications.push_back(mod);
}

void ReservationSystem::viewModifications() {
    int i = 1;
    if (this->modifications.size() < 1) {
        cout << "No Modifications. Press 0 to Exit." << endl;
    }
    else {
       for (auto m : this->modifications) {
            string mType;
            if (m.getModificationType() == CHANGE) {
                mType = "Change";
            }
            else {
                mType = "Cancel";
            }
            time_t temp_time = m.getTime();
            cout << i << ". ReservationID: " << m.getReservationId() << ", Modification: " << mType << ", New Time: " << ctime(&temp_time) << endl; 
        }
        cout << endl; 
    }
    
}

bool ReservationSystem::any_availibility(time_t time) {
        int courts_available = courts.size();
        for (auto c : courts) {
            if (this->court_availability(c.getCourtNumber(), time)) {
                return true;
            }
            return false;
        }
    }

bool ReservationSystem::court_availability(int court_number, time_t time) {
    for (auto r : reservations) {
        if (r->getReservationTime() == time && r->getCourtNumber() == court_number) {
            return false;
        }
    }
    return true;
}

int ReservationSystem::getNumOfReservations(string user_id, time_t time, bool week) {
    int count = 0;
    time_t timeRange[2];
    struct tm timeinfo;
    localtime_r(&time, &timeinfo); // Convert time_t to struct tm
    if (week) {
        // Calculate the start and end times of the week
        timeinfo.tm_wday = 0; // Set the day of the week to Sunday
        timeRange[0] = mktime(&timeinfo); // Start of the week
        timeinfo.tm_wday = 6; // Set the day of the week to Saturday
        timeinfo.tm_hour = 23;
        timeinfo.tm_min = 59;
        timeinfo.tm_sec = 59;
        timeRange[1] = mktime(&timeinfo); // End of the week
    }
    else {
        // Calculate the start and end times of the day
        timeinfo.tm_hour = 0;
        timeinfo.tm_min = 0;
        timeinfo.tm_sec = 0;
        timeRange[0] = mktime(&timeinfo); // Start of the day
        timeinfo.tm_hour = 23;
        timeinfo.tm_min = 59;
        timeinfo.tm_sec = 59;
        timeRange[1] = mktime(&timeinfo); // End of the day
    }
    for (auto r : reservations) {
        for (auto u : r->getUserIds()) {
            if (u == user_id) {
                if (time >= timeRange[0] && time <= timeRange[1]) 
                {
                    count++;
                }
            }
        }
    }
    return count;
}

int ReservationSystem::createReservationID() {
    int reservation_id;
    bool unique;
    do {
        unique = true;
        reservation_id = rand() % 900000 + 100000; // Generate a random 6-digit integer
        for (const auto& r : reservations) {
            if (r->getReservationId() == reservation_id) {
                unique = false;
                break;
            }
        }
    } while (!unique);
    return reservation_id;
}

void ReservationSystem::setCoachingHours(time_t start, time_t end) {
    coaching_hours[0] = start;
    coaching_hours[1] = end;
}

bool ReservationSystem::withinCoachingHours(time_t time) {
    struct tm timeinfo;
    localtime_r(&time, &timeinfo); // Convert time_t to struct tm

    struct tm start_tm = *localtime_r(&coaching_hours[0], &timeinfo);
    struct tm end_tm = *localtime_r(&coaching_hours[1], &timeinfo);

    // Normalize date info
    timeinfo.tm_year = start_tm.tm_year;
    timeinfo.tm_mon = start_tm.tm_mon;
    timeinfo.tm_mday = start_tm.tm_mday;
    time_t normalized_time = mktime(&timeinfo);

    return normalized_time >= coaching_hours[0] && normalized_time <= coaching_hours[1];
}


void ReservationSystem::setOpenPlayHours(time_t start, time_t end) {
    open_play[0] = start;
    open_play[1] = end;
}

bool ReservationSystem::withinOpenPlayHours(time_t time) {
    struct tm timeinfo;
    localtime_r(&time, &timeinfo); // Convert time_t to struct tm

    struct tm start_tm = *localtime_r(&open_play[0], &timeinfo);
    struct tm end_tm = *localtime_r(&open_play[1], &timeinfo);

    // Normalize date info
    timeinfo.tm_year = start_tm.tm_year;
    timeinfo.tm_mon = start_tm.tm_mon;
    timeinfo.tm_mday = start_tm.tm_mday;
    time_t normalized_time = mktime(&timeinfo);

    return normalized_time >= open_play[0] && normalized_time <= open_play[1];
}

//Modification Class
Modification::Modification(int reservation_id, ModificationType mod, time_t time) {
    this->reservation_id = reservation_id;
    this->mod = mod;
    this->time = time;
}

int Modification::getReservationId() {
    return this->reservation_id;
}

ModificationType Modification::getModificationType() {
    return this->mod;
}

time_t Modification::getTime() {
    return this->time;
}


// FileHandler class methods
//

// Constructor, initializes the file handler with given file paths for users and reservations
FileHandler::FileHandler(const string &users_file_path, const string &reservations_file_path)
    : users_file_path(users_file_path), reservations_file_path(reservations_file_path) {}

// Reads the user data from the file and returns a vector of pointers to User objects
vector<User*> FileHandler::readUsersFromFile() {
    vector<User*> users;
    ifstream file(users_file_path);

    if (file.is_open()) {
        string line;
        while (getline(file, line)) {
            stringstream ss(line);
            string userType, username, password, skillLevel;

            getline(ss, userType, ',');
            getline(ss, username, ',');
            getline(ss, password, ',');
            getline(ss, skillLevel, ',');

            char slchar = skillLevel[0];

            if (userType == "MEMBER") {
              Member* new_user = new Member(username, password, slchar);
              new_user->setUsertype(MEMBER);
                users.push_back(new_user);
            } else if (userType == "COACH") {
                Coach* new_user = new Coach(username, password);
                new_user->setUsertype(COACH);
                users.push_back(new_user);
            } else if (userType == "OFFICER") {
                Officer* new_user = new Officer(username, password);
                new_user->setUsertype(OFFICER);
                users.push_back(new_user);
            } else {
                throw runtime_error("Invalid user type in the file");
            }
        }
        file.close();
    } else {
        throw runtime_error("Unable to open users file");
    }

    return users;
}

// Writes the user data to the file
void FileHandler::writeUsersToFile(const vector<User*> &users) {
    ofstream file(users_file_path);

    if (file.is_open()) {
        for (const auto &user : users) {
            string userType;
            if (dynamic_cast<Member *>(user) != nullptr) {
                userType = "MEMBER";
            } else if (dynamic_cast<Coach *>(user) != nullptr) {
                userType = "COACH";
            } else if (dynamic_cast<Officer *>(user) != nullptr) {
                userType = "OFFICER";
            } else {
                throw runtime_error("Invalid user type in the users list");
            }

            file << userType << "," << user->getUsername() << "," << user->getPassword();

            if (userType == "MEMBER") {
                file << "," << dynamic_cast<Member *>(user)->getSkillLevel();
            }

            file << endl;
        }
        file.close();
    } else {
        throw runtime_error("Unable to open users file");
    }
}



// Reads the reservation data from the file and returns a vector of pointers to Reservation objects
vector<Reservation*> FileHandler::readReservationsFromFile() {
    vector<Reservation*> reservations;
    ifstream file(reservations_file_path);

    // Check if the file is open
    if (file.is_open()) {
        string line;
        // Read each line in the file
        while (getline(file, line)) {
            stringstream ss(line);
            string reservationId, username, timeStr, additionalPlayer, openplay, court_number;
            bool open_play;
            string line = "true";

            // Extract reservation data from the line
            getline(ss, reservationId, ',');
            getline(ss, court_number, ',');
            getline(ss, username, ',');
            getline(ss, openplay, ',');
            getline(ss, timeStr, ',');
            getline(ss, additionalPlayer, ',');

            open_play = (openplay == "true");

            // Convert time string to struct tm
            struct tm time = {};
            const char *timeStr_cstr = timeStr.c_str();
            char *ret = strptime(timeStr_cstr, "%Y-%m-%d %H:%M", &time);


            // Create a reservation object and add to the reservations vector
            Reservation* new_res = new Reservation(stoi(reservationId), stoi(court_number), username, mktime(&time), open_play);
            if (!additionalPlayer.empty()) {
                (*new_res).addPlayer(additionalPlayer);
            }

            // Add the reservation to the vector
            reservations.push_back(new_res);
        }
        file.close();
    } else {
        throw runtime_error("Unable to open reservations file");
    }

    return reservations;
}
// Writes the reservation data to the file
void FileHandler::writeReservationsToFile(const vector<Reservation*> &reservations) {
    ofstream file(reservations_file_path);

    // Check if the file is open
    if (file.is_open()) {
        for (const auto &reservation : reservations) {
            // Write reservation ID, court number, and user ID
            file << reservation->getReservationId() << "," << reservation->getCourtNumber() << "," << reservation->getUserId() << ",";

            // Write open_play value
            file << (reservation->isOpenPlay() ? "true" : "false") << ",";

            // Write reservation time
            time_t reservation_time = reservation->getReservationTime();
            struct tm *time_info = localtime(&reservation_time);
            char time_buffer[20];
            strftime(time_buffer, sizeof(time_buffer), "%Y-%m-%d %H:%M", time_info);
            file << time_buffer << ",";

            // Write additional player (if any)
            if (reservation->getNumberOfPlayers() > 1) {
                const auto &additionalPlayer = reservation->getUserIds()[1];
                file << additionalPlayer;
            }

            // Add a newline character to separate each reservation
            file << endl;
        }
        file.close();
    } else {
        throw runtime_error("Unable to open reservations file");
    }
}
