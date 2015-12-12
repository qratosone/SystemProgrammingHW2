#include <sys/types.h>
#include <unistd.h>
void *malloc_basic(size_t size){
    void *p;
    p=sbrk(0);
    if(sbrk(size)==(void *)-1){
        return NULL;
    }
    return p;
}
typedef struct s_block *t_block;
struct s_block{
    size_t size;
    t_block next;
    int free;
    int padding;
    char data[1];
};
t_block first_block;
t_block find_block_first(t_block *last,size_t size){
    t_block b=first_block;
    while(b&&!(b->free&&b->size>=size)){
        *last=b;
        b=b->next;
    }
    return b;
}
#define block_size 24
t_block extend_heap(t_block last,size_t s){
    t_block  b;
    b=sbrk(0);
    if(sbrk(block_size+s)==(void *)-1){
        return NULL;
    }
    b->size=s;
    b->next=NULL;
    if(last)
        last->next=b;
    b->free=0;
    return b;
}
void split_block(t_block b,size_t s){
    t_block newBlock;
    newBlock=b->data+s;
    newBlock->size=b->size-s-block_size;
    newBlock->next=b->next;
    newBlock->free=1;
    b->next=newBlock;
    b->size=s;
}
size_t align8(size_t s){
    if(s%8==0)
        return s;
    return ((s/8+1)*8);
}
void *myMalloc(size_t size){
    t_block b,last;
    size_t s;
    s=align8(size);
    if(first_block){
        last=first_block;
        b=find_block_first(&last,s);
        if(b){
            if((b->size-s)>=block_size+8){
                split_block(b,s);
                b->free=0;
            }
        }
        else{
            b=extend_heap(last,s);
            if(!b)
                return NULL;
        }
    }
    else{
        b=extend_heap(NULL,s);
        if(!b){
            return NULL;
        }
        first_block=b;
    }
    return b->data;
}