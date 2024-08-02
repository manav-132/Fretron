#include <iostream>
#include <vector>
#include <algorithm>
#include <numeric>
using namespace std;

void distributeApples(vector<int> &apples, int totalWeight) {
    vector<int> ram, sham, rahim;
    int ramShare = 50 * totalWeight / 100;
    int shamShare = 30 * totalWeight / 100;
    int rahimShare = 20 * totalWeight / 100;
    
    for (int weight : apples) {
        if (ramShare >= weight) {
            ram.push_back(weight);
            ramShare -= weight;
        } else if (shamShare >= weight) {
            sham.push_back(weight);
            shamShare -= weight;
        } else if (rahimShare >= weight) {
            rahim.push_back(weight);
            rahimShare -= weight;
        }
    }

    cout << "Ram: ";
    for (int weight : ram) cout << weight << " ";
    cout << endl;

    cout << "Sham: ";
    for (int weight : sham) cout << weight << " ";
    cout << endl;

    cout << "Rahim: ";
    for (int weight : rahim) cout << weight << " ";
    cout << endl;
}

int main() {
    vector<int> apples;
    int weight;
    
    while (true) {
        cout << "Enter apple weight in grams (-1 to stop): ";
        cin >> weight;
        
        if (weight == -1) {
            break;
        }
        
        apples.push_back(weight);
    }


    sort(apples.rbegin(), apples.rend()); // Sort in descending order

    int totalWeight = accumulate(apples.begin(), apples.end(), 0);
    distributeApples(apples, totalWeight);

    return 0;
}
