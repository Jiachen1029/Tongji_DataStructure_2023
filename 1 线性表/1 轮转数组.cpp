#include <iostream>
using namespace std;

int main() {
    int array_size, rotation_steps;
    cin >> array_size >> rotation_steps;
    int* elements;
    elements = new int[array_size];
    for (int index = 0; index < array_size; index++) 
        cin >> elements[(index + rotation_steps) % array_size];

    for (int index = 0; index < array_size; index++) 
        cout << elements[index] << " ";
    cout << endl;
    delete[] elements;
}
