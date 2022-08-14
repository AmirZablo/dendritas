#include <iostream>
#include <cmath>
#include <vector>
#include <fstream>
#include <ctime>
#include <cstdlib>
#include <string>
#include <sstream>

using namespace std;

enum Direction {
    LEFT, RIGHT, UP, DOWN, UPLEFT, UPRIGHT, DOWNLEFT, DOWNRIGHT, NUM_DIRS
};

int n=800; //Grid size
int cantWalkers=20000; //Amount of walkers

class rw;

//Declare a vector for all the walkers and a matrix for the fixed points
vector<rw> randomWalkers;
vector<vector<int> > puntosFijos; //-1 -> mobile; x>=0 -> fixed (sticks to structure at iteration x)

//Mod function to set boundary conditions
double mod(int a, int N)
{
    return (a%N +N)%N;
}

class rw
{
public:
    //Variables
    int x;
    int y;

    //Constructor
    rw(int x_,int y_)
    {
        x=x_;
        y=y_;
    }

    //Moves randomWalker in given direction (periodic border)
    void mover(Direction d)
    {
        switch(d)
        {
            case LEFT:
                x--;
                break;

            case RIGHT:
                x++;
                break;

            case UP:
                y++;
                break;

            case DOWN:
                y--;
                break;

            case UPLEFT:
                y++;
                x--;
                break;

            case UPRIGHT:
                x++;
                y++;
                break;

            case DOWNLEFT:
                y--;
                x--;
                break;

            case DOWNRIGHT:
                y--;
                x++;
                break;

            default:
                break;
        }
    }

   //Checks colition
    bool choca()
    {
        for(int i=-1;i<2;i++)
        {
            for(int j=-1;j<2;j++)
            {
                if(i!=0 || j!=0)
                {
                    if(puntosFijos.at(mod(x+i,n)).at(mod(y+j,n))>=0)
                    {
                        return true;
                    }
                }
            }
        }

        return false;
    }

};

int main()
{
    int cantChoques=0;
    srand(time(NULL));

    //Initialize puntosFijos with no fixed points (the walker in each cell contains the coordinates on the grid)
    puntosFijos.resize(n);
    for(int i=0;i<n;i++)
    {
        for(int j=0;j<n;j++)
        {
                puntosFijos.at(i).push_back(-1);
        }
    }

    //Set seed
    puntosFijos.at(n/2).at(n/2)=0;

    //Initialize walkers
    for(int i=0;i<cantWalkers; i++)
    {
        rw randWalker(rand()%n,rand()%n);
        randomWalkers.push_back(randWalker);
    }

    //Open text file
    ofstream oFile;
    oFile.open("Dendrita.dat");
    oFile.is_open();
    oFile<< "x " << "y "<<"iteracion"<< endl;

    //Cycle
    for(int i=0;i<2000000;i++)
    {
        //Moves every walker and checks if it colides with the structure
        for(int j=randomWalkers.size()-1;j>=0;j--)
        {
            randomWalkers.at(j).mover(Direction(rand() % NUM_DIRS));

            if(randomWalkers.at(j).choca())
            {
                cantChoques++;
                //Add walker to puntosFijos
                puntosFijos.at(mod(randomWalkers.at(j).x,n)).at(mod(randomWalkers.at(j).y,n))=i;
                //Delete walker from randomWalkers
                randomWalkers.erase(randomWalkers.begin()+j);

                cout<<"Compilando, banca un toque :D"<<"\n\n";
                cout<<"Cantidad de choques: "<<cantChoques<<'\n';
                cout<<"Iteracion: "<<i<<'\n';
                break;
            }
        }

        //If all the walkers are fixed, exit the loop
        if(cantChoques==cantWalkers)
        {
           break;
        }
    }

    //Save data
    for(int i=0; i<n; i++)
    {
        for(int j=0; j<n; j++)
        {
            if(puntosFijos.at(i).at(j)>=0)
            {
                oFile<<i<<" "<<j<<" "<<puntosFijos.at(i).at(j)<<'\n';
            }
        }
    }

    return 0;
}
