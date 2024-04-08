// Fall 2023

#include <iostream>
#include <string>
#include <vector>
using namespace std;

struct Person{
  string name;
  vector<string> likes; // vector of names of persons 
                        // in order this person likes them
  string spouse; // = "" if none
};

// This function randomizes the list of people that each person likes
// It will assign a random index to pull from, nameVect 
// If the name repeats or if it belongs to the person, then randomize again
void randLikes(Person* p, vector<string>& nameVect, const int& index, const int& size){
  // index tells us which person this struct belongs to and will only randomize the others
  for (int i = 0; i < size-1; i++){
    // we want a likes vector of size 2N-1
    int randi = rand() %size; // randomize
    if (randi != index){ 
      // if the # isn't the same as the person index, then make sure # doesn't give the same name as likes vector
      bool randomized = false;
      while (randomized == false){
        randomized = true; // will remain true unless detect a repetition in likes vector
        for (int n=0; n<p->likes.size(); n++){
          // check each element of likes vector
          if (nameVect[randi] == p->likes[n]){
            // if that randi index is already in the likes vector, try again
            randomized = false;
            randi = rand() %size;
          }
        }
      }
    }
    while (randi == index){
      // if the # is the same as the person index, randomize again
      randi = rand() %size;
      // if the # isn't the same as the person index, then make sure # doesn't give the same name as likes vector
      bool randomized = false;
      while (randomized == false){
        randomized = true; // will remain true unless detect a repetition in likes vector
        for (int n=0; n<p->likes.size(); n++){
          // check each element of likes vector
          if (nameVect[randi] == p->likes[n]){
            // if that randi index is already in the likes vector, try again
            randomized = false;
            randi = rand() %size;
          }
        }
      }
    } 
    p-> likes.push_back(nameVect[randi]);
  }
}

// This function randomly assign N-1 couples their random spouses
// If spouse == "" then the person is available for a spouse
// Assign a random index (randi) to grab a random person from nameVect and turn them into a spouse
// Then go through the list of people and make sure the random spouse isn't already taken
// If not taken then assign the spouse to that person, and also make that person the spouses's spouse
// add 1 to married couple
// if married couple == N-1, break
void randSpouse(vector<Person*>& people, vector<string>& nameVect, const int& size){
  // we will have N-1 couples married
  int marriedCouple = 0;
  for (int index=0; index<people.size(); index++){
    if (marriedCouple == ((size/2)-1)) {break;} // break if N-1 couples are married
    // going through each person and assign them a spouse or leave them unmarried
    int randi = rand() % size; 
    if (people[index]->spouse == ""){
      // will only marry if their spouse is empty
      bool randomized = false;
      while (randomized == false){
        // while married couple is less than N-1 and while the spouse from nameVect[randi] is taken, randomize again
        randomized = true; // will remain true unless detect that the potential spouse is taken
        while (randi==index){
          // if randi == index, randomize again, we don't want the person to marry themselves
          randi = rand() % size;
        }
        for (int i=0; i<people.size(); i++){
          // use this loop to check that the person getting married to isn't already married to someone else
          if (nameVect[randi] == people[i]->spouse){
            // if the person is already someone else's spouse, randomize again
            randomized = false;
            randi = rand() % size;
            break;
          }
        }
      }
      people[index]->spouse = nameVect[randi];
      for (int i=0; i<people.size(); i++){
        // go through people vector again
        if (people[i]->name == people[index]->spouse){
          // match the person[index] spouse with person[i] and make person[index] their spouse as well
          people[i]->spouse = people[index]->name;
        }
      }
      marriedCouple++;
    }
  }
}

// This function allocate each of the 2N names into the people vector
// It also assign a shuffled list of people that the perosn likes using randLikes function
// It doesn't assign a spouse at first and input "" for each member
// Then it assign random spouse using the randSpouse function
void allocate(vector<Person*>& people, vector<string>& nameVect, const int& size){
  // allocate pointers to each of 2N persons on the people vector
  for (int i=0; i<size; i++){
    Person* p = new Person;
    p->name = nameVect[i];
    randLikes(p, nameVect, i, size);
    p->spouse = ""; // we'll leave everyone unmarried until a people vector is established
    people.push_back(p);
  }
  // now we marry N-1 couples (just so that some people are married and allow cheating to occur)
  randSpouse(people, nameVect, size); 
}

// This function prints out the names of each person, their likes list, and their spouse (if any)
// Spouse will be indicated from the start of the game and will be capitalized
void print(vector<Person*>& people){
  for (int i=0; i<people.size(); i++){
    cout << people[i]->name << " - ";
    for (int n=0; n<people[i]->likes.size(); n++){
      if (people[i]->likes[n] == people[i]->spouse){
        for (int c=0; c<people[i]->spouse.size(); c++){
          // this loop converts the spouse's name to capilaized letters
          people[i]->spouse[c] = toupper(people[i]->spouse[c]);
          cout << people[i]->spouse[c];
        }
        // afterward, set spouse name equal to people[i]->likes[n]
        people[i]->spouse = people[i]->likes[n];
        cout << " ";
      } else {
        cout << people[i]->likes[n] << " ";
      }
    }
    cout << endl;
  }
}

// This function finds and return the index of any person on the people vector
int findPerson(vector<Person*>& people, const string& name){
  int index = 100;
  for (int i=0; i<people.size(); i++){
    if(people[i]->name == name){ // if matches, the index belongs to the person
      index = i;
      break;
    }
  }
  if (index== 100){cout << "People index error!";}
  return index;
}

// This function finds and return index of anyone in the like list of any person on the people vector
int findLike(Person* p, const string& name){
  int index = 100;
  for (int i=0; i<p->likes.size(); i++){
    if(p->likes[i] == name){ // if matches, the index (in the likes vector) belongs to the input name
      index = i;
      break;
    }
  }
  if (index== 100){cout << "Likes index error!";}
  return index;
}

// This function marries two people
// It will first check if person2's name is the same as person1's spouse
//   If it's not, then it will go through people vector, find person1's spouse and unmarry them (== "")
//   print out (person 1 cheats on person1's spouse)
// Then it will replace person1's spouse with person2's name (if it's the same then it shouldn't matter if it's replaced)
// Then repeat for person 2
void marry(vector<Person*>& people, const string& person1, const string& person2){
  int indexP1 = findPerson(people, person1);
  int indexP2 = findPerson(people, person2);
  // First, we make person2 the spouse of person1
  if ((person2 != people[indexP1]->spouse) && (people[indexP1]->spouse != "")){
    // if person 2 isn't the current spouse of person 1
    // this means person 1 is cheating
    int cheatIndex = findPerson(people, people[indexP1]->spouse); // find the person that's being cheated on
    people[cheatIndex]->spouse = ""; // unmarry them
    cout << people[indexP1]->name << " cheats on " << people[cheatIndex]->name << endl;
  }
  people[indexP1]->spouse = person2; // replace person1's spouse with person2's name
  if ((person1 != people[indexP2]->spouse) && (people[indexP2]->spouse != "")){
    // same thing as what we did with person 1
    // if person 1 isn't the current spouse of person 2
    // this means person 2 is cheating
    int cheatIndex = findPerson(people, people[indexP2]->spouse); // find the person that's being cheated on
    people[cheatIndex]->spouse = ""; // unmarry them
    cout << people[indexP2]->name << " cheats on " << people[cheatIndex]->name << endl;
  }
  people[indexP2]->spouse = person1; // replace person2's spouse
} 

// This function checks if everyone is married
// if yes, returns true, if no returns false
bool townMarried(vector<Person*>& people){
  bool married = true;
  for (int i=0; i<people.size(); i++){
    if(people[i]->spouse == ""){
      married = false;
      break;
    }
  }
  return married;
}

// After each marriage, we need to check if every couple is happy
// Each couple is happy if no one cheats (each couple is person1 and person2)
// Let person3 be someone else married that is outside the couple
// Each person can cheat if person1 likes person3 better than they like person 2 
// AND person 3 likes person1 better than they like their own spouse
// the same can happen to person2 with person3
bool happyCheck (vector<Person*>& people){
  bool happy = true;
  for (int i=0; i<people.size(); i++){
    if (people[i]->spouse != ""){
      // to compare who they like more, we need to find their spouse's index and compare it to the others on the likes list
      int spouseInd1 = findLike(people[i], people[i]->spouse);
      for (int n=0; n<spouseInd1; n++){
        int cheaterInd3 = findPerson(people, people[i]->likes[n]);
        // go through the list of people that person i likes more than their spouse and check if they're married
        if (people[cheaterInd3]->spouse !=""){
          // if they're married, then there's a potential to cheat
          // we then need to check if this person (that person i likes more) likes person i more than their spouse
          int spouseInd3 = findLike(people[cheaterInd3], people[cheaterInd3]->spouse); // find index of their spouse in likes
          for (int m=0; m<spouseInd3; m++){
            int cheaterInd1 = findPerson(people, people[cheaterInd3]->likes[m]); // find index of the person that the potential cheater likes more than their current spouse
            if (people[cheaterInd1]->name == people[i]->name){
              // if they like each other more than their spouse, then they can cheat
              happy = false;
              break;
            }
          }
        }
      }
    }
  }
  return happy;
}

// This function deallocate every person and set their pointer to nullprt
void deallocate(vector<Person*>& people){
  for (int i=0; i<people.size(); i++){
    delete people[i];
    people[i] = nullptr;
  }
}

int main() {
  // seeding rand
  srand(time(NULL));
  int N;
  cout << "Enter how many couples. Enter 0 to quit: ";
  cin >> N; 
  while (N!=0){
    // declaring vectors
    vector<Person*> people;
    vector<string> nameVect = {"Blake", "Parker", "Morgon" , "Riley", "Taylor", "Rowan", "Sage", "Quinn", "Ellis", "Dylan", "Angel", "Emery"};
    // bigger loop of the entire game (i.e. everytime the program ask for # of couple)
    int size = 2*N;
    allocate(people, nameVect, size);
    print(people);
    string game = "in play";
    while(game != "over"){
      // loop for each individual game (after player has declared how many couples)
      string person1;
      string person2;
      // ask user for 2 people to marry
      cout << "Enter 2 names to marry: ";
      cin >> person1 >> person2;
      cout << endl;
      marry(people, person1, person2);
      print(people);
      bool happy = happyCheck(people);
      bool married = townMarried(people);
      if (happy == false){
        cout << "At least one couple is unhappy " << endl;
        game = "in play";
      }
      if (married == false){
        cout << "At least one person is not married" << endl;
        game = "in play";}
      // check if everyone's happy, if so game == over, else game == "in play"
      if (happy == true && married == true){
        cout << "Yay, everyone is happy!";
        game = "over";
      }
    }
    cout << endl << endl << "Enter how many couples. Enter 0 to quit: ";
    cin >> N; 
    deallocate(people); // deallocate all pointers
  }
  cout << "Thank you for playing!";
  return 0;
}
