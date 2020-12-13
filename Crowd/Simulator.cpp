#include "Simulator.h"
#include <stdlib.h>

#include <GL/glut.h>  // (or others, depending on the system in use)
#include "Basic.h"

Simulator::Simulator(bool b) {

    tak = b;

    if(tak == false){

    char np;
    cout << "Do you want to change some setting (Y/N)?" << endl;
    cin >> np;

    if (np == 'y' || np == 'Y') {
        int nc;
        double ns, na, ra, mv, ma;

        cout << "How many agents do you want?" << endl;
        cin >> nc;
        cout << "What is the mass do you want your agents to be in?" << endl;
        cin >> na;
        cout << "What is the radius do you want your agents to be in?" << endl;
        cin >> ra;
        cout << "What is your maximum velocity?" << endl;
        cin >> mv;
        cout << "What is your maximum acceleration" << endl;
        cin >> ma;

        ofstream myfile("user.txt");
        if (myfile.is_open())
        {
            myfile << nc << endl;
            myfile << na << endl;
            myfile << ra << endl;
            myfile << mv << endl;
            myfile << ma << endl;
            myfile.close();
        }
        else cout << "Complete, your inputs are store in a file called user.txt";
    }
    }

        if (tak == true) {

            int nc;
            double ns, na, ra, mv, ma;

            ifstream myfile("user.txt");

            if (myfile.is_open())
            {
                while (myfile >> nc >> ns >> na >> ra >> mv >> ma)
                {
                    cout << nc;
                    cout << ns;
                    cout << na;
                    cout << ra;
                    cout << mv;
                    cout << ma;

                }
                myfile.close();
            }


            for (int i = 0; i < nc; i++) {
                //pos
                double x = 50 + 300 * (myRand() - 0.5);
                double y = 50 + 300 * (myRand() - 0.5);
                double z = 0;
                Vector3d pos(x, y, z);
                //vel
                double vx = 20 * (myRand() - 0.5);
                double vy = 20 * (myRand() - 0.5);
                double vz = 0;
                Vector3d vel(vx, vy, vz);
                double mass = 4 + myRand() * 10; //random between 4 and 14
                double maxVel = 1 + myRand() * 5; //random between 18 and 28
                //double maxAccel = 20+drand48()*10; //random between 20 and 30
                double maxAccel = 20 + myRand() * 10; //random between 20 and 30
                double viewRadius = 40 + myRand() * 10; //random between 40 and 50
                //agents.push_back( Vector3d(x,y) );
                Agent a_i;
                a_i.Init(i, pos, vel, na, ra, mv, ma);
                agents.push_back(a_i);

            }
        }


        else {
            for (int i = 0; i < 20; i++) {
                //pos
                double x = 50 + 300 * (myRand() - 0.5);
                double y = 50 + 300 * (myRand() - 0.5);
                double z = 0;
                Vector3d pos(x, y, z);
                //vel
                double vx = 20 * (myRand() - 0.5);
                double vy = 20 * (myRand() - 0.5);
                double vz = 0;
                Vector3d vel(vx, vy, vz);
                double mass = 4 + myRand() * 10; //random between 4 and 14
                double maxVel = 1 + myRand() * 5; //random between 18 and 28
                //double maxAccel = 20+drand48()*10; //random between 20 and 30
                double maxAccel = 20 + myRand() * 10; //random between 20 and 30
                double viewRadius = 40 + myRand() * 10; //random between 40 and 50
                //agents.push_back( Vector3d(x,y) );
                Agent a_i;
                a_i.Init(i, pos, vel, mass, maxVel, maxAccel, viewRadius);
                agents.push_back(a_i);

          }
    }
}


void Simulator::AddMember(int index, double x, double y) {
    int i = index;
    if (i < 0) i = agents.size(); //if send in negative, set id to last in list
    double z = 0;
    Vector3d pos(x, y, z);
    //vel
    double vx = 20 * (myRand() - 0.5);
    double vy = 20 * (myRand() - 0.5);
    double vz = 0;
    Vector3d vel(vx, vy, vz);
    double mass = 4 + myRand() * 10; //random between 4 and 14
    double maxVel = 8 + myRand() * 5; //random between 18 and 28
    //double maxAccel = 20+drand48()*10; //random between 20 and 30
    double maxAccel = 20 + myRand() * 10; //random between 20 and 30
    double viewRadius = 40 + myRand() * 10; //random between 40 and 50
    //agents.push_back( Vector3d(x,y) );
    Agent a_i;
    a_i.Init(i, pos, vel, mass, maxVel, maxAccel, viewRadius);
    agents.push_back(a_i);
}

void Simulator::Draw() {
  for(int i=0; i<(int)agents.size(); i++) {
    agents[i].Draw();
  }
}

void Simulator::Update() {
  double dt = 0.25;
  for(int i=0; i<(int)agents.size(); i++) {
    agents[i].Update(agents, dt);
  }


}
