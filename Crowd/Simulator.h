#ifndef SIMULATOR
#define SIMULATOR

#include "Vector.h"
using namespace mathtool;

#include "Agent.h"

#include <vector>
using namespace std;

class Simulator {
   public:
      Simulator(bool b); 

      void Draw();
      void Update();

      int GetNumAgents() { return agents.size(); }
      Agent& GetAgent(int i) { return agents[i]; }

      void AddMember(int index, double x, double y);


      bool getTak() { return tak; }
      void setTak(bool t) { tak = t; };

   private:
      vector<Agent> agents;
      bool tak;
};

#endif
