#include "odom.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <chrono>
#include <cfloat>
#include "avl.h"
#include "hashMap.h"
#include "windows.h" 

using namespace std;

//Helper function: Returns the updated root of the tree
Node* callInsert(Node* root, Odom* name, int id);
void findMin(Node* root, hashMap* u_map, string option, int max_time_);
void findMax(Node* root, hashMap* u_map, string option, int max_time_);
void findAverage(Node* root, hashMap* u_map, string option, int max_time_);

int main()
{
    Node* root = nullptr; //avl tree root node
    hashMap u_map; //hashmap
    vector<Odom*> listOdom;
    string operation;

    while (true)
    {
        cout << "\nWhat would you wish to do? Please select a number." << endl;
        cout << "0. Stop Program" << endl;
        cout << "1. Load Data" << endl;
        cout << "2. Find Min of a value" << endl;
        cout << "3. Find Max of a value" << endl;
        cout << "4. Find Average of a value" << endl;
        cout << "5. Find Data at a specific time" << endl;
        cout << endl << "Selection: ";
        cin >> operation;

        if (operation == "0") {
            cout << "Thank you!" << endl;
            break;
        }
        else if (operation == "1") //load data into data structures
        {

            ifstream ifs;
            string file;
            cout << "\nWhat file would you like to load?" << endl;
            cin >> file;
            cout << endl;
            ifs.open(file);
            if (!ifs) {
                cout << "\nInvalid file" << endl << endl;
            }
            else {

                string lineFromFile;
                istringstream line;
                double px, py, pz, lvx, lvy, lvz, avx, avy, avz;
                double maxlvx = 0;
                int t = 0; // time variable

                // gets column titles 
                getline(ifs, lineFromFile);

                //obtain all values from csv and push them to a vector to avoid file reading in timing
                while (getline(ifs, lineFromFile))
                {
                    line.str(lineFromFile);

                    string temp;
                    getline(line, temp, ',');
                    px = stod(temp);
                    getline(line, temp, ',');
                    py = stod(temp);
                    getline(line, temp, ',');
                    pz = stod(temp);
                    getline(line, temp, ',');
                    lvx = stod(temp);
                    getline(line, temp, ',');
                    lvy = stod(temp);
                    getline(line, temp, ',');
                    lvz = stod(temp);
                    getline(line, temp, ',');
                    avx = stod(temp);
                    getline(line, temp, ',');
                    avy = stod(temp);
                    getline(line, temp, ',');
                    avz = stod(temp);

                    Odom* point = new Odom(px, py, pz, lvx, lvy, lvz, avx, avy, avz, t);
                    listOdom.push_back(point);
                    t += 10;
                }
                cout << "\nThe number of elements in this dataset: " << listOdom.size() << endl;
                ifs.close();

                //----------------------insert elements into avl tree----------------------//
                auto o_map_start = chrono::system_clock::now();
                for (int i = 0; i < listOdom.size(); i++) {
                    root = callInsert(root, listOdom[i], listOdom[i]->t_());
                }
                auto o_map_stop = chrono::system_clock::now();
                double o_map_duration = chrono::duration_cast<chrono::milliseconds>(o_map_stop - o_map_start).count();
                cout << "\nAVL Tree load duration: " << o_map_duration << " ms" << endl;

                //----------------------insert elements into unordered map (hashMap)-----------------------//
                auto u_map_start = chrono::system_clock::now();
                for (int i = 0; i < listOdom.size(); i++) {
                    u_map.insert(listOdom[i]);
                }
                auto u_map_stop = chrono::system_clock::now();
                double u_map_duration = chrono::duration_cast<chrono::milliseconds>(u_map_stop - u_map_start).count();
                cout << "\nHashmap load duration: " << u_map_duration << " ms" << endl << endl;
            }
        }
        else if (operation == "2" && listOdom.size() != 0) { //find the min 
            int min_time = 0;
            int max_time = listOdom.size() * 10;
            cout << endl;
            cout << "Which value would you like to find the minimum for?" << endl;
            cout << "1. Position X (m)" << endl;
            cout << "2. Position Y (m)" << endl;
            cout << "3. Position Z (m)" << endl;
            cout << "4. Linear Velocity X (m/s)" << endl;
            cout << "5. Linear Velocity Y (m/s)" << endl;
            cout << "6. Linear Velocity Z (m/s)" << endl;
            cout << "7. Angular Velocity X (rad/s)" << endl;
            cout << "8. Angular Velocity Y (rad/s)" << endl;
            cout << "9. Angular Velocity Z (rad/s)" << endl;
            cin >> operation;

            findMin(root, &u_map, operation, listOdom.size() * 10);
        }
        else if (operation == "3" && listOdom.size() != 0) { //find the max
            int min_time = 0;
            int max_time = listOdom.size() * 10;
            cout << endl;
            cout << "Which value would you like to find the maximum for?" << endl;
            cout << "1. Position X (m)" << endl;
            cout << "2. Position Y (m)" << endl;
            cout << "3. Position Z (m)" << endl;
            cout << "4. Linear Velocity X (m/s)" << endl;
            cout << "5. Linear Velocity Y (m/s)" << endl;
            cout << "6. Linear Velocity Z (m/s)" << endl;
            cout << "7. Angular Velocity X (rad/s)" << endl;
            cout << "8. Angular Velocity Y (rad/s)" << endl;
            cout << "9. Angular Velocity Z (rad/s)" << endl;
            cin >> operation;

            findMax(root, &u_map, operation, listOdom.size() * 10);
        }
        else if (operation == "4" && listOdom.size() != 0) { //find the average
            int min_time = 0;
            int max_time = listOdom.size() * 10;
            cout << endl;
            cout << "Which value would you like to find the average for?" << endl;
            cout << "1. Position X (m)" << endl;
            cout << "2. Position Y (m)" << endl;
            cout << "3. Position Z (m)" << endl;
            cout << "4. Linear Velocity X (m/s)" << endl;
            cout << "5. Linear Velocity Y (m/s)" << endl;
            cout << "6. Linear Velocity Z (m/s)" << endl;
            cout << "7. Angular Velocity X (rad/s)" << endl;
            cout << "8. Angular Velocity Y (rad/s)" << endl;
            cout << "9. Angular Velocity Z (rad/s)" << endl;
            cin >> operation;

            findAverage(root, &u_map, operation, listOdom.size() * 10);
        }
        else if (operation == "5" && listOdom.size() != 0) { //find values at time t
            int time;
            cout << "\nEnter time:" << endl;
            cin >> time;
            
            auto avl_start = std::chrono::high_resolution_clock::now();
            Odom* avl_data = root->search(root, time);
            auto avl_stop = std::chrono::high_resolution_clock::now();
            double avl_duration = std::chrono::duration_cast<chrono::milliseconds>(avl_stop - avl_start).count();

            cout << "\nPosition X (m): " << avl_data->px_() << "\tLinear Velocity X (m/s): " << avl_data->lvx_() << "\tAngular Velocity X (rad/s): " << avl_data->avx_() << endl;
            cout << "Position Y (m): " << avl_data->py_() << "\tLinear Velocity Y (m/s): " << avl_data->lvy_() << "\tAngular Velocity Y (rad/s): " << avl_data->avy_() << endl;
            cout << "Position Z (m): " << avl_data->pz_() << "\tLinear Velocity Z (m/s): " << avl_data->lvz_() << "\tAngular Velocity Z (rad/s): " << avl_data->avz_() << endl;

            cout << "\nAVL Tree search duration: " << avl_duration << " ms" << endl;

            auto hash_start = chrono::high_resolution_clock::now();
            Odom* hash_data = u_map.get(time);
            auto hash_stop = chrono::high_resolution_clock::now();
            double hash_duration = chrono::duration_cast<chrono::milliseconds>(hash_stop - hash_start).count();

            cout << "\nPosition X (m): " << hash_data->px_() << "\tLinear Velocity X (m/s): " << hash_data->lvx_() << "\tAngular Velocity X (rad/s): " << hash_data->avx_() << endl;
            cout << "Position Y (m): " << hash_data->py_() << "\tLinear Velocity Y (m/s): " << hash_data->lvy_() << "\tAngular Velocity Y (rad/s): " << hash_data->avy_() << endl;
            cout << "Position Z (m): " << hash_data->pz_() << "\tLinear Velocity Z (m/s): " << hash_data->lvz_() << "\tAngular Velocity Z (rad/s): " << hash_data->avz_() << endl;

            cout << "\nHashmap search duration: " << hash_duration << " ms" << endl << endl;
        }
        else {
            cout << "Invalid Option. Please Try Again." << endl << endl;
        }
    }

    return 0;
}

Node* callInsert(Node* root, Odom* name, int id) {

    if (!root) { //If the root is null, create the node outside of the insert function
        return new Node(name, id);
    }
    //Finally, call the function
    //O(logN)
    return root->insert(name, id);
}

void findMin(Node* root, hashMap* u_map, string option, int max_time_) {
    int min_time = 0;
    int max_time = 0;

    cout << endl << "How many milliseconds into the simulation would you like to start?" << endl;
    cin >> min_time;
    cout << endl << "How many milliseconds into the simulation would you like to start? (Select -1 for end of simulation)" << endl;
    cin >> max_time;

    min_time = (min_time / 10) * 10; //allows us to truncate any value not divisible by ten
    if (max_time != -1 && max_time <= max_time_)
        max_time = (max_time / 10) * 10; //allows us to truncate any value not divisible by ten
    else
        max_time = max_time_;

    //Option is based on which physics value we want to find the min of
    if (option == "1" || option == "2" || option == "3" || option == "4" || 
        option == "5" || option == "6" || option == "7" || option == "8" || option == "9") {

        auto min_start = chrono::system_clock::now();
        cout << "\nThe min value for the avl tree is: " << root->minInorder(DBL_MAX, option, min_time, max_time) << endl;
        auto min_stop = chrono::system_clock::now();
        double min_duration = chrono::duration_cast<chrono::milliseconds>(min_stop - min_start).count();
        cout << "\nAVL Tree finding minimum duration: " << min_duration << " ms" << endl;

        auto min_start2 = chrono::system_clock::now();
        double min = DBL_MAX;
        double val = 0;
        for (int i = min_time; i < max_time; i += 10) {
            if (option == "1") {
                val = u_map->get(i)->px_();
            }
            else if (option == "2") {
                val = u_map->get(i)->py_();
            }
            else if (option == "3") {
                val = u_map->get(i)->pz_();
            }
            else if (option == "4") {
                val = u_map->get(i)->lvx_();
            }
            else if (option == "5") {
                val = u_map->get(i)->lvy_();
            }
            else if (option == "6") {
                val = u_map->get(i)->lvz_();
            }
            else if (option == "7") {
                val = u_map->get(i)->avx_();
            }
            else if (option == "8") {
                val = u_map->get(i)->avy_();
            }
            else if (option == "9") {
                val = u_map->get(i)->avz_();
            }

            if (val < min)
                min = val;
        }
        cout << "\nThe min value for the hashmap is: " << min << endl;
        auto min_stop2 = chrono::system_clock::now();
        double min_duration2 = chrono::duration_cast<chrono::milliseconds>(min_stop2 - min_start2).count();
        cout << "\nHashmap finding minimum duration: " << min_duration2 << " ms" << endl << endl;
    }
    else {
        cout << "Invalid Selection" << endl << endl;
    }
}

void findMax(Node* root, hashMap* u_map, string option, int max_time_) {
    int min_time = 0;
    int max_time = 0;

    cout << endl << "How many milliseconds into the simulation would you like to start?" << endl;
    cin >> min_time;
    cout << endl << "How many milliseconds into the simulation would you like to start? (Select -1 for end of simulation)" << endl;
    cin >> max_time;

    min_time = (min_time / 10) * 10; //allows us to truncate any value not divisible by ten
    if (max_time != -1 && max_time <= max_time_)
        max_time = (max_time / 10) * 10; //allows us to truncate any value not divisible by ten
    else
        max_time = max_time_;

    //Option is based on which physics value we want to find the min of
    if (option == "1" || option == "2" || option == "3" || option == "4" ||
        option == "5" || option == "6" || option == "7" || option == "8" || option == "9") {

        auto max_start = chrono::system_clock::now();
        cout << "\nThe max value for the avl tree is: " << root->maxInorder(0, option, min_time, max_time) << endl;
        auto max_stop = chrono::system_clock::now();
        double max_duration = chrono::duration_cast<chrono::milliseconds>(max_stop - max_start).count();
        cout << "\nAVL Tree finding maximum duration: " << max_duration << " ms" << endl;

        auto max_start2 = chrono::system_clock::now();
        double max = 0;
        double val = 0;
        for (int i = min_time; i < max_time; i += 10) {
            if (option == "1") {
                val = u_map->get(i)->px_();
            }
            else if (option == "2") {
                val = u_map->get(i)->py_();
            }
            else if (option == "3") {
                val = u_map->get(i)->pz_();
            }
            else if (option == "4") {
                val = u_map->get(i)->lvx_();
            }
            else if (option == "5") {
                val = u_map->get(i)->lvy_();
            }
            else if (option == "6") {
                val = u_map->get(i)->lvz_();
            }
            else if (option == "7") {
                val = u_map->get(i)->avx_();
            }
            else if (option == "8") {
                val = u_map->get(i)->avy_();
            }
            else if (option == "9") {
                val = u_map->get(i)->avz_();
            }

            if (val > max) {
                max = val;
            }
        }
        cout << "\nThe max value for the hashmap is: " << max << endl;
        auto max_stop2 = chrono::system_clock::now();
        double max_duration2 = chrono::duration_cast<chrono::milliseconds>(max_stop2 - max_start2).count();
        cout << "\nHashmap finding max duration: " << max_duration2 << " ms" << endl << endl;
    }
    else {
        cout << "Invalid Selection" << endl << endl;
    }
}

void findAverage(Node* root, hashMap* u_map, string option, int max_time_) {
    int min_time = 0;
    int max_time = 0;

    cout << endl << "How many milliseconds into the simulation would you like to start?" << endl;
    cin >> min_time;
    cout << endl << "How many milliseconds into the simulation would you like to start? (Select -1 for end of simulation)" << endl;
    cin >> max_time;

    min_time = (min_time / 10) * 10; //allows us to truncate any value not divisible by ten
    if (max_time != -1 && max_time <= max_time_)
        max_time = (max_time / 10) * 10; //allows us to truncate any value not divisible by ten
    else
        max_time = max_time_;

    //Option is based on which physics value we want to find the min of
    if (option == "1" || option == "2" || option == "3" || option == "4" ||
        option == "5" || option == "6" || option == "7" || option == "8" || option == "9") {

        auto average_start = chrono::system_clock::now();
        cout << "\nThe average value for the avl tree is: " << root->sumInorder(0, option, min_time, max_time) / (1 + (max_time - min_time) / 10) << endl;
        auto average_stop = chrono::system_clock::now();
        double average_duration = chrono::duration_cast<chrono::milliseconds>(average_stop - average_start).count();
        cout << "\nAVL Tree finding average duration: " << average_duration << " ms" << endl;

        auto average_start2 = chrono::system_clock::now();
        double sum = 0;
        double val = 0;
        for (int i = min_time; i < max_time; i += 10) {
            if (option == "1") {
                sum += u_map->get(i)->px_();
            }
            else if (option == "2") {
                sum += u_map->get(i)->py_();
            }
            else if (option == "3") {
                sum += u_map->get(i)->pz_();
            }
            else if (option == "4") {
                sum += u_map->get(i)->lvx_();
            }
            else if (option == "5") {
                sum += u_map->get(i)->lvy_();
            }
            else if (option == "6") {
                sum += u_map->get(i)->lvz_();
            }
            else if (option == "7") {
                sum += u_map->get(i)->avx_();
            }
            else if (option == "8") {
                sum += u_map->get(i)->avy_();
            }
            else if (option == "9") {
                sum += u_map->get(i)->avz_();
            }
        }
        auto average_stop2 = chrono::system_clock::now();
        cout << "\nThe average value for the hashmap is: " << sum / (1 + (max_time - min_time)/10) << endl;
        double average_duration2 = chrono::duration_cast<chrono::milliseconds>(average_stop2 - average_start2).count();
        cout << "\nHashmap finding average duration: " << average_duration2 << " ms" << endl << endl;
    }
    else {
        cout << "Invalid Selection" << endl << endl;
    }
}