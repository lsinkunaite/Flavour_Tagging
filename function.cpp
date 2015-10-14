#include "TCanvas.h" 
#include "TF1.h" 
#include "TGraph.h" 
 
int main(int argc, char *argv[]) 
{ 
    int n = 5; 
    double x[] = {1.0, 2.0, 3.0, 4.0, 5.0}, 
           y[] = {1.0, 4.0, 9.0, 16.0, 25.0}; 
 
    TGraph graph(n, x, y); 
    TF1 fsq("fsq", "x^2", 1.0, 5.0); 
 
    TCanvas canvas("c", "Canvas", 800, 600); 
    graph.Draw("A*"); 
    fsq.Draw("SAME"); 
    canvas.SaveAs("graph.pdf"); 
} 
