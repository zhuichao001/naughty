#include <stdio.h>
//Any two functions in a set of overloaded functions must have different argument lists.

class Entity{
    int e;
};

class Hub{
public:
    //int push(Entity en){printf("push(Entity)\n");return 0;}
    int push(Entity &en){printf("push(Entity&)\n");return 0;} /*the same to above*/
    
    //int push(const Entity e){return 0;} /*cannot be overloaded*/
    int push(const Entity &en){printf("push(const Entity&)\n");return 0;}

    int push(Entity *en){printf("push(Entity*)\n");return 0;}
    //int push(Entity * const en){printf("push(Entity*const)\n");return 0;} /*cannot be overloaded*/
    int push(const Entity *en){printf("push(const Entity*)\n");return 0;}
    //int push(Entity * const e){return 0;} /*cannot be overloaded*/

};

int main(){
    Hub h;
    Entity e;
    h.push(e);

    const Entity &f = e;
    h.push(f);

    Entity *p = &e;
    h.push(p);

    const Entity *q = &f;
    h.push(q);
    return 0;
}
