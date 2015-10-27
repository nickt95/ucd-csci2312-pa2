#include <iostream>
#include "clustering.h"

using namespace std;
using namespace clustering;

int main() {
    string input;
    string output;

    cout << "What is the file name for the input file?" << endl;
    cin >> input;

    cout << "What is the file name for the output file?" << endl;
    cin >> output;

    kMeans myMeans(input, output);

    int k;
    cout << "How many centroids?" << endl;
    cin >> k;

    return 0;
}