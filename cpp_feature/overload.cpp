#include <stdio.h>
//Any two functions in a set of overloaded functions must have different argument lists.

class Entity{
    int e;
};

class Hub{
public:
    //int push(Entity en){printf("push(Entity)\n");return 0;} /*给定类型和对该类型的引用将视为相同*/
    int push(Entity &en){printf("push(Entity&)\n");return 0;} /*the same to above*/
    
    //int push(const Entity e){return 0;} /*cannot be overloaded*/

    /*对于重载而言，由 const 或 volatile 修饰的类型的函数参数的处理方式与基类型没有什么不同。
     * 但是，函数重载机制可以区分由 const 和 volatile 限定的引用和对基类型的引用。*/
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
