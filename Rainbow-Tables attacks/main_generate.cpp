#include <iostream>
#include <string>
#include <time.h>
#include <stdlib.h>
#include <set>
#include <fstream>
//#include<thread>
#include "blake_ref.h"
//#include <mutex>

using namespace std;

string plaintext_from_hash3(string);
string create_hash_blake_256(string);
void task1(string);

int main(void){
//thread x;

string charset = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789!@";
set<string> myset;                  //for creating 1 million different plaintexts
set<string>::iterator it;
set<string>::iterator temp;
pair<set<string>::iterator,bool> ret;
ofstream output;
output.open("test.txt");	//output_10000 px
int duplicates = 0;
int length = charset.length();
//srand(10);
    srand(time(NULL));      //initializing randomness
    rand();

    cout<<"Length : "<<length<<endl;

    for(int i=0; i<1000000; i++){
        string str;
        int place;

        for(int j=0; j<6; j++){                                      //for each char in string generate a number between 0-63
            place = (int)(length*(double)rand()/RAND_MAX );
            while(place == 64)                                      //fixing a bug sometimes number 64 comes up!
                place = (int)(length*(double)rand()/RAND_MAX );
            str += charset[place];                                  //find a random char from "charset"!
        }


        ret = myset.insert(str);

        while (ret.second == false){                                    //for duplicates
            string new_str;
            for(int j=0; j<6; j++){                                      //for each char in string generate a number between 0-63
                place = (int)(length*(double)rand()/RAND_MAX );
                    while(place == 64)                                   //fixing a bug sometimes number 64 comes up!
                        place = (int)(length*(double)rand()/RAND_MAX );
                new_str += charset[place];                               //find a random char from "charset"!
            }
            ret = myset.insert(new_str);
            duplicates++;
        }



    }
cout<<"1 Million Plaintexts Created "<<endl;

string blake_hash;
string new_plaintext;
int counter = 0;
set<string> test;//// for collisions

for (it=myset.begin(); it!=myset.end(); it++){

    counter++;
    new_plaintext = *it;
    output<<new_plaintext<<"\t";
    for(int j=0; j<10000; j++){ //CHAIN SIZE!
        blake_hash = create_hash_blake_256(new_plaintext);
        new_plaintext = plaintext_from_hash3(blake_hash);
    }test.insert(create_hash_blake_256(new_plaintext));////
    output<<create_hash_blake_256(new_plaintext);
    temp = it;
    if( (++temp) != myset.end() )
        output<<endl;
    if( counter%100000 == 0 )
        cout<<counter/10000<<"% completed!"<<endl;
}

cout<<test.size()<<endl;//// for collisions

    return 0;
}

string create_hash_blake_256(string old_plaintext){

    BitSequence hash[64];
    BitSequence data[6];
    for(int i=0; i<6; i++)
        data[i] = old_plaintext[i];

    Hash( 256, data, 48, hash );

    char buffer[64];
    char *ap = buffer;
    for(int i=0; i<32; ++i)
        ap += sprintf(ap,"%02x",hash[i]);


    string result = buffer;

return result;
}

    /*Here is my Reduction Function after lots of tests!!! */

string plaintext_from_hash3(string my_hash){

string charset = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789!@";

    string result;
    int sum = 0;
    int counter=0;
        /* this is for our last XOR 50-60 with the last 4 chars + the 6 firsts! */
    int last_digits = my_hash[60]+my_hash[61]+my_hash[62]+my_hash[63];
    for(int i=0; i<6; i++)
        last_digits += my_hash[i];
        /* this is for our 5 XOR */
    int for_XOR[6];
    for(int i=0; i<6; i++){
            sum=0;
        for(int j=0; j<10; j++){
            if(my_hash[counter] == '0')
                sum += 0;
            else if(my_hash[counter] == '1')
                sum += 1;
            else if(my_hash[counter] == '2')
                sum += 2;
            else if(my_hash[counter] == '3')
                sum += 3;
            else if(my_hash[counter] == '4')
                sum += 4;
            else if(my_hash[counter] == '5')
                sum += 5;
            else if(my_hash[counter] == '6')
                sum += 6;
            else if(my_hash[counter] == '7')
                sum += 7;
            else if(my_hash[counter] == '8')
                sum += 8;
            else if(my_hash[counter] == '9')
                sum += 9;
            else if(my_hash[counter] == 'a')
                sum += 10;
            else if(my_hash[counter] == 'b')
                sum += 11;
            else if(my_hash[counter] == 'c')
                sum += 12;
            else if(my_hash[counter] == 'd')
                sum += 13;
            else if(my_hash[counter] == 'e')
                sum += 14;
            else if(my_hash[counter] == 'f')
                sum += 15;
            counter++;
        }
        for_XOR[i] = sum;
    }
    int res;
    res = for_XOR[0] ^ for_XOR[1];      //1st XOR
    if(res<0)
        res=-res;
    result+=charset[res%64];
    res = for_XOR[1] ^ for_XOR[2];      //2nd XOR
    if(res<0)
        res=-res;
    result+=charset[res%64];
    res = for_XOR[2] ^ for_XOR[3];      //3rd XOR
    if(res<0)
        res=-res;
    result+=charset[res%64];
    res = for_XOR[3] ^ for_XOR[4];      //4th XOR
    if(res<0)
        res=-res;
    result+=charset[res%64];
    res = for_XOR[4] ^ for_XOR[5];      //5th XOR
    if(res<0)
        res=-res;
    result+=charset[res%64];
    res = for_XOR[5] ^ last_digits;      //6th XOR
    if(res<0)
        res=-res;
    result+=charset[res%64];

    return result;

}


