#include <iostream>
#include <fstream>
#include <omp.h>
#include <cstring>
using namespace std;

int main(){    
    int time, cal_time, tid, k = 0, l = 2;
    int check[37] = {0};
    int size_i = 33, size_j = 8;
    int time_check[37] = {0};
    int data_count = 0;
    int point[26] = {0};
    int point_index = 0;

    string line;
    string day;
    string arr[33][8];
    string data[20];
    string sections[37] = {"1A", "1B", "1C", "1D", "1E", "1F", "1G", "1H", "1I", "1J", "1K", "1L", "1M",
                        "3A", "3B", "3C", "3D", "3E", "3F",
                        "5A", "5B", "5C", "5D", "5E", "5F", "5G", "5H", "5I", "5J",
                        "7A", "7B", "7C", "7D", "7E", "7F", "7G", "7H"};

    cout << endl << "ENTER DAY (MONDAY - FRIDAY): ";
    cin >> day;
    
    for (int i = 0; i < day.length(); i++)
        day[i] = toupper(day[i]);

    cout << "ENTER TIME (8 - 3): ";
    cin >> time;
    cout << endl;

    if (time >= 8 && time <= 12)
        cal_time = time - 8;
    else    
        cal_time = time + 4;
    
    omp_set_num_threads(2);
    #pragma omp parallel            //READ FILE FOR DAY
    {
        ifstream fp("Days\\" + day + ".txt"); 
        tid = omp_get_thread_num();

        if (tid == 0){
            for (int i = 0; i < size_i; i++){
                for (int j = 0; j < size_j; j = 2 + j){
                    fp.seekg(k) >> arr[i][j];
                    fp.clear();
                    k = k + 6;
                }
            }
        }

        else if (tid == 1){
            for (int i = 0; i < size_i; i++){
                for (int j = 1; j < size_j; j = 2 + j){
                    fp.seekg(l) >> arr[i][j];
                    fp.clear();
                    l = l + 6;
                }
            }
        }

        fp.close();            
    }
    
    omp_set_num_threads(4);
    #pragma omp parallel
    {   
        #pragma omp for collapse(3) nowait    
        for (int i = 0; i < size_i; i++){
            for (int j = 0; j < size_j; j++){
                for (int k = 0; k < 37; k++){
                    if (arr[i][j] == sections[k]){
                        check[k] = 1;
                    }
                }
            }
        }

        #pragma omp for collapse(3)
        for (int i = 0; i < size_i; i++){
            for (int j = cal_time; j < size_j; j++){
                for (int k = 0; k < 37; k++){
                    if (arr[i][j] == sections[k])
                        time_check[k] = 1;
                }
            }
        }    
    }
    
    cout << "TIMETABLE FOR " << day << ": "  << endl << endl;

    for (int i = 0; i < size_i; i++){
        for (int j = 0; j < size_j; j++){
            cout << arr[i][j] << "\t";
        }
        cout << endl;
    }

    cout << endl << "SECTIONS FREE ON " << day << " ARE: " << endl;
 
    for (int i = 0; i < 37; i++){
        if (check[i]  == 0){
            cout << sections[i] << endl;
            time_check[i] = 1;
        }       
    }

    cout << endl << "SECTIONS FREE ON " << day << " AFTER " << time << " ARE: " << endl;
 
    for (int i = 0; i < 37; i++){
        if (time_check[i]  == 0){
            cout << sections[i] << endl;
            data[data_count] = sections[i];
            data_count++;
        }
    }

    cout << endl;

    for (int i = 0; i <= data_count; i++){
        ifstream fp("Sections\\" + data[i] + ".txt");  
            
        if (fp.is_open()){
            while (getline(fp, line)){
                cout << line << endl;
                line = line.substr(line.length() - 2);
                point_index = stoi(line);
                point[point_index - 1] = point[point_index - 1] + 1;
            }  
        }
    }

    cout << endl;
    
    for (int i = 0; i < 26; i++)
        if (point[i] != 0)
            cout << "POINT NUMBER " << i+1 << " HAS " << point[i] << " STUDENTS" << endl;

    cout << endl;
}