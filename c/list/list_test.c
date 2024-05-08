#include <stdlib.h>
#include <stdio.h>




#define offsetof_xt(TYPE, MEMBER) __builtin_offsetof (TYPE, MEMBER)

#ifndef container_of_xt
#define container_of_xt(ptr, type, member) ({                        \
        const typeof(((type *)0)->member) * __mptr = (ptr);        \
        (type *)((char *)__mptr - offsetof_xt(type, member)); })
#endif

struct list_head_xt {
	struct list_head_xt *next, *prev;
};

#define LIST_HEAD_INIT_XT(name) { &(name), &(name) }

#define LIST_HEAD_XT(name) \
	struct list_head_xt name = LIST_HEAD_INIT_XT(name)

static inline void INIT_LIST_HEAD_XT(struct list_head_xt *list)
{
	list->next = list;
	list->prev = list;
}


static inline void __list_add(struct list_head_xt *new,
			      struct list_head_xt *prev,
			      struct list_head_xt *next)
{
	next->prev = new;
	new->next = next;
	new->prev = prev;
	prev->next = new;
}

static inline void list_add_xt(struct list_head_xt *new, struct list_head_xt *head)
{
	__list_add(new, head, head->next);
}

#define list_entry_xt(ptr, type, member) \
	container_of_xt(ptr, type, member)

#define list_first_entry_xt(ptr, type, member) \
	list_entry_xt((ptr)->next, type, member)

#define list_next_entry_xt(pos, member) \
	list_entry_xt((pos)->member.next, typeof(*(pos)), member)

#define list_last_entry_xt(ptr, type, member) \
	list_entry_xt((ptr)->prev, type, member)



#define list_for_each_entry_xt(pos, head, member)				\
	for (pos = list_first_entry_xt(head, typeof(*pos), member);	\
	     &pos->member != (head);					\
	     pos = list_next_entry_xt(pos, member))



typedef struct car{
    int     door_number;
    char    *color;
    char    *model;
    struct list_head_xt list;
} CAR;


LIST_HEAD_XT(carlist);

void main(void) {
    struct car *acar;
    struct car *redcar   = malloc(sizeof(CAR));

    INIT_LIST_HEAD_XT(&redcar->list);
    redcar->door_number   = 2;
    redcar->color         = "red";
    redcar->model         = "A";

    list_add_xt(&redcar->list,   &carlist);

    list_for_each_entry_xt(acar, &carlist, list) {
        printf("车门数[%d]\n",   acar->door_number);
        printf("车的颜色[%s]\n", acar->color      );
        printf("车的模具[%s]\n", acar->model      );
    }

}