#include <iostream>
using namespace std;
#include "dList.cpp"

int main(){
    const int size = 10;
    float avgTime[size]  =       { 93, 99, 96, 97, 95,100, 94, 98, 92, 98};
    float fuelEfficiency[size] = {194,201,196,156,193,100,194,198,200,140};
    
    dList dl = dList(avgTime,fuelEfficiency,size);
    cout << "First Output"<<endl;
    dl.out('a');
    //  92, 93,  95,  97,  98, 100
    //200, 194, 193, 156, 140, 100
    
    dl.insert(96,145);
    cout << endl <<"Second Output"<<endl;
    dl.out('a');
    //  92, 93,  95,  96,  98, 100
    //200, 194, 193, 145, 140, 100
    
    dl.insert(50,300);
    dl.insert(110,75);
    cout <<endl<< "Third Output"<<endl;
    dl.out();
    // 50,  92,  93,  95,  96,  98, 100, 110
    //300, 200, 194, 193, 145, 140, 100,  75
    
    dl.insert(97,70);
    cout << endl << "Fourth Output"<<endl;
    dl.out();
    // 50,  92,  93,  95,  96,  97
    //300, 200, 194, 193, 145,  70
    
    cout << endl << "Fifth Output"<<endl;
    dl.out('d');
    // 97,  96,  95,  93,  92,  50
    // 70, 145, 193, 194, 200, 300
    
    int x = dl.index_before(4), y = dl.index_after(4);
    cout << "Before is : " << x << "   After is : " << y << endl;
    
    for(int i=0;i<500000;i++) dl.insert(100+i%47 - (i%10001)/251.1 - .000001*i, 100 - i%91 + i%13 + .0001*i);
    cout << endl << "Sixth Output"<<endl;
    dl.out();
    
    for(int i=0;i<500000;i++) dl.insert(50 - .00001*i, 10+ .0001*i);
    cout << endl << "Seventh Output"<<endl;
    dl.out(10,'a');
    cout << endl << "At the back"<<endl;
    dl.out(10,'d');
    for(int i=1000013; i>500013; i-=5) dl.decrease_fuel(i,.00045);
    
    cout << endl << "Eighth Output"<<endl;
    dl.out(10,'a');
    cout << endl << "At the back"<<endl;
    dl.out(10,'d');
    //    dl.out();
    return 0;
}
