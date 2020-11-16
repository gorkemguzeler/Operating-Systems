//
//  main.cpp
//  gorkem_guzeler_hw1
//
//  Created by gorkemg on 5.11.2020.
//
// This program creates a simple Airline Reservation System which may be booked by only two agency.
#include <iostream>
#include <pthread.h>
#include <vector>
#include <stdlib.h> // for srand(time)
#include <time.h>   // for srand(time)
#include <cstdio>   // for printf

using namespace std;

bool busy_waiting = true;

int turn = 0;

int remaining_seats = 100 ;

vector<vector<int> > airline_matrix(2, vector<int>(50,0)); //M12 notations must be added by pointers

bool no_seat = false;

void *func1(void*  ){
    
    while(busy_waiting!= false){

        //**********do step A:********
        int random_seat_number;
        //create a random number
        random_seat_number = rand()%100 + 1;
        
        while(turn != 0 ){};
        //**********do step B:********
        
        if(remaining_seats > 0){ // check if there is any empty seat. if so, go and try to reserve one.
            
            cout << "Agency 1 Entered Critical Region" << endl;
            
                int remainder_random = (random_seat_number-1) % 50;
                
                if(random_seat_number <= 50){ //in the first row
                    
                    if(airline_matrix[0][remainder_random] == 0){
                        airline_matrix[0][remainder_random] = 1;
                        cout << "Seat Number " << random_seat_number << " is reserved by Agency 1" << endl;
                        remaining_seats--;
                        
                    }
                }
                else{ //in the second row
                    if(airline_matrix[1][remainder_random] == 0){
                        airline_matrix[1][remainder_random] = 1;
                        cout << "Seat Number " << random_seat_number << " is reserved by Agency 1"<< endl;
                        remaining_seats--;
                    }
                }
            cout << "Agency 1 Exit Critical Region" << endl;
            cout << endl;
        }
        
        else if(no_seat == false){
            cout << "Agency 1 Entered Critical Region" << endl;
            cout << "Agency 1 Exit Critical Region" << endl;
            cout << "No Seats Left" << endl;
            cout << endl;
            no_seat = true;
            busy_waiting = false;
        }

        turn = 1;

    }
    return 0;
}

void *func2(void *){
    
        while(busy_waiting!= false){

            //**********do step B:********
            int random_seat_number;
            //create a random number
            random_seat_number = rand() %100 +1;
            
            while(turn != 1 ){};
            //**********do step A:********
            
            if(remaining_seats > 0){ // check if there is any empty seat. if so, go and try to reserve one.
             
                cout << "Agency 2 Entered Critical Region" << endl;
                
                    int remainder_random = (random_seat_number-1) % 50;
                    
                    if(random_seat_number <= 50){ //in the first row
                        
                        if(airline_matrix[0][remainder_random] == 0){
                            airline_matrix[0][remainder_random] = 2;
                            cout << "Seat Number " << random_seat_number << " is reserved by Agency 2"<< endl;
                            remaining_seats--;
                            
                        }
                    }
                    else{ //in the second row
                        if(airline_matrix[1][remainder_random] == 0){
                            airline_matrix[1][remainder_random] = 2;
                            cout << "Seat Number " << random_seat_number << " is reserved by Agency 2"<< endl;
                            remaining_seats--;
                        }
                    }
                cout << "Agency 2 Exit Critical Region" << endl;
                cout << endl;
            }
            
            else if(no_seat == false){
                cout << "Agency 2 Entered Critical Region" << endl;
                cout << "Agency 2 Exit Critical Region" << endl;
                cout << "No Seats Left" << endl;
                cout << endl;
                no_seat = true;
                busy_waiting = false;
            }
        
            turn = 0;

        }
    return 0;
}

int main(){
    
    srand(time(NULL));
    
    pthread_t travel_agency1;
    pthread_t travel_agency2;

    pthread_create(&travel_agency1 , NULL,  func1 , NULL);
    pthread_create(&travel_agency2 , NULL,  func2 , NULL);


    pthread_join(travel_agency1, NULL);
    pthread_join(travel_agency2, NULL);

    cout << "Plane is full:" << endl;
    
    cout << endl;
    
    for(int i = 0 ; i< airline_matrix.size()  ;i++){
        for(int k = 0; k < airline_matrix[0].size() ; k++){
            cout << airline_matrix[i][k] ;

        }
        cout << endl;
    }
    
    return 0;
}
