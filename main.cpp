//MyFeetHurt by Shayne Pollock, Chance Onyiorah, Sebastian Martinez. Go somewhere random in Boston!
#include <iostream>
#include <string>
#include <unordered_map>
#include <vector>
#include <utility>
#include <set>
#include <fstream>
#include <sstream>

using namespace std;

class AdjacencyList {
    unordered_map<string, vector<pair <string, float> > > list;
    unordered_map<string, vector<pair <string, float> > > lyft;
    unordered_map<string, vector<pair <string, float> > > uber;
    set<string> vertices;

public:
    //read in data & populate maps
    void insertLyft(string from, string to, float price); //populate lyft map
    void insertUber(string from, string to, float price); //populate uber map
    void printLyft(); //checking if everything is loaded properly
    void printUber(); //checking if everything is loaded properly
    bool averagePrice(string to, string from, int price, string type);

    //our sorting methods
    void heapSort(vector<pair<string, float> >& v);
    void heapify(vector<pair<string, float> >& v, int n, int start);
    void buildHeap(vector<pair<string, float> >& v, int size);
    void superSort(vector<pair<string, float> >& v);
    vector<pair<string, float>> bubbleSort(vector<pair<string, float>>& v);

    //miscellaneous
    void getDestinations(string location, string sort);
    bool locationCheck(string location);
    void printLocations();
    void getTime(string location);
};

bool AdjacencyList::averagePrice(string to, string from, int price, string type) {
    //if the location is already there average the price, otherwise return false
    vector<pair <string, float>> val;
    bool dne=true;
    if (type == "uber")
        val=uber[from];
    if (type == "lyft")
        val=lyft[from];

    for (int i=0; i< val.size(); i++)
    {
        if (val[i].first == to) //if its already in there
        {
            val[i].second = ((val[i].second) / 2.0) + ((price) / 2.0);
            dne = false;
        }
    }
    return dne;
}

void AdjacencyList::insertLyft(string from, string to, float price) //populate map
{
    if (averagePrice(to,from,price,"lyft")) //this function returns true if its not in the map
    {
        lyft[from].push_back(make_pair(to, price));
        lyft[to].push_back(make_pair(from, price));
    }
    list[from].push_back(make_pair(to, price));
}

void AdjacencyList::insertUber(string from, string to, float price) //populate map
{
    if (averagePrice(to,from,price,"uber")) //this function returns true if its not in the map
    {
        uber[from].push_back(make_pair(to, price));
        uber[to].push_back(make_pair(from, price));
    }
    list[from].push_back(make_pair(to, price));
}

void AdjacencyList::printLyft() //checking if the maps were made properly
{
    for (auto it : lyft)
    {
        for (auto ve : it.second)
            cout << it.first << "," << ve.first << "," << ve.second << "\n";
    }
}

void AdjacencyList::printUber() //checking if the maps were made properly
{
    for (auto it : uber)
    {
        for (auto ve : it.second)
            cout << it.first << "," << ve.first << "," << ve.second << "\n";
    }
}

void AdjacencyList::printLocations()
{
    for (auto it : list)
        cout << it.first << "\n";
}
//check if location exists to prevent exit11
bool AdjacencyList::locationCheck(string location) {
    bool found=false;
    for (auto it : lyft) //checking lyft locations for user input
    {
        if(it.first == location )
            found=true;
    }

    for (auto it : uber) //checking uber locations for user input
    {
        if(it.first == location )
            found=true;
    }
    return found;
}

void AdjacencyList::getTime(string location)
{
    //comparing times of heap sort vs bubble sort
    vector<pair<string, float> > destinationsUber = uber[location];
    vector<pair<string, float> > destinationsLyft = lyft[location];

    //time for heap sort, Uber
    auto superStart = chrono::high_resolution_clock::now();
    superSort(destinationsUber);
    superSort(destinationsLyft);
    auto superStop = chrono::high_resolution_clock::now();
    auto superTime = chrono::duration_cast<chrono::nanoseconds>(superStop-superStart).count();

    auto bubbleStart = chrono::high_resolution_clock::now();
    bubbleSort(destinationsUber);
    bubbleSort(destinationsLyft);
    auto bubbleStop = chrono::high_resolution_clock::now();
    auto bubbleTime = chrono::duration_cast<chrono::nanoseconds>(bubbleStop-bubbleStart).count();

    cout << "\nTime taken for heap sort: " << superTime << " nanoseconds\n";
    cout << "Time taken for bubble sort: " << bubbleTime << " nanoseconds\n";
}

//algorithm 1 = heap sort: heapify, buildHeap, heapSort, superSort --> logarithmic sort
void AdjacencyList::heapify(vector<pair<string, float> >& v, int n, int start)
{
    int big = start;
    int left = 2*start + 1;
    int right = 2*start + 2;

    if (left < n && v[left].second > v[big].second) { //find biggest element in vector
        big = left;
    }

    if (right < n && v[right].second > v[big].second) { //find biggest
        big = right;
    }

    if (big != start) { //swap biggest
        swap(v[start], v[big]);
        heapify(v, n, big); //recursion
    }
}
void AdjacencyList::buildHeap(vector<pair<string, float> >& v, int size) {
    for (int i = size / 2 - 1; i >= 0; i--) { //build heap
        heapify(v, size, i);
    }
}
void AdjacencyList::heapSort(vector<pair<string, float> >& v)
{
    int size = v.size();
    buildHeap(v, size); //builds a heap
    for (int i = size - 1; i > 0; i--) { //remove 1 element at a time
        swap(v[0], v[i]);
        heapify(v, i, 0);
    }
}
void AdjacencyList::superSort(vector<pair<string, float> >& v) {
    bool sorted = true;
    for(int i = 0; i < v.size()-1; i++) { //check if vector is already sorted => best case
        if(v[i] > v[i+1]) {
            sorted = false;
        }
    }
    if(sorted) {
        return;
    }
    
    heapSort(v); //heap sort otherwise
}

//algorithm 2 = bubble sort --> quadratic sort
vector<pair<string, float>> AdjacencyList:: bubbleSort(vector<pair<string, float>>& v) {
    for(int i = v.size()-1; i > 0; i++) {
        bool swap = false;
        for(int j = 0; j < i; j++) {
            if(v[j].second > v[j+1].second) {
                pair<string, int> t = v[j];//swap
                v[j] = v[j+1];
                v[j+1] = t;
            }
        }
        if(!swap) //if there's no swap, vector is already sorted -> exit. Best case.
            return v;
    }
    return v;
}

//call these on the uber and lyft maps. Print out sorted vector.
void AdjacencyList::getDestinations(string location,string sort)
{
    float uberMin=0;
    float lyftMin=0;
    float min=0;
    string type="";
    string destination="";

    //first do uber
    vector<pair<string, float> > destinationsUber = uber[location];
    if (sort=="heap")
        superSort(destinationsUber);
    if (sort == "bubble")
        bubbleSort(destinationsUber);
    cout << "\nThese are all the places you can go with an Uber!\n";
    uberMin=destinationsUber[0].second; //get the min price
    for(int i = 0; i < destinationsUber.size(); i++) {
        cout << "Possible Destination: " << destinationsUber[i].first;
        cout << ", Price: $" << std::setprecision(2)<< destinationsUber[i].second << endl;
    }

    //second do lyft
    vector<pair<string, float> > destinationsLyft = lyft[location];
    if (sort=="heap")
        superSort(destinationsLyft);
    if (sort == "bubble")
        bubbleSort(destinationsLyft);
    cout << "\nThese are all the places you can go with a Lyft!\n";
    lyftMin=destinationsLyft[0].second; //get the min price
    for(int i = 0; i < destinationsLyft.size(); i++) {
        cout << "Possible Destination: " << destinationsLyft[i].first;
        cout << ", Price: $" << std::setprecision(2)<< destinationsLyft[i].second << endl;
    }

    //print minimum of uber price and lyft price
    if (uberMin<lyftMin) {
        destination = destinationsUber[0].first;
        min = uberMin;
        type="an Uber";
    }
    else if (lyftMin<uberMin) {
        destination = destinationsLyft[0].first;
        min = lyftMin;
        type="a Lyft";
    }
    cout << "\nBased on the price comparison between Uber and Lyft, we recommend you take " << type;
    cout << " to " << destination;
    cout << " for about " << min << "$. Have fun!!! Be safe!!\n";

}//end heap destinations

int main()
{
    //READ IN FILE DATA AND POPULATE MAPS
    AdjacencyList graph;
    ifstream file("data.txt");

    if (!file.is_open()) {
        cout << "file could not be opened\n";
    }

    if (file)
    {
        string type, from, to, line, extra, p;
        float price;

        while (getline(file, line))
        {
            stringstream linestream(line);

            getline(linestream, extra, ',');
            getline(linestream, type, ',');
            getline(linestream, extra, ',');
            getline(linestream, to, ',');
            getline(linestream, from, ',');
            getline(linestream, p, ',');
            getline(linestream, extra, ',');
            getline(linestream, extra, ',');
            getline(linestream, extra, ',');
            getline(linestream, extra, ',');

            if (p == "") //there's some that have the price field blank
                continue;

            price = stof(p);

            if (type == "Lyft")
                graph.insertLyft(from, to, price);

            else
                graph.insertUber(from, to, price);
        }
    }

    file.close();
    //graph.printLyft(); //for debugging purposes
    //graph.printUber(); //for debugging purposes

    //INTERACT WITH USER
    string location;
    cout<< "Hi! Do your feet hurt?\nAre you bored and want to go somewhere? anywhere? \nPerfect! Enter your location to start: ";
    getline(cin, location);

    //Check if location is in our database. If it's not, ask again. Give a list of locations if they want.
    while(!(graph.locationCheck(location)))//if locationCheck returns false the user input is invalid
    {
        cout << "Oh no! Did you make a typo? We don't have that location in our database! Maybe try entering a place nearby :)\n";
        cout << "\nWould you like to see a list of places in Boston? (yes/no): ";
        string yn;
        getline(cin,yn);
        if (yn == "yes") //print out list of locations in database
            graph.printLocations();
        cout << "\nTry entering a new place: ";
        getline(cin,location);
    }

    string sort;
    cout<< "Would you like to do a heap sort or a bubble sort? (heap/bubble): ";
    getline(cin, sort);

    //output to screen the sorted vectors and choose the minimum of each as a recommendation
    graph.getDestinations(location,sort);

    //output times for heap and bubble sort
    graph.getTime(location);

    //EXIT GRACEFULLY
    return 0;
}
