#include <iostream>
#include <string>
#include <iomanip>
#include <ctime>
#include <vector>
#include <sstream>
#include <limits>
#include "classes.hpp"

//main.cpp

using namespace std;

time_t convertStringToTimeT(const string& date, const string& time) {
    istringstream date_ss(date);
    istringstream time_ss(time);
    struct tm timeStruct = {0};
    char delimiter;

    date_ss >> timeStruct.tm_mon >> delimiter
            >> timeStruct.tm_mday >> delimiter
            >> timeStruct.tm_year;

    time_ss >> timeStruct.tm_hour >> delimiter
            >> timeStruct.tm_min;

    timeStruct.tm_mon -= 1; // Months are in the range [0, 11]
    timeStruct.tm_year -= 1900; // Years are counted from 1900
    timeStruct.tm_hour--;

    return mktime(&timeStruct);
}

int main() {

    time_t current_time = time(nullptr);

    string users_file_path = "users.txt";
    string reservations_file_path = "reservations.txt";
    FileHandler file_handler(users_file_path, reservations_file_path);
    ReservationSystem* rs = new ReservationSystem();

    cout << "Welcome to the Pickleball Court Reservation System!" << endl;

    bool exit_program = false;

    while (!exit_program) { // Loop for login and logout


    string has_account;
    cout << "Do you have an account? (y/n): ";
    cin >> has_account;

    string user_type, username, password;
    char skill_level = '-';

    if (has_account == "n") {
        vector<User*> users = file_handler.readUsersFromFile();

        cout << "Please enter your user type (MEMBER/COACH/OFFICER): ";
        cin >> user_type;

        bool username_exists;
        do {
            cout << "Please enter your desired username: ";
            cin >> username;

            username_exists = false;
            for (const User* user : users) {
                if (user->getUsername() == username) {
                    username_exists = true;
                    cout << "Username already exists. Please choose a different username." << endl;
                    break;
                }
            }
        } while (username_exists);

        cout << "Please enter your desired password: ";
        cin >> password;

        if (user_type == "MEMBER") {
            cout << "Please enter your skill level (A/B/C/D): ";
            cin >> skill_level;
        }

        // Create a new user object based on the user type
        User* new_user = nullptr;
        if (user_type == "MEMBER") {
            new_user = new Member(username, password, skill_level);
            new_user->setUsertype(MEMBER);
        } else if (user_type == "COACH") {
            new_user = new Coach(username, password);
            new_user->setUsertype(COACH);
        } else if (user_type == "OFFICER") {
            new_user = new Officer(username, password);
            new_user->setUsertype(OFFICER);
        }

        // Save the new user to the users file
        users.push_back(new_user);
        file_handler.writeUsersToFile(users);

        cout << "Account created successfully. Please login with your new credentials." << endl;
    }

    cout << "Please enter your username: ";
    cin >> username;

    cout << "Please enter your password: ";
    cin >> password;
  User* current_user = nullptr;

    // Authenticate the user by checking the file
    auto users = file_handler.readUsersFromFile();
    bool found = false;
    for (const auto &user : users) {
        if (user->getUsername() == username && user->getPassword() == password) {
            found = true;
          current_user = user;
            break;
        }
    }

    if (found) {
        cout << "Login successful!" << endl;
    } else {
        cout << "Invalid username or password." << endl;
        continue;
    }
    
    rs->addMultipleUsers(users);
    
    rs->setReservation(file_handler.readReservationsFromFile());

  // switch case for each type of users
  // each users should the following options
  // they should also be informed on how to go back to the menu
  if (found) {
    bool logout = false;

    cout << "Welcome, " << current_user->getUsername() << "!" << endl;
    while (!logout) {
        char user_option;


         // Add this line to ignore the newline character in the input buffer
        //cin.ignore(numeric_limits<streamsize>::max(), '\n');
       if (user_option == 'q') {
            exit_program = true;
            break;
            }

      string court_number_string;
      int court_number;
      int mod_num;
      int res_id, mod_res_id, join_res_id;
      string mod_string;
      ModificationType mod_type;
      string date_string, schedule_date_string;
      string time_string, schedule_time_string;
      string mod_date, mod_time;
      time_t time, schedule_time, new_time;


      switch (current_user->getUserType()) {
        case USER_TYPE::MEMBER:
          cout << "Please choose a number to select an option:\n" << endl;
          cout << "1. View daily schedule" << endl;
          cout << "2. Reserve a court" << endl;
          cout << "3. Cancel a reservation" << endl;
          cout << "4. View Reservation" << endl;
          cout << "5. Modify Reservation " << endl;
          cout << "6. Join Reservation \n" << endl;
          cout << "7. Log out\n" << endl;
          cout << "Press 'q' to quit program entirely.\n" << endl;
          cin >> user_option;
          cout << endl;
          
          if (user_option == 'q') {
            exit_program = true;
            break;
          }

          switch (user_option) {
                case '1':
                   cout << "Please enter the day you would like to view the schedule (mm/dd/yyyy): \n" << endl;
                   cin >> schedule_date_string;
                   cout << endl;
                   schedule_time_string = "00:00";
                   schedule_time = convertStringToTimeT(schedule_date_string, schedule_time_string);
                   cout << "Date: " << schedule_date_string << endl;
                   rs->viewDailySchedule(schedule_time);
                    break;
                case '2':
                    cout << "Reserve a court, please note that coaching hours are bewteen 13:00 and 15:00 and open plays is 23:00 to close:\n" << endl;
                    cout << "Please enter court number: " << endl;
                    cin >> court_number_string;
                    cout << endl;
                    court_number = stoi(court_number_string);
                    cout << "Please enter date (mm/dd/yyyy): " << endl;
                    cin >> date_string;
                    cout << endl;
                    cout << "Please enter time (in increments of 30 minutes) (military time) (hh:mm): " << endl;
                    cin >> time_string;
                    cout << endl;
                    time = convertStringToTimeT(date_string, time_string);
                    //current_user->reserveCourt(court_number, time, (*rs));
                    rs->addReservation(current_user, court_number, time);
                    break;
                case '3':
                    cout << "Please enter the reservation id to cancel. You can find this by viewing the reservation. \n" << endl;
                    cin >> res_id;
                    cout << endl;
                    rs->cancelReservation(res_id);
                    break;
                case '4':
                    cout << "Please find your reservations below: \n" << endl;
                    current_user->viewReservations(rs);
                    break;
                case '5':
                    cout << "Please enter reservation id: \n" << endl;
                    cin >> mod_res_id;
                    cout << endl;
                    cout << "Please select the type of modifiction you would like to make (CHANGE or CANCEL): \n" << endl;
                    cin >> mod_string;
                    cout << endl;
                    if (mod_string == "CANCEL" ) {
                        mod_type = CANCEL;
                        
                    } else {
                        mod_type = CHANGE;
                        cout << "Please enter the new date you would like (mm/dd/yyyy): \n" << endl;
                        cin >> mod_date;
                        cout << endl;
                        cout << "Please enter the new time you would like (in increments of 30 minutes) (military time) (hh:mm): \n " << endl;
                        cin >> mod_time;
                        cout << endl;
                        new_time = convertStringToTimeT(mod_date, mod_time);
                    }
                    dynamic_cast<Member *>(current_user)->requestReservationModification(mod_res_id, mod_type, new_time, rs);
                    break;
                case '6':
                    cout << "Please enter reservation id: \n" << endl;
                    cin >> join_res_id;
                    cout << endl;
                    dynamic_cast<Member *>(current_user)->joinReservation(join_res_id, rs);

                break;
                case '7':
                logout = true;

                break;

                default:
                    cout << "Invalid option. Please try again." << endl;
                    break;
          }
          break;
        case USER_TYPE::COACH:
            cout << "Please choose a number to select an option:\n" << endl;
            cout << "1. View daily schedule" << endl;
            cout << "2. Reserve a court" << endl;
            cout << "3. Cancel a reservation" << endl;
            cout << "4. Modify reservation\n" << endl;
            cout << "5. Log out\n" << endl;
            cout << "Press 'q' to quit program entirely.\n" << endl;
            cin >> user_option;
            cout << endl;

          if (user_option == 'q') {
            exit_program = true;
            break;
          }

          switch (user_option) {
                case '1':                   
                    cout << "Please enter the day you would like to view the schedule (mm/dd/yyyy): \n" << endl;
                    cin >> schedule_date_string;
                    cout << endl;
                    schedule_time_string = "00:00";
                    schedule_time = convertStringToTimeT(schedule_date_string, schedule_time_string);
                    cout << "Date: " << schedule_date_string << endl;
                    rs->viewDailySchedule(schedule_time);
                    break;
                case '2':
                    cout << "Reserve a court, please note that coaching hours are only between 13:00 and 15:00" << endl;
                    cout << " Reservation must also be made 2 days in advance and no more than 7 days ahead \n" << endl;
                    cout << "Please enter court number: " << endl;
                    cin >> court_number_string;
                    cout << endl;
                    court_number = stoi(court_number_string);
                    cout << "Please enter date (mm/dd/yyyy): " << endl;
                    cin >> date_string;
                    cout << endl;
                    cout << "Please enter time (in increments of 30 minutes) (military time) (hh:mm): " << endl;
                    cin >> time_string;
                    cout << endl;
                    time = convertStringToTimeT(date_string, time_string);
                    //current_user->reserveCourt(court_number, time, (*rs));
                    rs->addReservation(current_user, court_number, time);
                    break;
                case '3':
                    cout << "Please enter the reservation id to cancel. You can find this by viewing the reservation. \n" << endl;
                    cin >> res_id;
                    rs->cancelReservation(res_id);
                    break;
                case '4':
                    cout << "Please enter reservation id: \n" << endl;
                    cin >> mod_res_id;
                    cout << endl;
                    cout << "Please select the type of modifiction you would like to make (CHANGE or CANCEL): \n" << endl;
                    cin >> mod_string;
                    cout << endl;
                    if (mod_string == "CANCEL" ) {
                        mod_type = CANCEL;
                        
                    } else {
                        mod_type = CHANGE;
                        cout << "Please enter the new date you would like (mm/dd/yyyy): \n" << endl;
                        cin >> mod_date;
                        cout << "Please enter the new time you would like (in increments of 30 minutes) (military time) (hh:mm): \n " << endl;
                        cin >> mod_time;
                        cout << endl;
                        new_time = convertStringToTimeT(mod_date, mod_time);
                    }
                    dynamic_cast<Member *>(current_user)->requestReservationModification(mod_res_id, mod_type, new_time, rs);
                case '5':
                        logout = true;

                break;
                default:
                    cout << "Invalid option. Please try again." << endl;
                    break;
          }
        break;
        case USER_TYPE::OFFICER:
          cout << "Please choose a number to select an option:\n" << endl;
          cout << "1. View daily schedule" << endl;
          cout << "2. Reserve a court" << endl;
          cout << "3. Cancel a reservation" << endl;
          cout << "4. View Modification Requests" << endl;
          cout << "5. Log out\n" << endl;
          cout << "Press 'q' to quit program entirely.\n" << endl;
          cin >> user_option;
          cout << endl;

          if (user_option == 'q') {
            exit_program = true;
            break;
          }

            switch (user_option) {
                case '1':
                    cout << "Please enter the day you would like to view the schedule (mm/dd/yyyy): \n" << endl;
                    cin >> schedule_date_string;
                    cout << endl;
                    schedule_time_string = "00:00";
                    schedule_time = convertStringToTimeT(schedule_date_string, schedule_time_string);
                    cout << "Date: " << schedule_date_string << endl;
                    rs->viewDailySchedule(schedule_time);
                    break;
                case '2':
                    cout << "Reserve a court \n" << endl;
                    cout << "Please enter court number: " << endl;
                    cin >> court_number_string;
                    cout << endl;
                    court_number = stoi(court_number_string);
                    cout << "Please enter date (mm/dd/yyyy): " << endl;
                    cin >> date_string;
                    cout << endl;
                    cout << "Please enter time (in increments of 30 minutes) (military time) (hh:mm): " << endl;
                    cin >> time_string;
                    cout << endl;
                    time = convertStringToTimeT(date_string, time_string);
                    //current_user->reserveCourt(court_number, time, (*rs));
                    rs->addReservation(current_user, court_number, time);
                    break;
                case '3':
                    cout << "Please enter the reservation id to cancel. You can find this by viewing the reservation. \n" << endl;
                    cin >> res_id;
                    cout << endl;
                    rs->cancelReservation(res_id);
                    break;
                case '4':
                    cout << "View Modification Requests" << endl;
                    rs->viewModifications();
                    cout << "Select Request to fulfill: " << endl;
                    cin >> mod_num;
                    cout << endl;
                    for (int i = 0; i < rs->getModifications().size(); i++) {
                      if (mod_num == i) {
                        rs->modifyReservation(static_cast<Officer*>(current_user), rs->getModifications()[i].getReservationId(), rs->getModifications()[i].getTime());

                      }
                    }
                case '5':
                logout = true;

                break;
                default:
                    cout << "Invalid option. Please try again." << endl;
                break;
                    
          }
        break;
      }
    }
  }
    }
  

    file_handler.writeReservationsToFile(rs->getReservations());
    
    cout << endl;
    cout << "Thank you for visiting. Have a great rest of your day!" << endl;
return 0;
}
