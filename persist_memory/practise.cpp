#include <libpmemobj/atomic_base.h>
#include <libpmemobj/base.h>
#include <libpmemobj/iterator_base.h>
#include <libpmemobj/types.h>
#include <sys/stat.h>
#include <libpmemobj.h>


struct root_type{
    struct node_type *head;
    struct node_type *tail;
    int count;
};

struct node_type{
    struct node_type *next;
    int val;
};

const char *LAYOUT_NAME = "practice";

POBJ_LAYOUT_BEGIN(practice);
POBJ_LAYOUT_ROOT(practice, struct root_type);
POBJ_LAYOUT_TOID(practice, struct node_type);
POBJ_LAYOUT_END(practice);


void test(){
    const char *path = "practice.pool";
    PMEMobjpool *pop = pmemobj_create(path, POBJ_LAYOUT_NAME("practice"), PMEMOBJ_MIN_POOL, 0666);
    pmemobj_open(path, POBJ_LAYOUT_NAME("practice"));

    PMEMoid rootoid = pmemobj_root(pop, sizeof(struct root_type));
    struct root_type *root = (struct root_type *)pmemobj_direct(rootoid);
    {
        root->head = nullptr;
        root->tail = nullptr;
        root->count = 0;
    }
    pmemobj_persist(pop, root, sizeof(root_type));

    PMEMoid nodeoid;
    pmemobj_alloc(pop, &nodeoid, sizeof(node_type), TOID_TYPE_NUM(struct node_type), NULL, nullptr);
    struct node_type *node = (struct node_type *)pmemobj_direct(nodeoid);
    {
        node->next = nullptr;
        node->val = 1;
    }
    pmemobj_persist(pop, node, sizeof(node_type));

    //OID_INSTANCEOF(PMEMoid data, T);

    pmemobj_close(pop);
}

int main(){
    return 0;
}
