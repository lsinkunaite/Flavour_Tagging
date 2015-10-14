#include <iostream> 
 
#include <TFile.h> 
#include <TTree.h> 
#include <TRandom3.h> 
 
using namespace std; 
 
int main(int argc, char *argv[]) 
{ 
    TFile file("test.root", "recreate"); 
    if (!file.IsOpen()) { 
        cerr << "Can't open file." << endl; 
        return 1; 
    } 
 
    TRandom3 rand(0); 
 
    double x; 
    double y; 
 
    TTree tree("tree", "Test Tree"); 
    tree.Branch("x", &x); 
    tree.Branch("y", &y); 
 
    for (int i=0; i<10000; i++) { 
        //generates normally distributed data points
        x = rand.Gaus(1, 1); 
        y = rand.Gaus(1, 1); 
 
        tree.Fill(); //fills TTree with data
    } 
 
    tree.Write(); 
    file.Close(); 
 
    return 0; 
} 
