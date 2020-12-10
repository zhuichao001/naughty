#include <stdio.h>
//Any two functions in a set of overloaded functions must have different argument lists.

class Entity{
    int e;
};

class Hub{
public:
    int push(Entity e){return 0;}
    int push(Entity &e){return 0;}
    //int push(const Entity e){return 0;} /*cannot be overloaded*/
    int push(const Entity &e){return 0;}

    int find(Entity *e){return 0;}
    int find(const Entity *e){return 0;}
    //int find(Entity * const e){return 0;} /*cannot be overloaded*/

};

int main(){
    Hub h;
    return 0;
}
